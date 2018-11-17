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

class Qubit {
private:
	amplitude alpha;
	amplitude beta;
public:
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
	vector<Qubit> qubits;
public:
	Pulse(vector<Qubit> _qubits) {
		qubits = _qubits;
	}
};

class Sender  {

};

class Receiver {

};

int main() {
	srand(time(NULL));

	cout << endl << endl;
}