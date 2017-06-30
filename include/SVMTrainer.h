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

private:
	// member functions
	void determine_parameters ();
	void read_training_file ();
	void determine_training_data ();
	void svm_training ();

	// INPUT: SVM parameters -> set by determine_parameters ()
	struct svm_parameter parametersSlope;
	struct svm_parameter parametersOffset;
	struct svm_parameter parametersStrength;
	struct svm_parameter parametersDuration;

	// INPUT: training data -> set by determine_training_data ()
	struct svm_problem dataSlope;
	struct svm_problem dataOffset;
	struct svm_problem dataStrength;
	struct svm_problem dataDuration;

	// OUTPUT: SVM model definition -> set by svm_training ()
	struct svm_model modelSlope;
	struct svm_model modelOffset;
	struct svm_model modelStrength;
	struct svm_model modelDuration;

	// data IO
	std::string m_path;
	std::string m_trainingFile;
	std::vector<double> m_targetSlope;
	std::vector<double> m_targetOffset;
	std::vector<double> m_targetStrength;
	std::vector<double> m_targetDuration;
	std::vector< std::vector<svm_node> > m_features;
	svm_node **m_featureArray;
};



#endif /* SVMTRAINER_H_ */
