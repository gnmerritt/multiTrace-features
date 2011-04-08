/**
 * LearningRules.hpp
 *
 *	@brief Static factory for LearningRule derived classes
 *
 *	Static factory for different types of LearningRules. Slightly different from the
 *	other two because it returns a pointer instead of an object.
 *
 *  @date Mar 24, 2011
 *      @author Nathan Merritt
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

	/**
	 * @brief
	 * Builds and returns an object of an unknown type that inherits
	 * from LearningRule. Used for dynamic type control.
	 *
	 * This pointer is wrapped in a boost::shared_ptr once it's returned
	 *
	 * @param id ID of LearningRule derived class
	 * @param state AssemblyState pointer to be passed to returned object
	 * @param input Connection::vector pointer to be passed to returned object
	 * @see LearningRules::classes
	 * @return LearningRule derived object of the requested tyoe
	 */
	static LearningRule* instanceOf(int id, AssemblyState::ptr state, Connection::vector *input) {
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
