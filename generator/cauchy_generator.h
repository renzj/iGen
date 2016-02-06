#ifndef CAUCHY_GENERATOR_H
#define CAUCHY_GENERATOR_H

#include "generator.h"
#include "reflection.h"

namespace porcupine
{

class  CauchyGenerator : public Generator
{
public:
    
    // standard cauchy distribution
    CauchyGenerator() : mLocation(0), mScale(1) {}

    CauchyGenerator(double location, double scale);

    /* override */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();

private:
    // location parameter, x0
    double mLocation;
    // scale parameter, r
    double mScale;
};

typedef std::tr1::shared_ptr<CauchyGenerator> CauchyGeneratorPtr;
}

#endif
