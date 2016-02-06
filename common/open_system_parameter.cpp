#include "open_system_parameter.h"

namespace porcupine
{

OpenSystemParameter::OpenSystemParameter() : Parameter()
{
}

uint32_t OpenSystemParameter::GetArrivalTime() const
{
	return static_cast<uint32_t>(mArrivalTime->Get() * 1000000);
}

void OpenSystemParameter::SetArrivalTime(GeneratorPtr arrivalTime)
{
	mArrivalTime = arrivalTime;
}


}
