/*
 * PraatFileIO.h
 *
 *  Created on: 24 May 2017
 *      Author: patrick
 */

#include <vector>
#include <string>
#include <iostream>

#include "SystemTA.h"

#ifndef PRAATFILEIO_H_
#define PRAATFILEIO_H_

class PraatFileIO {
public:
	// constructor: analyze input files
	PraatFileIO(SystemTA &system, std::string &path);

	// generate output file
	void generate_output_file(SystemTA &system);

private:
	// parser for input files
	void analyze_config_file(SystemTA &system);
	void analyze_data_file(SystemTA &system);

	// member
	std::string m_path;
};

#endif /* PRAATFILEIO_H_ */
