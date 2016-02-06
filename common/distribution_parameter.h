#ifndef DISTRIBUTION_PARAMETER_H
#define DISTRIBUTION_PARAMETER_H

#include "generator.h"
#include "parameter.h"

namespace simulator
{
class DistributionParameter : public IParameter
{
public:
    DistributionParameter() {}

    /**
    * @brief add parameters
    */
    void Add(std::string name, GeneratorPtr generatorPtr);

    /* override */
    int64_t GetQps();

    /* override */
    std::map<std::string, std::string> GetParameter();
private:
    
    // map of parameters'name to distribution object.
    std::map<std::string, GeneratorPtr> mDistribution; 
};
}

#endif
