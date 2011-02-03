/*
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
	Assembly(AssemblyState *_state, UpdateModel *_model, LearningRule *_learningRule);
	virtual ~Assembly();

	void setIncomingConnections(ConnectionVector in);
	void setOutgoingConnections(ConnectionVector out);

	void tick(float regional_activation);

private:
	void initializeConnectionStrengths();

private:
	AssemblyState *state;
	UpdateModel *updateModel;
	LearningRule *learningRule;
	ConnectionVector *input, *output;
};

#endif /* ASSEMBLY_H_ */
