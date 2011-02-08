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

const float INITIAL_LTCS = 0;
const float INITIAL_STCS = 0;
const float INITIAL_ACTIVITY = 0;

class Connection {
public:
	Connection() : ltcs(INITIAL_LTCS), stcs(INITIAL_STCS), activity(INITIAL_ACTIVITY) {
		pthread_mutex_init(&lock, NULL);
	}
	virtual ~Connection() {
		pthread_mutex_destroy(&lock);
	}

	float getOutput() {
		pthread_mutex_lock(&lock);
		float output = (ltcs + stcs)*activity;
		pthread_mutex_unlock(&lock);

		return output;
	}

	void setLTCS(float _ltcs) {
		pthread_mutex_lock(&lock);
		ltcs = _ltcs;
		pthread_mutex_unlock(&lock);
	}

	void setSTCS(float _stcs) {
		pthread_mutex_lock(&lock);
		stcs = _stcs;
		pthread_mutex_unlock(&lock);
	}

	void setActivity(float _activity) {
		pthread_mutex_lock(&lock);
		activity = _activity;
		pthread_mutex_unlock(&lock);
	}

private:
	/** connection strengths, see Sonntag (1991, pg94) */
	float ltcs, stcs;

	/** activity of the presynaptic Assembly */
	float activity;

	pthread_mutex_t lock;
};

typedef std::vector<Connection*> ConnectionVector;

#endif /* CONNECTION_H_ */
