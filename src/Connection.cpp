/*
 * Connection.cpp
 *
 *  Created on: Feb 9, 2011
 *      Author: Nathan Merritt
 */

#include "Connection.hpp"

Connection::Connection() :
	ltcs(INITIAL_LTCS), stcs(INITIAL_STCS), activity(INITIAL_ACTIVITY) {
	pthread_mutex_init(&lock, NULL);
}

Connection::~Connection() {
	pthread_mutex_destroy(&lock);
}

float Connection::getOutput() {
	pthread_mutex_lock(&lock);
	float output = (ltcs + stcs) * activity;
	pthread_mutex_unlock(&lock);

	return output;
}

void Connection::setLTCS(float _ltcs) {
	pthread_mutex_lock(&lock);
	ltcs = _ltcs;
	pthread_mutex_unlock(&lock);
}

void Connection::setSTCS(float _stcs) {
	pthread_mutex_lock(&lock);
	stcs = _stcs;
	pthread_mutex_unlock(&lock);
}

void Connection::setActivity(float _activity) {
	pthread_mutex_lock(&lock);
	activity = _activity;
	pthread_mutex_unlock(&lock);
}
