#ifndef FILEANALYZER_H_
#define FILEANALYZER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

class FileAnalyzer {
public:
	FileAnalyzer(std::string featureFile, std::string targetFile, std::string trainingFile);
	void print_statistics();

private:
	// some helpers
	void split( std::vector<std::string> & theStringVector, const  std::string  & theString, const  std::string  & theDelimiter);
	double mean (std::vector<double> &data);
	double variance (std::vector<double> &data);

	// analysis
	void read_input_files();
	void write_to_output_file();
	void calculate_statistics();

	// members
	std::string m_featureFile;
	std::string m_targetFile;
	std::string m_trainingFile;
	std::map<std::string, std::string> m_featureMap;
	std::map<std::string, std::string> m_targetMap;
	unsigned int m_syllables;
	unsigned int m_skipped;

	std::vector<double> m_slope;
	std::vector<double> m_offset;
	std::vector<double> m_strength;
	std::vector<double> m_duration;
	std::vector<double> m_rmse;
	std::vector<double> m_corr;

};

#endif /* FILEANALYZER_H_ */
