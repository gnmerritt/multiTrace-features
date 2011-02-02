/*
 * SonntagUpdate.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "SonntagUpdate.h"

#import <cmath>

// used in ΔActivity
const float v = 0.01; // normalization of sensitivity, calculateV()
const float thetaC = 0;
const float thetaL = 0;

// used in calculateInput
const float phi = 0.5; // input resistance

// used in ΔLTCS
const float deltaLTCS = 0;

// used in ΔSTCS
const float sigmaG = 0;
const float sigmaD = 0;

// used in ΔFatigue
const float thetaG = 0;
const float thetaD = 0;

/*
 * Handles the updating of the Assembly state variables, as per the
 * Sonntag difference equations
 */
void SonntagUpdate::tick(AssemblyState *inState, ConnectionVector *input) {
	pthread_mutex_lock(&lock);

	// store parameters locally for helper methods
	currentState = new AssemblyState(*inState);
	currentInput = input;

	// now add the delta functions to the inState, updating the Assembly
	inState->output = currentState->activity;
	inState->activity += calculateDeltaActivity();
	inState->ltcs += calculateDeltaLTCS();
	inState->stcs += calculateDeltaSTCS();
	inState->fatigue += calculateDeltaFatigue();

	pthread_mutex_unlock(&lock);
}

/*
 * Calculates raw input by finding the linear sum of all inputs (Eq 4.12, pg87)
 * then "squashes" the input to range [0,1] (eq 4.13, pg87)
 */
float SonntagUpdate::calculateInput() {
	ConnectionVector::iterator input;
	float netInput = 0;

	for (input = currentInput->begin(); input != currentInput->end(); ++input) {
		netInput += (*input)->getOutput();
	}

	float squashedInput = 1 / (1 + (1 / pow(netInput, phi)));

	return squashedInput;
}

/*
 * See the derivations of ΔA in SonntagUpdate.h
 */
float SonntagUpdate::calculateDeltaActivity() {
	float A = currentState->activity;
	float I = calculateInput();
	float V = calculateV();

	// equation 4.6, pg79
	float deltaA = (A + I * (1 - A)) * (1 - A) * V - (pow(A, thetaL) + A * pow(
			(1 - A), thetaC)) * (1 - V);

	return deltaA;
}

/*
 * From equation 4.3, pg 78
 * See derivations in SonntagUpdate.h
 */
float SonntagUpdate::calculateV() {
	float L = currentState->ltcs;
	float S = currentState->stcs;
	float F = currentState->fatigue;

	return ((L + S)*(1 - F)) / v;
}

/*
 * Trivial, since ΔLTCS = 0 at all time points.
 * We aren't modeling internal learning, so this doesn't change
 */
float SonntagUpdate::calculateDeltaLTCS() {
	return deltaLTCS;
}

/*
 *
 */
float SonntagUpdate::calculateDeltaSTCS() {
	float A = currentState->activity;
	float S = currentState->stcs;

	return ((sigmaG * A) * pow((1 - S), 2)) - (sigmaD * S);
}

/*
 * Similar equation to Fatigue, see calculateDeltaSTCS & derivations
 * in Sonntag.h
 */
float SonntagUpdate::calculateDeltaFatigue() {
	float A = currentState->activity;
	float F = currentState->fatigue;

	return (thetaG * A) * pow((1 - F), 2) - (thetaD * F);
}
