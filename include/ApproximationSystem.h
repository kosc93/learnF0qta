/*
 * ApproximationSystem.h
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#ifndef APPROXIMATIONSYSTEM_H_
#define APPROXIMATIONSYSTEM_H_

#include <vector>

class ApproximationSystem {
public:

	// get functions
	unsigned int 		get_model_order() 			{ return m_modelOrder; };
	unsigned int 		get_number_samples() 		{ return m_numberSamples; };
	std::vector<double> get_initial_abbreviatives() { return m_initialAbbrev; };
	std::vector<double> get_final_abbreviatives() 	{ return m_finalAbbrev; };
	std::vector<double> get_sample_points() 		{ return m_samplePoints; };
	std::vector<double> get_original_F0() 			{ return m_originalF0; };
	std::vector<double> get_sythesized_F0() 		{ return m_sythesizedF0; };
	std::vector<double> get_search_bounds() 		{ return m_searchBounds; };
	std::vector<double> get_optimal_variables()		{ return m_optVariables; };
	double 				get_root_mean_square_error(){ return m_rmse; };
	double 				get_correlation_coefficient(){return m_rho; };

	// set functions
	void set_model_order			(unsigned int modelOrder) 			{ m_modelOrder = modelOrder; };
	void set_number_samples			(unsigned int numberSamples) 		{ m_numberSamples = numberSamples; };
	void set_initial_abbreviatives	(std::vector<double> initialAbbrev) { m_initialAbbrev = initialAbbrev; };
	void set_sample_points			(std::vector<double> samplePoints) 	{ m_samplePoints = samplePoints; };
	void set_original_F0			(std::vector<double> originalF0) 	{ m_originalF0 = originalF0; };
	void set_search_bounds			(std::vector<double> searchBounds) 	{ m_searchBounds = searchBounds; };

	// error function (to be optimized)
	static double error_function(const std::vector<double> &variables, std::vector<double> &grad, void* f_data);

	// inform system about optimum -> calculates m_finalAbbrev, m_sythesizedF0
	void set_optimum (std::vector<double> optVariables);


private:
	// helpers for intermediate calculations
	static double 				binomial_coefficient	(int n, int k);
	static double 				factorial				(int n)	{ return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;	};
	static double				root_mean_square_error	(std::vector<double> originalF0, std::vector<double> synthesizedF0);
	static double				correlation_coefficient	(std::vector<double> originalF0, std::vector<double> synthesizedF0);
	static std::vector<double>	system_answer 			(std::vector<double> variables, ApproximationSystem *system);
	static std::vector<double> 	system_coefficients		(unsigned int modelOrder, std::vector<double> initialAbbrev, std::vector<double> variables);
	static std::vector<double>  differentiation 		(std::vector<double> function, double stepSize);

	// calculate system state at optimum
	void system_answer_opt 		();			// set synthezized F0
	void system_derivatives_opt	();			// set final derivatives
	void system_derivatives_opt_num	();			// set final derivatives using numerical differentiation

	// members (set by PraatFileIO)
	unsigned int 		m_modelOrder;		// model order M
	unsigned int 		m_numberSamples;	// number of samples N
	std::vector<double> m_initialAbbrev;	// initial state
	std::vector<double> m_samplePoints;		// sample points t
	std::vector<double> m_originalF0;		// original F0 at sample points
	std::vector<double> m_searchBounds;		// search bounds: m_min,m_max,b_min,b_max,lambda_min,lambda_max

	// members (set by calling "set_optimum")
	std::vector<double> m_optVariables;		// optimal m,b,lambda
	std::vector<double> m_finalAbbrev;		// final state
	std::vector<double> m_sythesizedF0;		// synthesized F0 at sample points
	double m_rmse;							// root mean square error
	double m_rho;							// correleation coefficient
};

#endif /* APPROXIMATIONSYSTEM_H_ */
