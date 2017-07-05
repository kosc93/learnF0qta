/*
 * SVMTrainer.h
 *
 *  Created on: 05 July 2017
 *      Author: patrick
 */

#ifndef SVMVALIDATER_H_
#define SVMVALIDATER_H_

#include "svm.h"
#include <vector>
#include <string>

class SVMValidater{

public:
	SVMValidater(std::string sampleFile);
	~SVMValidater();

	void set_parameters(double coefficient, double gamma, double epsilon);
	std::vector<double> do_cross_validation();

private:
	// member functions
	void determine_parameters ();
	void read_training_file ();
	void determine_training_data ();

	// INPUT: SVM parameters -> set by determine_parameters ()
	struct svm_parameter m_parametersSlope;
	struct svm_parameter m_parametersOffset;
	struct svm_parameter m_parametersStrength;
	struct svm_parameter m_parametersDuration;

	// INPUT: training data -> set by determine_training_data ()
	struct svm_problem m_dataSlope;
	struct svm_problem m_dataOffset;
	struct svm_problem m_dataStrength;
	struct svm_problem m_dataDuration;

	// data IO
	std::string m_trainingFile;
	std::vector<double> m_targetSlope;
	std::vector<double> m_targetOffset;
	std::vector<double> m_targetStrength;
	std::vector<double> m_targetDuration;
	std::vector< std::vector<svm_node> > m_features;
	svm_node **m_featureArray;
};



#endif /* SVMVALIDATER_H_ */
