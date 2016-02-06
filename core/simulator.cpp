#include "simulator.h"
#include "helper.h"
#include <algorithm>
#include <stdexcept>

namespace porcupine
{
/**
* @brief Thread sleep(us) if there is no task in task queue.
*/
static const uint32_t sSleepTimeIfHasNoTask = 500;

Simulator::Simulator(std::string simulatorName, std::map<std::string, std::string> config) : mSimulatorName(simulatorName), mConfig(config)
{
    std::map<std::string, std::string>::iterator iter;
    
    // check whether so file config exists or not
    iter = mConfig.find("so");
    assert(iter != mConfig.end());

    // default number of thread is 10
    iter = mConfig.find("thread");
    if(iter == mConfig.end())
        mConfig["thread"] = std::string("10");

    // default runtime is 10s
    iter = mConfig.find("runtime");
    if(iter == mConfig.end())
        mConfig["runtime"] = std::string("600000");
    else
    {
        // parse second to millisecond
        mConfig["runtime"] = mConfig["runtime"] + "000"; 
    }
    TimeInMsec runtime = atoi(mConfig["runtime"].c_str());

    // create Record thread
    mRecord.reset(new Record(mSimulatorName));

    // instantiate parameters
    iter = mConfig.find("parameter");
    if(iter == mConfig.end())
    {
        LOG_ERROR(sLogger, ("Message", "Config file misses parameter."));
        throw std::logic_error("Error: Config file misses parameter.");
    }
    else
    {
        mParameter.reset(new DistributionParameter());
        std::vector<std::string> dis = Split(mConfig["parameter"], "|");
        for(size_t i = 0; i < dis.size(); ++i)
        {
            //std::cout << "dis: " << dis[i] << std::endl;
            dis[i] = Trim(dis[i]);
            size_t left = dis[i].find("(");
            size_t right = dis[i].find(")");
            std::string paramName = dis[i].substr(0, left);
            //std::cout << "ParaName: " << paramName << std::endl;
            //std::cout << dis[i].substr(left + 1, right - left - 1) << std::endl;
            std::vector<std::string> params = Split(dis[i].substr(left + 1, right - left - 1), ",");
            for(size_t j = 0;j < params.size(); ++j)
            {
                params[j] = Trim(params[j]);
            }
            if(!params.size())
            {
                LOG_ERROR(sLogger, ("Error:", "Distribution type is empty."));
                throw std::logic_error("Error: Distribution type is empty.");
            }

            // transform distribution name
            std::string disName = params[0];
            disName += "Generator";
            //std::cout << "DisName: " << disName << std::endl;
            // transform parameters
            std::map<std::string, double> paramsMap;
            std::map<std::string, std::string>  paramsMapForEmpirical;
            for(size_t j = 1;j < params.size(); ++j)
            {
                std::vector<std::string> tmp = Split(params[j], "=");
                for(size_t k = 0;k < tmp.size(); ++k)
                {
                    tmp[k] = Trim(tmp[k]);
                }
                if(tmp.size() != 2)
                {
                    LOG_ERROR(sLogger, ("Error:", "Distribution Parameter is Error"));
                    throw std::logic_error("Error: Distribution parameter is Error");
                }
                if(disName == "EmpiricalGenerator")
                {
                    paramsMapForEmpirical[tmp[0]] = tmp[1];
                }
                else
                {
                    paramsMap[tmp[0]] = StringTo<double>(tmp[1]);
                }
                //std::cout << tmp[0] << " = " << StringTo<double>(tmp[1]) << std::endl;
            }
            GeneratorPtr generatorPtr = GeneratorPtr((Generator*)(Factory::CreateObject(disName)));
            if(disName == "EmpiricalGenerator")
                generatorPtr->Init(paramsMapForEmpirical);
            else
                generatorPtr->Init(paramsMap);
            mParameter->Add(paramName, generatorPtr);
        }
    }

    // loading so file
    int32_t threads = atoi(mConfig["thread"].c_str());
    std::string caseName = mSimulatorName;
    if(mConfig["casename"] != "")
        caseName = mConfig["casename"];
    mOperation.reset(new OperatorAdapter(mConfig["so"], mRecord, caseName));
    mOperation->Init(threads);
    
    // create task queue
    mTaskQueue.reset(new TaskQueue(mParameter, mRecord));
    mTaskQueue->SetRuntime(runtime);

    // load custom parameter
    for(iter = mConfig.begin(); iter != mConfig.end(); ++iter)
    {
        if(iter->first != "thread" && iter->first != "so" && iter->first != "parameter" && iter->first != "runtime")
        {
            mCustomConfig[iter->first] = iter->second;
        }
    }

    // create thread pool
    mThreadPool = new apsara::common::ThreadPool(mConfig["so"], threads);
    if(!mThreadPool)
    {
        LOG_ERROR(sLogger, ("Error", "Threadpool failed to create"));
        exit(-1);
    }
    mThreadPool->Startup();
    LOG_INFO(sLogger, ("Message", "Threadpool startup"));
    //add task into thread pool
    for(int32_t i = 0;i < threads; ++i)
    {
        common::ThreadPool::Closure *task = common::NewClosure(this, &Simulator::Thread, i);
        mThreadPool->AddTask(task);
    }

}

void Simulator::Run()
{
    LOG_INFO(sLogger, ("Funtion", __FUNCTION__) ("Message", "Simulator Run"));
    mRecord->Run();
    LOG_INFO(sLogger, ("Message", "Record startup"));
    mTaskQueue->Run();
    LOG_INFO(sLogger, ("Message", "TaskQueue startup"));
}

void Simulator::Thread(int32_t i)
{
    while(1)
    {
        int32_t task = mTaskQueue->Get();
        if(task == TaskQueue::sTimeOut)
        {
            break;
        }
        else if(task == TaskQueue::sHasNoTask)
        {
            // sleep 1ms for efficiency
            usleep(sSleepTimeIfHasNoTask);
        }
        else
        {
            //LOG_INFO(sLogger, ("execute new task", task)); 
            std::map<std::string, std::string> item = mParameter->GetParameter();
            // add custom config
            item.insert(mCustomConfig.begin(), mCustomConfig.end());
            mOperation->Execute(i, item);
        }
    }
}

void Simulator::WaitRunning()
{
    mTaskQueue->WaitRunning();
}

Simulator::~Simulator()
{
    if(mThreadPool)
    {
        mThreadPool->Shutdown();
        delete mThreadPool;
        LOG_INFO(sLogger, ("Message", "Threadpool shutdown"));
    }
}

}
