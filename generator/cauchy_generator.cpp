#include "cauchy_generator.h"

namespace porcupine
{
REGISTER(CauchyGenerator);

CauchyGenerator::CauchyGenerator(double locatioon, double scale) : mLocation(locatioon), mScale(scale)
{
    assert(mScale > 0);
}

void CauchyGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("location") && params.count("scale"));
    mLocation = params["location"];
    mScale = params["scale"];
    assert(mScale > 0);
}

double CauchyGenerator::Get()
{
    double p = GetProbability();
    return mLocation + mScale * tan(PI * (p - 0.5));
}

}
