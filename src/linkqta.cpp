/*
 * main.cpp
 *
 *  Created on: 3 Jun 2017
 *      Author: patrick
 */

#include <iostream>
#include <string>
#include "FileAnalyzer.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 4)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <FEATURE-FILE> <TARGET-FILE> <OUTPUT-FILE>" << std::endl;
		return 1;
	}

	std::string featureFile (argv[1]), targetFile (argv[2]), trainingFile (argv[3]);
	FileAnalyzer readWrite (featureFile, targetFile, trainingFile);
	readWrite.print_statistics();

	return 0;
}


