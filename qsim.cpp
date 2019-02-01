#include <vector>
#include <algorithm>
#include <complex>
#include <iostream> 
#include <random>

#include <cstdlib>
#include <ctime>
#include <cmath>

#include "constants.h"
#include "quantum.h"
#include "devices.h"
#include "factories.h"
#include "transformers.h"

using namespace std;

int main() {
	srand(time(NULL));

	vector<PulseNumberFactoryInfo> PulseNumberFactories;
	auto idealPulseNumberFactory = PulseNumberFactoryInfo(
		"Ideal Pulse Number Factory", new IdealPulseNumberFactory());
	auto poissonPulseNumberFactory = PulseNumberFactoryInfo(
		"Poisson Pulse Number Factory", new PoissonPulseNumberFactory());
	PulseNumberFactories.push_back(idealPulseNumberFactory);
	PulseNumberFactories.push_back(poissonPulseNumberFactory);

	vector<BasisChoiceFactoryInfo> BasisChoiceFactories;
	auto idealBasisChoiceFactory = BasisChoiceFactoryInfo(
		"Ideal Basis Choice Factory", new IdealBasisChoiceFactory());
	auto alwaysZeroOneBasisChoiceFactory = BasisChoiceFactoryInfo(
		"Always |0>,|1> Basis Choice Factory", new AlwaysZeroOneBasisChoiceFactory());
	BasisChoiceFactories.push_back(idealBasisChoiceFactory);
	BasisChoiceFactories.push_back(alwaysZeroOneBasisChoiceFactory);


	vector<StateDeviationTransformerInfo> StateDeviationTransformers;
	auto idealStateDeviationTransformer = StateDeviationTransformerInfo(
		"Ideal State Deviation Transformer", new IdealStateDeviationTransformer());
	auto uniformCentiRadianStateDeviationTransformer = StateDeviationTransformerInfo(
		"Uniform 0.01-radian State Deviation Transformer",
		new UniformCentiRadianStateDeviationTransformer());
	StateDeviationTransformers.push_back(idealStateDeviationTransformer);
	StateDeviationTransformers.push_back(uniformCentiRadianStateDeviationTransformer);

	vector<QuantumEfficiencyFactoryInfo> QuantumEfficiencyFactories;
	auto idealQuantumEfficiencyFactory = QuantumEfficiencyFactoryInfo(
		"Ideal Quantum Efficiency Factory", new IdealQuantumEfficiencyFactory());
	QuantumEfficiencyFactories.push_back(idealQuantumEfficiencyFactory);

	vector<BasisDeviationTransformerInfo> BasisDeviationTransformers;
	auto idealBasisDeviationTransformer = BasisDeviationTransformerInfo(
		"Ideal Basis Deviation Transformer", new IdealBasisDeviationTransformer());
	BasisDeviationTransformers.push_back(idealBasisDeviationTransformer);

	vector<AbsorptionRateFactoryInfo> AbsorptionRateFactories;
	auto idealAbsorptionRateFactory = AbsorptionRateFactoryInfo(
		"Ideal Aborption Rate Factory", new IdealAbsorptionRateFactory());
	auto almostIdealAbsorptionRateFactory = AbsorptionRateFactoryInfo(
		"10% Absorption Rate Factory", new AlmostIdealAbsorptionRateFactory());
	AbsorptionRateFactories.push_back(idealAbsorptionRateFactory);
	AbsorptionRateFactories.push_back(almostIdealAbsorptionRateFactory);

	vector<GeneratorInfo> Generators;
	auto idealGenerator = Generator(idealPulseNumberFactory.generator,
									idealBasisChoiceFactory.generator,
									idealStateDeviationTransformer.generator);
	auto idealGeneratorInfo = GeneratorInfo("Ideal Generator", &idealGenerator,
											idealPulseNumberFactory.name,
											idealBasisChoiceFactory.name,
											idealStateDeviationTransformer.name);
	Generators.push_back(idealGeneratorInfo);

	vector<DetectorInfo> Detectors;
	auto idealDetector = Detector(0, idealQuantumEfficiencyFactory.generator,
								  idealBasisChoiceFactory.generator, 
								  idealBasisDeviationTransformer.generator);
	auto idealDetectorInfo = DetectorInfo("Ideal Detector", &idealDetector,
										0,
										idealQuantumEfficiencyFactory.name,
								  		idealBasisChoiceFactory.name, 
								  		idealBasisDeviationTransformer.name);
	Detectors.push_back(idealDetectorInfo);

	vector<ChannelInfo> Channels;
	auto idealChannel = Channel(idealAbsorptionRateFactory.generator,
								idealStateDeviationTransformer.generator);
	auto idealChannelInfo = ChannelInfo("Ideal Channel", &idealChannel,
										idealAbsorptionRateFactory.name,
										idealStateDeviationTransformer.name);
	Channels.push_back(idealChannelInfo);

	while(true) {
		cout << "(1) View Generators" << endl;
		cout << "(2) View Detectors" << endl;
		cout << "(3) View Channels" << endl;
		cout << "(4) Add  Generators" << endl;
		cout << "(5) Add  Detectors" << endl;
		cout << "(6) Add  Channels" << endl;
		cout << "(7) Run a QKD algorithm" << endl;
		cout << "(8) Turn Debug statements " << (DEBUGPRINT?"Off":"On") << endl;
		cout << "What would you like to do:";
		int choice;
		cin >> choice;
		cout << endl;
		switch(choice) {
			case 1:{
				int index = 1;
				for (auto info: Generators) {
					cout << index << ") " << info.name << endl;
					cout << "\tPulse Number Factory: " << info.pulseNumberFactoryName << endl;
					cout << "\tBasis Choice Factory: " << info.basisChoiceFactoryName << endl;
					cout << "\tState Deviation Transformer: " << info.stateDeviationTransformerName << endl;
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
					cout << "\tQuantum Efficiency Factory: " << info.quantumEfficiencyFactoryName << endl;
					cout << "\tBasis Choice Factory: " << info.basisChoiceFactoryName << endl;
					cout << "\tBasis Deviation Transformer: " << info.basisDeviationTransformerName << endl;
					cout << endl;
					index++;
				}
				break;
			}
			case 3:{
				int index = 1;
				for (auto info: Channels) {
					cout << index << ") " << info.name << endl;
					cout << "\tAbsorption Rate Factory: " << info.AbsorptionRateFactoryName << endl;
					cout << "\tState Deviation Transformer: " << info.stateDeviationTransformerName << endl;
					cout << endl;
					index++;
				}
				break;
			}
			case 4:{
				int index;
				int choice;

				index = 1;
				for (auto info: PulseNumberFactories) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which pulse number factory to use: ";
				cin >> choice;
				if (choice <= 0 || choice > PulseNumberFactories.size()){
					cout << "Out of Index png choice" << endl;
					throw -1;
				}
				auto png = PulseNumberFactories[choice-1];

				index = 1;
				for (auto info: BasisChoiceFactories) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis choice factory to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisChoiceFactories.size()){
					cout << "Out of Index bcg choice" << endl;
					throw -1;
				}
				auto bcg = BasisChoiceFactories[choice-1];

				index = 1;
				for (auto info: StateDeviationTransformers) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which state deviation transformer to use: ";
				cin >> choice;
				if (choice <= 0 || choice > StateDeviationTransformers.size()){
					cout << "Out of Index sdg choice" << endl;
					throw -1;
				}
				auto sdg = StateDeviationTransformers[choice-1];

				cout << "Name the generator: ";
				string name;
				cin >> name;

				auto generator = new Generator(png.generator,
										   bcg.generator, 
										   sdg.generator);
				Generators.push_back(GeneratorInfo(name, generator, png.name, bcg.name, sdg.name));
				break;
			}
			case 5:{
				int index;
				int choice;

				cout << "Enter Detector dark count rate in Hz: ";
				int darkCountRate;
				cin >> darkCountRate;

				index = 1;
				for (auto info: QuantumEfficiencyFactories) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which quantum efficiency factory to use: ";
				cin >> choice;
				if (choice <= 0 || choice > QuantumEfficiencyFactories.size()){
					cout << "Out of Index qeg choice" << endl;
					throw -1;
				}
				auto qeg = QuantumEfficiencyFactories[choice-1];

				index = 1;
				for (auto info: BasisChoiceFactories) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis choice factory to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisChoiceFactories.size()){
					cout << "Out of Index bcg choice" << endl;
					throw -1;
				}
				auto bcg = BasisChoiceFactories[choice-1];

				index = 1;
				for (auto info: BasisDeviationTransformers) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which basis deviation transformer to use: ";
				cin >> choice;
				if (choice <= 0 || choice > BasisDeviationTransformers.size()){
					cout << "Out of Index bdg choice" << endl;
					throw -1;
				}
				auto bdg = BasisDeviationTransformers[choice-1];

				cout << "Name the Detector: ";
				string name;
				cin >> name;

				auto detector = new Detector(darkCountRate,
										   qeg.generator,
										   bcg.generator, 
										   bdg.generator);
				Detectors.push_back(DetectorInfo(name, detector, darkCountRate,qeg.name, bcg.name, bdg.name));
				break;
			}
			case 6:{
				int index;
				int choice;

				index = 1;
				for (auto info: AbsorptionRateFactories) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which absorption rate factory to use: ";
				cin >> choice;
				if (choice <= 0 || choice > AbsorptionRateFactories.size()){
					cout << "Out of Index arg choice" << endl;
					throw -1;
				}
				auto arg = AbsorptionRateFactories[choice-1];

				index = 1;
				for (auto info: StateDeviationTransformers) {
					cout << index << ")" << info.name << endl;
					index++;
				}
				cout << "Choose which state deviation transformer to use: ";
				cin >> choice;
				if (choice <= 0 || choice > StateDeviationTransformers.size()){
					cout << "Out of Index sdg choice" << endl;
					throw -1;
				}
				auto sdg = StateDeviationTransformers[choice-1];

				cout << "Name the generator: ";
				string name;
				cin >> name;

				auto channel = new Channel(arg.generator,
										   sdg.generator);
				Channels.push_back(ChannelInfo(name, channel, arg.name, sdg.name));
				break;
			}
			case 7:{
				int index;
				int choice;

				cout << "(1) Standard (No Eve)" << endl;
				cout << "(2) Photon Splitting Attack" << endl;
				cout << "(3) Naive Eve" << endl;
				cout << "Choose which algortihm to run: ";
				cin >> choice;

				switch(choice) {
					case 1: {					
						index = 1;
						for (auto info: Generators) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose which generator to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Generators.size()){
							cout << "Out of Index generator choice" << endl;
							throw -1;
						}
						auto generator = Generators[choice-1].generator;

						index = 1;
						for (auto info: Detectors) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose which detector to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Detectors.size()){
							cout << "Out of Index detector choice" << endl;
							throw -1;
						}
						auto detector = Detectors[choice-1].detector;

						index = 1;
						for (auto info: Channels) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose which channel to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Channels.size()){
							cout << "Out of Index channel choice" << endl;
							throw -1;
						}
						auto channel = Channels[choice-1].channel;

						cout << "(1)Generate random bitstring to transmit" << endl;
						cout << "(2)Manually input bitstring to transmit" << endl;
						cout << "Choose:";
						cin >> choice;

						string bitstring;
						switch (choice) {
							case 1: {
								cout << "Enter bitstring length: ";
								int len;
								cin >> len;
								for (int i = 0; i < len; ++i) {
									bitstring += (rand()%2 == 0) ? "1":"0";
								}
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> bitstring;
								break;
							}
							default:{
								cout << "Invalid choice for bitstring" << endl;
								throw -1;
							}
						}

						cout << "(1)Generate random basis chocies to transmit" << endl;
						cout << "(2)Manually input basis choices as bitstring" << endl;
						cout << "Choose:";
						cin >> choice;

						string sourceBasisChoiceString;
						switch (choice) {
							case 1: {
								sourceBasisChoiceString = "auto";
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> sourceBasisChoiceString;
								if (sourceBasisChoiceString.size() != bitstring.size()){
									cout << "Mismatch in length of bitstring and basis choice bitstring" << endl;
									throw -1;
								}
								break;
							}
							default:{
								cout << "Invalid choice for basis choice bitstring" << endl;
								throw -1;
							}
						}

						cout << "(1)Generate random basis chocies for detector" << endl;
						cout << "(2)Manually input basis choices for detector" << endl;
						cout << "Choose:";
						cin >> choice;

						string detectorBasisChoiceString;
						switch (choice) {
							case 1: {
								detectorBasisChoiceString = "auto";
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> detectorBasisChoiceString;
								if (detectorBasisChoiceString.size() != bitstring.size()){
									cout << "Mismatch in length of bitstring and basis choice bitstring" << endl;
									throw -1;
								}
								break;
							}
							default:{
								cout << "Invalid choice for basis choice bitstring" << endl;
								throw -1;
							}
						}

						cout << "Source Bitstring:" << endl;
						cout << bitstring << endl;	
						string transmittedString = "";
						for (int i = 0; i < bitstring.size(); ++i) {
							bool bit = (bitstring[i] == '1');
							Pulse pulse = generator->createPulse(bit);
							pulse = (sourceBasisChoiceString == "auto") ? 
									 generator->createPulse(bit) :
									 generator->createPulse(bit, sourceBasisChoiceString[i]=='1');
							pulse = channel->propagate(pulse);
							if (DEBUGPRINT) {
								cout << "Pulse #" << i << ": ";
								for (int i = 0; i < pulse.size(); ++i) {
									cout << pulse[i]->alpha << ',' << pulse[i]->beta << "|";
								}
								cout << endl;
							}
							if (pulse.size() > 0) {
								bool observation = (((detectorBasisChoiceString == "auto")?
									 detector->detectPulse(pulse)
									:detector->detectPulse(pulse, detectorBasisChoiceString[i])=='1') == 1);
								if (DEBUGPRINT) {
									cout << "Algo observation: " << observation << endl;
									cout << "Transmitted so far: " << transmittedString << endl;
								}
								transmittedString += (observation) ? "1":"0";
							}
						}

						cout << "Transmitted String:" << endl;
						cout << transmittedString << endl;

						int matching = 0;
						for (int i = 0; i < transmittedString.size(); ++i) {
							if (transmittedString[i] == bitstring[i])
								matching++;
						}
						cout << "Accuracy of transmission: " << (matching*100.0/bitstring.size()) << "%" << endl;
						break;
					}
					case 2: {					
						index = 1;
						for (auto info: Generators) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose Alice's generator: ";
						cin >> choice;
						if (choice <= 0 || choice > Generators.size()){
							cout << "Out of Index generator choice" << endl;
							throw -1;
						}
						auto generator = Generators[choice-1].generator;

						index = 1;
						for (auto info: Detectors) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose Bob's detector to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Detectors.size()){
							cout << "Out of Index detector choice" << endl;
							throw -1;
						}
						auto detector = Detectors[choice-1].detector;

						index = 1;
						for (auto info: Detectors) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose Eve's detector to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Detectors.size()){
							cout << "Out of Index Edetector choice" << endl;
							throw -1;
						}
						auto Edetector = Detectors[choice-1].detector;


						index = 1;
						for (auto info: Generators) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose Eve's generator: ";
						cin >> choice;
						if (choice <= 0 || choice > Generators.size()){
							cout << "Out of Index Egenerator choice" << endl;
							throw -1;
						}
						auto Egenerator = Generators[choice-1].generator;

						index = 1;
						for (auto info: Channels) {
							cout << index << ")" << info.name << endl;
							index++;
						}
						cout << "Choose which channel to use: ";
						cin >> choice;
						if (choice <= 0 || choice > Channels.size()){
							cout << "Out of Index channel choice" << endl;
							throw -1;
						}
						auto channel = Channels[choice-1].channel;

						cout << "(1)Generate random bitstring to transmit" << endl;
						cout << "(2)Manually input bitstring to transmit" << endl;
						cout << "Choose:";
						cin >> choice;

						string bitstring;
						switch (choice) {
							case 1: {
								cout << "Enter bitstring length: ";
								int len;
								cin >> len;
								for (int i = 0; i < len; ++i) {
									bitstring += (rand()%2 == 0) ? "1":"0";
								}
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> bitstring;
								break;
							}
							default:{
								cout << "Invalid choice for bitstring" << endl;
								throw -1;
							}
						}

						cout << "(1)Generate random basis chocies to transmit" << endl;
						cout << "(2)Manually input basis choices as bitstring" << endl;
						cout << "Choose:";
						cin >> choice;

						string sourceBasisChoiceString;
						switch (choice) {
							case 1: {
								sourceBasisChoiceString = "auto";
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> sourceBasisChoiceString;
								if (sourceBasisChoiceString.size() != bitstring.size()){
									cout << "Mismatch in length of bitstring and basis choice bitstring" << endl;
									throw -1;
								}
								break;
							}
							default:{
								cout << "Invalid choice for basis choice bitstring" << endl;
								throw -1;
							}
						}

						cout << "(1)Generate random basis chocies for detector" << endl;
						cout << "(2)Manually input basis choices for detector" << endl;
						cout << "Choose:";
						cin >> choice;

						string detectorBasisChoiceString;
						switch (choice) {
							case 1: {
								detectorBasisChoiceString = "auto";
								break;
							}
							case 2: {
								// TODO: add check if string entered is bitstring
								cin >> detectorBasisChoiceString;
								if (detectorBasisChoiceString.size() != bitstring.size()){
									cout << "Mismatch in length of bitstring and basis choice bitstring" << endl;
									throw -1;
								}
								break;
							}
							default:{
								cout << "Invalid choice for basis choice bitstring" << endl;
								throw -1;
							}
						}

						cout << "Source Bitstring:" << endl;
						cout << bitstring << endl;	
						string transmittedString = "";
						string interceptedString = "";
						for (int i = 0; i < bitstring.size(); ++i) {
							bool bit = (bitstring[i] == '1');
							Pulse pulse = generator->createPulse(bit);
							pulse = (sourceBasisChoiceString == "auto") ? 
									 generator->createPulse(bit) :
									 generator->createPulse(bit, sourceBasisChoiceString[i]=='1');
							pulse = channel->propagate(pulse);
							auto splitPhoton = pulse.extract();
							bool observation = Edetector->detectPulse(Pulse(splitPhoton));
							interceptedString +=  (observation) ? "1":"0";
							if (pulse.size() == 0) {
								if (DEBUGPRINT) {
									cout << "Intercepted Single qubit pulse, Eve constructing new pulse" << endl;
								}
								pulse = Egenerator->createPulse(observation);
							}
							if (DEBUGPRINT) {
								cout << "Pulse #" << i << ": ";
								for (int i = 0; i < pulse.size(); ++i) {
									cout << pulse[i]->alpha << ',' << pulse[i]->beta << "|";
								}
								cout << endl;
							}
							if (pulse.size() > 0) {
								if (detectorBasisChoiceString == "auto") {
									observation = (detector->detectPulse(pulse) == 1);
								} else {
									bool basisChoice = (detectorBasisChoiceString[i]=='1');
									observation = (detector->detectPulse(pulse, basisChoice) == 1);
								}
								if (DEBUGPRINT) {
									cout << "Algo observation: " << observation << endl;
									cout << "Transmitted so far: " << transmittedString << endl;
								}
								transmittedString += (observation) ? "1":"0";
							}
						}

						cout << "Transmitted String:" << endl;
						cout << transmittedString << endl;
						cout << "interceptedString" << endl;
						cout << interceptedString << endl;

						int matching = 0;
						for (int i = 0; i < transmittedString.size(); ++i) {
							if (transmittedString[i] == bitstring[i])
								matching++;
						}
						cout << "Accuracy of Bob's bitstring: " << (matching*100.0/bitstring.size()) << "%" << endl;
						matching = 0;
						for (int i = 0; i < interceptedString.size(); ++i) {
							if (interceptedString[i] == bitstring[i])
								matching++;
						}
						cout << "Accuracy of Eve's bitstring: " << (matching*100.0/bitstring.size()) << "%" << endl;
						matching = 0;
						for (int i = 0; i < transmittedString.size(); ++i) {
							if (transmittedString[i] == interceptedString[i])
								matching++;
						}
						cout << "Correlation between Eve's and Bob's bitstring: " << (matching*100.0/bitstring.size()) << "%" << endl;
						break;
					}
					default:{
						cout << "Not implemented yet!" << endl;
						break;
					}
				}

				break;
			}
			case 8:{
				DEBUGPRINT = !DEBUGPRINT;
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