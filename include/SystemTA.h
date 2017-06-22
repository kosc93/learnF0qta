/*
 * SystemTA.h
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#ifndef SYSTEMTA_H_
#define SYSTEMTA_H_

#include <vector>
#include <types.h>

class SystemTA {
public:
	// get functions
	unsigned int 		get_model_order() 			{ return m_modelOrder; };
	unsigned int 		get_number_samples() 		{ return m_numberSamples; };
	std::vector<double> get_initial_abbreviatives() { return m_initialAbbrev; };
	std::vector<double> get_final_abbreviatives() 	{ return m_finalAbbrev; };
	std::vector<double> get_sample_points() 		{ return m_samplePoints; };
	std::vector<double> get_sample_points_equal()	{ return m_samplePointsEqual; };
	std::vector<double>	get_original_F0() 			{ return m_originalF0; };
	std::vector<double> get_sythesized_F0() 		{ return m_sythesizedF0; };
	std::vector<double> get_sythesized_F0_sampled() { return m_sythesizedF0Sampled; };
	std::vector<double> get_search_bounds() 		{ return m_searchBounds; };
	std::vector<double> get_optimal_variables()		{ return m_optVariables; };
	double 				get_root_mean_square_error(){ return m_rmse; };
	double 				get_correlation_coefficient(){return m_rho; };
	double 				get_lower_syllable_bound()	{return m_lowerSyllableBound; };
	double 				get_upper_syllable_bound()	{return m_upperSyllableBound; };

	// set functions
	void set_model_order			(unsigned int modelOrder) 			{ m_modelOrder = modelOrder; };
	void set_number_samples			(unsigned int numberSamples) 		{ m_numberSamples = numberSamples; };
	void set_initial_abbreviatives	(std::vector<double> initialAbbrev) { m_initialAbbrev = initialAbbrev; };
	void set_sample_points			(std::vector<double> samplePoints) 	{ m_samplePoints = samplePoints; };
	void set_original_F0			(std::vector<double> originalF0)	{ m_originalF0 = originalF0; };
	void set_search_bounds			(std::vector<double> searchBounds) 	{ m_searchBounds = searchBounds; };
	void set_syllable_bounds		(double lower, double upper) 		{ m_lowerSyllableBound = lower; m_upperSyllableBound=upper; };

	// error function (to be optimized)
	static double error_function(const std::vector<double> &variables, std::vector<double> &grad, void* f_data);

	// inform system about optimum -> calculates m_finalAbbrev, m_sythesizedF0
	void set_optimum (std::vector<double> optVariables);

private:
	// static functions called by error function
	static std::vector<double>	system_answer 			(std::vector<double> variables, SystemTA *system);
	static std::vector<double> 	system_coefficients		(unsigned int modelOrder, std::vector<double> initialAbbrev, std::vector<double> variables);

	// calculate system state at optimum
	void system_answer_opt 		();			// set synthezized F0
	void system_answer_opt_sampled ();		// set synthezized F0 equally sampled
	void system_derivatives_opt	();			// set final derivatives

	// members (set by PraatFileIO)
	unsigned int 		m_modelOrder;		// model order M
	unsigned int 		m_numberSamples;	// number of samples N
	std::vector<double> m_initialAbbrev;	// initial state
	std::vector<double> m_samplePoints;		// sample points t
	std::vector<double> m_samplePointsEqual;// sample points t
	std::vector<double>	m_originalF0;		// original F0 at sample points
	std::vector<double> m_searchBounds;		// search bounds: m_min,m_max,b_min,b_max,lambda_min,lambda_max

	// members (set by calling "set_optimum")
	std::vector<double> m_optVariables;		// optimal m,b,lambda
	std::vector<double> m_finalAbbrev;		// final state
	std::vector<double> m_sythesizedF0;		// synthesized F0 at sample points
	std::vector<double> m_sythesizedF0Sampled;	// synthesized F0 equally sampled
	double m_rmse;							// root mean square error
	double m_rho;							// correleation coefficient
	double m_lowerSyllableBound;
	double m_upperSyllableBound;
};

#endif /* SYSTEMTA_H_ */
