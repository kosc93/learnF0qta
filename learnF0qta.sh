START_TIME=$SECONDS

if [[ -r $1 ]]
then
	make all

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

	echo '>> Using the following configuration:'
	printf '\tslope\t [%s%s] st/sec\n' "$mmin,$mmax"
	printf '\toffset\t [%s%s] st\n' "$bmin,$bmax"
	printf '\tstrength [%s%s] 1/sec\n' "$lmin,$lmax"
	printf '\torder\t %s\n' "$N"
	printf '\tshift\t %s ms\n\n' "$sshift"

	##### get targets and generate plots #####
	cp tools/_PENTAtrainer1N.praat $path
	cp bin/findqta $path

	printf ">> Process Audio Files ... \n"
	tools/praat --run $path/_PENTAtrainer1N.praat 1 \"label\" \"Process all sounds without pause\" 100 600 10 100 0 -0.03 0.07 \"yes\" $mmin $mmax $bmin $bmax $lmin $lmax 150 $N \"no\" \"qTA_synthesis\" \"sil\" \"yes\" \"no\" no

	printf "\n>> Generate Ensemble File ... \n"
	tools/praat --run $path/_PENTAtrainer1N.praat 1 \"label\" \"Get emsemble files\" 100 600 10 100 0 -0.03 0.07 \"yes\" $mmin $mmax $bmin $bmax $lmin $lmax 150 $N \"no\" \"qTA_synthesis\" \"sil\" \"yes\" \"no\" no

	printf "\n>> Change Encoding of Target Ensemble File ... \n"
	iconv -f UTF-16 -t UTF-8 $path/targets.txt > $path/../TARGETS.csv

	printf "\n>> Generating Plots ... \n"
	bin/plotqta $path/../TARGETS.csv $path/

	rm $path/_PENTAtrainer1N.praat
	rm $path/findqta

	##### create training data #####
	printf "\n>> Calculate Feature Vectors ... \n"
	bin/sampa2vec -f $path/../SAMPA.csv $path/../FEATURES.csv

	printf "\n>> Link Features with Targets and Get Statistics... \n"
	bin/linkqta $path/../FEATURES.csv $path/../TARGETS.csv $path/../TRAINING.csv

	ELAPSED_TIME=$(($SECONDS - $START_TIME))
	echo "$(($ELAPSED_TIME/60)) min $(($ELAPSED_TIME%60)) sec"

	exit 0
else
	echo 'Wrong config specified:' $1
    echo 'Usage: ./find-training-qta.sh <config-file.xml>'
    exit 1
fi


