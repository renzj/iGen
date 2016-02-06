#ifndef PARTLYOPEN_SYSTEM_PARAMETER_H
#define PARTLYOPEN_SYSTEM_PARAMETER_H

namespace porcupine
{
class PartlyOpenSystemParameter : public Parameter
{
public:
	PartlyOpenSystemParameter();

	/* override */
	void SetArrivalTime(GeneratorPtr arrivalTime);

	/* override */
	uint32_t GetArrivalTime() const;

	/* override */
	void SetSession(GeneratorPtr session);

	/* override */
	uint32_t GetSession() const;

private:
	
	// arrival interval generator
	GeneratorPtr	mArrivalTime;

	// session generator
	GeneratorPtr	mSession;
};

typedef std::tr1::shared_ptr<PartlyOpenSystemParameter> PartlyOpenSystemParameterPtr;

}

#endif
