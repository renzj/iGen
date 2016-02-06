#include "constant_generator.h"

namespace porcupine
{
REGISTER(ConstantGenerator);

ConstantGenerator::ConstantGenerator(double value) : mValue(value)
{
}

void ConstantGenerator::Init(std::map<std::string, double> params)
{
    mValue = params["value"];
}

double ConstantGenerator::Get()
{
    return mValue;
}

}
