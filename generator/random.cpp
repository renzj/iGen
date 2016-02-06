#include "random.h"
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <algorithm>

using namespace std;
namespace porcupine
{

DefaultRandom::DefaultRandom()
{
    mSeed = getSeed();
	mReSeed = mSeed;
}

DefaultRandom::DefaultRandom(unsigned int seed)
{
    Reset(seed);
}

void DefaultRandom::Reset(unsigned seed)
{
    mSeed = mReSeed;
}

unsigned DefaultRandom::GetCurrentSeed()
{
	return mSeed;
}

int DefaultRandom::Get()
{
	//std::cout << "rand_r" << std::endl;
    return rand_r(&mSeed);
}

int DefaultRandom::Get(int min, int max)
{
    if (max <= min)
    {
        stringstream ss;
        ss << "min=" << min << ",max=" << max;
		throw ss.str();
    }
    int64_t r = Get();
    return r == RAND_MAX ? Get(min, max)
	: (int)(r * (max - min) / RAND_MAX) + min;
}

int DefaultRandom::getSeed()
{
	//std::cout << "getSeed" << std::endl;
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
    {
        return 0;
        //APSARA_THROW(apsara::ExceptionBase, "failed to open /dev/urandom device");
    }
    int seed;
    read(fd, &seed, sizeof(seed));
    close(fd);
    return seed;
}    

double DefaultRandom::GetProbability()
{
    return ((double)Get()) / ((double)RAND_MAX + 1);
}

int DefaultRandom::WeightedRandomSelect(std::vector<unsigned int>& sigmaWeight)
{
    if (sigmaWeight.empty())
        return 0;
    unsigned int randomNum = (unsigned int)(GetProbability() * sigmaWeight.back());
    return ((std::upper_bound(sigmaWeight.begin(), sigmaWeight.end(), randomNum)) - sigmaWeight.begin());
}

}

