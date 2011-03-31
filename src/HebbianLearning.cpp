/**
 * HebbianLearning.cpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 *  @see HebbianLearning.hpp
 */

#include "HebbianLearning.hpp"

/** default parameterization, @see setParameters */
static const float DEFAULT_PARAMETERS[] = { 0.01f, // learning strength
		0.0f, // receiving learning (start)
		0.0f, // receiving learning (stop)
		0.0f, // sending contrib threshold
		};

/**
 * Calls super constructor, and sets default parameters.
 *
 * @param state Pointer to the AssemblyState we read from
 * @param input Pointer to the input Connections we use for calculations
 */
HebbianLearning::HebbianLearning(AssemblyState *state, Connection::vector *input) :
	LearningRule(state, input), storedLearning(false) {
	setParameters(DEFAULT_PARAMETERS);
	resetStoredLearning();
}

HebbianLearning::~HebbianLearning() {
	// @todo Auto-generated destructor stub
}

/**
 * Main method for handling Hebbian update of input connections to an Assembly
 *
 * @see LearningRule.hpp
 */
void HebbianLearning::tick() {
	const float last_activity = postSynapticState->output;
	const float activity = postSynapticState->activity;
	const float activity_derivative = activity - last_activity;

	/** Assembly is being activated, record input connection contributions */
	if (activity > parameters[REC_LEARNING_LOWER] && activity < parameters[REC_LEARNING_UPPER]
			&& activity_derivative >= 0) {
		tallyContributions();
	}

	/** crossed the LEARNING_STOP boundary for the first time, mark our learning as stored
		this should get run when the Assembly has fired fully */
	else if (activity > parameters[REC_LEARNING_UPPER] && last_activity
			<= parameters[REC_LEARNING_UPPER]) {
		storedLearning = true;
	}

	/** Assembly fired, and now activity has dropped off. Apply stored learning. */
	else if (activity < parameters[REC_LEARNING_LOWER] && storedLearning) {
		storedLearning = false;
		applyLearningToConnections();
		resetStoredLearning();
	}

	/** Assembly failed to fire, delete any learning we've stored so far */
	else if (activity < parameters[REC_LEARNING_UPPER] && activity_derivative < 0 && !storedLearning) {
		resetStoredLearning();
	}

	// no default case on purpose
}

/**
 *	Main method for determining changes in input connection strengths (ie, learning).
 *  First, calculates the %contribution of each connection, then weights it by
 *  receivingCurve() and sendingCurve(). These weighted values are then stored, and
 *  if this Assembly becomes fully active than the incoming connection strengths
 *  of any Assemblies that helped it fire are strengthened.
 *
 * @see receivingCurve()
 * @see sendingCurve()
 * @see applyLearningToConnections()
 */
void HebbianLearning::tallyContributions() {

}

/**
 * Applies changes in connection strengths that have been already calculated. This
 * method determines the changes in the strength of each connection based on that
 * connection's recent activity.
 *
 * @see tallyContributions()
 */
void HebbianLearning::applyLearningToConnections() {

}

/**
 * Resets any learning that has been stored.
 */
void HebbianLearning::resetStoredLearning() {

}

/**
 * Implements phase-dependent learning
 *
 * @return learning coefficient for this point in the firing cycle
 */
float HebbianLearning::receivingCurve() {
	return 1.0f;
}

/**
 * Implements contribution-dependent learning, probably much more important
 * than @see receivingCurve
 *
 * @return how important this sending connection is, a function of its contribution
 */
float HebbianLearning::sendingCurve(float sendingContribution) {
	return 1.0f;
}

/**
 * Sets the parameters to a new array of values
 *
 * @param newParams an array of length HebbianLearning::PARAMETER_COUNT
 */
void HebbianLearning::setParameters(const float newParams[]) {
	for (int i = 0; i < PARAMETER_COUNT; ++i) {
		parameters[i] = newParams[i];
	}
}

/**
 * Sets a specific parameter to the desired value. Use HebbianLearning::PARAMETER_COUNT
 * to access the parameter you want to change. Fails silently if you specify an
 * index that's out of range.
 *
 * @param index position in parameters to update
 * @param value new value to be set
 */
void HebbianLearning::setParameter(int index, float value) {
	if (index < 0 || index >= PARAMETER_COUNT)
		return;
	parameters[index] = value;
}
