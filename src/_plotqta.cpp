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
		std::cerr << "Usage: " << std::string(argv[0]) << " <PATH> <SYLLABLE-SHIFT>" << std::endl;
		return 1;
	}

	/*************** qta optimal plots ***************/
	// create a file-reading object for target-file
	std::ifstream finTargets;
	finTargets.open(std::string(argv[1]) + "../corpus-targets.csv"); // open input file
	if (!finTargets.good())
		std::cerr << "ERROR: no target file in given directory file not found! " << std::string(argv[1]) << std::endl;

	// read from target file
	int cnt (0);
	std::string line;
	std::getline(finTargets, line); // ingnore first line (header)
	while (std::getline(finTargets, line))
	{
		try
		{
			PlotFileGenerator qtaPlot (line, std::string(argv[1]), "qta", std::stod(argv[2]));
			qtaPlot.generate_plot_file_qta();
			++cnt;
			std::cout <<"\r\tnumber of generated plots (qta): " << cnt;
		}
		catch (Exception &exception)
		{
			std::cout << exception.what() << std::endl;
		}
	}
	std::cout << std::endl;

	/*************** svm prediction plots ***************/
	// create a file-reading object for target-file
	std::ifstream finSvmTargets;
	finSvmTargets.open(std::string(argv[1]) + "../svm-prediction/svm-targets.csv"); // open input file
	if (!finSvmTargets.good())
		std::cerr << "ERROR: no target file in given directory file not found! " << std::string(argv[1]) << std::endl;

	// read from target file
	int cntSvm (0);
	std::getline(finSvmTargets, line); // ingnore first line (header)
	while (std::getline(finSvmTargets, line))
	{
		try
		{
			PlotFileGenerator qtaPlot (line, std::string(argv[1]), "svm", std::stod(argv[2]));
			qtaPlot.generate_plot_file_svm();
			++cntSvm;
			std::cout <<"\r\tnumber of generated plots (svm): " << cntSvm;
		}
		catch (Exception &exception)
		{
			std::cout << exception.what() << std::endl;
		}
	}
	std::cout << std::endl;

	return 0;
}


