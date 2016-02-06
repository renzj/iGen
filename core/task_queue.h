#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <tr1/memory>
#include <pthread.h>
#include <queue>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "parameter.h"
#include "record.h"

namespace porcupine{

/**
* @brief A class for controlling qps.
* @detail You should use {Run} to start work. It will collect qps from replay log. And for measuring client's efficiency of cosuming tasks, task numbers will be collected and logged in file automatically every n milliseconds. Thus, set log path before running.
*/
class TaskQueue
{
public:
    /**
    * @brief Default period of collection is 100ms.
    */
    TaskQueue(ParameterPtr parameterPtr, RecordPtr record) : mParameter(parameterPtr), mRecord(record), mRuntime(mDefaultRuntime)  , mTimeout(false), mTasks(0) {};
    ~TaskQueue();

    /**
    * @brief Set qps from replay cfg and add it to the task queue. This function should be bound with a thread. When 
    * all of replay logs are parsed, the thread will exit.
    * @todo Mechanism of setting qps.
    */
    //apsara::Any Set(); 
    int SetThreadFunc();

    /**
    * @brief Get a new task from queue.
    * @return num of tasks if there is qps, 0 if there is no qps at that time but there are still replay logs, -1 if all of qps are took out.
    */
    int32_t Get();

    /**
    * @brief Start to parse qps and collect data. It will create threads for Set and Run.
    */
    void Run();


    /**
    * @brief set runtime of program.
    * @param runtime Runtime.
    */
    void SetRuntime(TimeInMsec runtime);

    /**
    * @brief Get Runtime.
    * @return Runtime.
    */
    TimeInMsec GetRuntime();
    
    /**
    * @brief set time out, bound with a thread.
    */
    int TimeOut();

    /**
    * @brief Wait for time out.
    */
    void WaitRunning();

	/**
	* @brief Whether timeout or not
	*/
	static const int32_t sTimeOut;

	/**
	* @brief There is no task in task queue.
	*/
	static const int32_t sHasNoTask;

	/**
	* @bfief There is(are) still task(s) in task queue.
	*/
	static const int32_t sHasTakes;
private:

    // qps config file
    ParameterPtr  mParameter;    

    // mutex of mQps
	boost::mutex	mMutex;
    
    // Thread list
    std::vector<apsara::ThreadPtr>  mThreads;

    // metric recorder
    RecordPtr     mRecord;
    
    // time count
    TimeInMsec    mRuntime;

    // default run time, 2 minutes
    static const TimeInMsec    mDefaultRuntime = 1000 * 60 * 2;

    // timeout or not
    bool    mTimeout;

    int64_t mTasks;
};

typedef std::tr1::shared_ptr<TaskQueue> TaskQueuePtr;
}

#endif
