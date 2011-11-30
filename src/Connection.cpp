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
    ltcs(INITIAL_LTCS), stcs(NULL), distance(-1.0f), activity(NULL), last_activity(NULL), isInitialized(false)
{
    pthread_mutex_init(&lock, NULL);
}

Connection::~Connection() {
    pthread_mutex_destroy(&lock);
}

float Connection::getOutput() {
    pthread_mutex_lock(&lock);

    float output;
    if (isInitialized) {
	output = (ltcs + *stcs) * *activity; // :-(
    }
    else {
	output = 0.0f;
    }

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

void Connection::setSTCS(float* _stcs) {
    pthread_mutex_lock(&lock);
    stcs = _stcs;
    checkState();
    pthread_mutex_unlock(&lock);
}

void Connection::setActivity(float* _activity) {
    pthread_mutex_lock(&lock);
    activity = _activity;
    checkState();
    pthread_mutex_unlock(&lock);
}

void Connection::setLastActivity(float* _lastActivity) {
    pthread_mutex_lock(&lock);
    last_activity = _lastActivity;
    checkState();
    pthread_mutex_unlock(&lock);
}

void Connection::checkState() {
    if (stcs == NULL || activity == NULL) {
        isInitialized = false;
    }
    else {
        isInitialized = true;
    }
}
