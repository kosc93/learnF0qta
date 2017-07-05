########## Input Formular ##########
form qtaParameterSearch
	optionmenu Task: 1
		option Compute qTA parameters
		option Assemble ensemble file
	comment F0 analysis options:
		integer left_F0_range_(Hz) 100
		integer right_F0_range_(Hz) 600
	comment Target extraction options:
		real left_Target_slope_range_(st/s) -100
		real right_Target_slope_range_(st/s) 100
		real left_Target_height_range_(st) 80
		real right_Target_height_range_(st) 120
		real left_Strength_range 1
		real right_Strength_range 80
		real syllable_shift 0
		integer model_order 5
		integer algorithm 34
		integer random_iterations 5
endform

########## Main Script ##########

##### get input values
f0_min = left_F0_range
f0_max = right_F0_range
m_min = left_Target_slope_range
m_max = right_Target_slope_range
b_min = left_Target_height_range
b_max = right_Target_height_range
l_min = left_Strength_range
l_max = right_Strength_range
shift = syllable_shift
n = model_order
cnt = 0
pulse = 0
resynthesis = 1

##### check validity
assert f0_min < f0_max
assert m_min < m_max
assert b_min < b_max
assert l_min < l_max

##### find audio files
directory$ = "./"
Create Strings as file list... list 'directory$'*.wav
numberOfFiles = Get number of strings
if !numberOfFiles
	exit There are no sound files in the folder!
else
	more_file = 1
endif

if task = 1
	##### iterate over all aufio files
	for current_file from 1 to numberOfFiles

		##### get current audio file
		select Strings list
		fileName$ = Get string... current_file
		name$ = fileName$ - ".wav" - ".WAV"
		Read from file... 'fileName$'

		##### load TextGrid file
		if fileReadable ("'directory$''name$'.TextGrid")
			Read from file... 'directory$''name$'.TextGrid
		else
			exit Missing TextGrid File 'name$'
		endif

		nintervals = Get number of intervals... 1
		wordStart = Get start time of interval... 1 2
		wordEnd = Get start time of interval... 1 nintervals

		##### Get F0 PitchTier
		if (pulse)
			if fileReadable ("'directory$''name$'.pulse")
				Read from file... 'directory$''name$'.pulse
			else
				exit Missing pulse File 'name$'
			endif
	
			select PointProcess 'name$'
			maxperiod = 1/f0_min
			To PitchTier... maxperiod
			Rename... semitonef0
			Formula... 12 * ln (self) / ln(2); semitone
			Down to TableOfReal... Hertz
			Write to headerless spreadsheet file... 'directory$''name$'.semitonef0
			Remove
		else
			createDirectory: "'directory$'../data/plot-files"
			select Sound 'name$'
			To Manipulation... 0.01 f0_min f0_max
			Extract pitch tier
			Rename... semitonef0
			Formula... 12 * ln (self) / ln(2); semitone
			Down to TableOfReal... Hertz
			Write to headerless spreadsheet file... 'directory$'../data/plot-files/'name$'.semitonef0
			Remove
			select Manipulation 'name$'
			Remove
		endif

		##### find qTA parameters
		if resynthesis
			call qtaAnalysis
		endif

		##### save result
		select PitchTier fittedf0
		Down to TableOfReal... Hertz
		Write to headerless spreadsheet file... 'directory$'../data/plot-files/'name$'.qtaf0
		Remove
		select PitchTier fittedf0sampled
		Down to TableOfReal... Hertz
		Write to headerless spreadsheet file... 'directory$'../data/plot-files/'name$'.qtaf0sampled
		Remove
		select TableOfReal targets
		Write to headerless spreadsheet file... 'directory$''name$'.targets
		Remove

		##### create resynthesized audio file
		call qtaResynthesis

		##### clean up
		select PitchTier semitonef0
		Remove
		select PitchTier fittedf0
		Remove
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
	Create PitchTier... fittedf0 wordStart wordEnd
	Create TableOfReal... fittedf0 1 2
	Set column label (index)... 1 SampleTime
	Set column label (index)... 2 fittedf0 (st)

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
	
	##### temporary data???
	select PitchTier semitonef0
	Down to TableOfReal... Hertz
	Rename... trainingf0

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

			select PitchTier semitonef0
			index_first = Get high index from time... start
			index_last = Get low index from time... end
			# syllable shift can lead to problems if there is no F0 in interval
			if index_last = 0
				index_last = 1
			endif
			tInitial = Get time from index... index_first
			xInitial = Get value at index... index_first
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

			##### ./findqta
			filedelete 'directory$'config
			fileappend 'directory$'config 'm_min' 'm_max''newline$'
			fileappend 'directory$'config 'b_min' 'b_max''newline$'
			fileappend 'directory$'config 'l_min' 'l_max''newline$'
			fileappend 'directory$'config 'n''newline$'
			for i from 1 to n
				val = deriv[i]
    			fileappend 'directory$'config 'val' 
			endfor
			fileappend 'directory$'config 'newline$'
			fileappend 'directory$'config 'start' 'end''newline$'
			
			select TableOfReal trainingf0
			Extract row ranges... 'index_first':'index_last'
			Write to short text file... 'directory$'data
			Remove

			if fileReadable("./findqta")
				cnt = cnt + 1
				runSystem: "printf '\r\t calculated targets: 'cnt''"
				runSystem: "./findqta ./ 'algorithm' 'random_iterations'"
			else
				printline Cannot find findqta!
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
	
			#Third line: rmse and correlation results
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
	
			#Third line: rmse
			sep_index = index(sbuff$," ")
			pbuff$ = left$(sbuff$,sep_index-1)
			sbuff$ = mid$(sbuff$,sep_index+1,len_sbuff)
			rmse'interval_t' = 'pbuff$'
			rmse = rmse'interval_t'
	
			#Third line: correlation
			corr'interval_t' = 'sbuff$'
			corr = corr'interval_t'
	
			#Forth line: number of sample
			len_buff = length(buff$)
			line_index = index(buff$,newline$)
			sbuff$ = left$(buff$,line_index-1)
			buff$ = mid$(buff$,line_index+1,len_buff)
			len_sbuff = length(sbuff$)
			number_of_row = 'sbuff$'
	
			#Remaining: time and synthesized f
			select PitchTier fittedf0
			for i from 1 to number_of_row
				select TableOfReal fittedf0
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
				select PitchTier fittedf0
				Add point... sampletime qTAf0
			endfor
			
			#Save targets
			select TableOfReal targets
			nrows = Get number of rows
			Set value... nrows 1 m
			Set value... nrows 2 b
			Set value... nrows 3 lambda			
			Set value... nrows 4 d
			Set value... nrows 5 rmse
			Set value... nrows 6 corr
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
		endif
	endfor

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

	endfor
endproc

########## (Procedure) resynthesize with approximated F0	 ##########
procedure qtaResynthesis
	select Sound 'name$'
	To Manipulation... 0.01 75 600
	select PitchTier fittedf0sampled
	Formula... exp(self*ln(2)/12)
	plus Manipulation 'name$'
	Replace pitch tier
	select Manipulation 'name$'
	Get resynthesis (overlap-add)
	createDirectory: "'directory$'../audios"
	createDirectory: "'directory$'../audios/qta"
	nowarn Save as WAV file: "'directory$'../audios/qta/'name$'_qTA.wav"
	Remove
	select Manipulation 'name$'
	Remove
endproc
