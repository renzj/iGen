#include "exponential_fitting.h"

namespace porcupine
{
void ExponentialFitting::Init(const std::string& file)
{
	std::vector<double> x, y, tmpx, tmpy;
	Load(file, tmpx, tmpy);

	// remove points with y = 0
	for(size_t i = 0;i < tmpx.size(); ++i)
		if(tmpy[i])
		{
			x.push_back(tmpx[i]);
			y.push_back(tmpy[i]);
		}
	
	// sum of x^2 * y
	double sum1 = 0;	
	for(size_t i = 0;i < x.size(); ++i)
		sum1 += x[i] * x[i] * y[i];

	// sum of y * ln(y)
	double sum2 = 0;	
	for(size_t i = 0;i < x.size(); ++i)
		sum2 += y[i] * log(y[i]);

	// sum of x * y
	double sum3 = 0;	
	for(size_t i = 0;i < x.size(); ++i)
		sum3 += x[i] * y[i];

	// sum of x * y * ln(y)
	double sum4 = 0;	
	for(size_t i = 0;i < x.size(); ++i)
		sum4 += x[i] * y[i] * log(y[i]);

	// sum of y
	double sumOfy = accumulate(y.begin(), y.end(), 0);

	mIntercept = (sum1 * sum2 - sum3 * sum4) / (sumOfy * sum1 - sum3 * sum3);
	mSlope = (sumOfy * sum4 - sum3 * sum2) / (sumOfy * sum1 - sum3 * sum3);
}

double ExponentialFitting::Get(double x) const
{
	return pow(2.718281828459, mSlope * x + mIntercept);
}


}
