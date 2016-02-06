#include "uniform_generator.h"

namespace porcupine
{

REGISTER(UniformGenerator);

UniformGenerator::UniformGenerator(double a, double b) : mA(a), mB(b)
{
    assert(a <= b);
}

void UniformGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("ma") && params.count("mb"));
    mA = params["ma"];
    mB = params["mb"];
    assert(mA <= mB);
}

double UniformGenerator::Get()
{
    return GetProbability() * (mB - mA) + mA;
}

}
