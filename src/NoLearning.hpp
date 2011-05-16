/**
 * NoLearning.h
 *
 *  @date Feb 8, 2011
 *      @author Nathan Merritt
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
	NoLearning(AssemblyState::ptr state, Connection::vector *input) :
		LearningRule(state, input) {
	}
	virtual ~NoLearning() {
	}

	void tick(Connection::vector* newInput) {
	}

	void dropLearning() {
	}
};

#endif /* NOLEARNING_H_ */
