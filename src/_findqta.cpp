/*
 * main.cpp
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <nlopt.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <exception>

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
		std::cerr << "Usage: " << argv[0] << " <DIRECTORY> " << std::endl;
		return 1;
	}
	if (argc > 1)
	{
		path = std::string(argv[1]);
	}

	/********** Setup Parameter Search **********/
	// create class objects
	SystemTA system;

	// file: read from input files and initialize system; TBD: include file path
	PraatFileIO praatFiles (system, path);

	// determine search bounds
	std::vector<double> searchBounds = system.get_search_bounds();
	std::vector<double> lowerBounds {searchBounds[0], searchBounds[2], searchBounds[4]}; // m_min,b_min,lambda_min
	std::vector<double> upperBounds {searchBounds[1], searchBounds[3], searchBounds[5]}; // m_max,b_max,lambda_max

	// global optimizer
	nlopt::opt globalOpt(nlopt::GN_ORIG_DIRECT_L, 3);
	globalOpt.set_min_objective(system.error_function, (SystemTA*) &system);
	globalOpt.set_xtol_rel(1e-4);
	globalOpt.set_lower_bounds(lowerBounds);
	globalOpt.set_upper_bounds(upperBounds);
	globalOpt.set_maxtime(5);

	// local optimizer
	nlopt::opt localOpt(nlopt::LN_BOBYQA, 3);  // LN_NELDERMEAD
	localOpt.set_min_objective(system.error_function, (SystemTA*) &system);
	localOpt.set_xtol_rel(1e-4);
	localOpt.set_lower_bounds(lowerBounds);
	localOpt.set_upper_bounds(upperBounds);

	// initialization
	double minf;
	std::vector<double> x = lowerBounds;

	/********** Non-linear Optimization *********/
	try
	{
		// global optimization over search space
		globalOpt.optimize(x, minf);
		localOpt.optimize(x, minf);

		// local optimization with random guesses
		std::vector<double> xtmp;
		double ftmp;
		for (int i=0; i<5; ++i)
		{
			xtmp = {searchBounds[0]+((double)rand()/RAND_MAX)*(searchBounds[1]-searchBounds[0]), searchBounds[2]+((double)rand()/RAND_MAX)*(searchBounds[3]-searchBounds[2]), searchBounds[4]+((double)rand()/RAND_MAX)*(searchBounds[5]-searchBounds[4])};
			localOpt.optimize(xtmp, ftmp);
			if (ftmp < minf)
			{
				minf = ftmp;
				x = xtmp;
			}
		}
	}
	catch(std::runtime_error &exception)
	{
		std::cerr << "ERROR: " << exception.what() << std::endl;;
	}

	/********** Compute Results **********/
	system.set_optimum(x);
	x = system.get_optimal_variables();	// converts b

	// file: output file
	praatFiles.generate_output_file(system);

	// DEBUG: print results
	// std::cout << "(findqta) f(m=" << x[0] << ", b=" << x[1] << ", l=" << x[2] << ") = " << minf << std::endl;

	return 0;
}


