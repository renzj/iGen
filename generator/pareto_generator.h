#ifndef PARETO_GENERATOR
#define PARETO_GENERATOR

#include "generator.h"
#include "reflection.h"

namespace porcupine
{
class ParetoGenerator : public Generator
{
public:
	ParetoGenerator() : mAlpha(1), mLocation(1) {}	

	ParetoGenerator(double alpha, double location);

    /* override */
    void Init(std::map<std::string, double> params);
       
    /* override */
    double Get();
private:
	double mAlpha;
	double mLocation;
};

typedef std::tr1::shared_ptr<ParetoGenerator> ParetoGeneratorPtr;

}

#endif
