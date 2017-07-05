########## Input Formular ##########
form qtaParameterPrediction
	optionmenu Task: 1
		option Predict F0
		option Assemble ensemble file
	comment F0 analysis options:
		integer left_F0_range_(Hz) 75
		integer right_F0_range_(Hz) 600
	comment Target approximation options:
		word input_file ./path/to/samples-predict.csv
		word folder_name short-form
		real syllable_shift 0
		integer model_order 5
endform	

########## Main Script ##########

##### get input values
f0_min = left_F0_range
f0_max = right_F0_range
#targetFile$ = input_file
abb$ = folder_name$
shift = syllable_shift
n = model_order
cnt = 0
pulse = 0
resynthesis = 1

##### check validity
assert f0_min < f0_max

##### find audio files
directory$ = "./"
Create Strings as file list... list 'directory$'*.wav
numberOfFiles = Get number of strings
if !numberOfFiles
	exit There are no sound files in the folder!
else
	more_file = 1
endif

Read Table from comma-separated file... 'input_file$'
Rename... test
ntargets = Get number of rows

if task = 1
	##### iterate over all syllables (targets)
	for current_target from 1 to ntargets

		##### get information from file
		select Table test
		name$ = Get value... current_target name
		name$ = replace_regex$ (name$, "label:", "", 1)

		##### get current audio file
		Read from file... 'directory$''name$'.wav

		##### load TextGrid file
		if fileReadable ("'directory$''name$'.TextGrid")
			Read from file... 'directory$''name$'.TextGrid
		else
			exit Missing TextGrid File 'name$'
		endif

		nintervals = Get number of intervals... 1
		wordStart = Get start time of interval... 1 2
		wordEnd = Get start time of interval... 1 nintervals

		##### get predicted qTA parameters
		if resynthesis
			call qtaAnalysis
		endif

		##### save result
		select PitchTier fittedf0sampled
		Down to TableOfReal... Hertz
		Write to headerless spreadsheet file... 'directory$'/../data/plot-files/'name$'.'abb$'f0sampled
		Remove
		select TableOfReal targets
		Write to headerless spreadsheet file... 'directory$''name$'.targets
		#Remove

		##### create resynthesized audio file
		call qtaResynthesis

		##### clean up
		select PitchTier fittedf0sampled
		Remove
		select Sound 'name$'
		Remove
	endfor
else
	call assembleTargetFile
endif

