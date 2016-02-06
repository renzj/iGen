#include "closed_system.h"
#include <boost/thread.hpp>

namespace porcupine
{

void ClosedSystem::Run()
{
	//TODO: initialize operatorAdapter	
	mOperatorAdapter->Init(mParameter->GetNumOfThreads());
	mRecord->Run();
	std::cout << mParameter->GetNumOfThreads() << std::endl;
	for(uint32_t i = 0;i < mParameter->GetNumOfThreads(); ++i)
	{
		mThreads.create_thread(boost::bind(&ClosedSystem::ExecuteThreadFunc, this, i));
	}
	SetTimeOut();
}

void ClosedSystem::ExecuteThreadFunc(uint32_t threadId)
{
	uint32_t thinkTime = 0;
	while(!mTimeOut)
	{
		std::map<std::string, std::string> param = mParameter->GetParameter();
		TimeInNsec s = GetCurrentTimeInNanoSeconds();
		mOperatorAdapter->Execute(threadId, param);
		mRecord->AverageCount("latency", GetCurrentTimeInNanoSeconds() - s);
		thinkTime = mParameter->GetThinkTime();
		TimeInUsec now = GetCurrentTimeInMicroSeconds();
		if(thinkTime > 0 && !mTimeOut)
			usleep(thinkTime);
		//mRecord->Output("sleep_latency", GetCurrentTimeInMicroSeconds() - now);
		mRecord->Output("thinkTime", thinkTime);
	}
	//pthread_exit(NULL);
}

void ClosedSystem::WaitForFinish()
{
	mThreads.join_all();
}

}
