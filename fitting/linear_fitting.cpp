#include "linear_fitting.h"

namespace porcupine
{

void LinearFitting::Init(const std::string& file)
{
	std::vector<double> x, y;
	Load(file, x, y);
	double sumOfx = accumulate(x.begin(), x.end(), 0);
	double meanOfx = sumOfx / x.size();
	double sumOfy = accumulate(y.begin(), y.end(), 0);
	double meanOfy = sumOfy / y.size();
	double sumOfSquareOfx = accumulate(x.begin(), x.end(), 0, Square<double>());
	double sumOfSquareOfy = accumulate(y.begin(), y.end(), 0, Square<double>());
	std::vector<double> product(x.size(), 0);
	for(size_t i = 0;i < x.size(); ++i)
	{
		product[i] = x[i] * y[i];
	}
	double sumOfProductOfxy = accumulate(product.begin(), product.end(), 0);
	mIntercept = (meanOfy * sumOfSquareOfx - meanOfx * sumOfProductOfxy) / (sumOfSquareOfx - x.size() * meanOfx * meanOfx);
	mSlope = (sumOfProductOfxy - x.size() * meanOfx * meanOfy) / (sumOfSquareOfx - x.size() * meanOfx * meanOfx);
}

double LinearFitting::Get(double x) const
{
	return mSlope * x + mIntercept;
}

}
