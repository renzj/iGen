#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <vector>
#include <iostream>
#include <tr1/memory>
#include <assert.h>
#include <stdlib.h>
#include "operator_adapter.h"
#include "task_queue.h"
#include "record.h"
#include "parameter.h"
#include "distribution_parameter.h"
#include "reflection.h"

namespace porcupine
{

/**
* @brief Simulator 
* @detail Simulator should be initialized with config map and call {Run} to start work. It has following features:
* 1. Register a thread pool and add task into it.
* 2. Create a task queue to control qps.
*/
class Simulator
{
public:
    /**
    * @brief Init with config map.
    * @param config A configuration map corresponds to some section in .ini file, including number of threads, so file path, replay log path, etc.
    */
    Simulator(std::string simulatorName, std::map<std::string, std::string> config);

    ~Simulator();

    /**
    * @brief Run a simulator.
    * @todo Timer.
    */
    void Run();

    void Thread(int32_t operationId);

    /**
    * @brief Wait time out.
    */
    void WaitRunning();

private:
    
    // section name
    std::string          mSimulatorName;
    // Configuration map
    std::map<std::string, std::string> mConfig;
    
    //task queue
    TaskQueuePtr                       mTaskQueue;

    // parameter config
    ParameterPtr                       mParameter;

    // Record
    RecordPtr                          mRecord;

    // Operator adapter
    OperatorAdapterPtr                 mOperation;

    //Thread Pool
    apsara::common::ThreadPool*        mThreadPool;

    // custom config
    std::map<std::string, std::string>  mCustomConfig;
};

typedef std::tr1::shared_ptr<Simulator> SimulatorPtr;
}

#endif
