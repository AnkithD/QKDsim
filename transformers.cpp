#include <iostream>
#include <random>
#include <complex>
#include <string>

#include "transformers.h"

using namespace std;


IdealStateDeviationTransformer::IdealStateDeviationTransformer() {
	name = "Ideal State Deviation Transformer";
}
state IdealStateDeviationTransformer::operator()(state s) {
	return s;
}

UniformRadianStateDeviationTransformer::UniformRadianStateDeviationTransformer(double radians) {
	gen = default_random_engine();
	dist = uniform_real_distribution<double>(-radians, radians);
	name = to_string(radians) + " radian Uniform Random State Deviation Transformer";
}
UniformRadianStateDeviationTransformer::UniformRadianStateDeviationTransformer() {
	gen = default_random_engine();
	cout << "Enter maximum radian deviation for uniform distribution(r), i.e. Uniform distribution from [-r,r]: ";
	double radians;
	cin >> radians;
	dist = uniform_real_distribution<double>(-radians, radians);
	name = to_string(radians) + " radian Uniform Random State Deviation Transformer";
}
state UniformRadianStateDeviationTransformer::operator()(state s) {
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

StateTransformer* chooseStateDeviationTransformer() {
	StateTransformer* chosenTransformer;
	vector<string> transformers {"Ideal State Deviation Transformer, No deviation from generated state",
							  "Uniform Random Radian State Deviation Transformer, deviates Bloch Sphere angles uniformly random b/w [-r,r]"};

	int index = 1;
	for (auto name: transformers) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which State Deviation Transformer to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenTransformer = new IdealStateDeviationTransformer();
			break;
		}
		case 2: {
			chosenTransformer = new UniformRadianStateDeviationTransformer();
			break;
		}
		default:{
			cout << "Out of Index State Deviation Transformer choice" << endl;
			throw -1;
		}
	}

	return chosenTransformer;
}


IdealBasisDeviationTransformer::IdealBasisDeviationTransformer() {
	name = "Ideal Basis Deviation Transformer";
}
basis IdealBasisDeviationTransformer::operator()(basis b){
	return b;
}

BasisTransformer* chooseBasisDeviationTransformer() {
	BasisTransformer* chosenTransformer;
	vector<string> transformers {"Ideal Basis Deviation Transformer, able to measure qubit in basis with exact precision"};

	int index = 1;
	for (auto name: transformers) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which Basis Deviation Transformer to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenTransformer = new IdealBasisDeviationTransformer();
			break;
		}
		default:{
			cout << "Out of Index Basis Deviation Transformer choice" << endl;
			throw -1;
		}
	}

	return chosenTransformer;
}