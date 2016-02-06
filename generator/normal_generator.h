#ifndef NORMAL_GENERATOR_H
#define NORMAL_GENERATOR_H

#include "generator.h"
#include "reflection.h"

namespace porcupine
{

class NormalGenerator : public Generator
{
public:

    // standard normal distribution
    NormalGenerator() : mMu(0), mSigma(1) {}

    NormalGenerator(double mu, double sigma);

    /* override */
    void Init(std::map<std::string, double> params);
       
    /* override */
    double Get();
private:
    
    // mean value
    double mMu;
    
    // standard deviation
    double mSigma;
};

typedef std::tr1::shared_ptr<NormalGenerator> NormalGeneratorPtr;

}

#endif
