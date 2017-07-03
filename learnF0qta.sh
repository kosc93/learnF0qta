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
		cp tools/_qtaSearch.praat $path/corpus
		cp bin/findqta $path/corpus

		printf ">>> [praat] process annotated audio files ... \n" | tee -a $output
		tools/praat --run $path/corpus/_qtaSearch.praat \"Compute qTA parameters\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [praat] calculate ensemble file ... \n" | tee -a $output
		tools/praat --run $path/corpus/_qtaSearch.praat \"Assemble ensemble file\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [iconv] convert encoding of target ensemble file ... \n" | tee -a $output
		if [ "$(uchardet $path/corpus/targets.txt)" = "UTF-16" ]
		then
			iconv -f UTF-16 -t UTF-8 $path/corpus/targets.txt > tmp.csv
			cat tmp.csv > $path/data/TARGETS.csv
			rm tmp.csv
		else
			cp $path/corpus/targets.txt $path/data/TARGETS.csv
		fi

		rm $path/corpus/_qtaSearch.praat
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
		#rm $path/corpus/*.semitonef0; rm $path/corpus/*.qtaf0; rm $path/corpus/*.qtaf0sampled; rm $path/corpus/*.plot; rm $path/corpus/*.targets; rm $path/corpus/*.txt;
	fi

	if [ $doFile = 1 ]
	then
		##### create training data #####
		printf "\n>>> [sampa2vec] calculate feature vectors ... \n" | tee -a $output
		bin/sampa2vec -f $path/data/SAMPA.csv $path/data/FEATURES.csv | tee -a $output

		printf "\n>>> [linkqta] link features with targets, scale data and get statistics... \n" | tee -a $output
		bin/linkqta $path/data/ FEATURES.csv TARGETS.csv TRAINING.csv | tee -a $output
	fi

	if [ $doSVM = 1 ]
	then
		##### SVM training + prediction #####
		printf "\n>>> [trainsvm] train support vector regression and predict test samples ... \n" | tee -a $output
		bin/trainsvm $path/data/ corpus.training corpus.test
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


