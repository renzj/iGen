#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <thread>
#include <algorithm>
#include <string>
#include <queue>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
#include <inttypes.h>
#include "helper.h"
#include "lagrange_fitting.h"
#include "system_factory.h"
#include <glog/logging.h>
#include "exponent_generator.h"
#include "lognormal_generator.h"
#include "linear_fitting.h"
#include "random.h"
#include "exponential_fitting.h"
#include "normal_generator.h"
#include "logarithmic_fitting.h"
#include "polynomial_fitting.h"
#include "pareto_generator.h"
#include "constant_generator.h"
#include "uniform_generator.h"
#include "empirical_generator.h"
#include "cauchy_generator.h"
#include "geometric_generator.h"
#include "poisson_generator.h"

using namespace porcupine;
using namespace boost::property_tree;

int main(int argc, char* argv[])
{
	//Sample();
	//TestFileSize();
	//TestFileSize1();
//	TestWriteRequest();
//	TestKS();
	google::InitGoogleLogging(argv[0]);

	//TestForPeriod();
    // default config file
    std::string lConfigFile = gDefaultConfigFile;
	ptree lroot;
	read_json(lConfigFile, lroot);

	pid_t pid;
	for(int32_t i = 0;i < 2; ++i)
	{
		if((pid = fork()) ==0) // child
		{
			break;
		}
	}

	if(pid == 0) //child
	{
		int i = 0;
		for(ptree::iterator it = lroot.begin();it != lroot.end();++it)
		{
			systems.push_back(SystemFactory::CreateSystem(ParameterFactory::CreateParameter(it->second)));
			//ParameterPtr param = ParseCase(it->second);
			//if(param)
			//	LOG(INFO) << param->GetNumOfThreads() << std::endl;
			break;
		}
		// set start time
		gStartTime = GetCurrentTimeInSeconds();
		for(int i = 0;i < systems.size(); ++i)
		{
			systems[i]->Run();
			break;
		}
		//handler(0);
		systems[0]->WaitForFinish();
	}

    return 0;
}

