/*
 * main.cpp
 *
 *  Created on: 29 Jun 2017
 *      Author: patrick
 */

#include <iostream>
#include <string>

#include "SVMTrainer.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 4)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <TRAINING-FILE> <TEST-FILE> <OUTPUT-PATH>" << std::endl;
		return 1;
	}

	std::string trainingFile (argv[1]), testFile (argv[2]), outpath (argv[3]);
	SVMTrainer svmRegressor (trainingFile, outpath);
	svmRegressor.predict(testFile);
	return 0;
}
