/*
 * SonntagUpdate.cpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 *
 *  Parameters modified as suggested in:
 *  Chown E (2002) Reminiscence and arousal: a connectionist model.
 */

#include "SonntagUpdate.hpp"

#include <cmath>

/** default parameterization, see @parameters */
static const float CHOWN_2000_PARAMETERS[] =
	{ 1.5f, // sensitivity normalization
			9.0f, // decay due to competition
			5.0f, // decay due to loss
			9.0f, // input resistance (pos stimuli) these modulate the sigmoid: (1 + (1 + 1/x^phi))
			3.0f, // input resistance (neg stimuli)
			0.75f, // input resistance (regional inhibition)
			0.75f, // input resistance (lateral inhibition)
			0.5f, // external dampening
			0.0f, // delta LTCS (LTCS is constant)
			0.1f, // STCS growth
			0.001f, // STCS decline
			0.3f, // STCS gain
			0.007f, // Fatigue growth
			0.0001f // Fatigue decline
		};

//#define DEBUG_UPDATES

/**
 * @brief sets up the current default parameterization
 * @see setParameters
 */
SonntagUpdate::SonntagUpdate() {
	setParameters(CHOWN_2000_PARAMETERS);
}

/**
 * Handles the updating of the Assembly state variables, as per the
 * Sonntag difference equations
 *
 * @param inState pointer to the Assembly's state we're updating
 * @param input pointer to the Assembly's vector of input connections
 */
void SonntagUpdate::tick(AssemblyState::ptr inState, Connection::vector *input) {
	pthread_mutex_lock(&lock);

	// store parameters locally for helper methods
	currentState = inState;
	currentInput = input;

	// now add the delta functions to the inState, updating the Assembly
	inState->output = currentState->activity;
	inState->activity += calculateDeltaActivity();
	inState->stcs += calculateDeltaSTCS();
	inState->fatigue += calculateDeltaFatigue();
	inState->manual_input = -1.0f;

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
	const float phi_pos = parameters[PHI_POS];
	const float phi_neg = parameters[PHI_NEG];
	const float regionalInhibitionGain = parameters[REGIONAL_INHIBITION];
	const float lateralInhibitionGain = parameters[LATERAL_INHIBITION];
	float netPosInput = 0;

	for (input = currentInput->begin(); input != currentInput->end(); ++input) {
		float signal = (*input)->getOutput();
		netPosInput += signal;
	}

	const float regionalInhibition = regionalInhibitionGain * currentState->regional_activation;
	const float lateralInhibition = lateralInhibitionGain * currentState->lateral_inhibition;
	const float netNegInput = regionalInhibition + lateralInhibition;

	const float posInput = 1 / (1 + (1 / pow(netPosInput, phi_pos)));
	const float negInput = 1 / (1 + (1 / pow(netNegInput, phi_neg)));

	const float totalNetInput = posInput * (1 - negInput) * (1 - parameters[EXTERNAL_DAMPENING]);

	//printf("regional: %f lateral: %f netNegInput: %f negInput: %f totalInput: %f \n",
	//	regionalInhibition, lateralInhibition, netNegInput, negInput, totalNetInput);

#ifdef DEBUG_UPDATES
	printf("netPosInput: %f ", netPosInput);
	printf("netNegInput: %f ", netNegInput);
	printf("pos in: %f neg in: %f ", posInput, negInput);
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
	const float A = currentState->activity;
	const float V = calculateV();
	const float thetaC = parameters[DECAY_COMPETITION];
	const float thetaL = parameters[DECAY_LOSS];

	// allow for manual input (CSV, GUI, etc)
	float I;
	if (currentState->manual_input > 0) {
		I = currentState->manual_input;
	} else {
		I = calculateInput();
	}

	// equation 4.6, pg79
	const float deltaA = (A + I * (1 - A)) * (1 - A) * V - (pow(A, thetaL) + A * pow((1 - A),
			thetaC)) * (1 - V);

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
 *
 * @return V term for this tick
 */
float SonntagUpdate::calculateV() {
	const float L = currentState->ltcs;
	const float S = currentState->stcs;
	const float F = currentState->fatigue;
	const float v = parameters[NORM_SENSITIVITY];

	return ((L + S) * (1 - F)) / v;
}

/**
 * @see calculateDeltaFatigue()
 * @return Change to STCS for this tick
 */
float SonntagUpdate::calculateDeltaSTCS() {
	const float A = currentState->activity;
	const float S = currentState->stcs;
	const float sigmaG = parameters[SIGMA_GROWTH];
	const float sigmaD = parameters[SIGMA_DECLINE];

	const float deltaSTCS = ((sigmaG * A) * (1 - S)) - (sigmaD * S) * parameters[STCS_GAIN];

#ifdef DEBUG_UPDATES
	printf("deltaSTCS: %f\n", deltaSTCS);
#endif

	return deltaSTCS;
}

/**
 * Similar equation to STCS, see calculateDeltaSTCS & derivations
 * in SonntagUpdate.h
 *
 * Squared terms removed, @see SonntagUpdate.h
 *
 * @see calculateDeltaSTCS()
 * @return change to Fatigue term this tick
 */
float SonntagUpdate::calculateDeltaFatigue() {
	const float A = currentState->activity;
	const float F = currentState->fatigue;
	const float thetaG = parameters[FATIGUE_GROWTH];
	const float thetaD = parameters[FATIGUE_DECLINE];

	const float deltaFatigue = ((thetaG * A) * (1 - F)) - (thetaD * F);

#ifdef DEBUG_UPDATES
	printf("deltaFatigue: %f\n", deltaFatigue);
#endif

	return deltaFatigue;
}

/**
 * @brief sets the parameters to a new array
 * @param newParams array of values to use
 */
void SonntagUpdate::setParameters(const float newParams[]) {
	for (int i = 0; i < PARAMETER_COUNT; ++i) {
		parameters[i] = newParams[i];
	}
}
