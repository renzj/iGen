#ifndef geometric_generator_h
#define geometric_generator_h

#include "generator.h"
#include "reflection.h"

namespace porcupine
{

class GeometricGenerator : public Generator
{
public:
    GeometricGenerator() : mProbability(0.5) {}

    GeometricGenerator(double p);

    /* override */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();
private:
    // probability of success
    double  mProbability;
};

typedef std::tr1::shared_ptr<GeometricGenerator> GeometricGeneratorPtr;
}
#endif
