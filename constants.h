#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <complex>

using namespace std;

#define eps (0.0001)
#define root2 (sqrt(2))

extern bool DEBUGPRINT;

typedef complex<double> amplitude;
typedef pair<amplitude, amplitude> state;
typedef pair<state, state> basis;

extern state PLUS;
extern state MINUS;
extern state ONE;
extern state ZERO;

#endif