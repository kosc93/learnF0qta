#include "SVMTrainer.h"
#include "utilities.h"
#include "types.h"

#include <iostream>
#include <fstream>
#include <sys/stat.h>

SVMTrainer::SVMTrainer(std::string path, std::string trainingFile)
{
	m_path = path;
	m_trainingFile = trainingFile;

	determine_parameters ();
	read_training_file ();
	determine_training_data ();
	svm_training ();
}

SVMTrainer::~SVMTrainer()
{
	for (unsigned int i = 0; i < m_features.size(); ++i) // De-Allocate memory to prevent memory leak
	{
		delete [] m_featureArray[i];
	}
	delete [] m_featureArray;
}

void SVMTrainer::determine_parameters ()
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

	// individual parameters
	parametersSlope = parameters;
	parametersSlope.C = 32.0;

	parametersOffset = parameters;
	parametersOffset.C = 8.0;

	parametersStrength = parameters;
	parametersStrength.C = 0.03;

	parametersDuration = parameters;
	parametersDuration.C = 0.03;
}

void SVMTrainer::read_training_file()
{
	// create a file-reading object for sparse training-file
	std::ifstream fin;
	fin.open(m_path + m_trainingFile); // open input file
	if (!fin.good())
		std::cerr << "ERROR: feature-input file not found! " << std::endl;

	// read features to map
	std::string line;
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

void SVMTrainer::determine_training_data()
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
	dataSlope.l = m_targetSlope.size();
	dataSlope.y = m_targetSlope.data();
	dataSlope.x = m_featureArray;

	dataOffset.l = m_targetOffset.size();
	dataOffset.y = m_targetOffset.data();
	dataOffset.x = m_featureArray;

	dataStrength.l = m_targetStrength.size();
	dataStrength.y = m_targetStrength.data();
	dataStrength.x = m_featureArray;

	dataDuration.l = m_targetDuration.size();
	dataDuration.y = m_targetDuration.data();
	dataDuration.x = m_featureArray;
}

void SVMTrainer::svm_training()
{
	// calculate model
	svm_model *pmslope = &modelSlope;
	svm_model *pmoffset = &modelOffset;
	svm_model *pmstrength = &modelStrength;
	svm_model *pmduration = &modelDuration;
	pmslope = svm_train(&dataSlope, &parametersSlope);
	pmoffset = svm_train(&dataOffset, &parametersOffset);
	pmstrength = svm_train(&dataStrength, &parametersStrength);
	pmduration = svm_train(&dataDuration, &parametersDuration);

	// create directory for plots and output file (plot file)
	std::string path = m_path + "../svm/";
	const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err)
	{
		//std::cout << "ERROR creating directory: " << m_path + "plots/" << std::endl;
	}

	// save model
	std::string modelFile;
	modelFile = path + "slope.model";
	svm_save_model(modelFile.c_str(),pmslope);
	modelFile = path + "offset.model";
	svm_save_model(modelFile.c_str(),pmoffset);
	modelFile = path + "strength.model";
	svm_save_model(modelFile.c_str(),pmstrength);
	modelFile = path + "duration.model";
	svm_save_model(modelFile.c_str(),pmduration);
}
