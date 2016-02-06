#ifndef REFLECTION_H
#define REFLECTION_H

#include <iostream>
#include <string>
#include <map>
#include <tr1/functional>
#include <stdexcept>
//#include "helper.h"

//using namespace apsara::logging;

namespace porcupine
{

typedef std::tr1::function<void* ()> Handler;

/**
* @brief Factory Method
*/
class Factory
{
public:

    Factory(std::string className, Handler handler)
    {
        Factory::Register(className, handler);
    }

    static void Register(const std::string className, const Handler handler)
    {
        std::map<std::string, Handler>& mReflection = GetReflection();
        mReflection[className] = handler;
    }

    static void* CreateObject(const std::string className)
    {
        std::map<std::string, Handler>& mReflection = GetReflection();
        if(!mReflection.count(className))
        {
            std::string error = className + " is not registered";
            //LOG_ERROR(sLogger, ("Message",error)); 
            throw std::logic_error("Error: " + className + " is not registered");
            return NULL;
        }
        return mReflection[className]();
    }

    static std::map<std::string, Handler>& GetReflection()
    {
        static std::map<std::string, Handler> mReflection;
        return mReflection;
    }

private:
    // reflection from key to method of create object
    //static std::map<std::string, Handler>  mReflection;
};

template<typename T>
class AutoRegister
{
public:
    static void* CreateObject()
    {
        return new T();
    }
private:
    static Factory mFactory;
};

#define REGISTER(className) \
        class className; \
        template<> \
        Factory AutoRegister<className>::mFactory(#className, AutoRegister<className>::CreateObject)

}
#endif

