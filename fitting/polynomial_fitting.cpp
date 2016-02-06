#include "polynomial_fitting.h"

namespace porcupine
{

void PolynomialFitting::Init(const std::string& file)
{
	std::vector<double> x;
	std::vector<double> y;
	Load(file, x, y);	
	MatrixData X(x.size(), std::vector<double>(mPower + 1, 0));
	for(size_t i = 0;i < x.size(); ++i)
	{
		X[i][0] = 1;
		for(size_t j = 1; j <= mPower; ++j)
			X[i][j] = X[i][j - 1] * x[i];
	}

	MatrixData Y(1, std::vector<double>(y));

	Matrix xm(X), ym(Y);
	ym = ym.Transpose();
	Matrix res = xm.Transpose() * xm;
	res = res.Inverse() * xm.Transpose() * ym;
	X = res.Transpose().GetData();
	std::cout << "Value: " << std::endl;
	for(int i = 0;i < X[0].size(); ++i)
		std::cout << X[0][i] << " ";
	std::cout << std::endl;
	mCofficient = Polynomial(X[0]);
}


double PolynomialFitting::Get(double x) const
{
	return mCofficient.Get(x);
}

void PolynomialFitting::SetPower(uint32_t p)
{
	mPower = p;
}

}
