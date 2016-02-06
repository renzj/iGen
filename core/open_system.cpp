#include "open_system.h"
#include <boost/function.hpp>
#include <boost/ref.hpp>
#include <tr1/functional>
#include <sys/time.h>
#include <signal.h>

namespace porcupine
{

void OpenSystem::Run()
{
	mOperatorAdapter->Init(1);
	mRecord->Run();
	//mThreads.create_thread(boost::bind(&OpenSystem::ExecuteThreadFunc, this));
	mThreadPool.reset(new pool(20000));
	SetTimeOut();
}

void OpenSystem::ExecuteThreadFunc()
{
	//uint32_t arrivalTime = 0;
	//TimeInUsec now = GetCurrentTimeInMicroSeconds();
	//mRecord->Output("Tool", GetCurrentTimeInMicroSeconds());
	//boost threadpool
	mThreadPool->schedule(boost::bind(&OpenSystem::ExecuteTask, this));
	//std::cout << "Latency: " << GetCurrentTimeInMicroSeconds() - now << std::endl;
	return;
	while(!mTimeOut)
	{
		//std::map<std::string, std::string> param = mParameter->GetParameter();
		//mThreads.add_thread(new boost::thread(std::mem_fn(&OperatorAdapter::Execute), mOperatorAdapter, 0, param));
		//mTaskThreads.create_thread(boost::bind(&OpenSystem::ExecuteTaskThreadFunc, this, 0));
		//boost::thread *t = new boost::thread();
		// *t = boost::thread(boost::bind(&OpenSystem::ExecuteTaskThreadFunc, this, boost::ref(mThreads), t));
		mThreadPool->schedule(boost::bind(&OpenSystem::ExecuteTask, this));
		//arrivalTime = mParameter->GetArrivalTime();
		//std::cout << "Arrival Time: " << arrivalTime << std::endl;
		//TimeInUsec now = GetCurrentTimeInMicroSeconds();
		//if(arrivalTime > 0 && !mTimeOut)
		//	usleep(arrivalTime);
		//std::cout << "Latency:      " << GetCurrentTimeInMicroSeconds() - now << std::endl;
	}
}

void OpenSystem::ExecuteTask()
{
	std::map<std::string, std::string> param; //= mParameter->GetParameter();
	mOperatorAdapter->Execute(0, param);
}

void OpenSystem::ExecuteTaskThreadFunc(boost::thread_group& threadGroup, boost::thread* thisThread)
{
		std::map<std::string, std::string> param = mParameter->GetParameter();
		mOperatorAdapter->Execute(0, param);
		threadGroup.remove_thread(thisThread);
		delete thisThread;
}

void OpenSystem::WaitForFinish()
{
	//mThreads.join_all();
	//mTaskThreads.join_all();
	while(!mTimeOut)
	{
		;//usleep(100000);
	}
	mThreadPool->wait();
}

uint32_t OpenSystem::GetArrivalTime() const
{
	uint32_t arrival = mParameter->GetArrivalTime();
	while(!(arrival = mParameter->GetArrivalTime()))
			;
	return arrival;	
}

}
