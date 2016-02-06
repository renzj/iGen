#ifndef CLOSED_SYSTEM_PARAMETER_H
#define CLOSED_SYSTEM_PARAMETER_H

#include "parameter.h"

namespace porcupine
{
class ClosedSystemParameter : public Parameter
{
public:
	ClosedSystemParameter();

	~ClosedSystemParameter() {}
	
	/* override */
	void SetThinkTime(GeneratorPtr thinkTIme);

	/* override */
	uint32_t GetThinkTime() const;

private:
	GeneratorPtr	mThinkTime;
};


typedef std::tr1::shared_ptr<ClosedSystemParameter> ClosedSystemParameterPtr;

}

#endif
