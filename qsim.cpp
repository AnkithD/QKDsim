#include <vector>
#include <algorithm>
#include <complex>
#include <iostream> 

#include <cstdlib>
#include <ctime>
#include <cmath>

#include <iostream>

using namespace std;
#define root2 (sqrt(2))

typedef complex<double> amplitude;
typedef pair<amplitude, amplitude> state;

state PLUS  = make_pair(amplitude(1/root2), amplitude(1/root2));
state MINUS = make_pair(amplitude(1/root2), amplitude(-1/root2));
state ONE   = make_pair(amplitude(0), amplitude(1)); 
state ZERO  = make_pair(amplitude(1), amplitude(0)); 

class Qubit {
private:
	amplitude alpha;
	amplitude beta;
	bool perform_measure(amplitude zero_amp, amplitude one_amp) {
		bool observation;

		int probA = (int) (norm(zero_amp) * 100000000);
		int probB = (int) (norm(one_amp)  * 100000000);

		int randValue = rand() % (probA+probB);
		// cout << randValue << "(" << probA << "|" << probB << ")" << endl;
		if (randValue < probA) {
			observation = false;
		} else {
			observation = true;
		}
		return observation;
	}
public:
	Qubit(const Qubit&) = delete;
	Qubit& operator=(const Qubit&) = delete;

	Qubit(state s) {
		Qubit(s.first, s.second);
	}
	Qubit(amplitude a, amplitude b) {
		double squareSum = norm(a) + norm(b);
		if (squareSum == 0) {
			throw "0|0>+0|1> is an invalid quantum state!";
		} else if (squareSum != 1) {
			cout << "Renormalizing input!" << endl;
			a /= sqrt(squareSum);
			b /= sqrt(squareSum);
		}
		alpha = a;
		beta = b;
	};
	bool observe() {
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
	bool observe(pair<state, state> basis) {
		state first_state, second_state;
		first_state  = basis.first;
		second_state = basis.second;

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
};

class Pulse {
private:
	vector<Qubit*> qubits;
public:
	Pulse(vector<Qubit*>& _qubits) {
		for (auto q : _qubits) {
			qubits.push_back(q);
		}
	}

	Qubit* extract() {
		auto extractedQubit = qubits.back();
		qubits.pop_back();
		return extractedQubit;
	}
	int size() {
		return qubits.size();
	}
	void insert(Qubit *qubit) {
		qubits.push_back(qubit);
	} 
};

template <class T>
class RandomGenerator {
public:
	virtual T operator()();
};

class Generator  {
private:
RandomGenerator<int> 	pulseNumberGenerator;
RandomGenerator<bool> 	basisGenerator;
public:
	Generator(RandomGenerator<int> gen) {
		pulseNumberGenerator = gen;
	}
	Pulse createPulse(amplitude a, amplitude b) {
		int pulseSize = pulseNumberGenerator();
		vector<Qubit*> qubits;
		for (int i = 0; i < pulseSize; ++i)
		{
			qubits.push_back(new Qubit(a,b));
		}
		return Pulse(qubits);
	}
	Pulse createPulse(state s) {
		return createPulse(s.first, s.second);
	}
	Pulse createPulse(bool value, bool basis) {
		if (basis) {
			return value? createPulse(ONE):createPulse(ZERO);
		} else {
			return value? createPulse(MINUS):createPulse(PLUS);
		}
	}
	Pulse createPulse(bool value) {
		bool basis = basisGenerator();
		return createPulse(value, basis);
	}

};

class Detector {
private:
RandomGenerator<bool> 	darkCountGenerator;
RandomGenerator<bool>	detectionGenerator; 
public:
	Detector(RandomGenerator<bool> dcGen, RandomGenerator<bool> dGen) {
		darkCountGenerator = dcGen;
		detectionGenerator = dGen;
	}
	int detectPulse(Pulse pulse, pair<state,state> basis) {
		if (!detectionGenerator()) {
			return -1;
		}
		int size = pulse.size;
		Qubit &qubit = pulse[rand()%size];
		return qubit.observe(basis)? 1:0;
	}
	int tick() {
		if (darkCountGenerator()) {
			return rand() % 2;
		} else {
			return -1;
		}
	}
};

int main() {
	srand(time(NULL));

	cout << endl << endl;
}