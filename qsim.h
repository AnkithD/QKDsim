#include <vector>
#include <algorithm>
#include <complex>
#include <iostream> 
#include <random>

#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

template <class T>
class RandomGenerator {
public:
	 T operator()(){};
};

template <class T1, class T2>
class RandomTransformer {
public:
	 T1 operator()(T2) {};
};

struct PulseNumberGeneratorInfo {
	string name;
	RandomGenerator<int> *generator;
	PulseNumberGeneratorInfo(string _name, RandomGenerator<int> *png) {
		name = _name;
		generator = png;
	}
};

class IdealPulseNumberGenerator : public RandomGenerator<int> {
	int operator()()  override{
		return 1;
	}
};

class PoissonPulseNumberGenerator : public RandomGenerator<int> {
	private:
		default_random_engine gen = default_random_engine();
		poisson_distribution<int> dist = poisson_distribution<int>(0);
	public: 
	int operator()() override{
		return 1+dist(gen);
	}
};

struct BasisChoiceGeneratorInfo {
	string name;
	RandomGenerator<bool> *generator;
	BasisChoiceGeneratorInfo(string _name, RandomGenerator<bool> *bcg) {
		name = _name;
		generator = bcg;
	}
};

class IdealBasisChoiceGenerator : public RandomGenerator<bool> {
	bool operator()() override{
		return rand()%2;
	}
};

class AlwaysZeroOneBasisChoiceGenerator : public RandomGenerator<bool> {
	bool operator()() override{
		return 0;
	}
};

struct StateDeviationGeneratorInfo {
	string name;
	RandomTransformer<state, state> *generator;
	StateDeviationGeneratorInfo(string _name, RandomTransformer<state, state> *sdg) {
		name = _name;
		generator = sdg;
	}
};

class IdealStateDeviationGenerator : public RandomTransformer<state, state> {
	state operator()(state s) override{
		return s;
	}
};

class UniformCentiRadianStateDeviationGenerator : public RandomTransformer<state, state> {
private:
	default_random_engine gen = default_random_engine();
	uniform_real_distribution<double> dist = uniform_real_distribution<double>(-0.01, 0.01);
public:
	state operator()(state s) override{
		auto zero_amp = s.first;
		auto one_amp = s.second;

		complex<double> phaseDelta = zero_amp / abs(zero_amp);
		double phi = arg(one_amp) - arg(zero_amp);
		double theta = 2 * acos(abs(zero_amp));

		double deviated_phi = phi + dist(gen);
		double deviated_theta = theta + dist(gen);

		amplitude deviated_zero_amp = phaseDelta * cos(deviated_theta/2);
		amplitude deviated_one_amp  = phaseDelta * exp(complex<double>(0,1) * deviated_phi) * sin(deviated_theta/2);

		return make_pair(deviated_zero_amp, deviated_one_amp);
	}
};

struct QuantumEfficiencyGeneratorInfo {
	string name;
	RandomGenerator<bool> *generator;
	QuantumEfficiencyGeneratorInfo(string _name, RandomGenerator<bool> *qeg) {
		name = _name;
		generator = qeg;
	}
};

class IdealQuantumEfficiencyGenerator : public RandomGenerator<bool> {
	bool operator()() override{
		return true;
	}
};

struct BasisDeviationGeneratorInfo {
	string name;
	RandomTransformer<basis, basis> *generator;
	BasisDeviationGeneratorInfo(string _name, RandomTransformer<basis, basis> *bdg) {
		name = _name;
		generator = bdg;
	}
};

class IdealBasisDeviationGenerator : public RandomTransformer<basis, basis> {
	basis operator()(basis b) override{
		return b;
	}
};

struct AbsorptionRateGeneratorInfo {
	string name;
	RandomGenerator<bool> *generator;
	AbsorptionRateGeneratorInfo(string _name, RandomGenerator<bool> *arg) {
		name = _name;
		generator = arg;
	}
};

class IdealAbsorptionRateGenerator : public RandomGenerator<bool> {
	bool operator()() override{
		return false;
	}
};

class AlmostIdealAbsorptionRateGenerator : public RandomGenerator<bool> {
	bool operator()() override{
		return ((rand()%100) < 10);
	}
};