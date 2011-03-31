/**
 * NoLearning.h
 *
 *  Created on: Feb 8, 2011
 *      Author: Nathan Merritt
 *
 *  @see LearningRule.hpp
 *  @see HebbianLearning.hpp
 */

#ifndef NOLEARNING_H_
#define NOLEARNING_H_

#include "LearningRule.hpp"

/**
 * 	@brief Placeholder class since LearningRule is abstract
 *
 * 	Placeholder class for when we don't want the network to automatically update
 *	its connection strenths (ie: learn)
 *
 *	Exists because LearningRule is abstract and cannot be instantiated.
 */
class NoLearning: public LearningRule {
public:
	NoLearning(AssemblyState *state, Connection::vector *input) :
		LearningRule(state, input) {
	}
	virtual ~NoLearning() {
	}

	void tick() {
	}
};

#endif /* NOLEARNING_H_ */
