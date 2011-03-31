/**
 *	Connection.hpp
 *
 *  @date Feb 2, 2011
 *      @author Nathan Merritt
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include <vector>
#include <pthread.h>

#include <boost/shared_ptr.hpp>

const float INITIAL_LTCS = 0;
const float INITIAL_STCS = 0;
const float INITIAL_ACTIVITY = 0;

/**
 * 	@brief Stores a sending Assembly's recent output, and both Assembly pointers
 *
 *	Model of an inter-Assembly connection. Has long-term and short-term
 *	weights, and the activity of the pre-synaptic Assembly. Changes to the
 *	connection weights are managed by a LearningRule, and handled by the
 *	pre-synaptic Assembly.
 *
 *	Two assemblies have a pointer to each Connection, so all methods must be
 *	thread-safe.
 */
class Connection {
public:
	typedef boost::shared_ptr<Connection> ptr;
	typedef std::vector<Connection::ptr> vector;

public:
	Connection();
	virtual ~Connection();

	float getOutput();

	float getLTCS();
	void setLTCS(float _ltcs);
	void setSTCS(float _stcs);
	void setActivity(float _activity);

private:
	float ltcs, stcs; /** connection strengths, see Sonntag (1991, pg94) */

	float activity; /** activity of the presynaptic Assembly */
	float last_activity; /** activity(t-1), for derivatives */

	std::vector<float> changes;

	pthread_mutex_t lock;
};

#endif /* CONNECTION_H_ */
