#ifndef METRIC_H
#define METRIC_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <tr1/memory>
#include <stdexcept>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include "timer.h"

namespace porcupine
{

/**
* According to the way of collection and operation when dump, we sort metrics to be collected into four types:
* 1. Add: if key exists, add value to it, otherwise, add specific key and value. After dumping to file, key-value pairs won't be cleaned up.
* 2. Set: whenever key exists, set its value to what user delivers. After dumping to file, key-value pairs will be cleaned up.
* 3. Qps: record qps. Every time, value will be added by 1. After dumping to file, it'll be cleaned up.
* 4. Avg: calculate every key-pair's average value such latency. After dumping to file, it'll be cleaned up.
* 5. Put: calculate every key-pair's throught. After dumping fo file, it'll be cleaned up.
*/

extern const std::string sQpsType;
extern const std::string sAvgType;
extern const std::string sSetType;
extern const std::string sAddType;
extern const std::string sPutType;
extern const std::string sOutputType;
extern const std::string sDefaultCaseName;

class Metric
{
public:
    
    Metric () {}

    Metric(const std::string& caseName, const std::string& type) : mCaseName(caseName), mType(type) {}
   
    // set value through specified key, value is necessary
    virtual void Set(const std::string& key, const uint64_t value = 1) = 0;

    // get value through specified key
    uint64_t Get(const std::string& key)
    {
        std::map<std::string, uint64_t>::iterator iter = mValueMap.find(key);
        return iter == mValueMap.end() ? 0 : iter->second;
    }

    /**
    * @brief dump data to file.
    */
    virtual void Dump() = 0;
    
    /**
    * @brief Get filename of log to dump, the format is
    * {mCaseName}_{key}_{mType}.log
    */
    std::string GetFileName(const std::string& key)
    {
		//std::string pid = std::to_string(getpid());
        return mCaseName + "_" + key + "_" + mType + ".log";
    }
    /**
    * @brief dump map to file
    */
    template<typename T>
    void DumpToFile(std::map<std::string, T>& valueMap)
    {
        typename std::map<std::string, T>::iterator iter = valueMap.begin();
        TimeInMsec now = GetCurrentTimeInMilliSeconds();
        while(iter != valueMap.end())
        {
            std::string file = GetFileName(iter->first);
            if(!mOfstreamMap.count(file))
            {
                mOfstreamMap[file] = OfstreamPtr(new std::ofstream(file.c_str(), std::ios::out));
                *mOfstreamMap[file] << "Time         Value" << std::endl;
            }
            *mOfstreamMap[file] << now << "        " << iter->second  << std::endl;
            ++iter;
        }
    }


    /*
    * @brief copy {mValueMap} to {res}.
    * @param res container saving result.
    * @param cleanUp clean up key-value pair or not.
    */
    void GetValueMap(std::map<std::string, uint64_t>& res, bool cleanUp = true)
    {
        res.clear();
		boost::mutex::scoped_lock lock(mMutexOfMetric);
        res.insert(mValueMap.begin(), mValueMap.end());
        if(cleanUp)
        {
            mValueMap.clear();
        }
    }
   
    virtual ~Metric() {}

protected:

    // value map to record, format's key => value
    std::map<std::string, uint64_t> mValueMap;

    typedef std::tr1::shared_ptr<std::ofstream> OfstreamPtr;
    // value of ofstream
    std::map<std::string, OfstreamPtr> mOfstreamMap;

    // mutex
	boost::mutex   mMutexOfMetric;

    // case name
    std::string   mCaseName;

    // metric type, there's four defined, {mType} will be used as log filename's suffix
    std::string   mType;
};

/**
* @brief  Add metric.
*/
class AddMetric : public Metric
{
public:
    AddMetric();

    AddMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

    ~AddMetric() {}
};

/**
* @brief Set metric
*/
class SetMetric : public Metric
{
public:
    SetMetric();

    SetMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

    ~SetMetric() {}
};

/**
* @brief Qps metric
*/
class QpsMetric : public Metric
{
public:
    QpsMetric();

    QpsMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

    ~QpsMetric() {}
};

/**
* @brief Avg metric
*/
class AvgMetric : public Metric
{
public:
    AvgMetric();

    AvgMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

    void GetValueMap(std::map<std::string, double>& valueMap);

    ~AvgMetric() {};
private:
    // count of key
    std::map<std::string, uint64_t> mCountMap;
};

/**
* @brief Put metric
*/
class PutMetric : public Metric
{
public:
    PutMetric();

    PutMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

    ~PutMetric() {};
};

/**
* @brief Record every message
*/
class OutputMetric : public Metric
{
public:
	OutputMetric();

	OutputMetric(const std::string& caseName);

    /* override */
    void Set(const std::string& key, const uint64_t value = 1);

    /* override */
    void Dump();

	void GetBuffer(std::map<std::string, std::vector<uint64_t> >& buffer);

	~OutputMetric() {}

private:
	std::map<std::string, std::vector<uint64_t> > mBuffer;
};

}
#endif
