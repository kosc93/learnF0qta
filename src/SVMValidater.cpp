#include "SVMValidater.h"
#include "utilities.h"
#include "types.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

SVMValidater::SVMValidater(std::string sampleFile)
{
	m_trainingFile = sampleFile;

	determine_parameters ();
	read_training_file ();
	determine_training_data ();
}

SVMValidater::~SVMValidater()
{
	for (unsigned int i = 0; i < m_features.size(); ++i) // De-Allocate memory to prevent memory leak
	{
		delete [] m_featureArray[i];
	}
	delete [] m_featureArray;
}

void SVMValidater::determine_parameters ()
{
	// default values
	struct svm_parameter parameters;
	parameters.svm_type = EPSILON_SVR;
	parameters.kernel_type = RBF;
	parameters.degree = 3;
	parameters.gamma = 0.125;	// 1/num_features
	parameters.coef0 = 0;
	parameters.nu = 0.5;
	parameters.cache_size = 100;
	parameters.C = 1;
	parameters.eps = 1e-3;
	parameters.p = 0.1;
	parameters.shrinking = 1;
	parameters.probability = 0;
	parameters.nr_weight = 0;
	parameters.weight_label = NULL;
	parameters.weight = NULL;

	m_parametersSlope = parameters;
	m_parametersOffset = parameters;
	m_parametersStrength = parameters;
	m_parametersDuration = parameters;
}

void SVMValidater::set_parameters(double coefficient, double gamma, double epsilon)
{
	m_parametersSlope.C = coefficient;
	m_parametersOffset.C = coefficient;
	m_parametersStrength.C = coefficient;
	m_parametersDuration.C = coefficient;

	m_parametersSlope.gamma = gamma;
	m_parametersOffset.gamma = gamma;
	m_parametersStrength.gamma = gamma;
	m_parametersDuration.gamma = gamma;

	m_parametersSlope.p = epsilon;
	m_parametersOffset.p = epsilon;
	m_parametersStrength.p = epsilon;
	m_parametersDuration.p = epsilon;
}

void SVMValidater::read_training_file()
{
	// create a file-reading object for sparse training-file
	std::ifstream fin;
	fin.open(m_trainingFile); // open input file
	if (!fin.good())
		std::cerr << "ERROR: feature-input file not found! " << std::endl;

	// read features to vector
	std::string line;
	std::getline(fin, line); // ignore first line - header
	while (std::getline(fin, line))
	{
		std::vector<std::string> tokens;
		utilities::split(tokens, line, ",");

		// target values: slope, offset, strength, duration
		m_targetSlope.push_back(utilities::feat2value(tokens[1]));
		m_targetOffset.push_back(utilities::feat2value(tokens[2]));
		m_targetStrength.push_back(utilities::feat2value(tokens[3]));
		m_targetDuration.push_back(utilities::feat2value(tokens[4]));

		// features
		std::vector<svm_node> tmp;
		for (unsigned int j=5; j<tokens.size(); ++j)
		{
			tmp.push_back( (svm_node){utilities::feat2index(tokens[j]),utilities::feat2value(tokens[j])} );
		}
		tmp.push_back( (svm_node){-1,0} );

		m_features.push_back(tmp);
	}
}

void SVMValidater::determine_training_data()
{
	// convert to double pointer (svm_node**)
	unsigned int numExamples = m_features.size();
	m_featureArray = new svm_node*[numExamples];

	for (unsigned int i=0; i< numExamples; ++i) // Assign values
	{
		unsigned int numFeatures = m_features[i].size();
		m_featureArray[i] = new svm_node[numFeatures];
		for (unsigned int j=0; j<numFeatures; ++j)
		{
			m_featureArray[i][j] = m_features[i][j];
		}
	}

	// write data to structure
	m_dataSlope.l = m_targetSlope.size();
	m_dataSlope.y = m_targetSlope.data();
	m_dataSlope.x = m_featureArray;

	m_dataOffset.l = m_targetOffset.size();
	m_dataOffset.y = m_targetOffset.data();
	m_dataOffset.x = m_featureArray;

	m_dataStrength.l = m_targetStrength.size();
	m_dataStrength.y = m_targetStrength.data();
	m_dataStrength.x = m_featureArray;

	m_dataDuration.l = m_targetDuration.size();
	m_dataDuration.y = m_targetDuration.data();
	m_dataDuration.x = m_featureArray;
}

std::vector<double> SVMValidater::do_cross_validation()
{
	// data
	std::vector<double> rmse (4,0.0);
	int nfolds = 5;
	std::vector<double> resultTargets (m_targetSlope.size(), 0.0);

	svm_cross_validation(&m_dataSlope, &m_parametersSlope,nfolds,resultTargets.data());
	rmse[0] = utilities::rmse(m_targetSlope,resultTargets);

	svm_cross_validation(&m_dataOffset, &m_parametersOffset,nfolds,resultTargets.data());
	rmse[1] = utilities::rmse(m_targetOffset,resultTargets);

	svm_cross_validation(&m_dataStrength, &m_parametersStrength,nfolds,resultTargets.data());
	rmse[2] = utilities::rmse(m_targetStrength,resultTargets);

	svm_cross_validation(&m_dataDuration, &m_parametersDuration,nfolds,resultTargets.data());
	rmse[3] = utilities::rmse(m_targetDuration,resultTargets);

	return rmse;
}
