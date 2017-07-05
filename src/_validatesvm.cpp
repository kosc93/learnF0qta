/*
 * main.cpp
 *
 *  Created on: 05 July 2017
 *      Author: patrick
 */

#include <iostream>
#include <string>
#include <cmath>

#include "SVMValidater.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 2)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <SAMPLE-FILE>" << std::endl;
		return 1;
	}

	// optimal results
	std::vector<double> mOpt = {0.0,0.0,0.0};
	std::vector<double> bOpt = {0.0,0.0,0.0};
	std::vector<double> lOpt = {0.0,0.0,0.0};
	std::vector<double> dOpt = {0.0,0.0,0.0};

	std::vector<double> fmin = {1e3,1e3,1e3,1e3};

	// grid search
	int cnt (0);
	std::string sampleFile (argv[1]);

	#pragma omp parallel for schedule (dynamic) collapse(3)
	for (int expC = -5; expC <= 15; ++expC)
	{
		for (int expG = -15; expG <= 3; ++expG)
		{
			for (int expE = -5; expE <= 5; ++expE)
			{
				double coefficient = std::pow(2.0,(double)expC);
				double gamma = std::pow(2.0,(double)expG);
				double epsilon = std::pow(2.0,(double)expE);

				SVMValidater svmCrossValidater (sampleFile);
				svmCrossValidater.set_parameters(coefficient,gamma,epsilon);
				std::vector<double> cvrmse = svmCrossValidater.do_cross_validation();

				if (cvrmse[0] < fmin[0])
				{
					mOpt = {coefficient,gamma,epsilon};
					fmin[0] = cvrmse[0];
				}
				if (cvrmse[1] < fmin[1])
				{
					bOpt = {coefficient,gamma,epsilon};
					fmin[1] = cvrmse[1];
				}
				if (cvrmse[2] < fmin[2])
				{
					lOpt = {coefficient,gamma,epsilon};
					fmin[2] = cvrmse[2];
				}
				if (cvrmse[3] < fmin[3])
				{
					dOpt = {coefficient,gamma,epsilon};
					fmin[3] = cvrmse[3];
				}

				++cnt;
				std::cout << ">> " << cnt << " << (" << expC << "," << expG << "," << expE << ")" << std::endl;
			}
		}
	}

	std::cout << "\t\tC\tgam\teps" << std::endl;
	std::cout << "slope\t\t" << mOpt[0] << "\t" << mOpt[1] << "\t" << mOpt[2] << std::endl;
	std::cout << "offset\t\t" << bOpt[0] << "\t" << bOpt[1] << "\t" << bOpt[2] << std::endl;
	std::cout << "strength\t" << lOpt[0] << "\t" << lOpt[1] << "\t" << lOpt[2] << std::endl;
	std::cout << "duration\t" << dOpt[0] << "\t" << dOpt[1] << "\t" << dOpt[2] << std::endl;

	return 0;
}
