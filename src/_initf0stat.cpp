#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "utilities.h"

int main(int argc, char* argv[])
{
	// Check the number of parameters
	if (argc != 3)
	{
		std::cerr << "Usage: " << std::string(argv[0]) << " <TARGETS> <PATH>" << std::endl;
		return 1;
	}

	// create a file-reading object for target-file
	std::ifstream finTargets;
	finTargets.open(std::string(argv[1])); // open input file
	if (!finTargets.good())
		std::cerr << "ERROR: target file not found! " << std::string(argv[1]) << std::endl;

	// read from target file
	std::string line;
	std::getline(finTargets, line); // ingnore first line (header)

	// write to output file
	std::ofstream fout;
	fout.open(std::string(argv[2]) + "../corpus-initial.csv");
	fout << "name,initialf0,#syllables" << std::endl;

	while (std::getline(finTargets, line))
	{
		// get label and number of syllables
		std::vector<std::string> tokens;
		utilities::split(tokens, line, ",");
		std::string label = tokens[0];
		fout << label << "," << (tokens.size()-1)/6 << ",";

		std::ifstream finF0;
		finF0.open(std::string(argv[2]) + label + ".qtaf0sampled"); // open input file
		if (!finF0.good())
			std::cerr << "ERROR: F0 sample file not found! " << std::string(argv[2]) << std::endl;

		// read from target file
		std::getline(finF0, line); // ingnore first line (header)
		std::getline(finF0, line);
		tokens.clear();

		// read F0 value
		utilities::split(tokens, line, "\t");
		fout << std::stod(tokens[2]) << std::endl;
	}

	return 0;
}
