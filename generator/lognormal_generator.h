#ifndef LOGNORMAL_GENERATOR_H
#define LOGNORMAL_GENERATOR_H

#include "generator.h"
#include "reflection.h"
#include <map>
#include <string>

namespace porcupine
{

class LogNormalGenerator : public Generator
{
public:
    LogNormalGenerator() : mLogMu(1.0), mLogSigma(0) {}
    
    LogNormalGenerator(double logMu, double logSigma);

    /* override */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();


private:
    double  mLogMu;
    double  mLogSigma;
};

typedef std::tr1::shared_ptr<LogNormalGenerator> LogNormalGeneratorPtr;

}




#endif
