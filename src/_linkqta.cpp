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
	if (argc != 4)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <FEATURE-FILE> <TARGET-FILE> <OUTPUT-PATH>" << std::endl;
		return 1;
	}

	std::string featureFile (argv[1]), targetFile (argv[2]), outpath (argv[3]);
	TrainingFileGenerator readWrite (featureFile, targetFile, outpath);
	readWrite.print_statistics();

	return 0;
}


