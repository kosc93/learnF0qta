/*
 * main.cpp
 *
 *  Created on: 30 June 2017
 *      Author: patrick
 */

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

#include "SystemTA.h"
#include "PraatFileIO.h"

int main(int argc, char* argv[])
{
	/********** Command Line Arguments **********/
	std::string path ("./");

	// determine command line arguments
	if (argc > 2)
	{
		std::cerr << "ERROR: wrong command line arguments" << std::endl;
		std::cerr << "Usage: " << argv[0] << " <DIRECTORY>" << std::endl;
		return 1;
	}
	else if (argc > 1)
	{
		path = std::string(argv[1]);
	}

	/********** Setup Parameter Search **********/
	// create class objects
	SystemTA system;

	// file: read from input files and initialize system; TBD: include file path
	PraatFileIO praatFiles (system, path);

	/********** Compute Results **********/
	std::vector<double> searchBounds = system.get_search_bounds();
	std::vector<double> xPredicted = {searchBounds[0],searchBounds[2],searchBounds[4]}; // HACK -> change!!!
	system.set_optimum(xPredicted);

	// file: output file
	praatFiles.generate_output_file(system);

	// DEBUG: print results
	// std::cout << "(findqta) f(m=" << x[0] << ", b=" << x[1] << ", l=" << x[2] << ") = " << minf << std::endl;
}
