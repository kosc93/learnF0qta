START_TIME=$SECONDS

if [[ -r $1 ]]
then
	##### get parameters #####
	path="$( xmlstarlet sel -t -v 'config/path' "$1" )"
	mmin="$( xmlstarlet sel -t -v 'config/praat/slopemin' "$1" )"
	mmax="$( xmlstarlet sel -t -v 'config/praat/slopemax' "$1" )"
	bmin="$( xmlstarlet sel -t -v 'config/praat/offsetmin' "$1" )"
	bmax="$( xmlstarlet sel -t -v 'config/praat/offsetmax' "$1" )"
	lmin="$( xmlstarlet sel -t -v 'config/praat/strengthmin' "$1" )"
	lmax="$( xmlstarlet sel -t -v 'config/praat/strengthmax' "$1" )"
	N="$( xmlstarlet sel -t -v 'config/praat/systemorder' "$1" )"
	sshift="$( xmlstarlet sel -t -v 'config/praat/syllableshift' "$1" )"

	algo="$( xmlstarlet sel -t -v 'config/optimization/algorithm' "$1" )"
	iter="$( xmlstarlet sel -t -v 'config/optimization/randominit' "$1" )"

	##### define output file #####
	filename=$(basename $1 .xml)
	output=$path/../$filename.results

	printf '\n>>> [xml] reading configuration file ... \n' | tee $output
	printf '\tslope\t [%s%s] st/sec\n' "$mmin,$mmax" | tee -a $output
	printf '\toffset\t [%s%s] st\n' "$bmin,$bmax" | tee -a $output
	printf '\tstrength [%s%s] 1/sec\n' "$lmin,$lmax" | tee -a $output
	printf '\torder\t %s\n' "$N" | tee -a $output
	printf '\tshift\t %s ms\n' "$sshift" | tee -a $output
	printf '\talgorithm\t %s \n' "$algo" | tee -a $output
	printf '\trandom-iter\t %s \n\n' "$iter" | tee -a $output

	##### get targets and generate plots #####
	cp tools/_qtaParameterSearch.praat $path
	cp bin/findqta $path

	printf ">>> [praat] process annotated audio files ... \n" | tee -a $output
	tools/praat --run $path/_qtaParameterSearch.praat \"Compute qTA parameters\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $N $algo $iter

	printf "\n>>> [praat] calculate ensemble file ... \n" | tee -a $output
	tools/praat --run $path/_qtaParameterSearch.praat \"Assemble ensemble file\" 100 600 $mmin $mmax $bmin $bmax $lmin $lmax $N $algo $iter

	printf "\n>>> [iconv] convert encoding of target ensemble file ... \n" | tee -a $output
	if [ "$(uchardet $path/targets.txt)" = "UTF-16" ]
	then
		iconv -f UTF-16 -t UTF-8 $path/targets.txt > tmp.csv
		cat tmp.csv > $path/../TARGETS.csv
		rm tmp.csv
	else
		cp $path/targets.txt $path/../TARGETS.csv
	fi

	printf "\n>>> [plotqta] generating plots ... \n" | tee -a $output
	for i in $path/*.TextGrid; do
    	if [ "$(uchardet $i)" = "UTF-16" ]
		then
			iconv -f UTF-16 -t UTF-8 $i > tmp.TextGrid
			cat tmp.TextGrid > $i
			rm tmp.TextGrid
		fi
	done
	bin/plotqta $path/../TARGETS.csv $path/

	rm $path/_qtaParameterSearch.praat
	rm $path/findqta

	##### create training data #####
	printf "\n>>> [sampa2vec] calculate feature vectors ... \n" | tee -a $output
	bin/sampa2vec -f $path/../SAMPA.csv $path/../FEATURES.csv | tee -a $output

	printf "\n>>> [linkqta] link features with targets and get statistics... \n" | tee -a $output
	bin/linkqta $path/../FEATURES.csv $path/../TARGETS.csv $path/../TRAINING.csv | tee -a $output

	##### remove unneccessary files #####
	rm $path/*.semitonef0; rm $path/*.qtaf0; rm $path/*.qtaf0sampled; rm $path/*.plot; rm $path/*.targets; rm $path/*.txt;

	ELAPSED_TIME=$(($SECONDS - $START_TIME))
	echo 
	echo ">>> $(($ELAPSED_TIME/60)) min $(($ELAPSED_TIME%60)) sec <<<" | tee -a $output

	exit 0
else
	echo 'Wrong config specified:' $1 | tee -a $output
    echo 'Usage: ./find-training-qta.sh <config-file.xml>' | tee -a $output
    exit 1
fi


