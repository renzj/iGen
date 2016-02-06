#include "parameter.h"
#include <sstream>
#include <iomanip>

namespace porcupine
{

extern const uint32_t gClosedSystem = 0;
extern const uint32_t gOpenSystem = 1;
extern const uint32_t gPartlyOpenSystem = 2;

void Parameter::Add(const std::string& key, GeneratorPtr generator)
{
	mNumericParameters[key] = generator;
}

void Parameter::Add(const std::string& key, const std::string& value)
{
	mStringParameters[key] = value;
}

void Parameter::Add(const std::string& key, const double percent)
{
	mPercent[key] = percent;
}

std::map<std::string, std::string> Parameter::GetParameter()
{
	std::map<std::string, GeneratorPtr>::iterator iter = mNumericParameters.begin();
	std::map<std::string, std::string> res;
	for(;iter != mNumericParameters.end(); ++iter)
	{
		if(!mPercent.count(iter->first))
		{
			double val = iter->second->Get();
			std::stringstream ss;
			ss << std::fixed << std::setprecision(6) << val;
			res[iter->first] = ss.str();
		}
	}
	DefaultRandom pRandom;
	double percent = pRandom.Get(0, 101);
	double sum = 0;
	for(std::map<std::string, double>::iterator iter = mPercent.begin(); iter != mPercent.end(); ++iter)
	{
		sum += iter->second;
		if(percent <= sum)
		{
			double val = mNumericParameters[iter->first]->Get();
			std::stringstream ss;
			ss << std::fixed << std::setprecision(6) << val;
			res[iter->first] = ss.str();
			break;
		}
	}
	res.insert(mStringParameters.begin(), mStringParameters.end());
	return res;
}

void Parameter::SetRuntime(const uint32_t runtime)
{
	mRuntime = runtime;
}

uint32_t Parameter::GetRuntime() const
{
	return mRuntime;
}

void Parameter::SetNumOfThreads(const uint32_t numOfThreads)
{
	mNumOfThreads = numOfThreads;
}

uint32_t Parameter::GetNumOfThreads() const
{
	return mNumOfThreads;
}

}

