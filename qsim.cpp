#include <vector>
#include <algorithm>
#include <complex>
#include <iostream> 
#include <random>

#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;
#define root2 (sqrt(2))

typedef complex<double> amplitude;
typedef pair<amplitude, amplitude> state;
typedef pair<state, state> basis;

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
			throw "0|0> + 0|1> is an invalid quantum state!";
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
	bool observe(basis basisChoice) {
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
	Qubit* operator[] (int idx) {
		if (idx >= size()  || idx < 0){
			cout << "Index " << idx << " is out of bounds (size=" <<  size() << ")";
			cout << endl;
			throw -1;
		} else {
			return qubits[idx];
		}
	} 
};

template <class T>
class RandomGenerator {
public:
	T operator()();
};

template <class T1, class T2>
class RandomTransformer {
public:
	T1 operator()(T2);
};

class Generator  {
private:
RandomGenerator<int> 	pulseNumberGenerator;
RandomGenerator<bool> 	basisChoiceGenerator;
RandomTransformer<state, state> stateDeviationGenerator;
public:
	Generator(RandomGenerator<int> png, RandomGenerator<bool> bcg,
			  RandomTransformer<state, state> sdg) {
		pulseNumberGenerator = png;
		basisChoiceGenerator = bcg;
		stateDeviationGenerator = sdg;
	}
	Pulse createPulse(amplitude a, amplitude b) {
		int pulseSize = pulseNumberGenerator();
		vector<Qubit*> qubits;
		for (int i = 0; i < pulseSize; ++i)
		{
			state deviatedState = stateDeviationGenerator(make_pair(a,b));
			amplitude zero_amp  = deviatedState.first;
			amplitude one_amp   = deviatedState.second; 
			qubits.push_back(new Qubit(zero_amp, one_amp));
		}
		return Pulse(qubits);
	}
	Pulse createPulse(state s) {
		return createPulse(s.first, s.second);
	}
	Pulse createPulse(bool value, bool basisChoice) {
		if (basisChoice == false) {
			return value? createPulse(ONE):createPulse(ZERO);
		} else {
			return value? createPulse(MINUS):createPulse(PLUS);
		}
	}
	Pulse createPulse(bool value) {
		bool basisChoice = basisChoiceGenerator();
		return createPulse(value, basisChoice);
	}

};

class Detector {
private:
int darkCountRate;
RandomGenerator<bool>	quantumEfficiencyGenerator;
RandomGenerator<bool> 	basisChoiceGenerator;
RandomTransformer<basis, basis> basisDeviationGenerator;
public:
	Detector(int dcr, RandomGenerator<bool> qeGen,
	 		 RandomGenerator<bool> bcGen,
	 	     RandomTransformer<basis, basis> bdGen) {
		darkCountRate = dcr;
		quantumEfficiencyGenerator = qeGen;
		basisChoiceGenerator = bcGen;
		basisDeviationGenerator = bdGen;
	}
	int detectPulse(Pulse pulse, basis basisChoice) {
		if (!quantumEfficiencyGenerator()) {
			return -1;
		}
		int size = pulse.size();
		Qubit *qubit = pulse[rand()%size];
		return qubit->observe(basisDeviationGenerator(basisChoice))? 1:0;
	}
	int detectPulse(Pulse pulse) {
		basis basisChoice;
		if (basisChoiceGenerator()) {
			basisChoice = make_pair(PLUS, MINUS);
		} else {
			basisChoice = make_pair(ZERO, ONE);
		}
		detectPulse(pulse, basisChoice);
	}
};



struct PulseNumberGeneratorInfo {
	string name;
	RandomGenerator<int> *pulseNumberGenerator;
	PulseNumberGeneratorInfo(string _name, RandomGenerator<int> *png) {
		name = _name;
		pulseNumberGenerator = png;
	}
};

class IdealPulseNumberGenerator : public RandomGenerator<int> {
	int operator()()  {
		return 1;
	}
};