########## (Procedure) determine qTA parameters ##########
procedure qtaAnalysis
	
	##### save results
	Create PitchTier... fittedf0sampled wordStart wordEnd
	Create TableOfReal... fittedf0sampled 1 2
	Set column label (index)... 1 SampleTime
	Set column label (index)... 2 fittedf0 (st)

	Create TableOfReal... targets 1 6
	Set column label (index)... 1 slope
	Set column label (index)... 2 offset
	Set column label (index)... 3 strength
	Set column label (index)... 4 duration
	Set column label (index)... 5 rmse
	Set column label (index)... 6 corr

	interval_t = 0

	##### iterate over syllables
	for nm from 1 to nintervals
		select TextGrid 'name$'
		label$ = Get label of interval... 1 nm
		
		##### consider syllable shift
		tmpS = Get starting point... 1 nm
		if tmpS = wordStart
			start = tmpS
		else
			start = tmpS - shift/1000
		endif

		tmpE = Get end point... 1 nm
		if tmpE = wordEnd
			end = tmpE
		else
			end = tmpE - shift/1000
		endif

		interval_dur = end - start

		if not label$ = ""
			interval_t = interval_t + 1

			##### get initial F0
			Read from file... 'directory$'../data/plot-files/'name$'.qtaf0sampled
			tInitial = Get value... 1 Time
			xInitial = Get value... 1 F0
			xInitial = 94.4288
			tFinal = end

			if interval_t == 1
				deriv[1] = xInitial
				for i from 2 to n
    				deriv[i] = 0
				endfor
			else
				for i from 1 to n
    				deriv[i] = derivFinal[i]
				endfor
			endif

			##### get predicted qta parameters from file
			select Table test
			mstring$ = Get value... current_target slope
			mstring$ = replace_regex$ (mstring$, "m:", "", 1)
			m = number(mstring$)

			bstring$ = Get value... current_target offset
			bstring$ = replace_regex$ (bstring$, "b:", "", 1)
			b = number(bstring$)

			lstring$ = Get value... current_target strength
			lstring$ = replace_regex$ (lstring$, "l:", "", 1)
			l = number(lstring$)

			dstring$ = Get value... current_target duration
			dstring$ = replace_regex$ (dstring$, "d:", "", 1)
			d = number(dstring$)

			##### ./predictqta
			filedelete 'directory$'config
			fileappend 'directory$'config 'm' 'm''newline$'
			fileappend 'directory$'config 'b' 'b''newline$'
			fileappend 'directory$'config 'l' 'l''newline$'
			fileappend 'directory$'config 'n''newline$'
			for i from 1 to n
				val = deriv[i]
    			fileappend 'directory$'config 'val' 
			endfor
			fileappend 'directory$'config 'newline$'
			fileappend 'directory$'config 'start' 'end''newline$'

			#TODO original f0 in data file for rmse/corr
			filedelete 'directory$'data
			fileappend 'directory$'data 0'newline$'0'newline$'0'newline$'0'newline$'0'newline$'0'newline$'0'newline$'0'newline$'
				
			if fileReadable("./predictqta")
				cnt = cnt + 1
				runSystem: "printf '\r\t predicted targets: 'cnt''"
				runSystem: "./predictqta ./"
			else
				printline Cannot find predictqta!
				exit
			endif	
			buff$ < output
				
			##### analyze output file
			#First line: qTA parameters
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
	
			#First line: m
			sep_index = index(sbuff$," ")
			pbuff$ = left$(sbuff$,sep_index-1)
			sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
			mBest'interval_t' = 'pbuff$'
			m = mBest'interval_t'
	
			#First line: b
			sep_index = index(sbuff$," ")
			pbuff$ = left$(sbuff$,sep_index-1)
			sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
			bBest'interval_t' = 'pbuff$'
			b = bBest'interval_t'
	
			#First line: lambda
			lambdaBest'interval_t' = 'sbuff$'
			lambda = lambdaBest'interval_t'
	
			duration'interval_t' = end - start
			d = duration'interval_t'
			
			#Second line: final F0 articulatory state
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
	
			#Second line: Final state
			for i from 1 to n-1
    			sep_index = index(sbuff$," ")
				pbuff$ = left$(sbuff$,sep_index-1)
				sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
				derivFinal[i] = 'pbuff$'
			endfor

			#Second line: last derivative
			derivFinal[n] = 'sbuff$'
	
			##Third line: rmse and correlation results
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
	
			##Third line: rmse
			sep_index = index(sbuff$," ")
			pbuff$ = left$(sbuff$,sep_index-1)
			sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
			#rmse'interval_t' = 'pbuff$'
			#rmse = rmse'interval_t'
	
			##Third line: correlation
			#corr'interval_t' = 'sbuff$'
			#corr = corr'interval_t'
	
			#Forth line: number of sample
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
			number_of_row = 'sbuff$'
	
			#Save targets
			select TableOfReal targets
			nrows = Get number of rows
			Set value... nrows 1 m
			Set value... nrows 2 b
			Set value... nrows 3 l			
			Set value... nrows 4 d
			Set value... nrows 5 1
			Set value... nrows 6 0
			Insert row (index)... nrows + 1

			buff$ < dataoutput
				
			##### analyze dataoutput file: equally sampled F0
			#First line: number of samples
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
			number_samples = 'sbuff$'
		
			#Remaining: time and synthesized f (equally sampled)
			select PitchTier fittedf0sampled
			for i from 1 to number_samples
				select TableOfReal fittedf0sampled
				len_buff = length(buff$)
				line_index = index(buff$,newline$)
				sbuff$ = left$(buff$,line_index-1)
				buff$ = mid$(buff$,line_index+1,len_buff)
				len_sbuff = length(sbuff$)
	
				sep_index = index(sbuff$," ")
				pbuff$ = left$(sbuff$,sep_index-1)
				sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
				sampletime = 'pbuff$'
				qTAf0 = 'sbuff$'
	
				nrows = Get number of rows
				Set value... nrows 1 sampletime
				Set value... nrows 2 qTAf0
				Set row label (index)... nrows 'label$'
				Insert row (index)... nrows + 1
				select PitchTier fittedf0sampled
				Add point... sampletime qTAf0
			endfor
			current_target =  current_target + 1
		endif
	endfor

	current_target =  current_target - 1
	filedelete 'directory$'config
	filedelete 'directory$'data
	filedelete 'directory$'output
	filedelete 'directory$'dataoutput

	##### last row is empty
	select TableOfReal targets
	nrows = Get number of rows
	Remove row (index)... nrows

endproc

########## (Procedure) Create ensemble target file ##########
procedure assembleTargetFile

	##### create result container
	writeFileLine: "targets.txt", "slope,offset,strength,duration,rmse,corr"

	##### iterate over all files
	for current_file from 1 to numberOfFiles

		##### get current target file
		select Strings list
		fileName$ = Get string... current_file
		name$ = fileName$ - ".wav" - ".WAV"
		
		if fileReadable ("'name$'.targets")
			Read TableOfReal from headerless spreadsheet file... 'name$'.targets
			filedelete 'name$'.targets
			select TableOfReal 'name$'
			nrows = Get number of rows

			##### iterate over rows (syllable targets)
			appendFile: "targets.txt",name$
			for i from 1 to nrows
				#read targets
				select TableOfReal 'name$'
				m = Get value... i 1
				b = Get value... i 2
				l = Get value... i 3
				d = Get value... i 4
				r = Get value... i 5
				c = Get value... i 6
			
				#write targets
				appendFile: "targets.txt",",m:",m,",b:",b,",l:",l,",d:",d,",r:",r,",c:",c
			endfor
			appendFile: "targets.txt",newline$
		endif
	endfor
endproc

########## (Procedure) resynthesize with approximated F0 ##########
procedure qtaResynthesis
	select Sound 'name$'
	To Manipulation... 0.01 f0_min f0_max
	select PitchTier fittedf0sampled
	Formula... exp(self*ln(2)/12)
	plus Manipulation 'name$'
	Replace pitch tier
	select Manipulation 'name$'
	Get resynthesis (overlap-add)
	createDirectory: "'directory$'/../audios/svm"
	nowarn Save as WAV file: "'directory$'/../audios/svm/'name$'_svm.wav"
	Remove
	select Manipulation 'name$'
	Remove
endproc
