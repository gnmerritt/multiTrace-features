/*
 * Cortex.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#include "Cortex.h"

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::Cortex() {
	// TODO Auto-generated constructor stub

}

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::~Cortex() {
	// TODO Auto-generated destructor stub
}

/**
 * Runs tick() on each Layer in layers.
 *
 * TODO: some mechanism for caching average output, cortex-wide activation and
 * specific Assembly output (for the GUI)
 *
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::tick() {
	typename LayerVector::iterator layer; // c++ is stupid. this won't compile w/out the 'typename' preface

	float activation_sum = 0;

	for (layer = layers.begin(); layer != layers.end(); ++layer) {
		activation_sum += layer->tick();
	}

	if (layers.empty()) {
		averageLayerActivation = 0;
	} else {
		averageLayerActivation = activation_sum / layers.size();
	}
}

/**
 * Constucts layers as specified by various parameters, including the number of
 * Layers to make, how they're connected and their size.
 *
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::buildLayers() {

}
