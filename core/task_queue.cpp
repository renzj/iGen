#include <stdlib.h>
#include <tr1/functional>
#include <vector>
#include "task_queue.h"
#include "helper.h"

namespace porcupine
{
// add qps into task queue every 100(ms)
const int32_t TimeIntervalOfAddQps = 100;

const int32_t TaskQueue::sTimeOut = -1;
const int32_t TaskQueue::sHasNoTask = 0;
const int32_t TaskQueue::sHasTasks = 1;

int TaskQueue::SetThreadFunc()
{
    pthread_t tid = pthread_self();
    LOG_INFO(sLogger, ("Function", __FUNCTION__) ("Thread created", tid));
    while(!mTimeout)
    {
        int64_t qps = mParameter->GetQps();
        int64_t numOfIntervals = 1000 / TimeIntervalOfAddQps;
        int64_t qpsPerInterval = qps / numOfIntervals;
        int64_t restQps = qps % numOfIntervals;
        std::vector<int64_t> qpsPerIntervalVec(numOfIntervals, qpsPerInterval);
        for(int i = 0;i < restQps; ++i)
        {
            qpsPerIntervalVec[i]++;
        }

        for(size_t i = 0;i != qpsPerIntervalVec.size() && !mTimeout; ++i)
        {
            TimeInMsec start = GetCurrentTimeInMilliSeconds();
            apsara::common::AtomicAdd(&mTasks, qpsPerIntervalVec[i]);
            TimeInMsec end = GetCurrentTimeInMilliSeconds();
            TimeInMsec rest = TimeIntervalOfAddQps * 1000 - (end - start) * 1000;
            mRecord->Set("tasks_added_per100ms", qpsPerIntervalVec[i]);
            if(rest > 0 && !mTimeout)
            {
                usleep(rest);
            }
            mRecord->Set("tasks_rest_per100ms", mTasks);
        }
    }
    LOG_INFO(sLogger, ("Function", __FUNCTION__) ("Thread exits", tid));
    return 0;
}

int32_t TaskQueue::Get()
{
	boost::mutex::scoped_lock lock(mMutex);
    // time out
    if(mTimeout)
        return TaskQueue::sTimeOut;
    // still have time, but there's no task
    if(!mTasks) 
        return TaskQueue::sHasNoTask;
    // cosume a task
    --mTasks;
    return TaskQueue::sHasTasks;
}

void TaskQueue::Run()
{
    mThreads.push_back(CreateThread(std::tr1::bind(&TaskQueue::SetThreadFunc, this)));
    mThreads.push_back(CreateThread(std::tr1::bind(&TaskQueue::TimeOut, this)));
}

void TaskQueue::SetRuntime(TimeInMsec runtime)
{
    mRuntime = runtime;
}

TimeInMsec TaskQueue::GetRuntime()
{
    return mRuntime;
}


int TaskQueue::TimeOut()
{
    usleep(mRuntime * 1000);
    mTimeout = 1;
    mRecord->Stop();
    return 0;
}


void TaskQueue::WaitRunning()
{
    while(!mTimeout)
    {
        usleep(100);
    }
    for(size_t i = 0;i < mThreads.size(); ++i)
    {
        mThreads[i]->GetValue();
    }
}

TaskQueue::~TaskQueue()
{
}

}
