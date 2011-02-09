/*
 * Layer.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Layer.h"

/**
 * Builds a layer that has size rows*cols. This entails the following steps:
 *
 *	Initializing an UpdateModel (1 for now), per-column eventually
 *	Constructing the Assemblies
 *	For each Assembly, use the ConnectionPattern to build input/output ConnectionVectors
 *
 * @param rows number of rows in the Layer
 * @param cols number of columns in the layer
 */
Layer::Layer(int rows, int cols) {
	// initialize the update model
	SonntagUpdate *updateModel = new SonntagUpdate();

	// build a row by col sized AssemblyLayer (2d vector)
	for (int curRow = 0; curRow < rows; ++curRow) {
		AssemblyVector toInsert;
		toInsert.reserve(cols);

		for (int curCol = 0; curCol < cols; ++curCol) {
			Assembly_t *a = new Assembly_t(updateModel);
			toInsert.push_back(*a);
		}

		assemblies.push_back(toInsert);
	}

	// TODO: connection pattern
}

Layer::~Layer() {
	delete &assemblies;
}

/**
 * The main update method! Calculates regionional inhibition, then ticks
 * all the Assemblies within this Layer.
 */
void Layer::tick() {
	AssemblyLayer::iterator row;
	AssemblyVector::iterator col;

	float averageActivation = calculateRegionalInhibition();

	for (row = assemblies.begin(); row != assemblies.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			col->tick(averageActivation);
		}
	}
}

/**
 * Iterators over all the Assemblies in our Layer, and averages their
 * output. This serves as a measure of overall activity in the Layer.
 *
 * @returns average of Assembly->getOutput() over the entire Layer
 */
float Layer::calculateRegionalInhibition() {
	float sum = 0;
	int numAssemblies = 0;

	AssemblyLayer::iterator row;
	AssemblyVector::iterator col;

	for (row = assemblies.begin(); row != assemblies.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			numAssemblies++;
			sum += col->getOutput();
		}
	}

	if (numAssemblies == 0) {
		numAssemblies = 1;
	}

	return sum / numAssemblies;
}
