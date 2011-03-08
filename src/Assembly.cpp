/**
 * Assembly.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Assembly.h"

const float INITIAL_TOTAL_LTCS = 0.4f;

// used by debug printf's
#ifdef DEBUG_ASSEMBLY_OUTPUT
const std::string assembly_tick = "%d\t%f\t%f\t%f\t%f\t%f\n";
const std::string assembly_init =
		"Timestep\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n";
#endif

template<class LearningTemplate>
Assembly<LearningTemplate>::Assembly(int _id, UpdateModel::ptr _model) :
	id(_id), timestep(0), state(new AssemblyState()), updateModel(_model),
			input(Connection::vector()), output(Connection::vector()){
	initializeLearningRule();

#ifdef DEBUG_ASSEMBLY_OUTPUT
	std::stringstream out;
	out << "/tmp/assembly_" << getId() << ".xls";

	std::string filename = out.str();

	tick_f = fopen(filename.c_str(), "w");

	fprintf(tick_f, "%s", assembly_init.c_str());
#endif
}

template<class LearningTemplate>
Assembly<LearningTemplate>::~Assembly() {
	delete state;

#ifdef DEBUG_ASSEMBLY_OUTPUT
	fclose(tick_f);
#endif
}

/**
 * Used to wire up inter-Assembly connections
 *
 * @see connectAssemblyToAssembly()
 * @param newInput Connection added to our input vector
 * @sideeffect reinitializes the initial connection strengths (undoes learning)
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::addIncomingConnection(Connection::ptr newInput) {
	input.push_back(newInput);
}

/**
 * Used to wire up inter-Assembly connections
 *
 * @see connectAssemblyToAssembly()
 * @param newOutput Connection added to our output vector
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::addOutgoingConnection(
		Connection::ptr newOutput) {
	output.push_back(newOutput);
}

/**
 * Run the learning rule constructor
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::initializeLearningRule() {
	learningRule = LearningRule::ptr ( new LearningTemplate(state, &(input)) );
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
template<class LearningTemplate>
float Assembly<LearningTemplate>::tick(float regional_activation) {
	// update our internal state with layer data
	state->regional_activation = regional_activation;

	// tick the internal state
	updateModel->tick(state, &input);

	// adjust incoming connections
	learningRule->tick();

	// push our activity to our outgoing connections
	updateOutgoingConnections();

	// log timestep activity to our debug file in /tmp
#ifdef DEBUG_ASSEMBLY_OUTPUT
	fprintf(tick_f, assembly_tick.c_str(), ++timestep, getActivation(),
			getLTCS(), getSTCS(), getFatigue(), getRegionalInhibition());
#endif

	// return our activation to our Layer
	return state->output;
}

/**
 * Sets up a pre-configured array of input Connections, and assigns them
 * an initial LTCS based on in.size()
 *
 * @param in a pointer to a ConnectionVector which we will use as input
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::setIncomingConnections(Connection::vector in) {
	input = in;

	initializeLearningRule();
	initializeIncConnectionStrengths();
}

/**
 * Processing of weights is handled post-synaptically, so we don't need
 * to do anything to these connections except update their activity
 * level when we tick().
 *
 * @param out a pointer to a ConnectionVector which we will keep updated with our activity
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::setOutgoingConnections(Connection::vector out) {
	output = out;
}

/**
 * Sets the activation level of each outgoing connection to our output,
 * so that we communicate with connected assemblies.
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::updateOutgoingConnections() {
	Connection::vector::iterator out;

	for (out = output.begin(); out != output.end(); ++out) {
		(*out)->setActivity(state->output);
	}
}

/**
 * Intra-unit LTCS doesn't change.
 * So we have (1-LTCS) to spread among each of our n incoming connections
 *
 * @see updateOutgoingConnections()
 */
template<class LearningTemplate>
void Assembly<LearningTemplate>::initializeIncConnectionStrengths() {
	int numConnections = input.size();

	if (numConnections == 0) {
		numConnections = 1;
	}

	Connection::vector::iterator in;
	float perConnection = (1 - INITIAL_TOTAL_LTCS) / numConnections;

	for (in = input.begin(); in != input.end(); ++in) {
		(*in)->setLTCS(perConnection);
	}
}

