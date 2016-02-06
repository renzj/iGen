#ifndef PORCUPINE_RANDOM_H
#define PORCUPINE_RANDOM_H

//#include "timer.h"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <tr1/memory>
#include <inttypes.h>

namespace porcupine
{

class RandomBase
{
public:
    virtual ~RandomBase()
    {
    }
    
    /** Reset random generator with a seed
     */
    virtual void Reset(unsigned seed) = 0;

	/**
	 * Get the current seed
	*/
	virtual unsigned GetCurrentSeed() = 0;

    /** Get next random number
     */
    virtual int Get() = 0;

    /** Get a random number in [min, max)
     */
    virtual int Get(int min, int max) = 0;

    /**
     * Get a random number in [0, 1)
     */
    virtual double GetProbability() = 0;
    
    /** 
     *  Random select according to a weight vector
     * @param sigmaWeight sigmaWeight[0] = weight[0], sigmaWeight[i + 1] = sigmaWeight[i] + weight[i];
     * @return r, (0 <= r < sigmaWeight.size() - 1)
     *         0, if sigmaWeight.empty()
     */
    virtual int WeightedRandomSelect(std::vector<unsigned int>& sigmaWeight) = 0;
};

/** Default implementation
 * WARNING: poor performance in multi-thread
 */
class DefaultRandom: public RandomBase
{
public:
    virtual ~DefaultRandom()
    {
    }
    
    DefaultRandom();
    DefaultRandom(unsigned int seed);

    /* override */
    void Reset(unsigned seed);

    /* override */
    int Get();

    /* override */
    int Get(int min, int max);

    /**
     * Get a random number in [0, 1)
     */
    double GetProbability();

	unsigned GetCurrentSeed();

    /* override */
    int WeightedRandomSelect(std::vector<unsigned int>& sigmaWeight);
    
private:
    int getSeed();

private:
    unsigned mSeed;
	unsigned mReSeed;
};

}

#endif

