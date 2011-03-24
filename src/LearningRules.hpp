/*
 * LearningRules.hpp
 *
 *	Static factory for different types of LearningRules. Slightly different from the
 *	other two because it returns a pointer instead of an object.
 *
 *  Created on: Mar 24, 2011
 *      Author: Nathan Merritt
 */

#ifndef LEARNINGRULES_HPP_
#define LEARNINGRULES_HPP_

#include "LearningRule.hpp"
#include "NoLearning.hpp"
#include "HebbianLearning.hpp"

class LearningRules {
public:
	virtual ~LearningRules() {
	}

	enum classes {
		NO_LEARNING, HEBBIAN_LEARNING,
	};

	static LearningRule* instanceOf(int id, AssemblyState *state, Connection::vector *input) {
		switch (id) {
		case (NO_LEARNING):
			return new NoLearning(state, input);
		case (HEBBIAN_LEARNING):
			return new HebbianLearning (state, input);
		default:
			break;
		}
	}

private:
	LearningRules() {
	}

};

#endif /* LEARNINGRULES_HPP_ */
