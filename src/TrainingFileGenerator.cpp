#include "TrainingFileGenerator.h"
#include "utilities.h"
#include "types.h"
#include <sys/stat.h>
#include <cmath>
#include <cstdlib>
#include <time.h>

TrainingFileGenerator::TrainingFileGenerator(std::string featureFile, std::string targetFile, std::string outpath)
{
	m_outpath = outpath;
	m_featureFile = featureFile;
	m_targetFile = targetFile;
	m_trainingFile = outpath + "corpus-training.csv";

	//sclaing
	m_lower = 0.0;
	m_upper = 1.0;

	for (int j=0; j<NUM_SYLLABLE_FEATURES; ++j)
	{
		m_minFeatures.push_back(1.0);
		m_maxFeatures.push_back(1.0);
	}

	read_input_files();
	write_to_output_file();
	determine_scaling_factors();
	generate_sparse_training_file();
	separate_training_test_files();
}

void TrainingFileGenerator::read_input_files()
{
	// create a file-reading object for feature-file
	std::ifstream finFeatures;
	finFeatures.open(m_featureFile); // open input file
	if (!finFeatures.good())
		std::cerr << "ERROR: feature-input file not found! " << std::endl;

	// create a file-reading object for target-file
	std::ifstream finTargets;
	finTargets.open(m_targetFile); // open input file
	if (!finTargets.good())
		std::cerr << "ERROR: target-input file not found! " << std::endl;

	// read features to map
	std::string line;
	std::getline(finFeatures, line); // ingnore first line (header)
	while (std::getline(finFeatures, line))
	{
		auto pos = line.find(',');
		m_featureMap.insert(std::pair<std::string,std::string>(line.substr(0,pos), line.substr(pos+1,line.size())));
	}

	// read targets to map
	std::getline(finTargets, line); // ingnore first line (header)
	while (std::getline(finTargets, line))
	{
		auto pos = line.find(',');
		m_targetMap.insert(std::pair<std::string,std::string>(line.substr(0,pos), line.substr(pos+1,line.size())));
	}

	// check number of words
	if (m_targetMap.size() != m_featureMap.size())
	{
		std::cout << "\nWarning: unequal number of elements in feature and target file!\n";
		std::cout << "\t-> " << m_featureMap.size() << " words in feature file and " << m_targetMap.size() << " words in target file\n";
	}
}

void TrainingFileGenerator::write_to_output_file()
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open (m_trainingFile);

	// write header to outputfile
	fout << "word,S1,N1,P1,F1,G1,L1,O1,S2,N2,P2,F2,G2,L2,O2,S3,N3,P3,F3,G3,L3,O3,ZP,ZH,LR,LL,NL,S4,N4,P4,F4,G4,L4,O4,S5,N5,P5,F5,G5,L5,O5,S6,N6,P6,F6,G6,L6,O6,S7,N7,P7,F7,G7,L7,O7,AA,AV,AF,WA,WN,AS,NS,PO,PC,PV,PF,m_,b_,l_,d_,rmse,corr" << std::endl;

	// write data to output file
	for (std::map<std::string,std::string>::iterator it=m_featureMap.begin(); it!=m_featureMap.end(); ++it)
	{
		if (m_targetMap.find(it->first) != m_targetMap.end())
		{
			// tokenize strings
			std::vector<std::string> featuresAll;
			utilities::split(featuresAll, it->second, ",");
			std::vector<std::string> targetsAll;
			utilities::split(targetsAll, (m_targetMap.find(it->first))->second, ",");

			// get number of syllables
			const int numTar = 6;
			int numSyl = featuresAll.size()/NUM_SYLLABLE_FEATURES;

			// produce one output line for each syllable
			for (int i=0; i<numSyl; ++i)
			{
				fout << "label:" << it->first << ",";

				// get relevant features
				for (int j=0; j<NUM_SYLLABLE_FEATURES; ++j)
				{
					fout << featuresAll[j+i*NUM_SYLLABLE_FEATURES] << ",";
				}

				// get relevant data from target file: initialf0,mean_rmse,grand_correlation,1__slope,height,strength,duration,rmse,correlation
				fout << targetsAll[0+i*numTar] << "," << targetsAll[1+i*numTar] << "," << targetsAll[2+i*numTar] << ","	<< targetsAll[3+i*numTar] << "," << targetsAll[4+i*numTar] << "," << targetsAll[5+i*numTar] << std::endl;

				// store values for statistics
				std::string value = targetsAll[0+i*numTar];
				m_slope.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
				value = targetsAll[1+i*numTar];
				m_offset.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
				value = targetsAll[2+i*numTar];
				m_strength.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
				value = targetsAll[3+i*numTar];
				m_duration.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
				value = targetsAll[4+i*numTar];
				m_rmse.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
				value = targetsAll[5+i*numTar];
				m_corr.push_back(std::stod(value.substr(value.find(":")+1,value.size())));
			}
		}
	}
}

