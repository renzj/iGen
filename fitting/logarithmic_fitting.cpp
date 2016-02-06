#include "logarithmic_fitting.h"

namespace porcupine
{

void LogarithmicFitting::Init(const std::string& file)
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
	// sum of y*ln(x)
	double sum1 = 0;

	// sum of ln(x)
	double sum2 = 0;

	// sum of (ln(x))^2
	double sum3 = 0;
	for(size_t i = 0;i < x.size(); ++i)
	{
		sum1 += y[i] * log(x[i]);
		sum2 += log(x[i]);
		sum3 += pow(log(x[i]), 2);
	}

	// sum of y
	double sumOfy = accumulate(y.begin(), y.end(), 0);

	mSlope = (x.size() * sum1 - sumOfy * sum2) / (x.size() * sum3 - sum2 * sum2);
	mIntercept = (sumOfy - mSlope * sum2) / x.size();
}

double LogarithmicFitting::Get(double x) const
{
	return mIntercept + mSlope * log(x);
}

}
