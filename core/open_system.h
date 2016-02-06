#ifndef OPEN_SYSTEM_H
#define OPEN_SYSTEM_H


#include <boost/thread/thread.hpp>
#include "threadpool.hpp"
#include "closed_system_parameter.h"
#include "system.h"

using namespace boost::threadpool;

typedef std::tr1::shared_ptr<pool> ThreadPoolPtr;

namespace porcupine
{
class OpenSystem : public System
{
public:
	OpenSystem() {}

	OpenSystem(ParameterPtr param) : System(param) {}

	/* override */
	void Run();

	/* override */
	void ExecuteThreadFunc();

	/* override */
	void ExecuteTaskThreadFunc(boost::thread_group& threadGroup, boost::thread* thisThread);

	/* override */
	void ExecuteTask();

	/* override */
	void WaitForFinish();

	/* override */
	uint32_t GetArrivalTime() const;

private:
	boost::thread_group mThreads;
	boost::thread_group mTaskThreads;
	//boost::threadpool::pool	mThreadPool;
	ThreadPoolPtr	mThreadPool;
};

typedef std::tr1::shared_ptr<OpenSystem> OpenSystemPtr;

}

#endif
