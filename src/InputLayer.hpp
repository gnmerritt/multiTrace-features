/*
 * InputLayer.h
 *
 *	Abstract class defining the interface that external software must
 *  use to provide input to an Assembly network.
 *
 *	@see Layer.h
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#ifndef INPUTLAYER_H_
#define INPUTLAYER_H_

#include <queue>

#include "Layer.h"

template<class ConnectionTemplate, class Observation>
class InputLayer: public Layer<ConnectionTemplate> {
public:
	InputLayer() {
	}
	virtual ~InputLayer() {
	}

	virtual void tick() = 0;
	virtual void provideData(Observation input) = 0;

private:
	std::queue<Observation> currentInputs;
	int inputsPerTick;

	// how strongly this observation drives its target Assembly
	virtual float observationMagnitude(Observation o) = 0;
	virtual Assembly_t mapObservationToTarget(Observation o) = 0;
};

#endif /* INPUTLAYER_H_ */
