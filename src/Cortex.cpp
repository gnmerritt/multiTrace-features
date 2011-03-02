/*
 * Cortex.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#include "Cortex.h"

Cortex::Cortex() {
	// TODO Auto-generated constructor stub

}

Cortex::~Cortex() {
	// TODO Auto-generated destructor stub
}

/**
 * Runs tick() on each Layer in layers.
 *
 * TODO: some mechanism for caching average output, cortex-wide activation and
 * specific Assembly output (for the GUI)
 *
 */
void Cortex::tick() {
	LayerVector::iterator layer;

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
