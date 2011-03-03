/*
 * Cortex.h
 *
 * Manages a group of connection Layers, coordinating their construction,
 * wiring and interacting with a GUI to characterize their activity.
 *
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#ifndef CORTEX_H_
#define CORTEX_H_

#include <vector>

#include "Layer.h"

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
class Cortex {
	typedef std::vector<Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate> > LayerVector;
	// NOTE: you will need to declare iterators as 'typename LayerVector::iterator foo'

public:
	Cortex();
	virtual ~Cortex();

	void buildLayers(); // not run automatically by the constructor

	void tick(); // will tick all the layers

private:
	LayerVector layers;

	float averageLayerActivation;

	float arousal;
	float learningRate; // not sure this goes here?
};

#endif /* CORTEX_H_ */
