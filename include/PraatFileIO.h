/*
 * PraatFileIO.h
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <vector>
#include <string>
#include <iostream>

#include "ApproximationSystem.h"

#ifndef PRAATFILEIO_H_
#define PRAATFILEIO_H_

class PraatFileIO {
public:
	// constructor: analyze input files
	PraatFileIO(ApproximationSystem &system, std::string &path);

	// generate output file
	void generate_output_file(ApproximationSystem &system);

private:
	// small helper function
	static void split( std::vector<std::string> & theStringVector, const  std::string  & theString, const  std::string  & theDelimiter);

	// parser for input files
	void analyze_config_file(ApproximationSystem &system);
	void analyze_data_file(ApproximationSystem &system);

	// member
	std::string m_path;
};

#endif /* PRAATFILEIO_H_ */
