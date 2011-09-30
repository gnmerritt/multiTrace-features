/**
 * Connection.cpp
 *
 *  @date Feb 9, 2011
 *  @author Nathan Merritt
 */

#include <cstdio>
#include <cmath>

#include "Connection.hpp"

Connection::Connection() :
    ltcs(INITIAL_LTCS), stcs(INITIAL_STCS), distance(-1.0f), activity(INITIAL_ACTIVITY), last_activity(INITIAL_ACTIVITY) {
	pthread_mutex_init(&lock, NULL);
}

Connection::~Connection() {
	pthread_mutex_destroy(&lock);
}

float Connection::getOutput() {
	pthread_mutex_lock(&lock);
	const float output = (ltcs + stcs) * activity;
	pthread_mutex_unlock(&lock);

	return output;
}

float Connection::getLTCS() {
	pthread_mutex_lock(&lock);
	const float _ltcs = ltcs;
	pthread_mutex_unlock(&lock);
	return _ltcs;
}

void Connection::setLTCS(float _ltcs) {
	pthread_mutex_lock(&lock);
	const float delta = ltcs - _ltcs;
	changes.push_back(delta);
	ltcs = _ltcs;
	pthread_mutex_unlock(&lock);
}

/**
 * Initial connection strength is inversely related to the distance
 * the connection spans.
 *
 * @param _ltcs scaled initial strength
 */
void Connection::setInitialLTCS(float _ltcs) {
    if (distance > 0) {
	setLTCS(_ltcs/distance);
    }
    else {
	setLTCS(_ltcs);
    }
}

void Connection::setDistance(float _distanceSqd) {
    pthread_mutex_lock(&lock);
    distance = sqrt(_distanceSqd);
    pthread_mutex_unlock(&lock);
}

float Connection::getDistance() {
    pthread_mutex_lock(&lock);
    float _distance = distance;
    pthread_mutex_unlock(&lock);
    return _distance;
}

void Connection::setSTCS(float _stcs) {
	pthread_mutex_lock(&lock);
	stcs = _stcs;
	pthread_mutex_unlock(&lock);
}

void Connection::setActivity(float _activity) {
	pthread_mutex_lock(&lock);
	last_activity = activity;
	activity = _activity;
	pthread_mutex_unlock(&lock);
}
