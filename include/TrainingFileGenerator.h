#ifndef TRAINIGFILEGENERATOR_H_
#define TRAINIGFILEGENERATOR_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class TrainingFileGenerator {
public:
	TrainingFileGenerator(std::string path, std::string featureFile, std::string targetFile, std::string trainingFile);
	void print_statistics();

private:
	// analysis
	void read_input_files();
	void write_to_output_file();
	void generate_sparse_training_file();
	void separate_training_test_files();
	void determine_scaling_factors();
	void calculate_statistics();

	// members
	std::string m_featureFile;
	std::string m_targetFile;
	std::string m_trainingFile;
	std::string m_path;
	std::map<std::string, std::string> m_featureMap;
	std::map<std::string, std::string> m_targetMap;
	unsigned int m_syllables;
	unsigned int m_skipped;

	// statistics
	std::vector<double> m_slope;
	std::vector<double> m_offset;
	std::vector<double> m_strength;
	std::vector<double> m_duration;
	std::vector<double> m_rmse;
	std::vector<double> m_corr;
	unsigned int m_numTrain;
	unsigned int m_numTest;

	// scaling
	double m_lower;
	double m_upper;
	std::vector<double> m_minFeatures;
	std::vector<double> m_maxFeatures;

};

#endif /* TRAINIGFILEGENERATOR_H_ */
