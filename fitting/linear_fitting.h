#ifndef LINEAR_FITTING_H
#define LINEAR_FITTING_H

#include "fitting.h"

namespace porcupine
{
/**
* @brief fitting y = a + bx, a is {mIntecept} while be is {mSlope}
*/
class LinearFitting : public Fitting
{
public:
	LinearFitting () {}

	~LinearFitting() {}

	/* override */
	void Init(const std::string& file);

	/* override */
	double Get(double x) const;

private:

	double mIntercept;

	double mSlope;
};

typedef std::tr1::shared_ptr<LinearFitting> LinearFittingPtr;
}

#endif
