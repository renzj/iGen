#ifndef OPERATOR_H
#define OPERATOR_H

#include <tr1/functional>
#include <sstream>
#include <typeinfo>
#include "helper.h"
#include "random.h"
#include "record.h"

namespace porcupine
{
/**
* @breif Interface of operator.
*/
class IOperator
{
public:
    IOperator() {}

    /*
    * @brief Init members.
    * @param replayCfg Replay Config parser.
    * @param record Recorder.
    */
    void Init(RecordPtr record, std::string caseName, int32_t id)
    {
        mRecord = record;
        mCaseName = caseName;
        mId = id;
    }

    virtual ~IOperator() {};

    /**
    * @brief do some resuming work
    */
    virtual void Reset() {}
    
    /**
    * @brief Execute one operation.
    * @return Executed successfully{true} or not{false}.
    * @todo What the parameter which the operation needs is uncertain. Later the {param} will be formatted.
    */
    virtual bool Execute(std::map<std::string, std::string>& item) = 0;

    /**
    * @brief Do some preparatory work.
    * @return Preparatory work is done successfully{true} or not{false}.
    */
    virtual bool Prepare() {return true;};

    /**
    * @see {class Record::Set}
    */
    void Set(const std::string& key, uint64_t value)
    {
        mRecord->Set(key, value);
    }

    /**
    * @see {class Record::Add}
    */
    void Add(const std::string& key, uint64_t value)
    {
        mRecord->Add(key, value);
    }

    /**
    * @see {class Record::QpsCount}
    */
    void QpsCount(const std::string& key)
    {
        mRecord->QpsCount(key);
    }

    /**
    * @see {class Record::AverageCount}
    */
    void AverageCount(const std::string& key, uint64_t value)
    {
        mRecord->AverageCount(key, value);
    }
    
    /**
    * @see {class Record::ThroughputCount}
    */
    void ThroughputCount(const std::string& key, uint64_t value)
    {
        mRecord->ThroughputCount(key, value);
    }


	void Output(const std::string& key, uint64_t value)
	{
		mRecord->Output(key, value);
	}

    /**
    * @brief Get file name to create.
    * File name's format is pangu://localcluster/{case name}/{hostname}_{threadid}_{file type name}/{countid}/{proccessid}_{createtime}
    * @param fileType File type. see {enum FileType}
    */
    std::string GetWriteFile(int32_t fileType, std::string caseName = "")
    {
        //pid_t pid = getpid();
        //std::string fileTypeName = FileTypeList[fileType];
        //std::string hostName = GetHostName();
        //std::string fileName = std::string("pangu://localcluster/") +(caseName == "" ? mCaseName : caseName) + "/" + fileTypeName + "/" + IntToString(pid) + "_" + hostName + "/" + IntToString(mId) + "/" + IntToString(mFileCount[fileType]++) + "/";
        //struct tm *ptr;
        //time_t it;
        //it = time(NULL);
        //ptr = localtime(&it);

        //std::stringstream ss;
        //ss << ptr->tm_year + 1900 << "_" << ptr->tm_mon + 1 << "_" << ptr->tm_mday << "_" << ptr->tm_hour << "_" << ptr->tm_min << "_" << ptr->tm_sec;
        //return fileName + ss.str();
		return "";
    }

    std::string GetReadFile(std::string caseName = "")
    {
        //std::string prefix = "pangu://localcluster/" + (caseName == "" ? mCaseName: caseName) + "/";
        //if(!mFiles.size())
        //{
        //    GetAllPanguFile(prefix, mFiles);
        //}
        //if(!mFiles.size())
        //{
        //    //TODO: Add detail Exception for simulator.
        //    //throw std::logic_error("There's no file prepared.");
        //    LOG_ERROR(sLogger, ("Error", "There's no file prepared."));
        //    exit(-1);
        //}
        //DefaultRandom random;
        //return mFiles[random.Get() % mFiles.size()];
		return "";
    }

    void FillBuffer(char* buffer, int64_t bufferSize)
    {
        DefaultRandom r;
        size_t length = gComplexChars.length();
        for(int64_t i = 0;i < bufferSize; ++i)
        {
            buffer[i] = gComplexChars[r.Get() % length];
        }
    }

private:

    // recorder pointer
    RecordPtr   mRecord;

    // case name
    std::string mCaseName;

    //index of the object
    int32_t mId;

    // file in pangu://localcluster/{mCaseName}
    std::vector<std::string> mFiles;
};

typedef std::tr1::shared_ptr<IOperator> IOperatorPtr;

/**
* @brief This function should be realized in every .so file. It will create a object of subclass of IOperator.
*/
extern "C"
{
    IOperatorPtr CreateObj();
}

typedef IOperatorPtr (*CREATEOBJ)(); 

}

#endif
