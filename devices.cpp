#include <vector>
#include <complex>
#include <algorithm>
#include <iostream>

#include "devices.h"

using namespace std;


Generator::Generator(IntFactory *png, BoolFactory *bcg, StateTransformer *sdg) {
	pulseNumberFactory = png;
	basisChoiceFactory = bcg;
	stateDeviationTransformer = sdg;
}
Pulse Generator::createPulse(amplitude a, amplitude b) {
	int pulseSize = pulseNumberFactory->operator()();
	vector<Qubit*> qubits;
	for (int i = 0; i < pulseSize; ++i)
	{
		state deviatedState = stateDeviationTransformer->operator()(make_pair(a,b));
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
	bool basisChoice = basisChoiceFactory->operator()();
	return createPulse(value, basisChoice);
}


Detector::Detector(int dcr, BoolFactory *qeGen, BoolFactory *bcGen, BasisTransformer *bdGen) {
	darkCountRate = dcr;
	quantumEfficiencyFactory = qeGen;
	basisChoiceFactory = bcGen;
	basisDeviationTransformer = bdGen;
}
int Detector::detectPulse(Pulse pulse, basis basisChoice) {
	if (!(quantumEfficiencyFactory->operator()())) {
		return -1;
	}
	int size = pulse.size();
	Qubit *qubit = pulse[rand()%size];
	bool observation = qubit->observe(basisDeviationTransformer->operator()(basisChoice));
	if (DEBUGPRINT) {
		//cout << "Detecting qbit: " << qubit->alpha << "," << qubit->beta << endl;
	}
	return (observation)? 1:0;
}
int Detector::detectPulse(Pulse pulse) {
	basis basisChoice;
	if (basisChoiceFactory->operator()()) {
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
	absorptionRateFactory = arg;
	stateDeviationTransformer = sdg;
}
Pulse Channel::propagate(Pulse& pulse) {
	Pulse propagatedPulse = Pulse();
	while(pulse.size() > 0) {
		auto extractedQubit = pulse.extract();
		auto state = make_pair(extractedQubit->alpha, extractedQubit->beta);
		extractedQubit->changeState(stateDeviationTransformer->operator()(state));

		if (absorptionRateFactory->operator()() == false)
			propagatedPulse.insert(extractedQubit);
	}
	return propagatedPulse;
}

GeneratorInfo::GeneratorInfo(string _name, Generator *gen, string png, string bcg, string sdg) {
	name = _name;
	generator = gen;
	pulseNumberFactoryName = png;
	basisChoiceFactoryName = bcg;
	stateDeviationTransformerName = sdg;
}

DetectorInfo::DetectorInfo(string _name, Detector *det, int dcr, string qeg, string bcg, string bdg) {
	name = _name;
	detector = det;
	darkCountRate = dcr;
	quantumEfficiencyFactoryName = qeg;
	basisChoiceFactoryName = bcg;
	basisDeviationTransformerName = bdg;
}

ChannelInfo::ChannelInfo(string _name, Channel *chan, string arg, string sdg) {
	name = _name;
	channel = chan;
	AbsorptionRateFactoryName = arg;
	stateDeviationTransformerName = sdg;
}