# learnF0qta

## Description

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
...
```
## Usage
```
./find-training-qta.sh <config-file.xml>
```
