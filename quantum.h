#ifndef _QUANTUM_H_
#define _QUANTUM_H_

#include <vector>

#include "constants.h"

using namespace std;

class Qubit {
private:
	bool perform_measure(amplitude zero_amp, amplitude one_amp);
public:
	amplitude alpha;
	amplitude beta;
	Qubit(const Qubit&) = delete;
	Qubit& operator=(const Qubit&) = delete;

	Qubit(state s);
	Qubit(amplitude a, amplitude b);
	bool observe();
	bool observe(basis basisChoice);
	void changeState(amplitude a, amplitude b);
	void changeState(state s);
};

class Pulse {
private:
	vector<Qubit*> qubits;
public:
	Pulse();
	Pulse(vector<Qubit*>& _qubits);
	Pulse(Qubit* qubit);

	Qubit* extract();
	int size();
	void insert(Qubit *qubit);
	Qubit* operator[] (int idx);
};

#endif