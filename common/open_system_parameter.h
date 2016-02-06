#ifndef OPEN_SYSTEM_PARAMETER_H
#define OPEN_SYSTEM_PARAMETER_H

#include "parameter.h"

namespace porcupine
{
class OpenSystemParameter : public Parameter
{
public:
	OpenSystemParameter();

	/* override */
	void SetArrivalTime(GeneratorPtr arrivalTime);

	/* override */
	uint32_t GetArrivalTime() const;

	~OpenSystemParameter() {}

private:

	// arrival interval generator
	GeneratorPtr	mArrivalTime;
};

typedef std::tr1::shared_ptr<OpenSystemParameter> OpenSystemParameterPtr;
}

#endif
