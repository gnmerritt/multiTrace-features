/**
 * HebbianLearning.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "HebbianLearning.h"

HebbianLearning::HebbianLearning() {
	// TODO Auto-generated constructor stub

}

HebbianLearning::~HebbianLearning() {
	// TODO Auto-generated destructor stub
}

/**
 * Main method for handling Hebbian update of input connections to an Assembly
 *
 * @param state the state of the post-synaptic Assembly we're updating
 * @param input a pointer to the incoming connections onto this Assembly
 * @see LearningRule.h
 */
void HebbianLearning::tick(AssemblyState *state, ConnectionVector *input) {
	postSynapticState = state;
	incomingConnections = input;

	// TODO: think about this
}
