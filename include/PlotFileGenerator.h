#ifndef PLOTFILEGENERATOR_H_
#define PLOTFILEGENERATOR_H_

#include <iostream>
#include <string>
#include <vector>
#include "Exception.h"

class PlotFileGenerator {
public:
	PlotFileGenerator (std::string targetLine, std::string path);

private:
	// member functions
	void split( std::vector<std::string> & theStringVector, const  std::string  & theString, const  std::string  & theDelimiter);
	void read_data_file ();
	void analyze_target_line ();
	void generate_plot_file ();
	void call_gnuplot ();

	// members
	std::string m_path;
	std::vector<double> m_targetLineValues;
	std::string m_name;
	double m_timeOffset;			// set by read_data_file()
	double m_initialF0;
	std::vector<double> m_slope;
	std::vector<double> m_offset;
	std::vector<double> m_strength;
	std::vector<double> m_duration;
	std::vector<double> m_rmse;

};

#endif /* PLOTFILEGENERATOR_H_ */
