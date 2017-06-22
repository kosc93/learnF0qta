#include "utilities.h"
#include <cmath>

void utilities::split(std::vector<std::string> &theStringVector, const std::string &theString, const std::string &theDelimiter)
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

// calculate binomial coefficient
double utilities::binomial_coefficient	(const unsigned int &n, const unsigned int &k)
{
	double result = 1;
	unsigned int tmp = k;

	if ( tmp > n - tmp )
		tmp = n - tmp;

	for (unsigned int i = 0; i < tmp; ++i)
	{
		result *= (n - i);
		result /= (i + 1);
	}

	return result;
}

// calculate factorial recursively
double utilities::factorial (unsigned int n)
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

// calculate abbreviation numerically
void utilities::numerical_differentiation (const std::vector<double> &inputFunction, const double &stepSize, std::vector<double> &resultVector)
{
	for (unsigned int i = 1; i < inputFunction.size(); ++i)
	{
		resultVector.push_back((inputFunction[i] - inputFunction[i-1])/stepSize);
	}
}

// calculate mean
double utilities::mean (const std::vector<double> &x)
{
	double result = 0.0;
	unsigned int N = x.size();

	for (unsigned int i=0; i<N; ++i)
	{
		result += x[i];
	}

	return (result/N);
}

// calculate variance
double utilities::variance (const std::vector<double> &x)
{
	double result = 0.0;
	double meanVal = mean(x);
	unsigned int N = x.size();

	for (unsigned int i=0; i<N; ++i)
	{
		result += std::pow((x[i] - meanVal),2);
	}

	return (result/N);
}

// calculate root mean square error
double utilities::rmse (const std::vector<double> &x, const std::vector<double> &y)
{
	double result = 0.0;
	unsigned int N = x.size();

	for (unsigned int i=0; i<N; ++i)
	{
		result += (std::pow(y[i] - x[i],2));
	}

	result = std::sqrt(result/(double)N);
	return result;
}

// calculate Pearson correlation coefficient
double utilities::correlation (const std::vector<double> &x, const std::vector<double> &y)
{
	double result = 0.0;
	unsigned int N = x.size();

	// get means
	double xMean = 0.0, yMean = 0.0;
	for (unsigned int i=0; i<N; ++i)
	{
		xMean += x[i];
		yMean += y[i];
	}
	xMean /= N;
	yMean /= N;

	// get variance
	double xVar = 0.0, yVar = 0.0;
	for (unsigned int i=0; i<N; ++i)
	{
		xVar += std::pow((x[i] - xMean),2);
		yVar += std::pow((y[i] - yMean),2);
	}

	// calculate correlation coefficient
	for (unsigned int i=0; i<N; ++i)
	{
		result += ((x[i] - xMean)*(y[i] - yMean));
	}

	result /= (std::sqrt(xVar*yVar));
	if (std::isnan(result))
	{
		return 0.0;
	}

	return result;
}
