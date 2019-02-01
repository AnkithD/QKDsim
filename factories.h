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

struct PulseNumberFactoryInfo {
	string name;
	IntFactory *generator;
	PulseNumberFactoryInfo(string _name, IntFactory *png);
};

class IdealPulseNumberFactory : public IntFactory {
	int operator()();
};

class PoissonPulseNumberFactory : public IntFactory {
	private:
		default_random_engine gen = default_random_engine();
		poisson_distribution<int> dist = poisson_distribution<int>(0);
	public: 
	int operator()() override;
};

struct BasisChoiceFactoryInfo {
	string name;
	BoolFactory *generator;
	BasisChoiceFactoryInfo(string _name, BoolFactory *bcg);
};

class IdealBasisChoiceFactory : public BoolFactory {
	bool operator()() override;
};

class AlwaysZeroOneBasisChoiceFactory : public BoolFactory {
	bool operator()() override;
};

struct QuantumEfficiencyFactoryInfo {
	string name;
	BoolFactory *generator;
	QuantumEfficiencyFactoryInfo(string _name, BoolFactory *qeg);
};

class IdealQuantumEfficiencyFactory : public BoolFactory {
	bool operator()() override;
};

struct AbsorptionRateFactoryInfo {
	string name;
	BoolFactory *generator;
	AbsorptionRateFactoryInfo(string _name, BoolFactory *arg);
};

class IdealAbsorptionRateFactory : public BoolFactory {
	bool operator()() override;
};

class AlmostIdealAbsorptionRateFactory : public BoolFactory {
	bool operator()() override;
};

#endif