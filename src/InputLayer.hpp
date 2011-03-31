/**
 * InputLayer.h
 *
 *	@brief
 *	Abstract class defining the interface that external software must
 *  use to provide input to an Assembly network (usually a Layer).
 *
 *	@see Layer.hpp
 *  @date Feb 15, 2011
 *  @author Nathan Merritt
 */

#ifndef INPUTLAYER_H_
#define INPUTLAYER_H_

#include <queue>

#include "Layer.hpp"
#include "ConnectionPattern.hpp"

template<class Observation>
class InputLayer: public Layer {
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

	/** @brief how strongly this observation drives its target Assembly */
	virtual float observationMagnitude(Observation o) = 0;
	/** @brief returns an AssemblyLocation that this Obsevation stimulates */
	virtual AssemblyLocation mapObservationToTarget(Observation o) = 0;
};

#endif /* INPUTLAYER_H_ */
