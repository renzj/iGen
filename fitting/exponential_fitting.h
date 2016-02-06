#ifndef EXPONENTIAL_FITTING_H
#define EXPONENTIAL_FITTING_H

#include "fitting.h"

namespace porcupine
{

/**
* @brief fitting y = A*e^(Bx), transform it to 
* ln(y) = ln(A) + Bx, and ln(A) is {mSlope} while B is {mIntercept}
*/
class ExponentialFitting : public Fitting
{
public:
	ExponentialFitting() {}
	
	~ExponentialFitting() {}

	/* override */
	void Init(const std::string& file);

	/* override */
	double Get(double x) const;
private:

	double mSlope;

	double mIntercept;
};


typedef std::tr1::shared_ptr<ExponentialFitting> ExponentialFittingPtr;
}

#endif
