/*
 * Utilities.h
 *
 *  Created on: 14 June 2017
 *      Author: patrick
 */

#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <string>
#include <vector>

namespace utilities{

// string operations
void split(std::vector<std::string> &theStringVector, const std::string &theString, const std::string &theDelimiter);

// math operations
double binomial_coefficient	(const unsigned int &n, const unsigned int &k);
double factorial (unsigned int n);
void numerical_differentiation (const std::vector<double> &inputFunction, const double &stepSize, std::vector<double> &resultVector);

// statistic operations
double mean (const std::vector<double> &x);
double variance (const std::vector<double> &x);
double rmse (const std::vector<double> &x, const std::vector<double> &y); // root mean square error
double correlation (const std::vector<double> &x, const std::vector<double> &y); // Pearson correlation coefficient
}


#endif /* UTILITIES_H_ */
