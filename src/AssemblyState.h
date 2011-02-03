/*
 * AssemblyState.h
 *
 *	Contains the state variables for an Assembly.
 *
 *	  activity -> current activation of the Assembly
 *	  output -> output of the Assembly (A(t-1))
 *	  fatige -> grows with increasing activation
 *
 *	  Measures of the Assembly's ability to self-activate:
 *	  ltcs -> long term connection strength (locked at 0)
 *	  stcs -> short term connection strength
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef ASSEMBLYSTATE_H_
#define ASSEMBLYSTATE_H_

class AssemblyState {
public:
	AssemblyState();
	virtual ~AssemblyState();

	//Copy constructor
	AssemblyState(const AssemblyState &a) :
		activity(a.activity), output(a.output), ltcs(a.ltcs), stcs(a.stcs),
				fatigue(a.fatigue), regional_activation(a.regional_activation) {
	}
	// multiTRACE variables
	float activity;
	float output;
	float ltcs;
	float stcs;
	float fatigue;

	// set by Layer when it calls tick() on each assembly
	float regional_activation;
};

#endif /* ASSEMBLYSTATE_H_ */