void TrainingFileGenerator::generate_sparse_training_file()
{
	/*** SPARSE: create output file and write results to it ***/
	std::ofstream foutSparse;
	foutSparse.open (m_outpath + "samples-all.csv");
	foutSparse << "name,slope,offset,strength,duration,features(sparse)" << std::endl;

	// write data to output file
	for (std::map<std::string,std::string>::iterator it=m_featureMap.begin(); it!=m_featureMap.end(); ++it)
	{
		if (m_targetMap.find(it->first) != m_targetMap.end())
		{
			// tokenize strings
			std::vector<std::string> featuresAll;
			utilities::split(featuresAll, it->second, ",");
			std::vector<std::string> targetsAll;
			utilities::split(targetsAll, (m_targetMap.find(it->first))->second, ",");

			// get number of syllables
			const int numTar = 6;
			int numSyl = featuresAll.size()/NUM_SYLLABLE_FEATURES;

			// produce one output line for each syllable
			for (int i=0; i<numSyl; ++i)
			{
				foutSparse << "label:" << it->first << ",";
				/*** SPARSE: get relevant data from target file: initialf0,mean_rmse,grand_correlation,1__slope,height,strength,duration,rmse,correlation ***/
				foutSparse << "" << targetsAll[0+i*numTar] << "," << targetsAll[1+i*numTar] << "," << targetsAll[2+i*numTar] << "," << targetsAll[3+i*numTar];

				/*** SPARSE: get relevant features ***/
				for (int j=0; j<NUM_SYLLABLE_FEATURES; ++j)
				{
					std::string feature (featuresAll[j+i*NUM_SYLLABLE_FEATURES]);
					double value (std::stod(feature.substr(feature.find(":")+1,feature.size())));

					// scaling
					double diff = m_maxFeatures[j]-m_minFeatures[j];
					if (diff == 0)
					{
						value = 0;
					}
					else if (diff > 0)
					{
						value = m_lower + (m_upper-m_lower) * (value-m_minFeatures[j])/(m_maxFeatures[j]-m_minFeatures[j]);// scale value
					}
					else
					{
						std::cout << "ERROR: wrong scaling! max<min!";
					}

					if (value != 0)
					{
						foutSparse << "," << j+1 << ":" << value;
					}
				}
				foutSparse << std::endl;
			}
		}
	}
}

