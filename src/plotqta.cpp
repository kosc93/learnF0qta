/*
 * main.cpp
 *
 *  Created on: 3 Jun 2017
 *      Author: patrick
 */

#include <iostream>
#include <string>
#include <fstream>
#include "PlotFileGenerator.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 3)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <TARGET-FILE> <PATH>" << std::endl;
		return 1;
	}

	// create a file-reading object for target-file
	std::ifstream finTargets;
	finTargets.open(std::string(argv[1])); // open input file
	if (!finTargets.good())
		std::cerr << "ERROR: target-input file not found! " << std::string(argv[1]) << std::endl;

	// read from target file
	int cnt (0);
	std::string line;
	std::getline(finTargets, line); // ingnore first line (header)
	while (std::getline(finTargets, line))
	{
		try
		{
			PlotFileGenerator(line, std::string(argv[2]));
			++cnt;
			std::cout <<"\r(plotqta) number of generated plots: " << cnt;
		}
		catch (Exception &exception)
		{
			;//std::cout << exception.what() << std::endl;
		}
	}
	std::cout << std::endl;

	return 0;
}


