#include <dlfcn.h>
#include "operator_adapter.h"
#include <stdexcept>


namespace porcupine
{

OperatorAdapter::OperatorAdapter(std::string soPath, RecordPtr record, std::string caseName) : mSoFile(soPath), mRecord(record), mCaseName(caseName)
{
}

OperatorAdapter::~OperatorAdapter()
{
    // shared_ptr reset before unloading .so for preventing memory leak.
    for(size_t i = 0;i < mOperators.size(); ++i)
    {
        mOperators[i].reset();
    }
    if(mSo)
    {
        dlclose(mSo);
        //LOG_INFO(sLogger, ("Message", std::string("Close ") + mSoFile));
		LOG(INFO) << "Close " << mSoFile << std::endl;
        mSo = NULL;
    }
}

void OperatorAdapter::Init(int32_t threads)
{
    //LOG_INFO(sLogger, ("Message", std::string("Load ") + mSoFile));
	LOG(INFO) << "Load " << mSoFile << std::endl;
    mSo = dlopen(mSoFile.c_str(), RTLD_NOW | RTLD_LOCAL);
    if(!mSo)
    {
        //LOG_ERROR(sLogger, ("Message", std::string("Failed to load ") + mSoFile) ("Error", dlerror()));
		LOG(FATAL) << "Fail to load " << mSoFile << std::endl;
        throw std::logic_error(std::string(dlerror()));
    }

    //LOG_INFO(sLogger, ("Message", std::string("Obtain symbol CreateObj in ") + mSoFile));
	LOG(INFO) << "Obtain symbol CreateObj in " << mSoFile << std::endl;
    mCreateObj = (CREATEOBJ)dlsym(mSo, "CreateObj"); 
    if(!mCreateObj)
    {
        //LOG_ERROR(sLogger, ("Message", std::string("Failed to get symbol CreateObj in ") + mSoFile) ("Error", dlerror()));
		LOG(FATAL) << "Failed to get symbol CreateObj in " << mSoFile << std::endl;
        exit(-1);
    }
    //LOG_INFO(sLogger, ("Message", std::string("Get new Operator object in") + mSoFile));
	LOG(INFO) << "Get new Operator object in " << mSoFile << std::endl;
    for(int32_t i = 0;i < threads; ++i)
    {
        IOperatorPtr io = mCreateObj();
        io->Init(mRecord, mCaseName, i);
        mOperators.push_back(io);
    }
}

bool OperatorAdapter::Execute(uint32_t operatorId, std::map<std::string, std::string>& item)
{
    try
    {
		TimeInUsec now = GetCurrentTimeInMicroSeconds();
        bool reset = mOperators[operatorId]->Execute(item);
		mRecord->AverageCount("latency", GetCurrentTimeInMicroSeconds() - now);
		/*
        if(!reset)
        {
            mOperators[operatorId]->Reset();
        }*/
        //mRecord->Add("success", 1);
    }
    catch(const std::exception& e)
    {
        mOperators[operatorId]->Reset();
        mRecord->Add("fail", 1);
        //LOG_ERROR(sLogger, ("Execute Fail", std::string(e.what())));
		LOG(ERROR) << "Execute Faile" << std::string(e.what()) << std::endl;
    }
    mRecord->QpsCount("Execute");
    return true;
}

}
