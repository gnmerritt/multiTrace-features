/**
 * LearningRule.h
 *
 *	Abstract class for updating connection strength of a given Assembly.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef LEARNINGRULE_H_
#define LEARNINGRULE_H_

#include "AssemblyState.h"
#include "Connection.h"

class LearningRule {
public:
	LearningRule() {}
	virtual ~LearningRule() {}

	virtual void tick(AssemblyState *state, ConnectionVector *input) = 0;

protected:
	AssemblyState *postSynapticState;
	ConnectionVector *incomingConnections;
};

#endif /* LEARNINGRULE_H_ */
