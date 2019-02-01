#ifndef _TRANSFORMERS_H_
#define _TRANSFORMERS_H_

#include <random>
#include <complex>

#include "constants.h"

using namespace std;

class StateTransformer{
public:
	string name;
	virtual state operator()(state){};
};

class BasisTransformer{
public:
	string name;
	virtual basis operator()(basis){};
};


class IdealStateDeviationTransformer : public StateTransformer {
public:
	IdealStateDeviationTransformer();
	state operator()(state s) override;
};
class UniformRadianStateDeviationTransformer : public StateTransformer {
private:
	default_random_engine gen;
	uniform_real_distribution<double> dist;
public:
	UniformRadianStateDeviationTransformer();
	UniformRadianStateDeviationTransformer(double radians);
	state operator()(state s) override;
};
StateTransformer* chooseStateDeviationTransformer();


class IdealBasisDeviationTransformer : public BasisTransformer {
public:
	IdealBasisDeviationTransformer();
	basis operator()(basis b) override;
};
BasisTransformer* chooseBasisDeviationTransformer();

#endif