#ifndef _FACTORIES_H_
#define _FACTORIES_H_
#include <random>

#include "constants.h"

using namespace std;

class IntFactory {
public:
	string name;
	virtual int operator()(){};
};

class BoolFactory {
public:
	string name;
	virtual bool operator()(){};
};


class IdealPulseNumberFactory : public IntFactory {
public:
	IdealPulseNumberFactory();
	int operator()() override;
};
class PoissonPulseNumberFactory : public IntFactory {
private:
	default_random_engine gen;
	poisson_distribution<int> dist;
public: 
	PoissonPulseNumberFactory();
	PoissonPulseNumberFactory(int lambda);
	int operator()() override;
};
IntFactory* choosePulseNumberFactory();


class IdealBasisChoiceFactory : public BoolFactory {
public:
	IdealBasisChoiceFactory();
	bool operator()() override;
};
class AlwaysZeroOneBasisChoiceFactory : public BoolFactory {
public:
	AlwaysZeroOneBasisChoiceFactory();
	bool operator()() override;
};
BoolFactory* chooseBasisChoiceFactory();


class IdealQuantumEfficiencyFactory : public BoolFactory {
public:
	IdealQuantumEfficiencyFactory();
	bool operator()() override;
};
BoolFactory* chooseQuantumEfficiencyFactory();


class IdealAbsorptionRateFactory : public BoolFactory {
public:
	IdealAbsorptionRateFactory();
	bool operator()() override;
};
class PercentAbsorptionRateFactory : public BoolFactory {
	double percentAbsorbed;
public:
	PercentAbsorptionRateFactory();
	PercentAbsorptionRateFactory(double percent);
	bool operator()() override;
};
BoolFactory* chooseAbsorptionRateFactory();

#endif