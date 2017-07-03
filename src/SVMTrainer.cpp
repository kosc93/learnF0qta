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
	m_parametersSlope = parameters;
	m_parametersSlope.C = 32.0;

	m_parametersOffset = parameters;
	m_parametersOffset.C = 8.0;

	m_parametersStrength = parameters;
	m_parametersStrength.C = 0.03;

	m_parametersDuration = parameters;
	m_parametersDuration.C = 0.03;
}

void SVMTrainer::read_training_file()
{
	// create a file-reading object for sparse training-file
	std::ifstream fin;
	fin.open(m_path + m_trainingFile); // open input file
	if (!fin.good())
		std::cerr << "ERROR: feature-input file not found! " << std::endl;

	// read features to vector
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

void SVMTrainer::svm_training()
{
	// calculate model
	svm_model *pm1, *pm2, *pm3, *pm4;
	pm1 = svm_train(&m_dataSlope, &m_parametersSlope);
	m_modelSlope = *pm1;
	pm2 = svm_train(&m_dataOffset, &m_parametersOffset);
	m_modelOffset = *pm2;
	pm3 = svm_train(&m_dataStrength, &m_parametersStrength);
	m_modelStrength = *pm3;
	pm4 = svm_train(&m_dataDuration, &m_parametersDuration);
	m_modelDuration = *pm4;

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
	svm_save_model(modelFile.c_str(),pm1);
	modelFile = path + "offset.model";
	svm_save_model(modelFile.c_str(),pm2);
	modelFile = path + "strength.model";
	svm_save_model(modelFile.c_str(),pm3);
	modelFile = path + "duration.model";
	svm_save_model(modelFile.c_str(),pm4);
}

void SVMTrainer::predict(std::string testFile)
{
	m_testFile = testFile;

	// create a file-reading object for sparse test-file
	std::ifstream fin;
	fin.open(m_path + m_testFile); // open input file
	if (!fin.good())
		std::cerr << "ERROR: test-feature-input file not found! " << std::endl;

	// store results in file
	std::ofstream fout;
	fout.open(m_path + "../svm/svm.predict");

	// read features to vector
	std::string line;
	std::vector<double> slope, offset, strength, duration;
	std::vector< std::vector<svm_node> > features;
	std::vector<std::string> label;
	while (std::getline(fin, line))
	{
		std::vector<std::string> tokens;
		utilities::split(tokens, line, ",");

		// target values: slope, offset, strength, duration
		label.push_back(tokens[0]);
		slope.push_back(utilities::feat2value(tokens[1]));
		offset.push_back(utilities::feat2value(tokens[2]));
		strength.push_back(utilities::feat2value(tokens[3]));
		duration.push_back(utilities::feat2value(tokens[4]));

		// features
		std::vector<svm_node> tmp;
		for (unsigned int j=5; j<tokens.size(); ++j)
		{
			tmp.push_back( (svm_node){utilities::feat2index(tokens[j]),utilities::feat2value(tokens[j])} );
		}
		tmp.push_back( (svm_node){-1,0} );

		features.push_back(tmp);
	}

	// predict values
	unsigned cnt (0);
	for (std::vector<svm_node> f : features)
	{
		double slopePredict = svm_predict(&m_modelSlope, f.data());
		double offsetPredict = svm_predict(&m_modelOffset, f.data());
		double strengthPredict = svm_predict(&m_modelStrength, f.data());
		double durationPredict = svm_predict(&m_modelDuration, f.data());
		fout << label[cnt] << ",m:" << slopePredict << ",b:" << offsetPredict << ",l:" << strengthPredict << ",d:" << durationPredict << std::endl;
		cnt++;
	}
}
