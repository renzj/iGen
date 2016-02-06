#ifndef constant_generator_h
#define constant_generator_h

#include "generator.h"
#include "reflection.h"

namespace porcupine
{
class ConstantGenerator : public Generator
{
public:
    ConstantGenerator() : mValue(0) {}

    ConstantGenerator(double value);

    /* override */
    void Init(std::map<std::string, double> params);
       
    /* override */
    double Get();

private:
    double mValue;
};

typedef std::tr1::shared_ptr<ConstantGenerator> ConstantGeneratorPtr;
}
#endif
