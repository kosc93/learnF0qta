/*
 * ApproximationSystem.cpp
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <iostream>
#include <vector>
#include <cmath>

#include "SystemTA.h"
#include "utilities.h"

// calculate system coefficients
std::vector<double> SystemTA::system_coefficients(unsigned int modelOrder, std::vector<double> initialAbbrev, std::vector<double> variables)
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
			tmp += ( result[j]*std::pow((-1)*strength,n-j) * utilities::binomial_coefficient(n,j) * utilities::factorial(j) );
		}

		if (n == 1)
		{
			tmp += slope;	// minus changes in following term!
		}

		result[n] = (initialAbbrev[n] - tmp)/utilities::factorial(n);
	}

	return result;
}

// calculate system answer of Nth order system at time t with coefficients c parameters x
std::vector<double> SystemTA::system_answer (std::vector<double> variables, SystemTA *system)
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
double SystemTA::error_function(const std::vector<double> &variables, std::vector<double> &grad, void* f_data)
{
	// output
	double result = 0.0;

	// get parameters
	SystemTA *system = (SystemTA*) f_data;

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
void SystemTA::set_optimum (std::vector<double> optVariables)
{
	// optimal values m,b,lambda
	m_optVariables = optVariables;
	m_optVariables[1] = m_originalF0[m_numberSamples-1] - m_optVariables[0] * (m_samplePoints[m_numberSamples-1] - m_samplePoints[0]) + m_optVariables[1];

	// synthesized F0 with optimal parameters
	system_answer_opt();

	// final state (derivatives); note time shift
	system_derivatives_opt();

	// root mean square error
	m_rmse = utilities::rmse(m_originalF0, m_sythesizedF0);

	//correlation coefficient
	m_rho = utilities::correlation(m_originalF0, m_sythesizedF0);
}

// calculate derivative of system at end state (last sample point)
void SystemTA::system_derivatives_opt()
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
				q += (std::pow((-1)*m_optVariables[2], n-k) * utilities::binomial_coefficient(n,k) * coefficients[j+k]) * utilities::factorial(k+j)/utilities::factorial(j);
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

// calculate system answer for optimal parameters
void SystemTA::system_answer_opt()
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


