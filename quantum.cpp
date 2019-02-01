#include <random>
#include <iostream>

#include "quantum.h"

using namespace std;

bool Qubit::perform_measure(amplitude zero_amp, amplitude one_amp) {
	bool observation;

	int probA = (int) (norm(zero_amp) * (1<<((sizeof(int)*4)-1)));
	int probB = (int) (norm(one_amp)  * (1<<((sizeof(int)*4)-1)));

	int randValue = rand() % (probA+probB);
	if (randValue < probA) {
		observation = false;
	} else {
		observation = true;
	}
	if (DEBUGPRINT) {
		cout << "qubit:" << alpha << "," << beta << " observed to be " << observation << "(";
		cout << probA << "," << probB << "," << randValue << ")" << endl;
	}
	return observation;
}
Qubit::Qubit(state s) {
	Qubit(s.first, s.second);
}
Qubit::Qubit(amplitude a, amplitude b) {
	double squareSum = norm(a) + norm(b);
	if (abs(squareSum-0) <= eps) {
		cout << "0|0> + 0|1> is an invalid quantum state!" << endl;
		throw -1;
	} else if (abs(squareSum-1) > eps) {
		cout << "Renormalizing input!" << endl;
		a /= sqrt(squareSum);
		b /= sqrt(squareSum);
	}
	alpha = a;
	beta = b;
};
bool Qubit::observe() {
	bool observation = perform_measure(alpha, beta);
	if (observation) {
		alpha = 0;
		beta = 1;
	} else {
		alpha = 1;
		beta = 0;
	}

	return observation;
}
bool Qubit::observe(basis basisChoice) {
	state first_state, second_state;
	first_state  = basisChoice.first;
	second_state = basisChoice.second;

	amplitude a1,b1,a2,b2;
	a1 = first_state.first;
	b1 = first_state.second;
	a2 = second_state.first;
	b2 = second_state.second;

	amplitude new_alpha, new_beta;
	new_alpha = ((alpha*b2)-(beta*a2)) / ((a1*b2)-(b1*a2));
	new_beta  = ((alpha*b1)-(beta*a1)) / ((a2*b1)-(b2*a1));
	// cout << new_alpha << "|" << new_beta << ":";

	bool observation = perform_measure(new_alpha, new_beta);
	if (observation) {
		alpha = a2;
		beta = b2;
	} else {
		alpha = a1;
		beta = b1;
	}

	return observation;
}
void Qubit::changeState(amplitude a, amplitude b) {
	double squareSum = norm(a) + norm(b);
	if (abs(squareSum-0) < eps) {
		cout << "0|0> + 0|1> is an invalid quantum state!" << endl;
		throw -1;
	} else if (abs(squareSum-1) > eps) {
		cout << "Renormalizing input!" << endl;
		a /= sqrt(squareSum);
		b /= sqrt(squareSum);
	}
	alpha = a;
	beta = b;
}
void Qubit::changeState(state s) {
	changeState(s.first, s.second);
}


Pulse::Pulse() {
	qubits = vector<Qubit*>();
}
Pulse::Pulse(vector<Qubit*>& _qubits) {
	for (auto q : _qubits) {
		qubits.push_back(q);
	}
}
Pulse::Pulse(Qubit* qubit){
	qubits.push_back(qubit);
}

Qubit* Pulse::extract() {
	auto extractedQubit = qubits.back();
	qubits.pop_back();
	return extractedQubit;
}
int Pulse::size() {
	return qubits.size();
}
void Pulse::insert(Qubit *qubit) {
	qubits.push_back(qubit);
}
Qubit* Pulse::operator[] (int idx) {
	if (idx >= size()  || idx < 0){
		cout << "Index " << idx << " is out of bounds (size=" <<  size() << ")";
		cout << endl;
		throw -1;
	} else {
		return qubits[idx];
	}
}