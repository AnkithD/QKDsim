#ifndef _FACTORIES_H_
#define _FACTORIES_H_

#include <random>

#include "constants.h"

using namespace std;

class IntFactory {
public:
	virtual int operator()(){};
};

class BoolFactory {
public:
	virtual bool operator()(){};
};

struct PulseNumberGeneratorInfo {
	string name;
	IntFactory *generator;
	PulseNumberGeneratorInfo(string _name, IntFactory *png);
};

class IdealPulseNumberGenerator : public IntFactory {
	int operator()();
};

class PoissonPulseNumberGenerator : public IntFactory {
	private:
		default_random_engine gen = default_random_engine();
		poisson_distribution<int> dist = poisson_distribution<int>(0);
	public: 
	int operator()() override;
};

struct BasisChoiceGeneratorInfo {
	string name;
	BoolFactory *generator;
	BasisChoiceGeneratorInfo(string _name, BoolFactory *bcg);
};

class IdealBasisChoiceGenerator : public BoolFactory {
	bool operator()() override;
};

class AlwaysZeroOneBasisChoiceGenerator : public BoolFactory {
	bool operator()() override;
};

struct QuantumEfficiencyGeneratorInfo {
	string name;
	BoolFactory *generator;
	QuantumEfficiencyGeneratorInfo(string _name, BoolFactory *qeg);
};

class IdealQuantumEfficiencyGenerator : public BoolFactory {
	bool operator()() override;
};

struct AbsorptionRateGeneratorInfo {
	string name;
	BoolFactory *generator;
	AbsorptionRateGeneratorInfo(string _name, BoolFactory *arg);
};

class IdealAbsorptionRateGenerator : public BoolFactory {
	bool operator()() override;
};

class AlmostIdealAbsorptionRateGenerator : public BoolFactory {
	bool operator()() override;
};

#endif