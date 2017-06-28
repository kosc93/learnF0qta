START_TIME=$SECONDS

if [[ -r $1 ]]
then
	##### get parameters #####
	doSearch="$( xmlstarlet sel -t -v 'config/stages/parametersearch' "$1" )"
	doPlot="$( xmlstarlet sel -t -v 'config/stages/plotdata' "$1" )"
	doFile="$( xmlstarlet sel -t -v 'config/stages/trainingfile' "$1" )"
	doSVM="$( xmlstarlet sel -t -v 'config/stages/svmtraining' "$1" )"
	doKCV="$( xmlstarlet sel -t -v 'config/stages/kernelCV' "$1" )"

	mmin="$( xmlstarlet sel -t -v 'config/praat/slopemin' "$1" )"
	mmax="$( xmlstarlet sel -t -v 'config/praat/slopemax' "$1" )"
	bmin="$( xmlstarlet sel -t -v 'config/praat/offsetmin' "$1" )"
	bmax="$( xmlstarlet sel -t -v 'config/praat/offsetmax' "$1" )"
	lmin="$( xmlstarlet sel -t -v 'config/praat/strengthmin' "$1" )"
	lmax="$( xmlstarlet sel -t -v 'config/praat/strengthmax' "$1" )"
	N="$( xmlstarlet sel -t -v 'config/praat/systemorder' "$1" )"
	sshift="$( xmlstarlet sel -t -v 'config/praat/syllableshift' "$1" )"

	algo="$( xmlstarlet sel -t -v 'config/findqta/algorithm' "$1" )"
	iter="$( xmlstarlet sel -t -v 'config/findqta/randominit' "$1" )"

	eps="$( xmlstarlet sel -t -v 'config/svm/epsilon' "$1" )"
	kernel="$( xmlstarlet sel -t -v 'config/svm/kernel' "$1" )"

	Cslope="$( xmlstarlet sel -t -v 'config/svm/constantSlope' "$1" )"
	Gslope="$( xmlstarlet sel -t -v 'config/svm/gammaSlope' "$1" )"
	Coffset="$( xmlstarlet sel -t -v 'config/svm/constantOffset' "$1" )"
	Goffset="$( xmlstarlet sel -t -v 'config/svm/gammaOffset' "$1" )"
	Cstrength="$( xmlstarlet sel -t -v 'config/svm/constantStrength' "$1" )"
	Gstrength="$( xmlstarlet sel -t -v 'config/svm/gammaStrength' "$1" )"
	Cduration="$( xmlstarlet sel -t -v 'config/svm/constantDuration' "$1" )"
	Gduration="$( xmlstarlet sel -t -v 'config/svm/gammaDuration' "$1" )"

	##### input/output file #####
	path=$(dirname $1)
	filename=$(basename $1 .xml)
	output=$path/output.txt

	printf '\n>>> [xml] reading configuration file ... \n' | tee $output
	printf '\tslope\t [%s%s] st/sec\n' "$mmin,$mmax" | tee -a $output
	printf '\toffset\t [%s%s] st\n' "$bmin,$bmax" | tee -a $output
	printf '\tstrength [%s%s] 1/sec\n' "$lmin,$lmax" | tee -a $output
	printf '\torder\t %s\n' "$N" | tee -a $output
	printf '\tshift\t %s ms\n' "$sshift" | tee -a $output
	printf '\talgorithm\t %s \n' "$algo" | tee -a $output
	printf '\trandom-iter\t %s \n\n' "$iter" | tee -a $output

	if [ $doSearch = 1 ]
	then
		##### get optimal targets #####
		cp tools/_qtaParameterSearch.praat $path/corpus
		cp bin/findqta $path/corpus

		printf ">>> [praat] process annotated audio files ... \n" | tee -a $output
		tools/praat --run $path/corpus/_qtaParameterSearch.praat \"Compute qTA parameters\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [praat] calculate ensemble file ... \n" | tee -a $output
		tools/praat --run $path/corpus/_qtaParameterSearch.praat \"Assemble ensemble file\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [iconv] convert encoding of target ensemble file ... \n" | tee -a $output
		if [ "$(uchardet $path/corpus/targets.txt)" = "UTF-16" ]
		then
			iconv -f UTF-16 -t UTF-8 $path/corpus/targets.txt > tmp.csv
			cat tmp.csv > $path/data/TARGETS.csv
			rm tmp.csv
		else
			cp $path/corpus/targets.txt $path/data/TARGETS.csv
		fi

		rm $path/corpus/_qtaParameterSearch.praat
		rm $path/corpus/findqta
	fi

	if [ $doPlot = 1 ]
	then
		##### generate plots #####
		printf "\n>>> [plotqta] generating plots ... \n" | tee -a $output
		for i in $path/corpus/*.TextGrid; do
			if [ "$(uchardet $i)" = "UTF-16" ]
			then
				iconv -f UTF-16 -t UTF-8 $i > tmp.TextGrid
				cat tmp.TextGrid > $i
				rm tmp.TextGrid
			fi
		done
		bin/plotqta $path/data/TARGETS.csv $path/corpus/ $sshift

		##### remove unneccessary files #####
		rm $path/corpus/*.semitonef0; rm $path/corpus/*.qtaf0; rm $path/corpus/*.qtaf0sampled; rm $path/corpus/*.plot; rm $path/corpus/*.targets; rm $path/corpus/*.txt;
	fi

	if [ $doFile = 1 ]
	then
		##### create training data #####
		printf "\n>>> [sampa2vec] calculate feature vectors ... \n" | tee -a $output
		bin/sampa2vec -f $path/data/SAMPA.csv $path/data/FEATURES.csv | tee -a $output

		printf "\n>>> [linkqta] link features with targets and get statistics... \n" | tee -a $output
		bin/linkqta $path/data/ FEATURES.csv TARGETS.csv TRAINING.csv | tee -a $output
	fi

	if [ $doSVM = 1 ]
	then
		##### scale data + SVM training #####
		printf "\n>>> [svm-scale] scaling training data ... \n" | tee -a $output
		tools/svm-scale -l 0.0 -u 1.0 $path/svm/slope.training > $path/svm/slope.scaled
		tools/svm-scale -l 0.0 -u 1.0 $path/svm/offset.training > $path/svm/offset.scaled
		tools/svm-scale -l 0.0 -u 1.0 $path/svm/strength.training > $path/svm/strength.scaled
		tools/svm-scale -l 0.0 -u 1.0 $path/svm/duration.training > $path/svm/duration.scaled

		if [ $doKCV = 1 ]
		then
			printf "\n>>> [grid.py] determine optimal kernel parameters ... \n" | tee -a $output
			tmpP=$(python tools/grid.py -svmtrain tools/svm-train -gnuplot null $path/svm/slope.scaled)
			tmpP=${tmpP% *}; Gslope=${tmpP##* }; tmpP=${tmpP% *}; Cslope=${tmpP##*)}
			echo slope: C=$Cslope gamma=$Gslope | tee -a $output
			tmpP=$(python tools/grid.py -svmtrain tools/svm-train -gnuplot null $path/svm/offset.scaled)
			tmpP=${tmpP% *}; Goffset=${tmpP##* }; tmpP=${tmpP% *}; Coffset=${tmpP##*)}
			echo offset: C=$Coffset gamma=$Goffset | tee -a $output
			tmpP=$(python tools/grid.py -svmtrain tools/svm-train -gnuplot null $path/svm/strength.scaled)
			tmpP=${tmpP% *}; Gstrength=${tmpP##* }; tmpP=${tmpP% *}; Cstrength=${tmpP##*)}
			echo strength: C=$Cstrength gamma=$Gstrength | tee -a $output
			tmpP=$(python tools/grid.py -svmtrain tools/svm-train -gnuplot null $path/svm/duration.scaled)
			tmpP=${tmpP% *}; Gduration=${tmpP##* }; tmpP=${tmpP% *}; Cduration=${tmpP##*)}
			echo duration: C=$Cduration gamma=$Gduration | tee -a $output
		fi

		printf "\n>>> [svm-train] train support vector regression ... \n" | tee -a $output
		printf "slope: " | tee -a $output
		tools/svm-train -s 3 -p $eps -t $kernel -c $Cslope -g $Gslope $path/svm/slope.scaled $path/svm/slope.model | tee -a $output
		printf "offset: " | tee -a $output
		tools/svm-train -s 3 -p $eps -t $kernel -c $Coffset -g $Goffset $path/svm/offset.scaled $path/svm/offset.model | tee -a $output
		printf "strength: " | tee -a $output
		tools/svm-train -s 3 -p $eps -t $kernel -c $Cstrength -g $Gstrength $path/svm/strength.scaled $path/svm/strength.model | tee -a $output
		printf "duration: " | tee -a $output
		tools/svm-train -s 3 -p $eps -t $kernel -c $Cduration -g $Gduration $path/svm/duration.scaled $path/svm/duration.model | tee -a $output

		printf "\n>>> [svm-predict] predict TA parameters ... \n" | tee -a $output
		printf "slope: " | tee -a $output
		tools/svm-predict $path/svm/slope.scaled $path/svm/slope.model $path/svm/slope.output | tee -a $output
		printf "offset: " | tee -a $output
		tools/svm-predict $path/svm/offset.scaled $path/svm/offset.model $path/svm/offset.output | tee -a $output
		printf "strength: " | tee -a $output
		tools/svm-predict $path/svm/strength.scaled $path/svm/strength.model $path/svm/strength.output | tee -a $output
		printf "duration: " | tee -a $output
		tools/svm-predict $path/svm/duration.scaled $path/svm/duration.model $path/svm/duration.output | tee -a $output
	fi

	ELAPSED_TIME=$(($SECONDS - $START_TIME))
	echo 
	echo ">>> $(($ELAPSED_TIME/60)) min $(($ELAPSED_TIME%60)) sec <<<" | tee -a $output

	exit 0
else
	echo 'Wrong config specified:' $1 | tee -a $output
    echo 'Usage: ./find-training-qta.sh <config-file.xml>' | tee -a $output
    exit 1
fi


