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
	cp tools/_PENTAtrainer1N.praat $path
	cp bin/findqta $path

	printf ">>> [praat] process annotated audio files ... \n" | tee -a $output
	tools/praat --run $path/_PENTAtrainer1N.praat 1 \"label\" \"Process all sounds without pause\" 100 600 10 100 0 -0.03 0.07 \"yes\" $mmin $mmax $bmin $bmax $lmin $lmax 150 $N \"no\" \"qTA_synthesis\" \"sil\" \"yes\" \"no\" no $sshift $algo $iter

	printf "\n\n>>> [praat] generate ensemble files ... \n" | tee -a $output
	tools/praat --run $path/_PENTAtrainer1N.praat 1 \"label\" \"Get emsemble files\" 100 600 10 100 0 -0.03 0.07 \"yes\" $mmin $mmax $bmin $bmax $lmin $lmax 150 $N \"no\" \"qTA_synthesis\" \"sil\" \"yes\" \"no\" no $sshift $algo $iter | tee -a $output

	printf "\n>>> [iconv] convert encoding of target ensemble file ... \n" | tee -a $output
	iconv -f UTF-16 -t UTF-8 $path/targets.txt > $path/../TARGETS.csv | tee -a $output

	printf "\n>>> [plotqta] generating plots ... \n" | tee -a $output
	bin/plotqta $path/../TARGETS.csv $path/

	rm $path/_PENTAtrainer1N.praat
	rm $path/findqta

	##### create training data #####
	printf "\n>>> [sampa2vec] calculate feature vectors ... \n" | tee -a $output
	bin/sampa2vec -f $path/../SAMPA.csv $path/../FEATURES.csv | tee -a $output

	printf "\n>>> [linkqta] link features with targets and get statistics... \n" | tee -a $output
	bin/linkqta $path/../FEATURES.csv $path/../TARGETS.csv $path/../TRAINING.csv | tee -a $output

	##### remove unneccessary files #####
	printf "\n>>> [rm] remove unneccessary files ... \n" | tee -a $output
	rm $path/*.actutimenormf0; rm $path/*.f0; rm $path/*.f0velocity; rm $path/*.means; rm $path/*.normtimef0; rm $path/*.PitchTier; rm $path/*.PitchTier_semitone; rm $path/*.plot; rm $path/*.primitive_code; rm $path/*.qTAf0; rm $path/*.qTAvelocity; rm $path/*.rawf0; rm $path/*.samplef0; rm $path/*.semitonef0; rm $path/*.smoothf0; rm $path/*.target

	ELAPSED_TIME=$(($SECONDS - $START_TIME))
	echo 
	echo ">>> $(($ELAPSED_TIME/60)) min $(($ELAPSED_TIME%60)) sec <<<" | tee -a $output

	exit 0
else
	echo 'Wrong config specified:' $1 | tee -a $output
    echo 'Usage: ./find-training-qta.sh <config-file.xml>' | tee -a $output
    exit 1
fi


