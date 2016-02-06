#include "metric.h"

namespace porcupine
{
extern const std::string sQpsType = "qps";
extern const std::string sAvgType = "avg";
extern const std::string sSetType = "set";
extern const std::string sAddType = "add";
extern const std::string sPutType = "put";
extern const std::string sOutputType = "output";
extern const std::string sDefaultCaseName = "DefaultCase";

/********************************************************
*****************Implementation of AddMetric*************
*********************************************************/
AddMetric::AddMetric() : Metric(sDefaultCaseName, sAddType) {}

AddMetric::AddMetric(const std::string& caseName) : Metric(caseName, sAddType) {}

void AddMetric::Set(const std::string& key, const uint64_t value)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    mValueMap[key] = Get(key) + value;
}

void AddMetric::Dump()
{
    std::map<std::string, uint64_t> res;
    GetValueMap(res, false);
    DumpToFile(res);
}

/********************************************************
*****************Implementation of SetMetric*************
*********************************************************/
SetMetric::SetMetric() : Metric(sDefaultCaseName, sSetType) {}

SetMetric::SetMetric(const std::string& caseName) : Metric(caseName, sSetType) {}

void SetMetric::Set(const std::string& key, const uint64_t value)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    mValueMap[key] = value;
}

void SetMetric::Dump()
{
    std::map<std::string, uint64_t> valueMap;
    GetValueMap(valueMap, true);
    DumpToFile(valueMap);
}



/********************************************************
*****************Implementation of QpsMetric*************
*********************************************************/
QpsMetric::QpsMetric() : Metric(sDefaultCaseName, sQpsType) {}

QpsMetric::QpsMetric(const std::string& caseName) : Metric(caseName, sQpsType) {}

void QpsMetric::Set(const std::string& key, const uint64_t value)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    mValueMap[key] = Get(key) + 1;
}

void QpsMetric::Dump()
{
    std::map<std::string, uint64_t> valueMap;
    GetValueMap(valueMap, true);
    DumpToFile(valueMap);
}

/********************************************************
*****************Implementation of AvgMetric*************
*********************************************************/
AvgMetric::AvgMetric() : Metric(sDefaultCaseName, sAvgType) {}

AvgMetric::AvgMetric(const std::string& caseName) : Metric(caseName, sAvgType) {}

void AvgMetric::Set(const std::string& key, const uint64_t value)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    mValueMap[key] = Get(key) + value;
    if(!mCountMap.count(key))
        mCountMap[key] = 1;
    else
        ++mCountMap[key];
}

void AvgMetric::GetValueMap(std::map<std::string, double>& valueMap)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    std::map<std::string, uint64_t>::iterator iter = mValueMap.begin();
    std::map<std::string, uint64_t>::iterator countIter;
    while(iter != mValueMap.end())
    {
        countIter = mCountMap.find(iter->first);
        if(countIter != mCountMap.end() && countIter->second)
        {
            valueMap[iter->first] = iter->second * 1.0 / countIter->second;
        }
        ++iter;
    }
    mValueMap.clear();
    mCountMap.clear();
}

void AvgMetric::Dump()
{
    std::map<std::string, double> valueMap;
    GetValueMap(valueMap);
    DumpToFile(valueMap);
}


/********************************************************
*****************Implementation of PutMetric*************
*********************************************************/
PutMetric::PutMetric() : Metric(sDefaultCaseName, sPutType) {}

PutMetric::PutMetric(const std::string& caseName) : Metric(caseName, sPutType) {}

void PutMetric::Set(const std::string& key, const uint64_t value)
{
    boost::mutex::scoped_lock lock(mMutexOfMetric);
    mValueMap[key] = Get(key) + value;
}

void PutMetric::Dump()
{
    std::map<std::string, uint64_t> valueMap;
    GetValueMap(valueMap);
    DumpToFile(valueMap);
}

/********************************************************
*****************Implementation of OutputMetric**********
*********************************************************/
OutputMetric::OutputMetric() : Metric(sDefaultCaseName, sOutputType) {}

OutputMetric::OutputMetric(const std::string& caseName) : Metric(caseName, sOutputType) {}

void OutputMetric::Set(const std::string& key, const uint64_t value)
{
	boost::mutex::scoped_lock lock(mMutexOfMetric);
	mBuffer[key].push_back(value);
}

void OutputMetric::GetBuffer(std::map<std::string, std::vector<uint64_t> >& buffer)
{
	boost::mutex::scoped_lock lock(mMutexOfMetric);
	buffer = mBuffer;
	mBuffer.clear();
}

void OutputMetric::Dump()
{
	std::map<std::string, std::vector<uint64_t> > buffer;
	GetBuffer(buffer);
	std::map<std::string, std::vector<uint64_t> >::iterator iter = buffer.begin();

	for(;iter != buffer.end(); ++iter)
	{
		std::string file = GetFileName(iter->first);
		if(!mOfstreamMap.count(file))
		{

            mOfstreamMap[file] = OfstreamPtr(new std::ofstream(file.c_str(), std::ios::out));
            *mOfstreamMap[file] << "Time" << std::endl;
		}
		for(size_t i = 0;i < iter->second.size(); ++i)
			*mOfstreamMap[file] << iter->second[i] << std::endl;
	}
}


};
