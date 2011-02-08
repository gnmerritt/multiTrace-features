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

	virtual void tick() = 0;

private:
	// TODO: not sure if I want to store a reference, or only have one
	// instance of LearningRule active for all the Assemblies in a Layer
	AssemblyState *state;

};

#endif /* LEARNINGRULE_H_ */
