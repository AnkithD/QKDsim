#ifndef _DEVICES_H_
#define _DEVICES_H_

#include <random>

#include "constants.h"
#include "quantum.h"
#include "factories.h"
#include "transformers.h"

using namespace std;

class Generator  {
private:
IntFactory 			*pulseNumberFactory;
BoolFactory 		*basisChoiceFactory;
StateTransformer 	*stateDeviationTransformer;
public:
	Generator(IntFactory *png, BoolFactory *bcg, StateTransformer *sdg);
	Pulse createPulse(amplitude a, amplitude b);
	Pulse createPulse(state s);
	Pulse createPulse(bool value, bool basisChoice);
	Pulse createPulse(bool value);
};

class Detector {
private:
int darkCountRate;
BoolFactory	*quantumEfficiencyFactory;
BoolFactory 	*basisChoiceFactory;
BasisTransformer *basisDeviationTransformer;
public:
	Detector(int dcr, BoolFactory *qeGen, BoolFactory *bcGen, BasisTransformer *bdGen);
	int detectPulse(Pulse pulse, basis basisChoice);
	int detectPulse(Pulse pulse);
	int detectPulse(Pulse pulse, bool commonBasisChoice);
};

class Channel {
private:
	BoolFactory *absorptionRateFactory;
	StateTransformer *stateDeviationTransformer;
public:
	Channel(BoolFactory *arg, StateTransformer *sdg);
	Pulse propagate(Pulse& pulse);
};

struct GeneratorInfo {
	string name;
	Generator *generator;
	string pulseNumberFactoryName;
	string 	basisChoiceFactoryName;
	string 	stateDeviationTransformerName;
	GeneratorInfo(string _name, Generator *gen, string png, string bcg, string sdg);
};

struct DetectorInfo {
	string 	name;
	Detector *detector;
	int darkCountRate;
	string	quantumEfficiencyFactoryName;
	string 	basisChoiceFactoryName;
	string  basisDeviationTransformerName;
	DetectorInfo(string _name, Detector *det, int dcr, string qeg, string bcg, string bdg);
};

struct ChannelInfo {
	string name;
	Channel *channel;
	string AbsorptionRateFactoryName;
	string stateDeviationTransformerName;
	ChannelInfo(string _name, Channel *chan, string arg, string sdg);
};

#endif