#include "normal_generator.h"

namespace porcupine
{
REGISTER(NormalGenerator);

NormalGenerator::NormalGenerator(double mu, double sigma) : mMu(mu), mSigma(sigma)
{
}

void NormalGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("mu") && params.count("sigma"));
    mMu = params["mu"];
    mSigma = params["sigma"];
}

double NormalGenerator::Get()
{
    return mMu + GetGaussian() * mSigma;
}

}
