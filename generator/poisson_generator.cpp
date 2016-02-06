#include "poisson_generator.h"

namespace porcupine
{
REGISTER(PoissonGenerator);
PoissonGenerator::PoissonGenerator(double lambda) : mLambda(lambda)
{
    assert(lambda >= 0);
}

void PoissonGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("lambda"));
    mLambda = params["lambda"];
    assert(mLambda >= 0);
}

double PoissonGenerator::Get()
{
    double num = -1;
    double log1, log2;
    log1 = 0;
    log2 = -mLambda;
    do
    {
        double p = GetProbability();
        log1 += log(p);
        num++;
    }while(log1 >= log2);
    return num;
}

}
