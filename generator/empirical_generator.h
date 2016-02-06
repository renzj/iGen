#ifndef EMPIRICAL_GENERATOR_H
#define EMPIRICAL_GENERATOR_H

#include "generator.h"
#include "reflection.h"

namespace porcupine
{

struct Bin
{
    // probability of bin
    double probability;

    // standard deviation
    double sd;

    // mean
    double mean;

    // max value
    double max;
    
    // min value
    double min;
};

class EmpiricalGenerator : public Generator
{
public:
    EmpiricalGenerator() {} 

    EmpiricalGenerator(std::string dataFile);

    /**
    * @brief override, useless
    */
    void Init(std::map<std::string, double> params) {}

    /* override */
    void Init(std::map<std::string, std::string> params);

    /* override */
    void Load(std::string dataFile);

    /* override */
    double Get();
private:

    // data file
    std::string mDataFile;

    // number of bins to divide when loading datafile
    static const int32_t mBinCount;   
    
    // every bin's characteristic
    std::vector<Bin> mBins;
};

typedef std::tr1::shared_ptr<EmpiricalGenerator> EmpiricalGeneratorPtr;
}

#endif
