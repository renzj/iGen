#include "pareto_generator.h"

namespace porcupine
{

REGISTER(ParetoGenerator);

ParetoGenerator::ParetoGenerator(double alpha, double location) : mAlpha(alpha), mLocation(location)
{
}

void ParetoGenerator::Init(std::map<std::string, double> params)
{
	assert(params.count("alpha"));
	assert(params.count("location"));
	mAlpha = params["alpha"];
	mLocation = params["location"];
}

double ParetoGenerator::Get()
{
	return mLocation / pow(GetProbability(), 1 / mAlpha);
}



}
