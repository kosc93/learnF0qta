/*
 * main.cpp
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#include "Word.h"
#include "Exception.h"
#include "StringOps.h"
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	// user message
	std::string message;
	message = "Usage:\n(file)\t./sampa2vec -f <INPUT-FILE>  <OUTPUT-FILE>\n(word)\t./sampa2vec -w <SAMPA-TRANS> <ACCENT-PATTERN>\n(debug)\t./sampa2vec -d <SAMPA-TRANS> <ACCENT-PATTERN>\n";

    // Check the number of parameters
    if (argc != 4)
    {
		std::cerr << message;
		return 1;
    }

    try
    {
    	std::string option (argv[1]);
    	if (option == "-w")
    	{
    		Word word (argv[2], argv[3]);
    		word.print();
    	}
    	else if (option == "-d")
    	{
    		Word word (argv[2], argv[3]);
    		word.print_debug();
    	}
    	else if (option == "-f")
		{
    		// create a file-reading object
    		std::ifstream fin;
    		fin.open(std::string(argv[2])); // open input file
    		if (!fin.good())
    			std::cerr << "ERROR: input file not found! " << std::string(argv[2]) << std::endl;

    		// create output file and write results to it
			std::ofstream fout;
			fout.open (argv[3]);
			fout << "Word,Features" << std::endl;

    		// data
    		std::string line;
    		std::vector<std::string> tokens;

    		int cnt (0);
    		// compute input file
    		std::getline(fin, line); // ingnore first line (header)
    		while (std::getline(fin, line))
    		{
    			++cnt;
    			split(tokens, line, ",");
    			Word word (tokens[1], tokens[2]);
    			fout << tokens[0] << "," << word.get_output_string() << std::endl;
    			// std::cout << "(" << cnt << ") processed: " << tokens[0] << " [" << tokens[1] << "]" << std::endl;
    			tokens.clear();
    		}

    		std::cout << "\tCalculated " << cnt << " feature vectors successfully!" << std::endl;
		}
    	else
    	{
    		std::cerr << message;
    		return 1;
    	}

    }
    catch(std::exception& e)
    {
    	std::cout << e.what() << std::endl;
    }

    return 0;
}



