#ifndef SYSTEM_H
#define SYSTEM_H

#include <iostream>
#include <string>
#include <map>
#include <tr1/memory>
#include <signal.h>
#include <unistd.h>
#include <inttypes.h>
#include "operator_adapter.h"
#include "record.h"
#include "parameter.h"

namespace porcupine
{

class System
{
public:

	System():mTimeOut(false) {}

	System(ParameterPtr param) : mTimeOut(false), mParameter(param)
	{
		mRecord.reset(new Record(mParameter->GetCaseName()));
		mOperatorAdapter.reset(new OperatorAdapter(mParameter->GetSoFile(), mRecord, mParameter->GetCaseName()));
	}

	/**
	* @brief Interface making system run.
	*/
	virtual void Run() = 0;

//	void SetAlarm(uint32_t runtime)
//	{
//		signal(SIGALRM, System::AlarmHandler);
//		alarm(runtime);
//	}
//
//	static void AlarmHandler(int signal)
//	{
//		mTimeOut = true;
//	}

	void SetParameter(ParameterPtr param)
	{
		mParameter = param;
	}

	void SetTimeOut()
	{
		mTimeOutThread.reset(new boost::thread(&System::TimeOutThreadFunc, this));
	}

	void TimeOutThreadFunc()
	{
		std::cout << "before sleep" << std::endl;
		sleep(mParameter->GetRuntime());
		mTimeOut = true;
		std::cout << "after sleep" << std::endl;
	}

	virtual void WaitForFinish() = 0;

	virtual uint32_t GetArrivalTime() const {return 0;}

	virtual ~System() {}

protected:
	bool	mTimeOut;

	OperatorAdapterPtr	mOperatorAdapter;

	RecordPtr			mRecord;

	ParameterPtr		mParameter;	
	
	std::tr1::shared_ptr<boost::thread>	mTimeOutThread;
};

typedef std::tr1::shared_ptr<System> SystemPtr;
}

#endif
