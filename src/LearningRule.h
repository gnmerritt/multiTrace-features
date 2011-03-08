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

#include <boost/shared_ptr.hpp>

#include "AssemblyState.h"
#include "Connection.h"

class LearningRule {
public:
	typedef boost::shared_ptr<LearningRule> ptr;

public:
	LearningRule(AssemblyState *state, Connection::vector *input) :
		postSynapticState(state), incomingConnections(input) {
	}
	virtual ~LearningRule() {
	}

	virtual void tick() = 0;

protected:
	AssemblyState *postSynapticState;
	Connection::vector *incomingConnections;
};

#endif /* LEARNINGRULE_H_ */
