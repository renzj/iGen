#include "lagrange_fitting.h"
#include <iostream>
#include <stdexcept>

namespace porcupine
{
void LagrangeFitting::Init(const std::string& file)
{
	std::vector<double> x_array;
	std::vector<double> y_array;
	Load(file, x_array, y_array);	
	// check if there is two points with the same x value
	for(size_t i = 0;i < x_array.size(); ++i)
	{
		for(size_t j = i + 1;j < x_array.size(); ++j)
			if(x_array[i] == x_array[j])
			{
				std::string error = std::string("There are two points with the same x value");
				LOG(FATAL) << error << std::endl;
				throw std::logic_error(error);
			}
	}
	mPolynomial = Polynomial();
	for(size_t i = 0;i < x_array.size(); ++i)
	{
		std::vector<double> v(1, y_array[i]);	
		Polynomial tmp(v);
		for(size_t j = 0;j < x_array.size(); ++j)
			if(i != j)
			{
				tmp = tmp / (x_array[i] - x_array[j]);
				std::vector<double> v2(2);
				v2[0] = -1 * x_array[j];
				v2[1] = 1;
				tmp = tmp * Polynomial(v2);
			}
		mPolynomial = mPolynomial + tmp;
	}
}

double LagrangeFitting::Get(double x) const
{
	return mPolynomial.Get(x);
}

}
