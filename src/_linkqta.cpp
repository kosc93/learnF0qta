/*
 * main.cpp
 *
 *  Created on: 3 Jun 2017
 *      Author: patrick
 */

#include <iostream>
#include <string>

#include "TrainingFileGenerator.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 5)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <PATH> <FEATURE-FILE> <TARGET-FILE> <OUTPUT-FILE>" << std::endl;
		std::cerr << "(Type in FILEs without path, just the name.)" << std::endl;
		return 1;
	}

	std::string path (argv[1]), featureFile (argv[2]), targetFile (argv[3]), trainingFile (argv[4]);
	TrainingFileGenerator readWrite (path, featureFile, targetFile, trainingFile);
	readWrite.print_statistics();

	return 0;
}


