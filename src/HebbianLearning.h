/**
 * HebbianLearning.h
 *
 *	Changes long term connection strength for each connection according
 *	to compensatory Hebbian learning. The update model deals with modifying
 *	the incoming connections of a given Assembly, according to it's current
 *	levels of Fatigue, etc.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef HEBBIANLEARNING_H_
#define HEBBIANLEARNING_H_

#include "LearningRule.h"

class HebbianLearning : public LearningRule {
public:
	HebbianLearning();
	virtual ~HebbianLearning();

	void tick(AssemblyState *state, ConnectionVector *input);
};

#endif /* HEBBIANLEARNING_H_ */