void TrainingFileGenerator::separate_training_test_files()
{
	// create a file-reading object for feature-file
	std::ifstream fin;
	fin.open(m_outpath + "samples-all.csv"); // open input file
	if (!fin.good())
		std::cerr << "ERROR: samples-all.csv file not found! " << std::endl;

	// read lines to vector
	std::string line;
	std::getline(fin, line); // ignore header
	std::vector<std::string> trainSamples;
	while (std::getline(fin, line))
	{
		trainSamples.push_back(line);
	}

	// divide set into training and test
	double fraction = 0.75;
	unsigned int numSamples = trainSamples.size();
	unsigned int numTraining = std::ceil(fraction * numSamples);
	unsigned int numTest = numSamples - numTraining;

	// choose test data randomly
	std::vector<std::string> testSamples;
	srand (time(NULL)); // srand (0);
	for (unsigned int i=0; i<numTest; )
	{
		// random index + initialize
		unsigned int index = std::rand() % (trainSamples.size());
		bool prev = true, next = true;
		unsigned int radius = 1;
		unsigned int indexStart = index, indexEnd = index;

		while(prev || next)
		{
			std::string label, prevLabel, nextLabel;
			std::vector<std::string> tokens;

			// get label
			utilities::split(tokens,trainSamples[index],",");
			label = ( tokens[0].substr( tokens[0].find(":")+1, tokens[0].size()-1) );
			tokens.clear();

			if (index-radius >= 0)
			{
				utilities::split(tokens,trainSamples[index-radius],",");
				prevLabel = ( tokens[0].substr( tokens[0].find(":")+1, tokens[0].size()-1) );
				tokens.clear();
			}

			if (index+radius < trainSamples.size())
			{
				utilities::split(tokens,trainSamples[index+radius],",");
				nextLabel = ( tokens[0].substr( tokens[0].find(":")+1, tokens[0].size()-1) );
				tokens.clear();
			}

			// compare label
			if (label == prevLabel)
			{
				indexStart--;
			}
			else
			{
				prev = false;
			}

			if (label == nextLabel)
			{
				indexEnd++;
			}
			else
			{
				next = false;
			}

			radius++;
		}

		// copy and erase data
		for (unsigned int j = indexStart; j <= indexEnd; ++j)
		{
			testSamples.push_back(trainSamples[j]);
			++i;
		}
		trainSamples.erase(trainSamples.begin()+indexStart, trainSamples.begin()+indexEnd+1);
	}

	// write to files
	std::ofstream foutTrain, foutTest;
	foutTrain.open (m_outpath + "samples-training.csv");
	foutTrain << "name,slope,offset,strength,duration,features(sparse)" << std::endl;
	foutTest.open (m_outpath + "samples-test.csv");
	foutTest << "name,slope,offset,strength,duration,features(sparse)" << std::endl;

	for (std::string s : trainSamples)
	{
		foutTrain << s << std::endl;
	}

	for (std::string s : testSamples)
	{
		foutTest << s << std::endl;
	}

	m_numTrain = trainSamples.size();
	m_numTest = testSamples.size();
}

void TrainingFileGenerator::print_statistics()
{
	// print processing information
	std::cout << "\n\tNumber of processed words:\t\t" << std::min(m_targetMap.size(),m_featureMap.size()) << std::endl;
	std::cout << "\tNumber of non-processed words:\t\t" << std::abs((int)m_targetMap.size()-(int)m_featureMap.size()) << std::endl;
	std::cout << "\tNumber of assembled syllables:\t\t" << m_slope.size() << std::endl;
	std::cout << "\tNumber of syllables for training:\t" << m_numTrain << std::endl;
	std::cout << "\tNumber of syllables for testing:\t" << m_numTest << std::endl;

	// print statistics
	std::cout << "\tSlope (m):\t\t\t" << utilities::mean(m_slope) << " +/- " << utilities::variance(m_slope) << std::endl;
	std::cout << "\tOffset (b):\t\t\t" << utilities::mean(m_offset) << " +/- " << utilities::variance(m_offset) << std::endl;
	std::cout << "\tStrength (lambda):\t\t" << utilities::mean(m_strength) << " +/- " << utilities::variance(m_strength) << std::endl;
	std::cout << "\tRoot-Mean-Square-Error:\t\t" << utilities::mean(m_rmse) << " +/- " << utilities::variance(m_rmse) << std::endl;
	std::cout << "\tCorrelation-Coefficient:\t" << utilities::mean(m_corr) << " +/- " << utilities::variance(m_corr) << std::endl;
}

void TrainingFileGenerator::determine_scaling_factors()
{
	// write data to output file
	for (std::map<std::string,std::string>::iterator it=m_featureMap.begin(); it!=m_featureMap.end(); ++it)
	{
		if (m_targetMap.find(it->first) != m_targetMap.end())
		{
			// tokenize strings
			std::vector<std::string> featuresAll;
			utilities::split(featuresAll, it->second, ",");

			// get number of syllables
			int numSyl = featuresAll.size()/NUM_SYLLABLE_FEATURES;

			// produce one output line for each syllable
			for (int i=0; i<numSyl; ++i)
			{
				// get relevant features
				for (int j=0; j<NUM_SYLLABLE_FEATURES; ++j)
				{
					std::string feature (featuresAll[j+i*NUM_SYLLABLE_FEATURES]);
					double value (std::stod(feature.substr(feature.find(":")+1,feature.size())));
					m_minFeatures[j] = std::min(value, m_minFeatures[j]);
					m_maxFeatures[j] = std::max(value, m_maxFeatures[j]);
				}
			}
		}
	}
}
