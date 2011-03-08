/*
 * SonntagUpdate.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 *
 *  Parameters modified as suggested in:
 *  Chown E (2002) Reminiscence and arousal: a connectionist model.
 */

#include "SonntagUpdate.h"

#include <cmath>

/** used in ΔActivity */
const float v = 1.5; /** normalization of sensitivity, calculateV() */
/** C=Competition, L=Loss (exponential decay terms) */
const float thetaC = 9;
const float thetaL = 5;

/** used in calculateInput */
const float phi_pos = 9.0; /** input resistance */
const float phi_neg = 5.0;
const float external_dampening = 0.5;

/** used in ΔLTCS */
const float deltaLTCS = 0;

/** used in ΔSTCS
 G=Growth, D=Decline */
const float sigmaG = 0.1;
const float sigmaD = 0.0001;
const float STCS_GAIN = 0.5;

/** used in ΔFatigue */
const float thetaG = 0.007;
const float thetaD = 0.0001;

//#define DEBUG_UPDATES

/**
 * Constructor. Nothing really happens here, but see implicitly called UpdateModel()
 */
SonntagUpdate::SonntagUpdate() {

}

/**
 * Handles the updating of the Assembly state variables, as per the
 * Sonntag difference equations
 *
 * @param inState pointer to the Assembly's state we're updating
 * @param input pointer to the Assembly's vector of input connections
 */
void SonntagUpdate::tick(AssemblyState *inState, Connection::vector *input) {
	pthread_mutex_lock(&lock);

	// store parameters locally for helper methods
	currentState = new AssemblyState(*inState);
	currentInput = input;

	// now add the delta functions to the inState, updating the Assembly
	inState->output = currentState->activity;
	inState->activity += calculateDeltaActivity();
	inState->stcs += (calculateDeltaSTCS() * STCS_GAIN);
	inState->fatigue += calculateDeltaFatigue();

	pthread_mutex_unlock(&lock);
}

/**
 * Calculates raw input by finding the linear sum of all inputs (Eq 4.12, pg87)
 * then "squashes" the input to range [0,1] (eq 4.13, pg87)
 *
 * "Squashing" is done once for positive and once for negative inputs. Then,
 * excitatory input is multiplied by the inverse of the inhibitory input to produce
 * total net input. Inhibitory input is a measure of the total regional activation
 * See: Sonntag (eq5.6, pg104)
 */
float SonntagUpdate::calculateInput() {
	Connection::vector::iterator input;
	float netPosInput = 0;
	float netNegInput = currentState->regional_activation;

	for (input = currentInput->begin(); input != currentInput->end(); ++input) {
		float signal = (*input)->getOutput();
		netPosInput += signal;
	}

	float posInput = 1 / (1 + (1 / pow(netPosInput, phi_pos)));
	float negInput = 1 / (1 + (1 / pow(netNegInput, phi_neg)));

	float totalNetInput = posInput * (1 - negInput) * (1 - external_dampening);

#ifdef DEBUG_UPDATES
	printf("totalNetInput: %f\n", totalNetInput);
#endif

	return totalNetInput;
}

/**
 * See the derivations of ΔA in SonntagUpdate.h
 *
 * @see SonntagUpdate.h
 */
float SonntagUpdate::calculateDeltaActivity() {
	float A = currentState->activity;
	float I = calculateInput();
	float V = calculateV();

	// equation 4.6, pg79
	float deltaA = (A + I * (1 - A)) * (1 - A) * V - (pow(A, thetaL) + A * pow(
			(1 - A), thetaC)) * (1 - V);

#ifdef DEBUG_UPDATES
	printf("current A: %f\n", A);
	printf("current I: %f\n", I);
	printf("V term: %f\n", V);

	printf("deltaA: %f\n", deltaA);
#endif

	return deltaA;
}

/**
 * From equation 4.3, pg 78
 * See derivations in SonntagUpdate.h
 */
float SonntagUpdate::calculateV() {
	float L = currentState->ltcs;
	float S = currentState->stcs;
	float F = currentState->fatigue;

	return ((L + S) * (1 - F)) / v;
}

/**
 * @see calculateDeltaFatigue()
 */
float SonntagUpdate::calculateDeltaSTCS() {
	float A = currentState->activity;
	float S = currentState->stcs;

	float deltaSTCS = ((sigmaG * A) * (1 - S)) - (sigmaD * S);

#ifdef DEBUG_UPDATES
	printf("deltaSTCS: %f\n", deltaSTCS);
#endif

	return deltaSTCS;
}

/*
 * Similar equation to STCS, see calculateDeltaSTCS & derivations
 * in SonntagUpdate.h
 *
 * Squared terms removed, @see SonntagUpdate.h
 *
 * @see calculateDeltaSTCS()
 */
float SonntagUpdate::calculateDeltaFatigue() {
	float A = currentState->activity;
	float F = currentState->fatigue;

	float deltaFatigue = ((thetaG * A) * (1 - F)) - (thetaD * F);

#ifdef DEBUG_UPDATES
	printf("deltaFatigue: %f\n", deltaFatigue);
#endif

	return deltaFatigue;
}
