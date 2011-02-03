/*
 * Assembly.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Assembly.h"

const float INITIAL_TOTAL_LTCS = 0.5f;

const float STCS_GAIN = 0.0;

Assembly::Assembly(AssemblyState *_state, UpdateModel *_model, LearningRule *_learningRule) :
	state(_state), model(_model), learningRule(_learningRule) {


}

Assembly::~Assembly() {
	// TODO Auto-generated destructor stub
}

/*
 * Passes our state & connections to the UpdateModel
 */
void Assembly::tick(float regional_activation) {
	state->regional_activation = regional_activation;

	updateModel->tick(state, input);


}

/*
 * Intra-unit LTCS = 0.5, and doesn't change.
 * So we have 0.5 LTCS to spread among each of our n incoming connections
 */
void Assembly::initializeConnectionStrengths() {
	ConnectionVector::iterator in;
	float perConnection = INITIAL_TOTAL_LTCS / input.size();

	for (in = input->begin(); in != input->end(); ++in) {
		(*in)->setLTCS(perConnection);
	}
}

