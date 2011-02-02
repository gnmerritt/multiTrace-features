/*
 * Connection.h
 *
 *	Model of an inter-Assembly connection. Has long-term and short-term
 *	weights, and the activity of the pre-synaptic Assembly. Changes to the
 *	connection weights are managed by a LearningRule, and handled by the
 *	pre-synaptic Assembly.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <vector>

class Connection {
public:
	Connection();
	virtual ~Connection();

	// connection strengths, see Sonntag (1991, pg94)
	float ltcs, stcs;

	// activity of the Assembly on the presynaptic end
	float activity;
};

typedef std::vector<Connection*> ConnectionVector;

#endif /* CONNECTION_H_ */
