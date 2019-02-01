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

struct StateDeviationTransformerInfo {
	string name;
	StateTransformer *generator;
	StateDeviationTransformerInfo(string _name, StateTransformer *sdg);
};

class IdealStateDeviationTransformer : public StateTransformer {
	state operator()(state s) override;
};

class UniformCentiRadianStateDeviationTransformer : public StateTransformer {
private:
	default_random_engine gen = default_random_engine();
	uniform_real_distribution<double> dist = uniform_real_distribution<double>(-0.01, 0.01);
public:
	state operator()(state s) override;
};

struct BasisDeviationTransformerInfo {
	string name;
	BasisTransformer *generator;
	BasisDeviationTransformerInfo(string _name, BasisTransformer *bdg);
};

class IdealBasisDeviationTransformer : public BasisTransformer {
	basis operator()(basis b) override;
};

#endif