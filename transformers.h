#ifndef _TRANSFORMERS_H_
#define _TRANSFORMERS_H_

#include <random>
#include <complex>

#include "constants.h"

using namespace std;

class StateTransformer{
public:
	virtual state operator()(state){};
};

class BasisTransformer{
public:
	virtual basis operator()(basis){};
};

struct StateDeviationGeneratorInfo {
	string name;
	StateTransformer *generator;
	StateDeviationGeneratorInfo(string _name, StateTransformer *sdg);
};

class IdealStateDeviationGenerator : public StateTransformer {
	state operator()(state s) override;
};

class UniformCentiRadianStateDeviationGenerator : public StateTransformer {
private:
	default_random_engine gen = default_random_engine();
	uniform_real_distribution<double> dist = uniform_real_distribution<double>(-0.01, 0.01);
public:
	state operator()(state s) override;
};

struct BasisDeviationGeneratorInfo {
	string name;
	BasisTransformer *generator;
	BasisDeviationGeneratorInfo(string _name, BasisTransformer *bdg);
};

class IdealBasisDeviationGenerator : public BasisTransformer {
	basis operator()(basis b) override;
};

#endif