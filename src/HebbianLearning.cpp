/**
 * HebbianLearning.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "HebbianLearning.h"

HebbianLearning::HebbianLearning(AssemblyState *state, Connection::vector *input) :
	LearningRule(state, input) {
	// TODO Auto-generated constructor stub

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
