/**
 * HebbianLearning.cpp
 *
 *  Created on: Feb 2, 2011
 *  @author Nathan Merritt
 */

#include "HebbianLearning.hpp"

/** default parameterization, @see setParameters*/
const float DEFAULT_PARAMETERS = { 0.01f, // learning strength
		0.0f, // receiving learning (start)
		0.0f, // receiving learning (stop)
		0.0f, // sending contrib threshold
		};

HebbianLearning::HebbianLearning(AssemblyState *state, Connection::vector *input) :
	LearningRule(state, input) {
	setParameters(DEFAULT_PARAMETERS);

}

HebbianLearning::~HebbianLearning() {
	// TODO Auto-generated destructor stub
}

/**
 * Main method for handling Hebbian update of input connections to an Assembly
 *
 * @see LearningRule.h
 */
void HebbianLearning::tick() {

	// TODO: think about this
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
 * to access the parameter you want to change.
 *
 * @param index position in parameters to update
 * @param value new value to be set
 */
void HebbianLearning::setParameter(int index, float value) {
	if (index < 0 || index >= PARAMETER_COUNT)
		return;
	parameters[index] = value;
}
