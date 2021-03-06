/**
 * LearningRule.hpp
 *
 *      @brief Abstract class for updating input connection strength of a given Assembly
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 */

#ifndef LEARNINGRULE_H_
#define LEARNINGRULE_H_

#include <boost/shared_ptr.hpp>

#include "AssemblyState.hpp"
#include "Connection.hpp"

class LearningRule {
public:
     typedef boost::shared_ptr<LearningRule> ptr;

public:
     LearningRule(AssemblyState::ptr state, Connection::vector *input) :
          postSynapticState(state), incomingConnections(input) {
     }
     virtual ~LearningRule() {
     }

     virtual void tick(Connection::vector*) = 0;

     virtual void dropLearning() = 0; // called if the Assembly adds/deletes connections

protected:
     AssemblyState::ptr postSynapticState;
     Connection::vector *incomingConnections;
};

#endif /* LEARNINGRULE_H_ */
