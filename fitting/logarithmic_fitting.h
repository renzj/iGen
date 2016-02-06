#ifndef LOGARITHMIC_FITTING_H
#define LOGARITHMIC_FITTING_H

#include "fitting.h"

namespace porcupine
{

/**
* @brief fitting y = A + B * ln(x) where A is {mIntercept} and B is {mSlope}
*/
class LogarithmicFitting : public Fitting
{
public:
	LogarithmicFitting() {}

	~LogarithmicFitting() {}

	/* override */
	void Init(const std::string& file);

	/* override */
	double Get(double x) const;

private:
	double mSlope;

	double mIntercept;
};

}

#endif
