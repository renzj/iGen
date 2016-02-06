#include "timer_generator.h"
#include "helper.h"
#include "timer.h"

namespace porcupine
{
REGISTER(Timer1Generator);

REGISTER(Timer2Generator);

REGISTER(Timer3Generator);

double Timer1Generator::Get()
{
	return GetCurrentTimeInSeconds() - gStartTime;
}

double Timer2Generator::Get()
{
	uint64_t res = GetCurrentTimeInSeconds() - gStartTime;
	return res;
}
//
void Timer3Generator::Init(std::map<std::string, std::string> params)
{
	assert(params.count("file"));
	mLagrange.Init(params["file"]);
}

double Timer3Generator::Get()
{
	return mLagrange.Get(GetCurrentTimeInSeconds() - gStartTime);
}

}
