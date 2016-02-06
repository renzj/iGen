#include "closed_system_parameter.h"

namespace porcupine
{

ClosedSystemParameter::ClosedSystemParameter() : Parameter()
{
}

void ClosedSystemParameter::SetThinkTime(GeneratorPtr thinkTIme)
{
	mThinkTime = thinkTIme;
}

uint32_t ClosedSystemParameter::GetThinkTime() const
{
	return static_cast<uint32_t>(mThinkTime->Get());
}

}