class PoissonPulseNumberGenerator : public RandomGenerator<int> {
	private:
		default_random_engine gen = default_random_engine();
		poisson_distribution<int> dist = poisson_distribution<int>(0);
	public: 
	int operator()(){
		return 1+dist(gen);
	}
};

struct BasisChoiceGeneratorInfo {
	string name;
	RandomGenerator<bool> *basisChoiceGenerator;
	BasisChoiceGeneratorInfo(string _name, RandomGenerator<bool> *bcg) {
		name = _name;
		basisChoiceGenerator = bcg;
	}
};

class IdealBasisChoiceGenerator : public RandomGenerator<bool> {
	bool operator()() {
		return rand()%2;
	}
};

class AlwaysZeroOneBasisChoiceGenerator : public RandomGenerator<bool> {
	bool operator()() {
		return 0;
	}
};

struct StateDeviationGeneratorInfo {
	string name;
	RandomTransformer<state, state> *stateDeviationGenerator;
	StateDeviationGeneratorInfo(string _name, RandomTransformer<state, state> *sdg) {
		name = _name;
		stateDeviationGenerator = sdg;
	}
};

class IdealStateDeviationGenerator : public RandomTransformer<state, state> {
	state operator()(state s) {
		return s;
	}
};

class UniformCentiRadianStateDeviationGenerator : public RandomTransformer<state, state> {
private:
	default_random_engine gen = default_random_engine();
	uniform_real_distribution<double> dist = uniform_real_distribution<double>(-0.01, 0.01);
public:
	state operator()(state s) {
		auto zero_amp = s.first;
		auto one_amp = s.second;

		complex<double> phaseDelta = zero_amp / abs(zero_amp);
		double phi = arg(one_amp) - arg(zero_amp);
		double theta = 2 * acos(abs(zero_amp));

		double deviated_phi = phi + dist(gen);
		double deviated_theta = theta + dist(gen);

		amplitude deviated_zero_amp = phaseDelta * cos(deviated_theta/2);
		amplitude deviated_one_amp  = phaseDelta * exp(complex<double>(0,1) * deviated_phi) * sin(deviated_theta/2);

		return make_pair(deviated_zero_amp, deviated_one_amp);
	}
};

struct QuantumEfficiencyGeneratorInfo {
	string name;
	RandomGenerator<bool> *quantumEfficiencyGenerator;
	QuantumEfficiencyGeneratorInfo(string _name, RandomGenerator<bool> *qeg) {
		name = _name;
		quantumEfficiencyGenerator = qeg;
	}
};

class IdealQuantumEfficiencyGenerator : public RandomGenerator<bool> {
	bool operator()(){
		return true;
	}
};

struct BasisDeviationGeneratorInfo {
	string name;
	RandomTransformer<basis, basis> *basisDeviationGenerator;
	BasisDeviationGeneratorInfo(string _name, RandomTransformer<basis, basis> *bdg) {
		name = _name;
		basisDeviationGenerator = bdg;
	}
};

class IdealBasisDeviationGenerator : public RandomTransformer<basis, basis> {
	basis operator()(basis b) {
		return b;
	}
};

struct GeneratorInfo {
	string name;
	Generator *generator;
	string pulseNumberGeneratorName;
	string 	basisChoiceGeneratorName;
	string 	stateDeviationGeneratorName;
	GeneratorInfo(string _name, Generator *gen, string png, string bcg, string sdg) {
		name = _name;
		generator = gen;
		pulseNumberGeneratorName = png;
		basisChoiceGeneratorName = bcg;
		stateDeviationGeneratorName = sdg;
	}
};

