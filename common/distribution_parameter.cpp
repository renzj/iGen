#include <sstream>
#include <iomanip>
#include "distribution_parameter.h"

namespace simulator
{

static const int64_t MaxQps = 10000000000;
void DistributionParameter::Add(std::string name, GeneratorPtr generatorPtr)
{
    mDistribution[name] = generatorPtr;
}

int64_t DistributionParameter::GetQps()
{
    std::map<std::string, int64_t> res;
    if(!mDistribution.count("Qps"))
    {
        return MaxQps;
    }
    return static_cast<int64_t>(mDistribution["Qps"]->Get());
}

std::map<std::string, std::string> DistributionParameter::GetParameter()
{
    std::map<std::string, GeneratorPtr>::iterator iter = mDistribution.begin();
    std::map<std::string, std::string> res;
    for(; iter != mDistribution.end(); ++iter)
    {
        if(iter->first == "Qps")
            continue;
        double val = iter->second->Get();
        std::stringstream ss;
        // if the predision is more than 1, apsara::StringTo throws error
        ss << std::fixed << std::setprecision(0) << val;
        res[iter->first] = ss.str();
    }
    return res;
}

}
