#ifndef EXPONENT_GENERATOR_H
#define EXPONENT_GENERATOR_H

#include "generator.h"
#include "reflection.h"

namespace porcupine
{
class ExponentGenerator : public Generator
{
public:
    ExponentGenerator() : mLambda(1) {}
    
    ExponentGenerator(double lambda);

    /* override */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();

private:
    // average value
    double mLambda;
};

typedef std::tr1::shared_ptr<ExponentGenerator> ExponentGeneratorPtr;
}

#endif
