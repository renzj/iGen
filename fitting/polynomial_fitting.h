#ifndef POLYNOMIAL_FITTING_H
#define POLYNOMIAL_FITTING_H

#include "fitting.h"

namespace porcupine
{
class PolynomialFitting : public Fitting
{
public:
	PolynomialFitting () {}

	~PolynomialFitting() {}

	/* override */
	void Init(const std::string& file);

	/* override */
	double Get(double x) const;

	void SetPower(uint32_t p);

private:

	Polynomial mCofficient;

	uint32_t mPower;
};

}

#endif