struct DetectorInfo {
	string 	name;
	Detector *detector;
	int darkCountRate;
	string	quantumEfficiencyGeneratorName;
	string 	basisChoiceGeneratorName;
	string  basisDeviationGeneratorName;
	DetectorInfo(string _name, Detector *det, int dcr, string qeg, string bcg, string bdg) {
		name = _name;
		detector = det;
		darkCountRate = dcr;
		quantumEfficiencyGeneratorName = qeg;
		basisChoiceGeneratorName = bcg;
		basisDeviationGeneratorName = bdg;
	}
};

int main() {
	srand(time(NULL));

	vector<PulseNumberGeneratorInfo> PulseNumberGenerators;
	auto idealPulseNumberGenerator = PulseNumberGeneratorInfo(
		"Ideal Pulse Number Generator", new IdealPulseNumberGenerator());
	auto poissonPulseNumberGenerator = PulseNumberGeneratorInfo(
		"Poisson Pulse Number Generator", new PoissonPulseNumberGenerator());
	PulseNumberGenerators.push_back(idealPulseNumberGenerator);
	PulseNumberGenerators.push_back(poissonPulseNumberGenerator);

	vector<BasisChoiceGeneratorInfo> BasisChoiceGenerators;
	auto idealBasisChoiceGenerator = BasisChoiceGeneratorInfo(
		"Ideal Basis Choice Generator", new IdealBasisChoiceGenerator());
	auto alwaysZeroOneBasisChoiceGenerator = BasisChoiceGeneratorInfo(
		"Always |0>,|1> Basis Choice Generator", new AlwaysZeroOneBasisChoiceGenerator());
	BasisChoiceGenerators.push_back(idealBasisChoiceGenerator);
	BasisChoiceGenerators.push_back(alwaysZeroOneBasisChoiceGenerator);


	vector<StateDeviationGeneratorInfo> StateDeviationGenerators;
	auto idealStateDeviationGenerator = StateDeviationGeneratorInfo(
		"Ideal State Deviation Generator", new IdealStateDeviationGenerator());
	auto uniformCentiRadianStateDeviationGenerator = StateDeviationGeneratorInfo(
		"Uniform 0.01-radian State Deviation Generator",
		new UniformCentiRadianStateDeviationGenerator());
	StateDeviationGenerators.push_back(idealStateDeviationGenerator);
	StateDeviationGenerators.push_back(uniformCentiRadianStateDeviationGenerator);

	vector<QuantumEfficiencyGeneratorInfo> QuantumEfficiencyGenerators;
	auto idealQuantumEfficiencyGenerator = QuantumEfficiencyGeneratorInfo(
		"Ideal Quantum Efficiency Generator", new IdealQuantumEfficiencyGenerator());
	QuantumEfficiencyGenerators.push_back(idealQuantumEfficiencyGenerator);

	vector<BasisDeviationGeneratorInfo> BasisDeviationGenerators;
	auto idealBasisDeviationGenerator = BasisDeviationGeneratorInfo(
		"Ideal Basis Deviation Generator", new IdealBasisDeviationGenerator());
	BasisDeviationGenerators.push_back(idealBasisDeviationGenerator);

	vector<GeneratorInfo> Generators;
	auto idealGenerator = Generator(IdealPulseNumberGenerator(),
									IdealBasisChoiceGenerator(),
									IdealStateDeviationGenerator());
	auto idealGeneratorInfo = GeneratorInfo("Ideal Generator", &idealGenerator,
											 "Ideal Pulse Number Generator",
											 "Ideal Basis Choice Generator",
											 "Ideal State Deviation Generator");
	Generators.push_back(idealGeneratorInfo);

	vector<DetectorInfo> Detectors;
	auto idealDetector = Detector(0, IdealQuantumEfficiencyGenerator(),
								  IdealBasisChoiceGenerator(), 
								  IdealBasisDeviationGenerator());
	auto idealDetectorInfo = DetectorInfo("Ideal Detector", &idealDetector,
										  0,
										  "Ideal Quantum Efficiency Generator",
										  "Ideal Basis Choice Generator",
										  "Ideal Basis Deviation Generator");
	Detectors.push_back(idealDetectorInfo);

	while(true) {
		cout << "(1) View Generators" << endl;
		cout << "(2) View Detectors" << endl;
		cout << "(3) Add  Generators" << endl;
		cout << "(4) Add  Detectors" << endl;
		cout << "(5) Run a QKD algorithm" << endl;
		cout << "What would you like to do:";
		int choice;
		cin >> choice;
		cout << endl;
		switch(choice) {
			case 1:{
				int index = 1;
				for (auto info: Generators) {
					cout << index << ") " << info.name << endl;
					cout << "\tPulse Number Generator: " << info.pulseNumberGeneratorName << endl;
					cout << "\tBasis Choice Generator: " << info.basisChoiceGeneratorName << endl;
					cout << "\tState Deviation Generator: " << info.stateDeviationGeneratorName << endl;
					cout << endl;
					index++;
				}
				break;
			}
			case 2:{
				int index = 1;
				for (auto info: Detectors) {
					cout << index << ") " << info.name << endl;
					cout << "\tDark Count Rate: " << info.darkCountRate << " Hz" << endl;
					cout << "\tQuantum Efficiency Generator: " << info.quantumEfficiencyGeneratorName << endl;
					cout << "\tBasis Choice Generator: " << info.basisChoiceGeneratorName << endl;
					cout << "\tBasis Deviation Generator: " << info.basisDeviationGeneratorName << endl;
					cout << endl;
					index++;
				}
				break;
			}
			case 3:{
				int index;
				int choice;

				index = 1;
				for (auto info: PulseNumberGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which pulse number generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > PulseNumberGenerators.size()){
					throw "Out of Index png choice";
				}
				auto png = PulseNumberGenerators[choice-1];

				index = 1;
				for (auto info: BasisChoiceGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis choice generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisChoiceGenerators.size()){
					throw "Out of Index bcg choice";
				}
				auto bcg = BasisChoiceGenerators[choice-1];

				index = 1;
				for (auto info: StateDeviationGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which state deviation generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > StateDeviationGenerators.size()){
					throw "Out of Index sdg choice";
				}
				auto sdg = StateDeviationGenerators[choice-1];

				cout << "Name the generator: ";
				string name;
				cin >> name;

				auto generator = new Generator(*png.pulseNumberGenerator,
										   *bcg.basisChoiceGenerator, 
										   *sdg.stateDeviationGenerator);
				Generators.push_back(GeneratorInfo(name, generator, png.name, bcg.name, sdg.name));
				break;
			}
			case 4:{
				int index;
				int choice;

				cout << "Enter Detector dark count rate in Hz: ";
				int darkCountRate;
				cin >> darkCountRate;

				index = 1;
				for (auto info: QuantumEfficiencyGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which quantum efficiency generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > QuantumEfficiencyGenerators.size()){
					throw "Out of Index qeg choice";
				}
				auto qeg = QuantumEfficiencyGenerators[choice-1];

				index = 1;
				for (auto info: BasisChoiceGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis choice generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisChoiceGenerators.size()){
					throw "Out of Index bcg choice";
				}
				auto bcg = BasisChoiceGenerators[choice-1];

				index = 1;
				for (auto info: BasisDeviationGenerators) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis deviation generator to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisDeviationGenerators.size()){
					throw "Out of Index bdg choice";
				}
				auto bdg = BasisDeviationGenerators[choice-1];

				cout << "Name the Detector: ";
				string name;
				cin >> name;

				auto detector = new Detector(darkCountRate,
										   *qeg.quantumEfficiencyGenerator,
										   *bcg.basisChoiceGenerator, 
										   *bdg.basisDeviationGenerator);
				Detectors.push_back(DetectorInfo(name, detector, darkCountRate,qeg.name, bcg.name, bdg.name));
				break;
			}
			default:{
				cout << "Invalid Choice" << endl;
				return 0;
			}
		}
	}

	cout << endl << endl;

	return 0;
}