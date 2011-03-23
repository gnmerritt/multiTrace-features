/**
 * Connection.h
 *
 *	Model of an inter-Assembly connection. Has long-term and short-term
 *	weights, and the activity of the pre-synaptic Assembly. Changes to the
 *	connection weights are managed by a LearningRule, and handled by the
 *	pre-synaptic Assembly.
 *
 *	Two assemblies have a pointer to each Connection, so all methods must be
 *	thread-safe.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <vector>
#include <pthread.h>

#include <boost/shared_ptr.hpp>

const float INITIAL_LTCS = 0;
const float INITIAL_STCS = 0;
const float INITIAL_ACTIVITY = 0;

class Connection {
public:
	typedef boost::shared_ptr<Connection> ptr;
	typedef std::vector<Connection::ptr> vector;

public:
	Connection();
	virtual ~Connection();

	float getOutput();

	void setLTCS(float _ltcs);
	void setSTCS(float _stcs);
	void setActivity(float _activity);

public:
	/** connection strengths, see Sonntag (1991, pg94) */
	float ltcs, stcs;

	/** activity of the presynaptic Assembly */
	float activity;

	pthread_mutex_t lock;
};

#endif /* CONNECTION_H_ */
