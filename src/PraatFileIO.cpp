/*
 * PraatFileIO.cpp
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>

#include "PraatFileIO.h"
#include "utilities.h"

PraatFileIO::PraatFileIO(SystemTA &system, std::string &path)
{
	m_path = path;
	analyze_config_file(system);
	analyze_data_file(system);
}

void PraatFileIO::analyze_config_file(SystemTA &system)
{
	// create a file-reading object
	std::ifstream fin;
	fin.open(m_path+"config"); // open a file
	if (!fin.good())
		std::cerr << "ERROR: config file not found! " << std::endl;

	// hold data in container
	unsigned int modelOrder;
	std::vector<double> searchBounds;
	std::vector<double> abbreviatives;

	// compute file
	std::string line;
	std::vector<std::string> tokens;

	// first line
	std::getline(fin, line);
	utilities::split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // m_min
	searchBounds.push_back(std::stod(tokens[1])); // m_max,
	tokens.clear();

	// second line
	std::getline(fin, line);
	utilities::split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // b_min
	searchBounds.push_back(std::stod(tokens[1])); // b_max
	tokens.clear();

	// third third
	std::getline(fin, line);
	utilities::split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // lambda_min
	searchBounds.push_back(std::stod(tokens[1])); // lambda_max
	tokens.clear();

	// fourth line
	std::getline(fin, line);
	modelOrder = std::stoi(line);

	// fifth line
	std::getline(fin, line);
	utilities::split(tokens, line, " ");
	for (unsigned int n=0; n<modelOrder; ++n)
		abbreviatives.push_back(std::stod(tokens[n]));
	tokens.clear();

	// sixth line
	std::getline(fin, line);
	utilities::split(tokens, line, " ");

	// initialize system
	system.set_model_order(modelOrder);
	system.set_search_bounds(searchBounds);
	system.set_initial_abbreviatives(abbreviatives);
	system.set_syllable_bounds(std::stod(tokens[0]), std::stod(tokens[1]));
}

void PraatFileIO::analyze_data_file(SystemTA &system)
{
	// create a file-reading object
	std::ifstream fin;
	fin.open(m_path+"data"); // open a file
	if (!fin.good())
		std::cerr << "ERROR: data file not found!" << std::endl;

	// hold data in container
	double numberSamples;
	std::vector<double> samplePoints;
	std::vector<double> originalF0;

	// compute file
	std::string line;
	std::vector<std::string> tokens;

	// skip first six lines
	for (int i=0; i<6; i++)
		std::getline(fin, line);

	// 7th line
	std::getline(fin, line);
	utilities::split(tokens, line, " ");
	numberSamples = std::stod(tokens[0]);
	tokens.clear();

	// resulting lines
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		std::getline(fin, line);
		std::size_t pos = line.find_first_of('\t');
		line.erase(line.begin(),line.begin()+pos+1);
		utilities::split(tokens, line, "\t");
		samplePoints.push_back(std::stod(tokens[0]));
		originalF0.push_back(std::stod(tokens[1]));
		tokens.clear();
	}

	// adopt search bounds for bMin/bMax
	std::vector<double> searchBounds = system.get_search_bounds();
	//searchBounds[2] += originalF0[0];
	//searchBounds[3] += originalF0[0];
	//system.set_search_bounds(searchBounds);

	// initialize system
	system.set_number_samples(numberSamples);
	system.set_sample_points(samplePoints);
	system.set_original_F0(originalF0);
}

void PraatFileIO::generate_output_file(SystemTA &system)
{
	// create output file and write results to it
	std::ofstream fout;
	fout.open (m_path+"output");
	fout << std::fixed << std::setprecision(6);

	// line 1: optimal variables
	fout << system.get_optimal_variables()[0] << " " << system.get_optimal_variables()[1] << " " << system.get_optimal_variables()[2] << std::endl;

	// line 2: final state (abbreviatives)
	for (unsigned int n=0; n<system.get_model_order(); ++n)
	{
		fout << system.get_final_abbreviatives()[n] << " ";
	}

	// line 3: root mean square error and correlation coefficient
	fout << std:: endl << system.get_root_mean_square_error() << " " << system.get_correlation_coefficient() << std::endl;

	// line 4: number of samples
	fout << system.get_number_samples() << std::endl;

	// from line 5: sample points and resynthesized F0
	for (unsigned int i=0; i<system.get_number_samples(); ++i)
	{
		fout << system.get_sample_points()[i] << " " << system.get_sythesized_F0()[i] << std::endl;
	}

	// close output file
	fout.close();

	/****************************************/

	// create additional output file for equally sampled F0
	std::ofstream fout2;
	fout2.open (m_path+"dataoutput");
	fout2 << std::fixed << std::setprecision(6);

	// line 1: number of samples
	unsigned numberSamples = system.get_sample_points_equal().size();
	fout2 << numberSamples << std::endl;

	// from line 2: sample points and resynthesized F0 (equally sampled)
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		fout2 << system.get_sample_points_equal()[i] << " " << system.get_sythesized_F0_sampled()[i] << std::endl;
	}

	// close output file
	fout2.close();
}

