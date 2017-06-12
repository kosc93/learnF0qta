# learnF0qta

## Description

## Installation
Use the installation script for locally installing the external libraries and starting the build process.
```
./install.sh
```
## Input Data
An example xml configuration is given below.
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
		<randominit>50</randominit>
		<algorithm>LN_NELDERMEAD</algorithm>
	</optimization>
</config>
```

## Usage
```
./find-training-qta.sh <config-file.xml>
```
