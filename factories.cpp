#include <random>

#include "factories.h"

using namespace std;

PulseNumberGeneratorInfo::PulseNumberGeneratorInfo(string _name, IntFactory *png) {
	name = _name;
	generator = png;
}

int IdealPulseNumberGenerator::operator()()  {
	return 1;
}

int PoissonPulseNumberGenerator::operator()() {
	return 1+dist(gen);
}

BasisChoiceGeneratorInfo::BasisChoiceGeneratorInfo(string _name, BoolFactory *bcg) {
	name = _name;
	generator = bcg;
}

bool IdealBasisChoiceGenerator::operator()() {
	return (rand()%2 == 0);
}

bool AlwaysZeroOneBasisChoiceGenerator::operator()() {
	return 0;
}

QuantumEfficiencyGeneratorInfo::QuantumEfficiencyGeneratorInfo(string _name, BoolFactory *qeg) {
	name = _name;
	generator = qeg;
}

bool IdealQuantumEfficiencyGenerator::operator()() {
	return true;
}

AbsorptionRateGeneratorInfo::AbsorptionRateGeneratorInfo(string _name, BoolFactory *arg) {
	name = _name;
	generator = arg;
}

bool IdealAbsorptionRateGenerator::operator()(){
	return false;
}

bool AlmostIdealAbsorptionRateGenerator::operator()() {
	return ((rand()%100) < 10);
}