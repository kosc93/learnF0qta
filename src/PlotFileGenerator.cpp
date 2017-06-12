#include "PlotFileGenerator.h"

#include <fstream>
#include <sys/stat.h>

void PlotFileGenerator::split( std::vector<std::string> & theStringVector,  /* Altered/returned value */
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

PlotFileGenerator::PlotFileGenerator (std::string targetLine, std::string path)
{
	// get input information
	m_path = path;
	std::vector<std::string> tokens;
	split(tokens, targetLine, "\t");
	m_name = tokens[0];
	tokens.erase(tokens.begin());
	for (std::vector<std::string>::iterator it = tokens.begin() ; it != tokens.end(); ++it)
	{
		m_targetLineValues.push_back(std::stod(*it));
	}

	// compute input information
	read_data_file();
	analyze_target_line();
	generate_plot_file();
	call_gnuplot();
}

void PlotFileGenerator::read_data_file()
{
	// create a file-reading object for data file (semitone f0)
	std::string fileName = m_path + m_name + ".semitonef0";
	std::ifstream finF0;
	finF0.open(fileName); // open input file
	if (!finF0.good())
		throw Exception("ERROR: semitonef0 file not found! " + fileName);

	// read from file
	std::string line;
	std::vector<std::string> tokens;
	std::getline(finF0, line); // ingnore first line (header)
	std::getline(finF0, line); // read second line
	split(tokens, line, "\t");

	// get relevant data
	m_timeOffset = std::stod(tokens[1]);	// contains first sample time point
}

void PlotFileGenerator::analyze_target_line()
{
	// get initial f0
	m_initialF0 = m_targetLineValues[0];

	// get target specific values
	for (unsigned int i=3; i < m_targetLineValues.size(); i+=6)
	{
		m_slope.push_back(m_targetLineValues[i+0]);
		m_offset.push_back(m_targetLineValues[i+1]);
		m_strength.push_back(m_targetLineValues[i+2]);
		m_duration.push_back(m_targetLineValues[i+3]);
		m_rmse.push_back(m_targetLineValues[i+4]);
	}
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
	fout << std::endl << "###### general settings #####" << std::endl;
	fout << "stats '" << m_name <<".semitonef0' using 2:3 nooutput" << std::endl;
	fout << "set xrange [STATS_min_x:STATS_max_x]" << std::endl;
	fout << "set yrange [STATS_min_y-2:STATS_max_y+2]" << std::endl;
	fout << "set title 'F0 - " << m_name <<"'" << std::endl;
	fout << "set xlabel 'Time [sec]'" << std::endl;
	fout << "set ylabel 'Frequency [st]'" << std::endl;
	fout << "set datafile missing '0'" << std::endl;

	// write syllable bound information to plot file
	double current = m_timeOffset;
	fout << std::endl << "##### plot syllable bounds #####" << std::endl;
	fout << "set arrow from " << std::to_string(current) << ",STATS_min_y-2 to " << std::to_string(current) << ",STATS_max_y+2 nohead dt 3 lt -1" << std::endl;
	for (std::vector<double>::iterator it = m_duration.begin() ; it != m_duration.end(); ++it)
	{
		current += *it;
		fout << "set arrow from " << std::to_string(current) << ",STATS_min_y-2 to " << std::to_string(current) << ",STATS_max_y+2 nohead dt 3 lt -1" << std::endl;
	}

	// write target information
	current = m_timeOffset;
	fout << std::endl << "###### plot targets #####" << std::endl;
	for (unsigned int i=0; i<m_duration.size(); ++i)
	{
		double t1 = current;
		double t2 = t1 + m_duration[i];
		current = t2;
		double f2 = m_initialF0 + m_offset[i];
		double f1 = f2 - m_slope[i]*m_duration[i];
		fout << "set arrow from " << std::to_string(t1) << "," << std::to_string(f1) << " to " << std::to_string(t2) << "," << std::to_string(f2) << " nohead dt 4" << std::endl;
	}

	// write f0 curve information
	fout << std::endl << "###### plot curves from data points #####" << std::endl;
	fout << "plot '" << m_name <<".semitonef0' using 2:3 title 'Original F0' with linespoints lt -1 lc black lw 2 dt 1, '" << m_name <<".qTAf0' using 2:3 title 'Resynthesized F0' with linespoints lt -1 lc black lw 1 dt 2" << std::endl;
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
