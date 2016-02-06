#include "exponent_generator.h"

namespace porcupine
{
REGISTER(ExponentGenerator);

ExponentGenerator::ExponentGenerator(double lambda) : mLambda(lambda) 
{
    assert(mLambda > 0);
}

void ExponentGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("lambda"));
    mLambda = params["lambda"];
    assert(mLambda > 0);
}

double ExponentGenerator::Get()
{
    double res = -1.0 / mLambda * log(1.0 - GetProbability());
	if(!res)
		res = 1.0 / mLambda;
	return res;
}

}
