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

	if [ $doSearch = 1 ]
	then
		##### get optimal targets #####
		cp tools/_qtaSearch.praat $path/corpus
		cp bin/findqta $path/corpus

		printf ">>> [praat] process annotated audio files ... \n"
		tools/praat --run $path/corpus/_qtaSearch.praat \"Compute qTA parameters\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [praat] calculate ensemble file ... \n"
		tools/praat --run $path/corpus/_qtaSearch.praat \"Assemble ensemble file\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $sshift $N $algo $iter

		printf "\n>>> [iconv] convert encoding of target ensemble file ... \n"
		if [ "$(uchardet $path/corpus/targets.txt)" = "UTF-16" ]
		then
			iconv -f UTF-16 -t UTF-8 $path/corpus/targets.txt > targets.txt
			cat targets.txt > $path/data/corpus-targets.csv
			rm $path/corpus/targets.txt targets.txt
		else
			cp $path/corpus/targets.txt $path/data/corpus-targets.csv
		fi

		rm $path/corpus/_qtaSearch.praat $path/corpus/findqta
	fi

	if [ $doFile = 1 ]
	then
		##### create training data #####
		printf "\n>>> [sampa2vec] calculate feature vectors ... \n"
		bin/sampa2vec -f $path/data/corpus-sampa.csv $path/data/corpus-features.csv

		printf "\n>>> [linkqta] link features with targets, scale data and get statistics... \n"
		bin/linkqta $path/data/corpus-features.csv $path/data/corpus-targets.csv $path/data/
	fi

	if [ $doSVM = 1 ]
	then
		##### SVM training + prediction #####
		printf "\n>>> [trainsvm] train support vector regression and predict test samples ... \n"
		bin/trainsvm $path/data/samples-training.csv $path/data/samples-test.csv $path/data/

		printf "\n>>> [praat] resythesize audio files with predicted qta parameters ... \n"
		cp tools/_qtaPredict.praat $path/corpus
		cp bin/predictqta $path/corpus

		tools/praat --run $path/corpus/_qtaPredict.praat \"Predict F0\" 100 600 "../data/svm-prediction/samples-predict.csv" "svm" $sshift $N
		tools/praat --run $path/corpus/_qtaPredict.praat \"Assemble ensemble file\" 100 600 "../data/svm-prediction/samples-predict.csv" "svm" $sshift $N

		if [ "$(uchardet $path/corpus/targets.txt)" = "UTF-16" ]
		then
			iconv -f UTF-16 -t UTF-8 $path/corpus/targets.txt > targets.txt
			cat targets.txt > $path/data/svm-prediction/svm-targets.csv
			rm $path/corpus/targets.txt targets.txt
		else
			cp $path/corpus/targets.txt $path/data/svm-prediction/svm-targets.csv
		fi

		rm $path/corpus/_qtaPredict.praat $path/corpus/predictqta
	fi

	if [ $doPlot = 1 ]
	then
		##### generate plots #####
		printf "\n>>> [plotqta] generating plots ... \n"
		for i in $path/corpus/*.TextGrid; do
			if [ "$(uchardet $i)" = "UTF-16" ]
			then
				iconv -f UTF-16 -t UTF-8 $i > tmp.TextGrid
				cat tmp.TextGrid > $i
				rm tmp.TextGrid
			fi
		done
		bin/plotqta $path/data/plot-files/ $sshift
	fi

	ELAPSED_TIME=$(($SECONDS - $START_TIME))
	echo 
	echo 
	echo ">>> $(($ELAPSED_TIME/60)) min $(($ELAPSED_TIME%60)) sec <<<"

	exit 0
else
	echo 'Wrong config specified:' $1
    echo 'Usage: ./find-training-qta.sh <config-file.xml>'
    exit 1
fi


