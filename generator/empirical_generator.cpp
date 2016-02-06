#include <unistd.h>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "empirical_generator.h"

namespace porcupine
{

REGISTER(EmpiricalGenerator);

const int32_t EmpiricalGenerator::mBinCount = 32437;

EmpiricalGenerator::EmpiricalGenerator(std::string dataFile) : mDataFile(dataFile)
{
}

void EmpiricalGenerator::Init(std::map<std::string, std::string> params)
{
    assert(params.count("datafile"));
    mDataFile = params["datafile"];
    Load(mDataFile);
}

void EmpiricalGenerator::Load(std::string dataFile)
{
    if(access(dataFile.c_str(), F_OK))
    {
        throw std::logic_error("Error: data file " + dataFile + "doesn't exist");
    }
    std::ifstream in(dataFile.c_str());
    std::string line;

    // find min and max
    double lmin, lmax;
    int64_t num = 0;
    while(getline(in, line))
    {
        ++num;
        if(num == 1)
        {
            continue;
        }
        lmin = std::min(lmin, atof(line.c_str()));
        lmax = std::max(lmax, atof(line.c_str()));
    }
	//mBinCount = num / 10;
	//mBinCount = 10;
	//if(!mBinCount)
	//	mBinCount = 1;
    
    double intervalLength = (lmax - lmin) / mBinCount;

    std::vector<double> lvalue;
    double l = lmin;
    for(int32_t i = 0;i < mBinCount; ++i, l += intervalLength)
    {
        lvalue.push_back(l);
    }
    // reset the pointer to first line
    in.clear();
    in.seekg(0, std::ios::beg);
    std::vector<std::vector<double> > bins(mBinCount);
    while(getline(in, line))
    {
        double val = atof(line.c_str());
        size_t i = 0;
        for(i = 0;i < lvalue.size(); ++i)
        {
            if(lvalue[i] <= val && val < lvalue[i] + intervalLength)
            {
                bins[i].push_back(val);
                break;
            }
        }
        if(i == lvalue.size())
        {
            bins[mBinCount - 1].push_back(val);
        }
        //bins[id].push_back(val);
    }

    size_t i = 0;
    while(i < bins.size())
    {
        if(!bins[i].size())
        {
            ++i;
            continue;
        }
        Bin per;
        per.probability = bins[i].size() * 1.0 / num;
        per.mean = std::accumulate(bins[i].begin(), bins[i].end(), 0.0) / bins[i].size();
        per.max = *std::max_element(bins[i].begin(), bins[i].end());
        per.min = *std::min_element(bins[i].begin(), bins[i].end());
        per.sd = 0;
        for(size_t j = 0;j < bins[i].size(); ++j)
        {
            per.sd += (bins[i][j] - per.mean) * (bins[i][j] - per.mean);
        }
        if(bins[i].size() == 1)
            per.sd = 0.0;
        else
            per.sd = sqrt(per.sd / (bins[i].size() - 1));
        mBins.push_back(per);
        ++i;
    }
}

double EmpiricalGenerator::Get()
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

    double u = GetProbability();
    
    double p = 0;
    for(size_t i = 0; i < mBins.size(); ++i)
    {
        p += mBins[i].probability;
        if(u <= p)
        {
            int32_t maxRetry = 0;
            while(1)
            {
                double res = mBins[i].mean + z * mBins[i].sd;
                if(mBins[i].min <= res && res <= mBins[i].max)
                    return res;
                if(++maxRetry >= 10)
                {
                    return mBins[i].mean;
                }
            }
        }
    }
    return 0.0;
}

}
