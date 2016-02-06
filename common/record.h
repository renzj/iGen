#ifndef RECORD_H
#define RECORD_H

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
#include <tr1/memory>
#include <signal.h>
#include <sys/time.h>
#include "metric.h"
#include "helper.h"

namespace porcupine
{

// default dump time: 100ms
extern const int32_t cDefaultDumpTime;

/**
* @brief Class to collect data. Item is a key-value pair. Client uses {Add} to add record, and the framework will write items into log.
* @usage
* @todo Collect statistics according to second(ms).
*/
class Record
{
public:
    /**
    * @brief Default collection period is 100ms.
    */
    Record(const std::string& caseName) : mCaseName(caseName), 
                                          mQpsMetric(caseName), 
                                          mAvgMetric(caseName),
                                          mAddMetric(caseName),
                                          mSetMetric(caseName),
                                          mPutMetric(caseName),
										  mOutputMetric(caseName),
                                          mDumpTime(cDefaultDumpTime), mStop(false)
    {
    }


    /**
    * @brief Set key's value
    */
    void Set(const std::string& key, uint64_t value)
    {
        mSetMetric.Set(key, value);
    }

    /**
    * @brief Add value.
    */
    void Add(const std::string& key, uint64_t value)
    {
        mAddMetric.Set(key, value);
    }

    /**
    * @brief count qps.
    */
    void QpsCount(const std::string& key)
    {
        mQpsMetric.Set(key);
    }

    /**
    * @brief Count average value.
    */
    void AverageCount(const std::string& key, uint64_t value)
    {
        mAvgMetric.Set(key, value);
    }

    /**
    * @brief record throught
    */
    void ThroughputCount(const std::string& key, uint64_t value)
    {
        mPutMetric.Set(key, value);
    }

	/**
	* @brief output metric
	*/
	void Output(const std::string& key, uint64_t value)
	{
		mOutputMetric.Set(key, value);
	}

    /**
    * @brief collection function, bind it with a thread.
    */
    int PollingThreadFunc()
    {
        pthread_t tid = pthread_self();
        //LOG_INFO(sLogger, ("FUNCTION", __FUNCTION__) ("Thread created(Record::PollingThreadFunc)", tid));
		LOG(INFO) << __FUNCTION__ << "Thread start(pid: " << tid <<  ")" << std::endl;
        while(!mStop)
        {
            TimeInMsec start = GetCurrentTimeInMilliSeconds();
            Dump();
            TimeInMsec end = GetCurrentTimeInMilliSeconds();
            TimeInMsec rest = mDumpTime - (end - start);
            if(!mStop && rest > 0)
            {
                usleep(rest * 1000); 
            }
        }
        Dump();
        //LOG_INFO(sLogger, ("FUNCTION", __FUNCTION__) ("Thread exits(Record::PollingThreadFunc)", tid));
		LOG(INFO) << __FUNCTION__ << "Thread exits(" << tid << ")" << std::endl;
        return 0;
    }

    /**
    * @brief Bind with a thread
    */
    void Run()
    {
        //mPollingThread = CreateThread(std::tr1::bind(&Record::PollingThreadFunc, this));
		mThreads.create_thread(boost::bind(&Record::PollingThreadFunc, this));
    }


    /*
    * @brief Dump data into log.
    */
    void Dump()
    {
        mQpsMetric.Dump();
        mAvgMetric.Dump();
        mAddMetric.Dump();
        mSetMetric.Dump();
        mPutMetric.Dump();
		mOutputMetric.Dump();
    }

    /**
    * @brief Stop recording.
    */
    void Stop()
    {
        mStop = true;
		mThreads.join_all();
    }

    ~Record() {}

private:
    // log prefix, case name
    std::string     mCaseName;

    // for qps metric
    QpsMetric       mQpsMetric;
    
    // for avg metric
    AvgMetric       mAvgMetric;

    // for add metric
    AddMetric       mAddMetric;
    
    // for set metric
    SetMetric       mSetMetric;
    
    // for throughput metric
    PutMetric       mPutMetric;

	// for Output metric
	OutputMetric	mOutputMetric;

    // check whether the record should be dump to logs every mDumpTime(ms).
    TimeInMsec  mDumpTime;

    // stop recording or not
    bool        mStop;
    
    // default dump time(ms)
    static const TimeInMsec mDefaultDumpTime = 100;

    // thread for polling
    //ThreadPtr   mPollingThread;
	boost::thread_group		mThreads;
};

typedef std::tr1::shared_ptr<Record> RecordPtr;

}
#endif
