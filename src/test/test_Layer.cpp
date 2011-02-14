/*
 * test_Layer.cpp
 *
 *  Created on: Feb 10, 2011
 *      Author: Nathan Merritt
 */

#include "test_Layer.h"

bool noInputLayer1_1() {
	Layer<UNR> *layer = new Layer<UNR> (1, 1, 1); // 1x1, layerID=1
	int i;

	for (i = 0; i < 500; ++i) {
		float avgOutput = layer->getLastRegionalActivation();

		if (avgOutput > (float) 0.1f) {
			return false;
		}

		layer->tick();
	}

	return true;
}

bool noInputLayer30_30() {
	Layer<UNR> *layer = new Layer<UNR> (30, 30, 1); // 30x30, layerID=1
	int i;

	for (i = 0; i < 500; ++i) {
		layer->tick();

		float avgOutput = layer->getLastRegionalActivation();

		if (avgOutput > (float) 0.1f) {
			return false;
		}
	}

	return true;
}

bool singleInputLayer10_10() {
	Layer<UNR> layer(10, 10, 1);

	AssemblyLayer *assemblies = layer.getAssemblyLayer().first;

	AssemblyLayer::iterator row;
	AssemblyVector::iterator col;

	Connection *c = new Connection();

	// input on all the assemblies in the first row
	for (row = assemblies->begin(); row != assemblies->end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			col->addIncomingConnection(c);
			col->initializeIncConnectionStrengths();
		}
	}

	c->setActivity(99);

	for (int i = 0; i < 500; ++i) {
		if (i == 10) {
			c->setActivity(0);
		}

		layer.tick();

		float avgOutput = layer.getLastRegionalActivation();

		//printf("%d\t%f\n", i, avgOutput);

		if (avgOutput > 1) {
			return false;
		}
	}

	return true;
}
