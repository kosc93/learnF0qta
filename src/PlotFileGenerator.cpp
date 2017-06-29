#include "PlotFileGenerator.h"

#include <fstream>
#include <sys/stat.h>
#include "utilities.h"

PlotFileGenerator::PlotFileGenerator (std::string targetLine, std::string path, double shift)
{
	// get input information
	m_path = path;
	m_shift = shift;
	std::vector<std::string> tokens;
	utilities::split(tokens, targetLine, ",");
	m_name = tokens[0];
	tokens.erase(tokens.begin());
	for (std::vector<std::string>::iterator it = tokens.begin() ; it != tokens.end(); ++it)
	{
		std::string value = it->substr(it->find(":")+1, it->size());
		m_targetLineValues.push_back(std::stod(value));
	}

	// compute input information
	analyze_target_line();
	analyze_syllable_bounds();
	generate_plot_file();
	call_gnuplot();
}

void PlotFileGenerator::analyze_target_line()
{
	// get target specific values
	for (unsigned int i=0; i < m_targetLineValues.size(); i+=6)
	{
		m_slope.push_back(m_targetLineValues[i+0]);
		m_offset.push_back(m_targetLineValues[i+1]);
		m_strength.push_back(m_targetLineValues[i+2]);
		m_duration.push_back(m_targetLineValues[i+3]);
		m_rmse.push_back(m_targetLineValues[i+4]);
		m_corr.push_back(m_targetLineValues[i+5]);
	}

	// calculate rmseMean
	m_rmseMean = 0;
	for (unsigned int i=0; i<m_rmse.size(); ++i)
	{
		m_rmseMean += m_rmse[i];
	}
	m_rmseMean /= m_rmse.size();
}

void PlotFileGenerator::analyze_syllable_bounds()
{
	// create a file-reading object for TextGrid file (syllable bounds)
	std::string fileName = m_path + m_name + ".TextGrid";
	std::ifstream finF0;
	finF0.open(fileName); // open input file
	if (!finF0.good())
		throw Exception("ERROR: TextGrid file not found! " + fileName);

	std::string line;
	std::vector<std::string> tokens;

	// ignore first 13 lines
	for (int i=0; i<13; ++i)
		std::getline(finF0, line);

	// get number of intervals
	std::getline(finF0, line);
	tokens.clear();
	utilities::split(tokens, line, " = ");
	int numberIntervals = std::stoi(tokens[1]);

	// get first syllable bound
	std::getline(finF0, line); // ingnore line "intervals [1]:"
	std::getline(finF0, line); // ingnore line "xmin = 0 "
	std::getline(finF0, line);
	tokens.clear();
	utilities::split(tokens, line, " = ");
	m_bounds.push_back(std::stod(tokens[1]));

	// get further syllable bounds
	for (int i=1; i<numberIntervals-2; ++i)
	{
		std::getline(finF0, line); // ingnore line "intervals [1]:"
		std::getline(finF0, line); // ingnore line "xmin = 0 "
		std::getline(finF0, line);
		std::getline(finF0, line);
		tokens.clear();
		utilities::split(tokens, line, " = ");
		m_bounds.push_back(std::stod(tokens[1])-(m_shift/1000));
	}

	// get last syllable bounds
	std::getline(finF0, line); // ingnore line "intervals [1]:"
	std::getline(finF0, line); // ingnore line "xmin = 0 "
	std::getline(finF0, line);
	std::getline(finF0, line);
	tokens.clear();
	utilities::split(tokens, line, " = ");
	m_bounds.push_back(std::stod(tokens[1]));
}

void PlotFileGenerator::generate_plot_file()
{
	// create directory for plots and output file (plot file)
	std::string path = m_path + "../plots/";
	const int dir_err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	if (-1 == dir_err)
	{
		//std::cout << "ERROR creating directory: " << m_path + "plots/" << std::endl;
	}

	std::ofstream fout;
	std::string fileName = m_path + m_name + ".plot";
	fout.open(fileName);

	/***** write image file information to plot file *****/
	fout << "##### output file options #####" << std::endl;
	fout << "set terminal pngcairo size 700,254 enhanced font 'Verdana,10'" << std::endl;
	fout << "set output '../plots/" << m_name <<".png'" << std::endl;

	// write general plot settings to plot file
	fout << std::endl << "##### general settings #####" << std::endl;
	fout << "set datafile missing '0'" << std::endl;
	fout << "stats '" << m_name <<".qtaf0sampled' using 2:3 nooutput" << std::endl;
	fout << "set xrange [" << m_bounds[0] << ":" << m_bounds[m_bounds.size()-1] <<"]" << std::endl;
	fout << "set yrange [STATS_min_y-2:STATS_max_y+2]" << std::endl;
	fout << "set title 'F0 - " << m_name <<" (rmse=" << m_rmseMean <<")'" << std::endl;
	fout << "set xlabel 'Time [sec]'" << std::endl;
	fout << "set ylabel 'Frequency [st]'" << std::endl;

	//  write syllable bound information to plot file
	fout << std::endl << "##### plot syllable bounds #####" << std::endl;
	for (unsigned int i=0; i<m_bounds.size(); ++i)
	{
		fout << "set arrow from " << std::to_string(m_bounds[i]) << ",STATS_min_y-2 to " << std::to_string(m_bounds[i]) << ",STATS_max_y+2 nohead dt 3 lt -1" << std::endl;
	}

	// write target information
	fout << std::endl << "###### plot targets #####" << std::endl;
	for (unsigned int i=0; i<m_slope.size(); ++i)
	{
		double t1 = m_bounds[i];
		double t2 = m_bounds[i+1];

		double f1 = m_offset[i];
		double f2 = m_offset[i] + m_slope[i]*(m_bounds[i+1] - m_bounds[i]);
		fout << "set arrow from " << std::to_string(t1) << "," << std::to_string(f1) << " to " << std::to_string(t2) << "," << std::to_string(f2) << " nohead dt 4" << std::endl;
	}

	// write f0 curve information
	fout << std::endl << "###### plot curves from data points #####" << std::endl;
	fout << "plot '" << m_name <<".semitonef0' using 2:3 title 'Original F0' with points lt 3 lc '#0000FF' lw 1,\\" << std::endl;
	fout << " '" 	 << m_name <<".qtaf0' using 2:3 title 'Resynthesized F0' with points lt 1 lc black lw 2,\\" << std::endl;
	fout << " '" 	 << m_name <<".qtaf0sampled' using 2:3 notitle with linespoints lt -1 lc black lw 1" << std::endl;

}

void PlotFileGenerator::call_gnuplot()
{
	std::string command = "cd " + m_path + "; gnuplot " + m_name + ".plot";
	const int dir_err = system(command.c_str());
	if (-1 == dir_err)
	{
		std::cout << "ERROR opening gnuplot: " << command << std::endl;
	}
}
