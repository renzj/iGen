#ifndef POISSON_GENERATOR_H
#define POISSON_GENERATOR_H

#include "generator.h"
#include "reflection.h"
#include <map>
#include <string>

namespace porcupine
{

class PoissonGenerator : public Generator
{
public:
    PoissonGenerator() : mLambda(1) {}

    PoissonGenerator(double lambda);

    /* override */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();
private:
    // average value
    double  mLambda;
};

typedef std::tr1::shared_ptr<PoissonGenerator> PoissonGeneratorPtr;
}

#endif
