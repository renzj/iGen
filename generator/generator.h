#ifndef GENERATOR_H
#define GENERATOR_H

#include <math.h>
#include <assert.h>
#include <string>
#include <map>
#include <tr1/memory>
#include <inttypes.h>
#include "random.h"

namespace porcupine
{

const double PI = 3.1415926;
const double E = 2.718281828459;

class Generator
{
public:

    /**
    * @brief Get next random number.
    * @return Random number.
    */
    virtual double Get() = 0;

    /**
    * @brief Init function for subclass except EmpiricalGenerator
    */
    virtual void Init(std::map<std::string, double> params) = 0;

    /**
    * @brief Init function for EmpiricalGenerator
    */
    virtual void Init(std::map<std::string, std::string> params) {}

    /**
    * @brief Load data from file, it's for EmpiricalGenerator
    */
    virtual void Load(std::string dataFile) {}

    virtual ~Generator() {}

    /**
    * @brief Get a random number.
    */
    int32_t GetRandom()
    {
        return mRandom.Get();
    }

    /**
    * @brief Get a random number in [min, max)
    */
    int32_t GetRandom(int min, int max)
    {
        return mRandom.Get(min, max);
    }
    
    /**
    * @brief Get probability.
    */
    double GetProbability()
    {
        return mRandom.GetProbability();
    }

	/*
	**
	*/
	void ResetSeed()
	{
		unsigned s = mRandom.GetCurrentSeed();
		//std::cout << "before: " << s << std::endl;
		mRandom.Reset(s);
		//std::cout << "after: " << mRandom.GetCurrentSeed() << std::endl;
	}

    double GetGaussian()
    {
        static int flag = 0;

        double u1 = GetProbability();
        double u2 = GetProbability();

        double r = sqrt(-2 * log(u2));
        double theta = 2 * PI * u1;

        double z = 0.0;
        if(!flag)
            z = r * sin(theta);
        else
            z = r * cos(theta);
        flag = 1 - flag;
        return z;
    }

private:
    DefaultRandom   mRandom;
};

typedef std::tr1::shared_ptr<Generator> GeneratorPtr;

}
#endif
