#include "geometric_generator.h"

namespace porcupine
{
REGISTER(GeometricGenerator);

GeometricGenerator::GeometricGenerator(double p) : mProbability(p)
{
    assert(p > 0.0 && p <= 1.0);
}

void GeometricGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("p") && params["p"] > 0.0 && params["p"] <= 1.0);
    mProbability = params["p"];
}

double GeometricGenerator::Get()
{
    double u = GetProbability();
	return floor(log(1 - u) / log(1 - mProbability));
}

}
