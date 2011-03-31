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
	LearningRule(state, input), storedContributions(SynapseHistory()), hasStoredLearning(false) {
	setParameters(DEFAULT_PARAMETERS);
	resetStoredLearning();
}

HebbianLearning::~HebbianLearning() {
	resetStoredLearning();
}

/**
 * 	@brief
 * 	Main method for handling Hebbian update of input connections to an Assembly
 *
 *	There are four important states for learning:
 *		- Activity is above the lower learning threshold, but not yet full.
 *		   In this state we take note of what other Assemblies activating us
 *		- Activity becomes fully active (1 tick). Remember this, so that we
 *		   can learn (later) based on which inputs caused us to fire.
 *		- After being fully active, activity has calmed down. Apply learning.
 *		- Activity lower than in the last tick, and we haven't reached full
 *		  activation yet. The earlier inputs did not cause us to fire, so
 *		  forget about them.
 *
 *	Other states (fully active, weakly active...) aren't important to this model of
 *	learning, so they aren't treated here.
 *
 *	@see LearningRule.hpp
 */
void HebbianLearning::tick() {
	const float last_activity = postSynapticState->output;
	const float activity = postSynapticState->activity;
	const float activity_derivative = activity - last_activity;

	// Assembly is being activated, record input connection contributions
	if (activity > parameters[REC_LEARNING_LOWER] && activity < parameters[REC_LEARNING_UPPER]
			&& activity_derivative >= 0) {
		tallyContributions();
	}

	// crossed the LEARNING_STOP boundary for the first time, mark our learning as stored
	// this should get run when the Assembly has fired fully
	else if (activity > parameters[REC_LEARNING_UPPER] && last_activity
			<= parameters[REC_LEARNING_UPPER]) {
		hasStoredLearning = true;
	}

	// Assembly fired, and now activity has dropped off. Apply stored learning.
	else if (activity < parameters[REC_LEARNING_LOWER] && hasStoredLearning) {
		applyLearningToConnections();
		resetStoredLearning();
	}

	// Assembly failed to fire, delete any learning we've stored so far
	else if (activity < parameters[REC_LEARNING_UPPER] && activity_derivative < 0
			&& !hasStoredLearning) {
		resetStoredLearning();
	}

	// no default case on purpose
}

/**
 * @brief
 * Resets any stored learning, used only when the Assembly adds or deletes Connection s
 *
 */
void HebbianLearning::dropLearning() {
	resetStoredLearning(); // nothing else that needs to happen yet
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
	// pass 1: determine total activation among all inputs
	double totalInput = 0;
	Connection::vector::iterator c;
	for (c = incomingConnections->begin(); c != incomingConnections->end(); ++c) {
		totalInput += (*c)->activity; // double dereferenced b/c Connection::vector is of pointers
	}

	// pass 2: for each input, scale contribution by receivingCurve() and sendingCurve(),
	// and add it to storedContributions
	float receivingWeight = receivingCurve(postSynapticState->activity);
	for (unsigned int i = 0; i < incomingConnections->size(); ++i) {
		Connection::ptr input = incomingConnections->at(i);
		const float percentContribution = input->activity / totalInput;
		const float weightedContribution = percentContribution * sendingCurve(percentContribution)
				* receivingWeight;
		ConnectionContribution thisInput(i, weightedContribution);
		storedContributions.push_front(thisInput);
	}
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
	hasStoredLearning = false;
	storedContributions.clear();
}

/**
 * Implements phase-dependent learning
 *
 * @param receivingActivity Current activity of the Assembly receiving input
 * @return learning coefficient for this point in the firing cycle
 */
float HebbianLearning::receivingCurve(float receivingActivity) {
	return 1.0f;
}

/**
 * Implements contribution-dependent learning, probably much more important
 * than @see receivingCurve
 *
 * @param sendingContribution activity(thisConnection) / SUM (All connections activity)
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
