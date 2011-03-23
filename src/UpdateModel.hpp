/**
 * UpdateModel.h
 *
 *	Abstract class responsible for updating the state variables of an
 *	assembly.
 *
 *	Since there will be only one instance of this class defined per-layer
 *	(for now) the tick() method has to be thread-safe.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef UPDATEMODEL_H_
#define UPDATEMODEL_H_

#include <pthread.h>

#include "AssemblyState.hpp"
#include "Connection.hpp"

#include <boost/shared_ptr.hpp>

class UpdateModel {
public:
	typedef boost::shared_ptr<UpdateModel> ptr;

public:
	UpdateModel() {
		pthread_mutex_init(&lock, NULL);
	}
	virtual ~UpdateModel() {
		pthread_mutex_destroy(&lock);
	}

	virtual void tick(AssemblyState *state, Connection::vector *input) = 0;

protected:
	AssemblyState *currentState;
	Connection::vector *currentInput;

	pthread_mutex_t lock;
};

#endif /* UPDATEMODEL_H_ */
