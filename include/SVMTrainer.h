/*
 * SVMTrainer.h
 *
 *  Created on: 29 June 2017
 *      Author: patrick
 */

#ifndef SVMTRAINER_H_
#define SVMTRAINER_H_

#include "svm.h"

class SVMTrainer{

private:
	// INPUT: SVM parameters -> set by determine_parameters ()
	struct svm_parameter parametersSlope;
	struct svm_parameter parametersOffset;
	struct svm_parameter parametersStrength;
	struct svm_parameter parametersDuration;

	// INPUT: training data -> set by determine_tdata ()
	struct svm_problem dataSlope;
	struct svm_problem dataOffset;
	struct svm_problem dataStrength;
	struct svm_problem dataDuration;

	// OUTPUT: SVM model definition -> set by svm_training ()
	struct svm_model modelSlope;
	struct svm_model modelOffset;
	struct svm_model modelStrength;
	struct svm_model modelDuration;

};



#endif /* SVMTRAINER_H_ */
