/**
 * HebbianLearning.h
 *
 *	Changes long term connection strength for each connection according
 *	to compensatory Hebbian learning.
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

	void tick();
};

#endif /* HEBBIANLEARNING_H_ */
