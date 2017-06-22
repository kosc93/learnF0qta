## Description
learnF0qta is a software tool for learning qTA parameters of a speech-corpus for F0 prediction utilizing PENTAtrainer1. The main part of the software is a shell script calling different C++ programs and managing the file input and output. A modified version of [PENTAtrainer1](http://www.homepages.ucl.ac.uk/~uclyyix/PENTAtrainer1/) is used for extracting optimal qTA targets for F0-resythesis. Based on this data and the SAMPA transcription, a training file is generated which is used for the training of different machine learning methods. Additional features like generating plots and some statistics are also implemented. The whole software works full automatically without any user interaction or GUI.

![overview](overview.png?raw=true | width=70)

## Requirements
The software is developed on a 64-bit Linux system with Ubuntu 16.04 LTS. The `g++` compiler with `c++11` support is required. Only the Praat binary is delivered, the other code is build natively by the installation script.
Most tools (Praat, PENTAtrainer, NLopt, LIBSVM, etc.) are delivered with the source code and are compiled and installed in the local directory. Additionally, some other software is neccessary in order to run the code which is listed below. 

`gnuplot`: Is used for generating plots from different data.
```
sudo apt-get install gnuplot
```
`xmlstarlet`: Is a small command line tool for analyzing xml files which is used for configuration files.
```
sudo apt-get install xmlstarlet
```
Additionally, the following command line tools which are pre-installed on every Ubuntu system are usedin the shell script.

`iconv`: Converts the encoding of the target file generated from the praat script to the systems encoding.

`baseline`: extract filename from a given path

`tee`: redirect output to a file AND stdout

`cp`: copy file

`mkdir`: create a directory

`printf`: write to stdout
## Installation
Use the installation script for locally installing the external libraries and starting the build process.
```
./install.sh
```
## Input Data
Put all the files of the annotated corpus in a directory e.g. called `corpus/`. Create a configuration xml file in the directory above `../corpus/` and name it for instance `config.xml`. An example xml configuration is given below.
```
<config>
	<path>/home/patrick/Documents/Diplomarbeit/03_Software/03_Entwicklung/data/corpus</path>
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
		<randominit>10</randominit>
		<algorithm>34</algorithm>
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
A complete example is included in the `test` directory and can be executed by `make test`.
## Usage
Start the program by calling the shell-script with the path to the xml configuration file as command line parameter.
```
./find-training-qta.sh <config-file.xml>
```
All the calculated results are found in the parent directory of the defined path to corpus in the xml config file. Additionally, a directory `plots` is created containing a plot for each original file which looks as the following:

![qTA resynthesis](example.png?raw=true)
 
## Miscellaneous
If there are words with 'Umlaut' (ä,ö,ü), the target file returned from the Praat script will be encoded with UTF-16. Otherwise the encoding is UTF-8. `learnF0qta` uses the command line tool `iconv` for encoding conversion. The user should check the correct encoding for the used system. 
