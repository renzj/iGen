#ifndef OPERATOR_ADAPTER_H
#define OPERATOR_ADAPTER_H

#include <string>
#include <iostream>
#include <tr1/memory>
#include <vector>
#include <inttypes.h>
#include "operator.h"
#include "record.h"
#include <glog/logging.h>

namespace porcupine
{

/**
* @brief This class is an adapter between framework and user's .so file, it will
* 1. Load .so file dynamically.
* 2. Create object of subclass of IOperator according to {CreateObj} in .so.
* 3. Collect data and write them into logs.
*/
class OperatorAdapter
{
public:

    OperatorAdapter() {}

    /**
    * @brief Init class with .so file path and replay cfg
    * @param soPath Path of .so file.
    * @param replayCfg Replay cfg.
    */
    OperatorAdapter(std::string soPath, RecordPtr record, std::string caseName);

    ~OperatorAdapter();

    /**
    * @brief This function will do following things:
    * 1. Load .so file dynamically.
    * 2. Create object of IOperator.
    * @parem threads Deciding number of IOperators to be created.
    */
    void Init(int32_t threads);
    
    /**
    * @brief Execute one operation once.
    * @return Executed successfully{true} or not{false}.
    */
    bool Execute(uint32_t operationId, std::map<std::string, std::string>& item);

private:
    // Path of .so file
    std::string         mSoFile;

    // .so handler
    void*               mSo;

    // Function of CREATEOBJ
    CREATEOBJ           mCreateObj; 

    // operator object
    std::vector<IOperatorPtr>        mOperators; 
    
    // Record
    RecordPtr           mRecord;

    // case name
    std::string mCaseName;

};

typedef std::tr1::shared_ptr<OperatorAdapter> OperatorAdapterPtr;

}
#endif
