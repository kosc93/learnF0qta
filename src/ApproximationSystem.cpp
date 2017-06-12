/*
 * ApproximationSystem.cpp
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <iostream>
#include <vector>
#include <cmath>
#include "ApproximationSystem.h"

// calculate binomial coefficient C(n, k)
double ApproximationSystem::binomial_coefficient(int n, int k)
{
    double result = 1;

    if ( k > n - k )
        k = n - k;

    for (int i = 0; i < k; ++i)
    {
    	result *= (n - i);
    	result /= (i + 1);
    }

    return result;
}

// calculate root mean square error between two vectors
double ApproximationSystem::root_mean_square_error	(std::vector<double> originalF0, std::vector<double> synthesizedF0)
{
	double result = 0.0;
	unsigned int numberSamples = originalF0.size();

	for (unsigned int i=0; i<numberSamples; ++i)
	{
		result += (std::pow(synthesizedF0[i] - originalF0[i],2));
	}

	result = std::sqrt(result/(double)numberSamples);
	return result;
}

// calculate correlation coefficient between two vectors
double ApproximationSystem::correlation_coefficient	(std::vector<double> originalF0, std::vector<double> synthesizedF0)
{
	double result = 0.0;
	unsigned int numberSamples = originalF0.size();

	// get means
	double originalF0Mean = 0.0, sythezisedF0Mean = 0.0;
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		originalF0Mean += originalF0[i];
		sythezisedF0Mean += synthesizedF0[i];
	}
	originalF0Mean /= numberSamples;
	sythezisedF0Mean /= numberSamples;

	// get variance
	double originalF0Var = 0.0, sythezisedF0Var = 0.0;
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		originalF0Var += std::pow((originalF0[i] - originalF0Mean),2);
		sythezisedF0Var += std::pow((synthesizedF0[i] - sythezisedF0Mean),2);
	}

	// calculate correlation coefficient
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		result += ((originalF0[i] - originalF0Mean)*(synthesizedF0[i] - sythezisedF0Mean));
	}

	return result/(std::sqrt(originalF0Var*sythezisedF0Var));
}

std::vector<double>  ApproximationSystem::differentiation (std::vector<double> function, double stepSize)
{
	std::vector<double> abbreveation;
	for (unsigned int i = 1; i < function.size(); ++i)
	{
		abbreveation.push_back((function[i] - function[i-1])/stepSize);
	}

	return abbreveation;
}

// calculate system coefficients
std::vector<double> ApproximationSystem::system_coefficients(unsigned int modelOrder, std::vector<double> initialAbbrev, std::vector<double> variables)
{
	// variables
	const double& slope = variables[0];
	const double& offset = variables[1];
	const double& strength = variables[2];

	// output
	std::vector<double> result (modelOrder, 0.0);

	result[0] = initialAbbrev[0] - offset;
	for (unsigned int n=1; n<modelOrder; ++n)
	{
		double tmp = 0.0;
		for (unsigned int j=0; j<=n-1; ++j)
		{
			tmp += ( result[j]*std::pow((-1)*strength,n-j) * binomial_coefficient(n,j) * factorial(j) );
		}

		if (n == 1)
		{
			tmp += slope;	// minus changes in following term!
		}

		result[n] = (initialAbbrev[n] - tmp)/factorial(n);
	}

	return result;
}

// calculate system answer of Nth order system at time t with coefficients c parameters x
std::vector<double> ApproximationSystem::system_answer (std::vector<double> variables, ApproximationSystem *system)
{
	// get parameters
	unsigned int 		modelOrder 		= system->get_model_order();
	unsigned int 		numberSamples 	= system->get_number_samples();
	std::vector<double> samplePoints 	= system->get_sample_points();
	std::vector<double> originalF0 		= system->get_original_F0();
	std::vector<double> initialAbbrev 	= system->get_initial_abbreviatives();

	// modify variables
	const double slope = variables[0];
	const double offset = originalF0[numberSamples-1] - variables[0] * (samplePoints[numberSamples-1] - samplePoints[0]) + variables[1];
	const double strength = variables[2];

	// output
	std::vector<double> result (numberSamples, 0.0);

	// calculate system coefficients
	std::vector<double> coefficients = system_coefficients(modelOrder, initialAbbrev, {slope, offset, strength});

	// calculate system answer for every sample point
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		// note time shift
		double sampleTime = samplePoints[i]-samplePoints[0];
		for (unsigned int n=0; n<modelOrder; ++n)
		{
			result[i] += (coefficients[n] * std::pow(sampleTime,n));
		}

		result[i] *= std::exp((-1)*sampleTime*strength);
		result[i] += (offset + slope*sampleTime);
	}

	return result;
}

// calculate SSE error function depending on parameters (M,N,Fn,ti,F0i) and variables (m,b,lambda)
double ApproximationSystem::error_function(const std::vector<double> &variables, std::vector<double> &grad, void* f_data)
{
	// output
	double result = 0.0;

	// get parameters
	ApproximationSystem *system = (ApproximationSystem*) f_data;

	// get system answer and original
	std::vector<double> synthesizedF0 = system_answer(variables, system);

	// calculate error function (SSE criteria); consider time shift
	for (unsigned int i=0; i<system->get_number_samples(); ++i)
	{
		result += std::pow(system->get_original_F0()[i] - synthesizedF0[i],2);
	}

	return result;	// logarithmic error criteria, cross entropy?
}

// set optimal variables to system and calculate final state and resulting F0 approximation
void ApproximationSystem::set_optimum (std::vector<double> optVariables)
{
	// optimal values m,b,lambda
	m_optVariables = optVariables;
	m_optVariables[1] = m_originalF0[m_numberSamples-1] - m_optVariables[0] * (m_samplePoints[m_numberSamples-1] - m_samplePoints[0]) + m_optVariables[1];

	// synthesized F0 with optimal parameters
	system_answer_opt();

	// final state (derivatives); note time shift
	system_derivatives_opt();

	// root mean square error
	m_rmse = root_mean_square_error(m_originalF0, m_sythesizedF0);

	//correlation coefficient
	m_rho = correlation_coefficient(m_originalF0, m_sythesizedF0);
}

// calculate derivative of system at end state (last sample point)
void ApproximationSystem::system_derivatives_opt()
{
	// sample time
	double sampleTime = m_samplePoints[m_numberSamples-1]-m_samplePoints[0];

	// output
	std::vector<double> result (m_modelOrder, 0.0);

	// calculate coefficients
	std::vector<double> coefficients = system_coefficients(m_modelOrder, m_initialAbbrev, m_optVariables);

	for (unsigned int n=0; n<m_modelOrder; ++n)
	{
		double tmp = 0.0;

		// calculate value of nth derivative
		for (unsigned int j=0; j<=m_modelOrder-1; ++j)
		{
			// pre-calculate q
			double q = 0.0;
			for (unsigned int k=0; k<=std::min(m_modelOrder-1,n); ++k)
			{
				q += (std::pow((-1)*m_optVariables[2], n-k) * binomial_coefficient(n,k) * coefficients[j+k]) * factorial(k+j)/factorial(j);
			}

			tmp += (std::pow(sampleTime, j) * q);
		}

		tmp *= (std::exp((-1)*m_optVariables[2]*sampleTime));
		result[n] = tmp;
	}

	if (m_modelOrder > 1)
		result[0] += (m_optVariables[1] + m_optVariables[0]*sampleTime);
	if (m_modelOrder > 2)
		result[1] += m_optVariables[0];

	m_finalAbbrev = result;
}

// calculate derivative of system at end state (last sample point) with numeric differentiation
void ApproximationSystem::system_derivatives_opt_num()
{
	// sample time
	double sampleTime = m_samplePoints[1]-m_samplePoints[0];

	// output
	std::vector<double> result (m_modelOrder, 0.0);

	std::vector<double> abbreveative = m_sythesizedF0;
	result[0] = m_sythesizedF0[m_numberSamples-1];
	for (unsigned int i = 1; i<m_modelOrder; ++i)
	{
		abbreveative = differentiation(abbreveative, sampleTime);
		result[i] = abbreveative[abbreveative.size()-1];
	}

	m_finalAbbrev = result;
}

// calculate system answer for optimal parameters
void ApproximationSystem::system_answer_opt()
{
		// output
	std::vector<double> result (m_numberSamples, 0.0);

	// calculate system coefficients
	std::vector<double> coefficients = system_coefficients(m_modelOrder, m_initialAbbrev, m_optVariables);

	// calculate system answer for every sample point
	for (unsigned int i=0; i<m_numberSamples; ++i)
	{
		// note time shift
		double sampleTime = m_samplePoints[i]-m_samplePoints[0];
		for (unsigned int n=0; n<m_modelOrder; ++n)
		{
			result[i] += (coefficients[n] * std::pow(sampleTime,n));
		}

		result[i] *= std::exp((-1)*sampleTime*m_optVariables[2]);
		result[i] += (m_optVariables[1] + m_optVariables[0]*sampleTime);
	}

	m_sythesizedF0 = result;
}


