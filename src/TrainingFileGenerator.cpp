#include "TrainingFileGenerator.h"
#include "utilities.h"
#include "types.h"
#include <cmath>

TrainingFileGenerator::TrainingFileGenerator(std::string featureFile, std::string targetFile, std::string trainingFile)
{
	m_featureFile = featureFile;
	m_targetFile = targetFile;
	m_trainingFile = trainingFile;

	read_input_files();
	write_to_output_file();
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
		auto pos = line.find('\t');
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
	fout << "word,S1,N1,P1,F1,G1,L1,O1,S2,N2,P2,F2,G2,L2,O2,S3,N3,P3,F3,G3,L3,O3,ZP,ZH,LR,LL,NL,S4,N4,P4,F4,G4,L4,O4,S5,N5,P5,F5,G5,L5,O5,S6,N6,P6,F6,G6,L6,O6,S7,N7,P7,F7,G7,L7,O7,AA,AV,AF,WA,WN,AS,NS,PO,PC,PV,PF,F_i,m_,b_,l_,d_,rmse,corr" << std::endl;

	// write data to output file
	for (std::map<std::string,std::string>::iterator it=m_featureMap.begin(); it!=m_featureMap.end(); ++it)
	{
		if (m_targetMap.find(it->first) != m_targetMap.end())
		{
			// tokenize strings
			std::vector<std::string> featuresAll;
			utilities::split(featuresAll, it->second, ",");
			std::vector<std::string> targetsAll;
			utilities::split(targetsAll, (m_targetMap.find(it->first))->second, "\t");

			// get number of syllables
			const int numTar = 6;
			int numSyl = featuresAll.size()/NUM_SYLLABLE_FEATURES;

			// produce one output line for each syllable
			for (int i=0; i<numSyl; ++i)
			{
				fout << it->first << ",";

				// get relevant features
				for (int j=0; j<NUM_SYLLABLE_FEATURES; ++j)
				{
					fout << featuresAll[j+i*NUM_SYLLABLE_FEATURES] << ",";
				}

				// get relevant data from target file: initialf0,mean_rmse,grand_correlation,1__slope,height,strength,duration,rmse,correlation
				fout << targetsAll[0] << "," << targetsAll[3+i*numTar] << "," << targetsAll[4+i*numTar] << "," << targetsAll[5+i*numTar] << ","	<< targetsAll[6+i*numTar] << "," << targetsAll[7+i*numTar] << "," << targetsAll[8+i*numTar] << std::endl;

				// store values for statistics
				m_slope.push_back(std::stod(targetsAll[3+i*numTar]));
				m_offset.push_back(std::stod(targetsAll[4+i*numTar]));
				m_strength.push_back(std::stod(targetsAll[5+i*numTar]));
				m_duration.push_back(std::stod(targetsAll[6+i*numTar]));
				m_rmse.push_back(std::stod(targetsAll[7+i*numTar]));
				m_corr.push_back(std::stod(targetsAll[8+i*numTar]));
			}
		}
	}
}

void TrainingFileGenerator::print_statistics()
{
	// print processing information
	std::cout << "\tNumber of processed words:\t" << std::min(m_targetMap.size(),m_featureMap.size()) << std::endl;
	std::cout << "\tNumber of non-processed words:\t" << std::abs((int)m_targetMap.size()-(int)m_featureMap.size()) << std::endl;
	std::cout << "\tNumber of assembled syllables:\t" << m_slope.size() << std::endl;

	// print statistics
	std::cout << "\tSlope (m):\t\t\t" << utilities::mean(m_slope) << " +/- " << utilities::variance(m_slope) << std::endl;
	std::cout << "\tOffset (b):\t\t\t" << utilities::mean(m_offset) << " +/- " << utilities::variance(m_offset) << std::endl;
	std::cout << "\tStrength (lambda):\t\t" << utilities::mean(m_strength) << " +/- " << utilities::variance(m_strength) << std::endl;
	std::cout << "\tRoot-Mean-Square-Error:\t\t" << utilities::mean(m_rmse) << " +/- " << utilities::variance(m_rmse) << std::endl;
	std::cout << "\tCorrelation-Coefficient:\t" << utilities::mean(m_corr) << " +/- " << utilities::variance(m_corr) << std::endl;
}