#include "helper.h"
#include "random.h"
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>
#include <string>

namespace porcupine
{

extern const std::string gComplexChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";

extern const std::string gDefaultConfigFile = "porcupine.json";

TimeInSec gStartTime;

void InitPorcupine()
{
}

std::vector<std::string> Split(std::string source, std::string sep, int num)
{
    source = Trim(source);
    // set times of split, it should be [0, source.size()]
    if(num < 0)
        num = 0;
    num = std::min(num, (int)source.size());
    std::vector<std::string> res;
    size_t pos = source.find(sep);
    while(pos != std::string::npos && num)
    {
        std::string sub = source.substr(0, pos);
        //cout << "sub: " << sub << endl;
        //if there are continuous blank spaces and sep is blank space, continuous blank spaces will be ignored
        if(sep != " " || sub != "")
        {
            res.push_back(sub);
            --num;
        }
        source = source.substr(pos + 1);
        pos = source.find(sep);
    }
    if(sep != " " || source != "")
        res.push_back(source);
    return res;

}

std::string Ltrim(std::string source, char character)
{
    size_t i = 0;
    while(i != source.size() && source[i] == character)
        i++;
    return source.substr(i);  
}

std::string Rtrim(std::string source, char character)
{
    int i = source.size() - 1;
    while(i >= 0 && source[i] == character)
        i--;
    if(i < 0)
        return std::string("");
    return source.substr(0, i + 1);
}

std::string Trim(std::string source, char character)
{
    return Ltrim(Rtrim(source, character), character);
}

std::string GetRandomString(int32_t length)
{
    DefaultRandom r;
    std::string str;
    str.resize(length);
    for(int32_t i = 0;i < length;i++)
    {
        int32_t tmp = r.Get() % gComplexChars.size();
        str[i] = gComplexChars[tmp];
    }
    return str;
}

std::string IntToString(int64_t x)
{
    std::string res;
    bool negative = false;
    if(x < 0)
    {
        negative = true;
        x *= -1;
    }
    while(x >= 10)
    {
        char c = '0' + x % 10;
        res.insert(0, 1, c);
        x /= 10;
    }
    char c = '0' + x % 10;
    res.insert(0, 1, c);
    if(negative)
    {
        res.insert(0, 1, '-');
    }
    return res;
}

std::string GetHostName()
{
    char buffer[32];
    gethostname(buffer, sizeof(buffer));
    return std::string(buffer);
}

template<>
int32_t GetParameter(std::map<std::string, std::string>& params, const std::string& key, int32_t value)
{
    if(params.count(key))
    {
        //return std::stoi(params[key]);
		return 1;
    }
    return value;
}

template<>
int64_t GetParameter(std::map<std::string, std::string>& params, const std::string& key, int64_t value)
{
    if(params.count(key))
    {
        //return std::stoll(params[key]);
		return 1;
    }
    return value;
}

template<>
double GetParameter(std::map<std::string, std::string>& params, const std::string& key, double value)
{
    if(params.count(key))
    {
        //return std::stod(params[key]);
		return 1.0;
    }
    return value;
}

template<>
std::string GetParameter(std::map<std::string, std::string>& params, const std::string& key, std::string value)
{
    if(params.count(key))
    {
        return params[key];
    }
    return value;
}

std::vector<double> PloyNormialMultiplication(const std::vector<double> p1, const std::vector<double> p2)
{
	assert(p1.size() && p2.size());
	std::vector<double> res(p1.size() - 1 + p2.size() - 1, 0);
	for(size_t i = 0;i < p1.size(); ++i)
	{
		for(size_t j = 0;j < p2.size(); ++j)
		{
			res[i + j] += p1[i] * p2[j];
		}
	}
	return res;
}

}
