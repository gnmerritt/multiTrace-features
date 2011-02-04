/*
 * Assembly.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Assembly.h"

const float INITIAL_TOTAL_LTCS = 0.5f;

const float STCS_GAIN = 0.0;

Assembly::Assembly(UpdateModel *_model, LearningRule *_learningRule) :
	state(new AssemblyState()), updateModel(_model), learningRule(_learningRule) {

	// TODO: figure out connections
	// until then, set these points so we don't blow up
	input = new ConnectionVector();
	output = new ConnectionVector();
}

Assembly::~Assembly() {
	// TODO Auto-generated destructor stub
}

/*
 * Passes our state & connections to the UpdateModel
 *
 */
void Assembly::tick(float regional_activation) {
	state->regional_activation = regional_activation;

	updateModel->tick(state, input);

	// TODO: update outgoing connections
}

/*
 * Sets up a pre-configured array of input Connections, and assigns them
 * an initial LTCS based on in.size()
 */
void Assembly::setIncomingConnections(ConnectionVector *in) {
	input = in;

	initializeConnectionStrengths();
}

/*
 * Processing of weights is handled post-synaptically, so we don't need
 * to do anything to these connections except update their activity
 * level when we tick().
 */
void Assembly::setOutgoingConnections(ConnectionVector *out) {
	output = out;
}

/*
 * Intra-unit LTCS = 0.5, and doesn't change.
 * So we have 0.5 LTCS to spread among each of our n incoming connections
 */
void Assembly::initializeConnectionStrengths() {
	int numConnections = input->size();

	if (numConnections == 0) {
		numConnections = 1;
	}

	ConnectionVector::iterator in;
	float perConnection = INITIAL_TOTAL_LTCS / numConnections;

	for (in = input->begin(); in != input->end(); ++in) {
		(*in)->setLTCS(perConnection);
	}
}

