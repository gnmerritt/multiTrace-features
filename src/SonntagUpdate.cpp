/*
 * SonntagUpdate.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "SonntagUpdate.h"

#import <cmath>

const float v = 0; // normalization of sensitivity;
const float thetaC = 0;
const float thetaL = 0;
const float phi = 0.5; // input resistance

const float deltaLTCS = 0;

SonntagUpdate::SonntagUpdate() {
	// TODO Auto-generated constructor stub

}

SonntagUpdate::~SonntagUpdate() {
	// TODO Auto-generated destructor stub
}

/*
 *
 */
void SonntagUpdate::tick(AssemblyState *state, ConnectionVector *input) {
	// store parameters locally for helper methods
	currentState = state;
	currentInput = input;

	float inputNet = calculateInput();
}

/*
 * Calculates raw input by finding the linear sum of all inputs (Eq 4.12, pg87)
 * then "squashes" the input to range [0,1] (eq 4.13, pg87)
 */
float SonntagUpdate::calculateInput() {
	ConnectionVector::iterator input;
	float netInput = 0;

	for (input = currentInput->begin(); input != currentInput->end(); ++input) {
		netInput += *input->getOutput();
	}

	float squashedInput = 1 / (1 + (1 / pow(netInput, phi)));

	return squashedInput;
}

/*
 * See the derivations of ^A in SonntagUpdate.h
 */
float SonntagUpdate::calculateDeltaActivity() {
	return 0;
}

/*
 * Trivial, since ^LTCS = 0 at all time points.
 * We aren't modeling internal learning, so this doesn't change
 */
float SonntagUpdate::calculateDeltaLTCS() {
	return deltaLTCS;
}

/*
 *
 */
float SonntagUpdate::calculateDeltaSTCS() {
	return 0;
}

/*
 *
 */
float SonntagUpdate::calculateDeltaFatigue() {
	return 0;
}
