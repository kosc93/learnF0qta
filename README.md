# learnF0qta

## Description
learnF0qta is a software tool for learning qTA parameters of a speech-corpus for F0 prediction utilizing PENTAtrainer1. The main part of the software is a shell script calling different C++ programs and managing the file input and output. A modified version of [PENTAtrainer1](http://www.homepages.ucl.ac.uk/~uclyyix/PENTAtrainer1/) is used for extracting optimal qTA targets for F0-resythesis. Based on this data and the SAMPA transcription, a training file is generated which is used for the training of different machine learning methods. Additional features like generating plots and some statistics are also implemented. The whole software works full automatically without any user interaction or GUI.

## Installation
Use the installation script for locally installing the external libraries and starting the build process.
```
./install.sh
```
## Input Data
Put all the files of the annotated corpus in a directory e.g. called `corpus/`. Create a configuration xml file in the directory above `../corpus/` and name it for instance `config.xml`. An example xml configuration is given below.
```
<config>
	<path>/home/patrick/Documents/Diplomarbeit/03_Software/03_Entwicklung/data/corpus/</path>
	<praat>
		<systemorder>5</systemorder>
		<syllableshift>30</syllableshift>
		<slopemin>-110</slopemin>
		<slopemax>110</slopemax>
		<offsetmin>-60</offsetmin>
		<offsetmax>60</offsetmax>
		<strengthmin>1</strengthmin>
		<strengthmax>300</strengthmax>
	</praat>
	<optimization>
		<randominit>50</randominit>
		<algorithm>LN_NELDERMEAD</algorithm>
	</optimization>
</config>
```
Additionally, place the SAMPA transcription file in csv format at the same directory `../corpus/` and name it for instance `SAMPA.csv`. It also has to include the accent pattern. An example file content is shown below.
```
Word,SAMPA,Pattern
Stil,Sti:l,4
Stille,St"I.l@,41
Filet,fi.l"e:,24
Nation,na.tsi_^"o:n,24
Beet,be:t,4
Bett,bEt,4
Eleganz,e.le.g"ants,224
Gerät,g@.R"E:t,14
```
## Usage
Start the program by calling the shell-script with the path to the xml configuration file as command line parameter.
```
./find-training-qta.sh <config-file.xml>
```
