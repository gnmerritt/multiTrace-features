/**
 * NoLearning.h
 *
 *	Placeholder class for when we don't want the network to automatically update
 *	its connection strenths (ie: learn)
 *
 *	Exists because LearningRule is abstract and cannot be instantiated.
 *
 *  Created on: Feb 8, 2011
 *      Author: Nathan Merritt
 *
 *  @see LearningRule.h
 *  @see HebbianLearning.h
 */

#ifndef NOLEARNING_H_
#define NOLEARNING_H_

#include "LearningRule.h"

class NoLearning: public LearningRule {
public:
	NoLearning() {}
	virtual ~NoLearning() {}

	void tick() {}
};

#endif /* NOLEARNING_H_ */
