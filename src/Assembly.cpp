/**
 * Assembly.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Assembly.h"

const float INITIAL_TOTAL_LTCS = 0.4f;

template <class LearningTemplate>
Assembly<LearningTemplate>::Assembly(UpdateModel *_model) :
	state(new AssemblyState()), updateModel(_model) {

	// set these as empty by default
	input = new ConnectionVector();
	output = new ConnectionVector();

	initializeLearningRule();
}

template <class LearningTemplate>
Assembly<LearningTemplate>::~Assembly() {
	// TODO Auto-generated destructor stub
}

template <class LearningTemplate>
void Assembly<LearningTemplate>::initializeLearningRule() {
	delete(learningRule);

	learningRule = new LearningTemplate(state, input);
}

/**
 * Passes our state & connections to the UpdateModel, where they are updated.
 * Next, updates incoming connections by our LearningRule
 * Finally, updates outgoing connections to reflect our current activity
 *
 * @param regional_activation sum of A(t) for all Assemblies in this layer
 * @see UpdateModel.h
 * @see LearningRule.h
 */
template <class LearningTemplate>
void Assembly<LearningTemplate>::tick(float regional_activation) {
	state->regional_activation = regional_activation;

	updateModel->tick(state, input);

	learningRule->tick();

	updateOutgoingConnections();
}

/**
 * Sets up a pre-configured array of input Connections, and assigns them
 * an initial LTCS based on in.size()
 *
 * @param in a pointer to a ConnectionVector which we will use as input
 */
template <class LearningTemplate>
void Assembly<LearningTemplate>::setIncomingConnections(ConnectionVector *in) {
	delete(input);

	input = in;

	initializeLearningRule();
	initializeConnectionStrengths();
}

/**
 * Processing of weights is handled post-synaptically, so we don't need
 * to do anything to these connections except update their activity
 * level when we tick().
 *
 * @param out a pointer to a ConnectionVector which we will keep updated with our activity
 */
template <class LearningTemplate>
void Assembly<LearningTemplate>::setOutgoingConnections(ConnectionVector *out) {
	delete(output);
	output = out;
}

/**
 * Sets the activation level of each outgoing connection to our output,
 * so that we communicate with connected assemblies.
 */
template <class LearningTemplate>
void Assembly<LearningTemplate>::updateOutgoingConnections() {
	ConnectionVector::iterator out;

	for (out = output->begin(); out != output->end(); ++out) {
		(*out)->setActivity(state->output);
	}
}

/**
 * Intra-unit LTCS doesn't change.
 * So we have (1-LTCS) to spread among each of our n incoming connections
 *
 * @see updateOutgoingConnections()
 */
template <class LearningTemplate>
void Assembly<LearningTemplate>::initializeConnectionStrengths() {
	int numConnections = input->size();

	if (numConnections == 0) {
		numConnections = 1;
	}

	ConnectionVector::iterator in;
	float perConnection = (1-INITIAL_TOTAL_LTCS) / numConnections;

	for (in = input->begin(); in != input->end(); ++in) {
		(*in)->setLTCS(perConnection);
	}
}

