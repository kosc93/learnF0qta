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
		std::cerr << "Usage: " << std::string(argv[0]) << "<PATH> <TRAINING-FILE> <TEST-FILE>" << std::endl;
		return 1;
	}

	std::string path (argv[1]), trainingFile (argv[2]), testFile (argv[3]);
	SVMTrainer svmRegressor (path, trainingFile);
	svmRegressor.predict(testFile);
	return 0;
}
