#include "partlyopen_system_parameter.h"

namespace porcupine
{

PartlyOpenSystemParameter::PartlyOpenSystemParameter() : Parameter()
{
}

PartlyOpenSystemParameter::PartlyOpenSystemParameter(uint32_t runtim) : Parameter(runtime)
{
}

void PartlyOpenSystemParameter::SetArrivalTime(GeneratorPtr arrivalTime)
{
	mArrivalTime = arrivalTime;
}

uint32_t PartlyOpenSystemParameter::GetArrivalTime() const
{
	return static_cast<uint32_t>(mArrivalTime->Get());
}

void PartlyOpenSystemParameter::SetSession(GeneratorPtr session)
{
	mSession = session;
}

uint32_t PartlyOpenSystemParameter::GetSession() const
{
	return static_cast<uint32_t>(mSession->Get());
}

}
