#include <complex>
#include <algorithm>

#include "constants.h"

bool DEBUGPRINT = false;

state PLUS  = make_pair(amplitude(1/root2), amplitude(1/root2));
state MINUS = make_pair(amplitude(1/root2), amplitude(-1/root2));
state ONE   = make_pair(amplitude(0), amplitude(1)); 
state ZERO  = make_pair(amplitude(1), amplitude(0));