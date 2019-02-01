#include <random>
#include <complex>

#include "transformers.h"

using namespace std;

StateDeviationTransformerInfo::StateDeviationTransformerInfo(string _name, StateTransformer *sdg) {
	name = _name;
	generator = sdg;
}

state IdealStateDeviationTransformer::operator()(state s) {
	return s;
}

state UniformCentiRadianStateDeviationTransformer::operator()(state s) {
	auto zero_amp = s.first;
	auto one_amp = s.second;

	double theta, phi;
	complex<double> phaseDelta;
	if (abs(zero_amp) == 0 || abs(one_amp) == 0) {
		theta = (abs(zero_amp) == 0) ? 3.14159 : 0;
		phi = 0;
		phaseDelta = 1;
	} else {
		phaseDelta = zero_amp / abs(zero_amp);
		phi = arg(one_amp) - arg(zero_amp);
		theta = 2 * acos(abs(zero_amp));
	}


	double deviated_phi = phi + dist(gen);
	double deviated_theta = theta + dist(gen);

	amplitude deviated_zero_amp = phaseDelta * cos(deviated_theta/2);
	amplitude deviated_one_amp  = phaseDelta * exp(complex<double>(0,1) * deviated_phi) * sin(deviated_theta/2);

	return make_pair(deviated_zero_amp, deviated_one_amp);
}

BasisDeviationTransformerInfo::BasisDeviationTransformerInfo(string _name, BasisTransformer *bdg) {
	name = _name;
	generator = bdg;
}

basis IdealBasisDeviationTransformer::operator()(basis b){
	return b;
}