#ifndef TIMER_GENERATOR_H
#define TIMER_GENERATOR_H

#include "generator.h"
#include "reflection.h"
#include <map>
#include <string>
#include "timer.h"
#include "fitting.h"
#include "lagrange_fitting.h"

namespace porcupine
{
class Timer1Generator : public Generator
{
public:
	Timer1Generator() {} 

	/* override */
    void Init(std::map<std::string, double> params) {}

	/* override */
	double Get();
};

typedef std::tr1::shared_ptr<Timer1Generator> Timer1GeneratorPtr;

class Timer2Generator : public Generator
{
public:
	Timer2Generator() {}

	/* override */
    void Init(std::map<std::string, double> params) {}

	/* override */
	double Get();
};

class Timer3Generator : public Generator
{
public:
	/* override */
	void Init(std::map<std::string, std::string> params);

	/* override */
    void Init(std::map<std::string, double> params) {mLagrange.Init("./data");}

	/* override */
	double Get();
private:
	LagrangeFitting	mLagrange;
};

typedef std::tr1::shared_ptr<Timer2Generator> Timer2GeneratorPtr;
}

#endif
