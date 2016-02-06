#include <iostream>
#include "operator.h"

using namespace std;
using namespace porcupine;

class Demo : public IOperator
{
public:
    Demo() {}
    
    bool Execute(std::map<std::string, std::string>& params)
    {
		//Output("arrivalExe", GetCurrentTimeInMicroSeconds());
		//QpsCount("arrivalQPS");
        return true;
    }

    ~Demo() {}
};


extern "C" IOperatorPtr CreateObj()
{
    return IOperatorPtr(new Demo());
}

