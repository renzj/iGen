#ifndef UNIFORM_GENERATOR_H
#define UNIFORM_GENERATOR_H

#include "generator.h"
#include "reflection.h"
#include <string>
#include <map>

namespace porcupine
{

class UniformGenerator : public Generator
{
public:

    UniformGenerator() : mA(0), mB(10) {}

    /**
    * @brief Construct with [a, b]
    */
    UniformGenerator(double a, double b);

    /**
    * @brief Construct with map, map's keys are parameters in string.
    */
    void Init(std::map<std::string, double> params);

    /* override */
    double Get();

private:

    // left value
    double mA;

    // right value
    double mB;
};


typedef std::tr1::shared_ptr<UniformGenerator> UniformGeneratorPtr;
}
#endif
