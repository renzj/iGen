#include "lognormal_generator.h"

namespace porcupine
{

REGISTER(LogNormalGenerator);

LogNormalGenerator::LogNormalGenerator(double logMu, double logSigma) : mLogMu(logMu), mLogSigma(logSigma)
{
}

void LogNormalGenerator::Init(std::map<std::string, double> params)
{
    assert(params.count("logmu") && params.count("logsigma"));
    mLogMu = params["logmu"];
    mLogSigma = params["logsigma"];
}

double LogNormalGenerator::Get()
{
    static int flag = 0;

    double u1 = GetProbability();
    double u2 = GetProbability();

    double r = sqrt(-2 * log(u2));
    double theta = 2 * PI * u1;

    double z = 0.0;
    if(!flag)
        z = r * sin(theta);
    else
        z = r * cos(theta);
    flag = 1 - flag;
    return exp(mLogMu + z * mLogSigma);
}

}
