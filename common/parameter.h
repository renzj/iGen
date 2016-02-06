#ifndef PARAMETER_H
#define PARAMETER_H

#include <iostream>
#include <string>
#include <map>
#include <tr1/memory>
#include "generator.h"

namespace porcupine
{

// system type
extern const uint32_t gClosedSystem;
extern const uint32_t gOpenSystem;
extern const uint32_t gPartlyOpenSystem;

/**
* @brief Handle parameters from json file.
*/
class Parameter
{
public:
    Parameter() {}

    /**
    * @brief Add numeric parameters
    */
    void Add(const std::string& key, GeneratorPtr generator);

	/**
	* @brief Add string parameters
	*/
	void Add(const std::string& key, const std::string& value);

	/**
	* @brief Add percentage
	*/
	void Add(const std::string& key, const double percent);

    /**
    * @brief Get item.
    */
    std::map<std::string, std::string> GetParameter();

	void SetRuntime(const uint32_t runtime);

	uint32_t GetRuntime() const;

	void SetNumOfThreads(const uint32_t numOfThreads);

	uint32_t GetNumOfThreads() const;

	void SetCaseName(std::string caseName)
	{
		mCaseName = caseName;
	}

	std::string GetCaseName() const
	{
		return mCaseName;
	}

	void SetSoFile(std::string soFile)
	{
		mSoFile = soFile;
	}

	std::string GetSoFile() const
	{
		return mSoFile;
	}

	void SetSystemType(const std::string& systemType)
	{
		mSystemType = systemType;
	}

	std::string GetSystemType() const
	{
		return mSystemType;
	}

    virtual ~Parameter() {}

	// for closed system
	virtual void SetThinkTime(GeneratorPtr thinkTime) {}

	virtual uint32_t GetThinkTime() const {return 0;}

	// for open system
	virtual void SetArrivalTime(GeneratorPtr arrivalTime) {}

	virtual uint32_t GetArrivalTime() const {return 0;}


	// for partly-open system
	virtual void SetSession(GeneratorPtr session) {}

	virtual uint32_t GetSession() const {return 0;}

private:
	
	// Runtime, default value is 60s.
	uint32_t	mRuntime;

	// number of thread
	uint32_t	mNumOfThreads;

	// caseName
	std::string	mCaseName;

	// class
	std::string	mSoFile;

	// system type
	std::string mSystemType;
	
	// numeric parameters, every key corresponds to a numeric distribution.
	std::map<std::string, GeneratorPtr> mNumericParameters;

	//percentage
	std::map<std::string, double>	mPercent;

	// string parameters, every key corresponds to a string value.
	std::map<std::string, std::string> mStringParameters;
};

typedef std::tr1::shared_ptr<Parameter> ParameterPtr;

}
#endif
