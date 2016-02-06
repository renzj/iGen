#ifndef LAGRANGE_FITTING_H
#define LAGRANGE_FITTING_H

#include "fitting.h"

namespace porcupine
{

class LagrangeFitting : public Fitting
{
public:
	LagrangeFitting() {}

	~LagrangeFitting() {}

	/* override */
	void Init(const std::string& file);

	/* override */
	double Get(double x) const;
private:
	Polynomial	mPolynomial;
};

typedef std::tr1::shared_ptr<LagrangeFitting> LagrangeFittingPtr;

}


#endif
