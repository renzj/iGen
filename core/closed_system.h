#ifndef CLOSED_SYSTEM_H
#define CLOSED_SYSTEM_H

#include <boost/thread/thread.hpp>
#include "closed_system_parameter.h"
#include "system.h"

namespace porcupine
{

class ClosedSystem : public System
{
public:
	ClosedSystem() {}

	ClosedSystem(ParameterPtr param) : System(param) {}

	/* override */
	void Run();

	/**
	* @brief every client will call this method.
	*/
	void ExecuteThreadFunc(uint32_t threadId);

	/* override */
	void WaitForFinish();
private:
	boost::thread_group	mThreads;
};

typedef std::tr1::shared_ptr<ClosedSystem> ClosedSystemPtr;

}
#endif
