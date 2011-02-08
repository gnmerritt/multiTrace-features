/**
 * Assembly.h
 *
 *	The unit of basic computation in a multiTrace network. Each assembly
 *	models a well-defined neurological Cell Assembly, and can sustain
 * 	its own activity.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef ASSEMBLY_H_
#define ASSEMBLY_H_

#include "AssemblyState.h"
#include "LearningRule.h"
#include "UpdateModel.h"
#include "Connection.h"

class Assembly {
public:
	Assembly(UpdateModel *_model, LearningRule *_learningRule);
	virtual ~Assembly();

	void setIncomingConnections(ConnectionVector *in);
	void setOutgoingConnections(ConnectionVector *out);

	float getActivation() const { // A(t)
		return state->activity;
	}
	float getOutput() const { // A(t-1)
		return state->output;
	}
	float getLTCS() const { // L(t) (constant)
		return state->ltcs;
	}
	float getSTCS() const { //S(t)
		return state->stcs;
	}
	float getFatigue() const { // F(t)
		return state->fatigue;
	}
	float getRegionalInhibition() const {
		return state->regional_activation; // set from our Layer by tick()
	}

	void tick(float regional_activation);

private:
	void initializeConnectionStrengths();
	void updateOutgoingConnections();

private:
	AssemblyState *state;
	UpdateModel *updateModel;
	LearningRule *learningRule;
	ConnectionVector *input, *output;
};

#endif /* ASSEMBLY_H_ */
