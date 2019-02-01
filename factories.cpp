#include <random>

#include "factories.h"

using namespace std;

PulseNumberFactoryInfo::PulseNumberFactoryInfo(string _name, IntFactory *png) {
	name = _name;
	generator = png;
}

int IdealPulseNumberFactory::operator()()  {
	return 1;
}

int PoissonPulseNumberFactory::operator()() {
	return 1+dist(gen);
}

BasisChoiceFactoryInfo::BasisChoiceFactoryInfo(string _name, BoolFactory *bcg) {
	name = _name;
	generator = bcg;
}

bool IdealBasisChoiceFactory::operator()() {
	return (rand()%2 == 0);
}

bool AlwaysZeroOneBasisChoiceFactory::operator()() {
	return 0;
}

QuantumEfficiencyFactoryInfo::QuantumEfficiencyFactoryInfo(string _name, BoolFactory *qeg) {
	name = _name;
	generator = qeg;
}

bool IdealQuantumEfficiencyFactory::operator()() {
	return true;
}

AbsorptionRateFactoryInfo::AbsorptionRateFactoryInfo(string _name, BoolFactory *arg) {
	name = _name;
	generator = arg;
}

bool IdealAbsorptionRateFactory::operator()(){
	return false;
}

bool AlmostIdealAbsorptionRateFactory::operator()() {
	return ((rand()%100) < 10);
}