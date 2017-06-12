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

void PraatFileIO::split( std::vector<std::string> & theStringVector,  /* Altered/returned value */
       const  std::string  & theString,
       const  std::string  & theDelimiter)
{
    size_t  start = 0, end = 0;

    while ( end != std::string::npos)
    {
        end = theString.find( theDelimiter, start);

        // If at end, use length=maxLength.  Else use length=end-start.
        theStringVector.push_back( theString.substr( start,
                       (end == std::string::npos) ? std::string::npos : end - start));

        // If at end, use start=maxSize.  Else use start=end+delimiter.
        start = (   ( end > (std::string::npos - theDelimiter.size()) )
                  ?  std::string::npos  :  end + theDelimiter.size());
    }
}

PraatFileIO::PraatFileIO(ApproximationSystem &system, std::string &path)
{
	m_path = path;
	analyze_config_file(system);
	analyze_data_file(system);

}

void PraatFileIO::analyze_config_file(ApproximationSystem &system)
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
	split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // m_min
	searchBounds.push_back(std::stod(tokens[1])); // m_max,
	tokens.clear();

	// second line
	std::getline(fin, line);
	split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // b_min
	searchBounds.push_back(std::stod(tokens[1])); // b_max
	tokens.clear();

	// first third
	std::getline(fin, line);
	split(tokens, line, " ");
	searchBounds.push_back(std::stod(tokens[0])); // lambda_min
	searchBounds.push_back(std::stod(tokens[1])); // lambda_max
	tokens.clear();

	// fourth line
	std::getline(fin, line);
	modelOrder = std::stoi(line);

	// fifth line
	std::getline(fin, line);
	split(tokens, line, " ");
	for (unsigned int n=0; n<modelOrder; ++n)
		abbreviatives.push_back(std::stod(tokens[n]));
	tokens.clear();

	// sixth line
	std::getline(fin, line);

	// initialize system
	system.set_model_order(modelOrder);
	system.set_search_bounds(searchBounds);
	system.set_initial_abbreviatives(abbreviatives);

}

void PraatFileIO::analyze_data_file(ApproximationSystem &system)
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
	split(tokens, line, " ");
	numberSamples = std::stod(tokens[0]);
	tokens.clear();

	// resulting lines
	for (unsigned int i=0; i<numberSamples; ++i)
	{
		std::getline(fin, line);
		std::size_t pos = line.find_first_of('\t');
		line.erase(line.begin(),line.begin()+pos+1);
		split(tokens, line, "\t");
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

void PraatFileIO::generate_output_file(ApproximationSystem &system)
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
}

