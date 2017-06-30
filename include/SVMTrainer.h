/*
 * SVMTrainer.h
 *
 *  Created on: 29 June 2017
 *      Author: patrick
 */

#ifndef SVMTRAINER_H_
#define SVMTRAINER_H_

#include "svm.h"
#include <vector>
#include <string>

class SVMTrainer{

public:
	SVMTrainer(std::string path, std::string trainingFile);
	~SVMTrainer();

	void predict(std::string testFile);

private:
	// member functions
	void determine_parameters ();
	void read_training_file ();
	void determine_training_data ();
	void svm_training ();

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

	// OUTPUT: SVM model definition -> set by svm_training ()
	struct svm_model m_modelSlope;
	struct svm_model m_modelOffset;
	struct svm_model m_modelStrength;
	struct svm_model m_modelDuration;

	// data IO
	std::string m_path;
	std::string m_testFile;
	std::string m_trainingFile;
	std::vector<double> m_targetSlope;
	std::vector<double> m_targetOffset;
	std::vector<double> m_targetStrength;
	std::vector<double> m_targetDuration;
	std::vector< std::vector<svm_node> > m_features;
	svm_node **m_featureArray;
};



#endif /* SVMTRAINER_H_ */
