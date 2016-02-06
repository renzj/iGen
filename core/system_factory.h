#ifndef SYSTEM_FACTORY_H
#define SYSTEM_FACTORY_H

#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "reflection.h"
#include "parameter.h"
#include "closed_system_parameter.h"
#include "open_system_parameter.h"
#include "system.h"
#include "closed_system.h"
#include "open_system.h"

using namespace boost::property_tree;

namespace porcupine
{

GeneratorPtr ParseGenerator(ptree& node)
{
	std::string distr = node.get<std::string>("distr") + "Generator";
    GeneratorPtr generatorPtr = GeneratorPtr((Generator*)(Factory::CreateObject(distr)));
	std::map<std::string, double> param;
	for(ptree::iterator it = node.begin(); it != node.end(); ++it)
	{
		if(it->first != std::string("distr"))
			param[it->first] = node.get<double>(it->first);
	}
	generatorPtr->Init(param);
	return generatorPtr;
}

class ParameterFactory
{
public:
	static ParameterPtr CreateParameter(ptree& node)
	{
		ParameterPtr param;
		SystemPtr system;	
		ptree systemNode = node.get_child("system");
		std::string systemType = systemNode.get_child("type").data();
		if(systemType == std::string("closed"))
		{
			param.reset(new ClosedSystemParameter());
			// set number of thread
			param->SetNumOfThreads(systemNode.get<uint32_t>("threads"));
			// set thinktime
			param->SetThinkTime(ParseGenerator(systemNode.get_child("thinkTime")));
		}
		else if(systemType == std::string("open"))
		{
			param.reset(new OpenSystemParameter());
			// set arrival time
			param->SetArrivalTime(ParseGenerator(systemNode.get_child("arrivalTime")));
		}/*
		else if(systemType == std::string("partly-open"))
		{
			param.reset(new PartlyOpenSystemParameter());
		}*/

		// set common parameters
		// set system type
		param->SetSystemType(systemType);
		// set so file path
		param->SetSoFile(node.get<std::string>("class"));
		// set runtime
		param->SetRuntime(node.get<uint32_t>("runtime"));
		// set caseName
		param->SetCaseName(node.get<std::string>("caseName"));
		ptree paramNode = node.get_child("param");
		for(ptree::iterator it = paramNode.begin(); it != paramNode.end(); ++it)
		{
			if(it->first == std::string("percent"))
			{
				for(ptree::iterator pit = it->second.begin(); pit != it->second.end(); ++pit)
				{
					param->Add(pit->first, it->second.get<double>(pit->first));
				}
			}
			else
			{
				param->Add(it->first, ParseGenerator(it->second));
			}
		}
		return param;
	}
};

class SystemFactory
{
public:
	static SystemPtr CreateSystem(ParameterPtr param)
	{
		std::string systemType = param->GetSystemType();
		if(systemType == "open")
			return SystemPtr(new OpenSystem(param));
		if(systemType == "closed")
			return SystemPtr(new ClosedSystem(param));
		//if(systemType == "partly-open")
		//	return SystemPtr(new PartlyOpenSystem(param));
		return SystemPtr();
	}
};

}

#endif
