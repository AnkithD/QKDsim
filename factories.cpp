#include <random>
#include <iostream>
#include <string>

#include "factories.h"

using namespace std;


IdealPulseNumberFactory::IdealPulseNumberFactory() {
	name = "Ideal Pulse number Factory";
}
int IdealPulseNumberFactory::operator()()  {
	return 1;
}

PoissonPulseNumberFactory::PoissonPulseNumberFactory(int lambda) {
	gen  = default_random_engine();
	dist = poisson_distribution<int>(lambda);
	name = string("Poisson Pulse Number Factory, lambda = ") + to_string(lambda);
}
PoissonPulseNumberFactory::PoissonPulseNumberFactory() {
	gen  = default_random_engine();
	cout << "Enter lambda,i.e. mean of Distribution: ";
	int lambda;
	cin >> lambda;
	dist = poisson_distribution<int>(lambda);
	name = string("Poisson Pulse Number Factory, lambda = ") + to_string(lambda);
}
int PoissonPulseNumberFactory::operator()() {
	return 1+dist(gen);
}

IntFactory* choosePulseNumberFactory() {
	IntFactory* chosenFactory;
	vector<string> factories {"Ideal Pulse Number Factory, Always generate single pulse",
							  "Poisson Pulse Number Factory, Pulses generated in Poisson Distribution according to Fock States"};

	int index = 1;
	for (auto name: factories) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which pulse number factory to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenFactory = new IdealPulseNumberFactory();
			break;
		}
		case 2: {
			chosenFactory = new PoissonPulseNumberFactory();
			break;
		}
		default:{
			cout << "Out of Index Pulse Number Factory choice" << endl;
			throw -1;
		}
	}

	return chosenFactory;
}


IdealBasisChoiceFactory::IdealBasisChoiceFactory() {
	name = "Ideal Basis Choice Factory";
}
bool IdealBasisChoiceFactory::operator()() {
	return (rand()%2 == 0);
}

AlwaysZeroOneBasisChoiceFactory::AlwaysZeroOneBasisChoiceFactory() {
	name = "Always <0|,<1| Basis Choice Factory";
}
bool AlwaysZeroOneBasisChoiceFactory::operator()() {
	return 0;
}

BoolFactory* chooseBasisChoiceFactory() {
	BoolFactory* chosenFactory;
	vector<string> factories {"Ideal Basis Choice Factory, 50% Psuedo-Random chance of either Basis",
							  "Always <0|,<1| Basis Choice Factory"};

	int index = 1;
	for (auto name: factories) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which basis choice factory to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenFactory = new IdealBasisChoiceFactory();
			break;
		}
		case 2: {
			chosenFactory = new AlwaysZeroOneBasisChoiceFactory();
			break;
		}
		default:{
			cout << "Out of Index Basis Choice Factory choice" << endl;
			throw -1;
		}
	}

	return chosenFactory;
}


IdealQuantumEfficiencyFactory::IdealQuantumEfficiencyFactory() {
	name = "Ideal Quantum Efficiency Factory";
}
bool IdealQuantumEfficiencyFactory::operator()() {
	return true;
}

BoolFactory* chooseQuantumEfficiencyFactory() {
	BoolFactory* chosenFactory;
	vector<string> factories {"Ideal Quantum Efficiency Factory, Every pulse is detected"};

	int index = 1;
	for (auto name: factories) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which Quantum Efficiency factory to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenFactory = new IdealQuantumEfficiencyFactory();
			break;
		}
		default:{
			cout << "Out of Index Quantum Efficiency Factory choice" << endl;
			throw -1;
		}
	}

	return chosenFactory;
}


IdealAbsorptionRateFactory::IdealAbsorptionRateFactory() {
	name = "Ideal Absorption Rate Factory";
}
bool IdealAbsorptionRateFactory::operator()(){
	return false;
}

PercentAbsorptionRateFactory::PercentAbsorptionRateFactory(double percent) {
	percentAbsorbed = percent;
	name = to_string(percentAbsorbed) + "% Absorption Rate Factory";
}
PercentAbsorptionRateFactory::PercentAbsorptionRateFactory() {
	cout << "Enter percent(0%-100%) of pulses absorbed: ";
	cin >> percentAbsorbed;
	name = to_string(percentAbsorbed) + "% Absorption Rate Factory"; 
}
bool PercentAbsorptionRateFactory::operator()() {
	return ((rand()%100000) < (percentAbsorbed*1000));
}

BoolFactory* chooseAbsorptionRateFactory() {
	BoolFactory* chosenFactory;
	vector<string> factories {"Ideal Absorption Rate Factory, No qubits absorbed",
							  "Percent Absorption Rate Factory, User defined percent of qubits absorbed"};

	int index = 1;
	for (auto name: factories) {
		cout << index << ")" << name << endl;
		index++;
	}
	cout << "Choose which Absorption Rate Factory to use: ";
	int choice;
	cin >> choice;
	switch(choice) {
		case 1: {
			chosenFactory = new IdealAbsorptionRateFactory();
			break;
		}
		case 2: {
			chosenFactory = new PercentAbsorptionRateFactory();
			break;
		}
		default:{
			cout << "Out of Index Absorption Rate Factory choice" << endl;
			throw -1;
		}
	}

	return chosenFactory;
}