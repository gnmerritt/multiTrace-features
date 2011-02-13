/*
 * test_Layer.cpp
 *
 *  Created on: Feb 10, 2011
 *      Author: Nathan Merritt
 */

#include "test_Layer.h"

bool noInputLayer1_1() {
	Layer<UNR> *layer = new Layer<UNR> (1, 1, 0); // 1x1, layerID=0
	int i;

	for (i = 0; i < 500; ++i) {
		float avgOutput = layer->calculateRegionalInhibition();

		//printf("layer avg output at time %d: %f\n", i, avgOutput);

		if (avgOutput != (float) 0.0f) {
			return false;
		}

		layer->tick();
	}

	return true;
}

bool noInputLayer30_30() {
	Layer<UNR> *layer = new Layer<UNR> (30, 30, 0); // 1x1, layerID=0
	int i;

	for (i = 0; i < 500; ++i) {
		layer->tick();

		float avgOutput = layer->calculateRegionalInhibition();

		//printf("layer avg output at time %d: %f\n", i, avgOutput);

		if (avgOutput > (float) 0.1f) {
			return false;
		}
	}

	return true;
}
