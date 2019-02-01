#include <vector>
#include <complex>
#include <algorithm>
#include <iostream>

#include "devices.h"

using namespace std;


Generator::Generator(IntFactory *png, BoolFactory *bcg, StateTransformer *sdg) {
	pulseNumberGenerator = png;
	basisChoiceGenerator = bcg;
	stateDeviationGenerator = sdg;
}
Pulse Generator::createPulse(amplitude a, amplitude b) {
	int pulseSize = pulseNumberGenerator->operator()();
	vector<Qubit*> qubits;
	for (int i = 0; i < pulseSize; ++i)
	{
		state deviatedState = stateDeviationGenerator->operator()(make_pair(a,b));
		amplitude zero_amp  = deviatedState.first;
		amplitude one_amp   = deviatedState.second; 
		qubits.push_back(new Qubit(zero_amp, one_amp));
	}
	return Pulse(qubits);
}
Pulse Generator::createPulse(state s) {
	return createPulse(s.first, s.second);
}
Pulse Generator::createPulse(bool value, bool basisChoice) {
	if (basisChoice == false) {
		return value? createPulse(ONE):createPulse(ZERO);
	} else {
		return value? createPulse(MINUS):createPulse(PLUS);
	}
}
Pulse Generator::createPulse(bool value) {
	bool basisChoice = basisChoiceGenerator->operator()();
	return createPulse(value, basisChoice);
}


Detector::Detector(int dcr, BoolFactory *qeGen, BoolFactory *bcGen, BasisTransformer *bdGen) {
	darkCountRate = dcr;
	quantumEfficiencyGenerator = qeGen;
	basisChoiceGenerator = bcGen;
	basisDeviationGenerator = bdGen;
}
int Detector::detectPulse(Pulse pulse, basis basisChoice) {
	if (!(quantumEfficiencyGenerator->operator()())) {
		return -1;
	}
	int size = pulse.size();
	Qubit *qubit = pulse[rand()%size];
	bool observation = qubit->observe(basisDeviationGenerator->operator()(basisChoice));
	if (DEBUGPRINT) {
		//cout << "Detecting qbit: " << qubit->alpha << "," << qubit->beta << endl;
	}
	return (observation)? 1:0;
}
int Detector::detectPulse(Pulse pulse) {
	basis basisChoice;
	if (basisChoiceGenerator->operator()()) {
		if (DEBUGPRINT){
			cout << "Choose diagonal basis" << endl;
		}
		basisChoice = make_pair(PLUS, MINUS);
	} else {
		if (DEBUGPRINT){
			cout << "Choose normal basis" << endl;
		}
		basisChoice = make_pair(ZERO, ONE);
	}
	return detectPulse(pulse, basisChoice);
}
int Detector::detectPulse(Pulse pulse, bool commonBasisChoice) {
	basis basisChoice;
	if (commonBasisChoice) {
		if (DEBUGPRINT){
			cout << "Choose diagonal basis" << endl;
		}
		basisChoice = make_pair(PLUS, MINUS);
	} else {
		if (DEBUGPRINT){
			cout << "Choose normal basis" << endl;
		}
		basisChoice = make_pair(ZERO, ONE);
	}
	return detectPulse(pulse, basisChoice);
}


Channel::Channel(BoolFactory *arg, StateTransformer *sdg) {
	absorptionRateGenerator = arg;
	stateDeviationGenerator = sdg;
}
Pulse Channel::propagate(Pulse& pulse) {
	Pulse propagatedPulse = Pulse();
	while(pulse.size() > 0) {
		auto extractedQubit = pulse.extract();
		auto state = make_pair(extractedQubit->alpha, extractedQubit->beta);
		extractedQubit->changeState(stateDeviationGenerator->operator()(state));

		if (absorptionRateGenerator->operator()() == false)
			propagatedPulse.insert(extractedQubit);
	}
	return propagatedPulse;
}

GeneratorInfo::GeneratorInfo(string _name, Generator *gen, string png, string bcg, string sdg) {
	name = _name;
	generator = gen;
	pulseNumberGeneratorName = png;
	basisChoiceGeneratorName = bcg;
	stateDeviationGeneratorName = sdg;
}

DetectorInfo::DetectorInfo(string _name, Detector *det, int dcr, string qeg, string bcg, string bdg) {
	name = _name;
	detector = det;
	darkCountRate = dcr;
	quantumEfficiencyGeneratorName = qeg;
	basisChoiceGeneratorName = bcg;
	basisDeviationGeneratorName = bdg;
}

ChannelInfo::ChannelInfo(string _name, Channel *chan, string arg, string sdg) {
	name = _name;
	channel = chan;
	AbsorptionRateGeneratorName = arg;
	stateDeviationGeneratorName = sdg;
}