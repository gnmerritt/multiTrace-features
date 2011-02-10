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
 *	  Initially these are only intra-Layer
 *
 * @param rows number of rows in the Layer
 * @param cols number of columns in the layer
 */
template<class ConnectionTemplate>
Layer<ConnectionTemplate>::Layer(int rows, int cols, int _layerID) :
	layerID(_layerID) {
	// initialize the update model
	// TODO: make this threaded
	SonntagUpdate *updateModel = new SonntagUpdate();

	assemblies.reserve(rows);

	// build a row by col sized AssemblyLayer (2d vector)
	for (int curRow = 0; curRow < rows; ++curRow) {
		assemblies.push_back(AssemblyVector());

		for (int curCol = 0; curCol < cols; ++curCol) {
			Assembly_t *a = new Assembly_t(updateModel);
			assemblies.back().push_back(*a);
		}
	}

	// and wire up our intra-Layer connections
	AssemblyLayer_ID thisLayer(&assemblies, layerID);
	connectLayerToLayer(thisLayer, thisLayer);
}

template<class ConnectionTemplate>
Layer<ConnectionTemplate>::~Layer() {
	delete &assemblies;
}

/**
 * The main update method! Calculates regional inhibition, then ticks
 * all the Assemblies within this Layer.
 */
template<class ConnectionTemplate>
void Layer<ConnectionTemplate>::tick() {
	AssemblyLayer::iterator row;
	AssemblyVector::iterator col;

	float averageActivation = calculateRegionalInhibition();

	for (row = assemblies.begin(); row != assemblies.end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			col->tick(averageActivation);
		}
	}
}

/** Iterate over the AssemblyLater and check connectivity between
 *  any two Assemblies. Runs n^2, but we have to do it... This method
 *  implements the first part of the iterator, which runs on every Assembly
 *  in the sending layer
 *
 *	@see connectAssemblyToLayer()
 *  @sideeffect populates output connections of Assemblies in sendingLayer
 *  @param sender Layer&ID we're projecting connections from (efferent)
 *  @param receiver Layer&ID we're connecting to (afferent)
 */
template<class ConnectionTemplate>
void Layer<ConnectionTemplate>::connectLayerToLayer(AssemblyLayer_ID sendingLayer,
		AssemblyLayer_ID receivingLayer) {
	AssemblyLayer *projecting = sendingLayer.first;
	int projectingID = sendingLayer.second;

	// do all the connecting
	for (unsigned int row = 0; row < projecting->size(); ++row) {
		for (unsigned int col = 0; col < projecting->at(row).size(); ++col) {
			AssemblyLocation location(row, col, projectingID);

			LocalizedAssembly sendingAssembly(&(projecting->at(row)[col]), &location);

			connectAssemblyToLayer(sendingAssembly, receivingLayer);
		}
	}

	// and now recalculate the incoming connection weights in the receivingLayer
	AssemblyLayer::iterator row;
	AssemblyVector::iterator col;
	AssemblyLayer *receiving = receivingLayer.first;

	for (row = receiving->begin(); row != receiving->end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			col->initializeIncConnectionStrengths();
		}
	}
}

/**
 * Cruise through the entire receiving layer, checking if sender is connected to
 * each assembly and running the last helper method if the ConnectionTemplate
 * says we should.
 *
 * @see connectAssemblyToAssembly()
 * @param sender The Assembly whose output we're building
 * @param receivingLayer the Layer we're connecting to
 */
template<class ConnectionTemplate>
void Layer<ConnectionTemplate>::connectAssemblyToLayer(LocalizedAssembly sender,
		AssemblyLayer_ID receivingLayer) {
	AssemblyLayer *receiving = receivingLayer.first;
	int receivingID = receivingLayer.second;

	AssemblyLocation sendingLoc = *sender.second;

	ConnectionTemplate connectionPattern;

	for (unsigned int row = 0; row < receiving->size(); ++row) {
		for (unsigned int col = 0; col < receiving->at(row).size(); ++col) {

			AssemblyLocation receivingLoc(row, col, receivingID);
			Assembly_t *receivingAssembly = &(receiving->at(row).at(col));

			if (connectionPattern.areConnected(sendingLoc, receivingLoc)) {
				connectAssemblyToAssembly(sender.first, receivingAssembly);
			}
		}
	}
}

/**
 * This method handles the wiring the two Assemblies together
 *
 * @param sending Add a Connection to this Assembly's output
 * @param receiving Add a Connection to this Assembly's input
 *
 */
template<class ConnectionTemplate>
void Layer<ConnectionTemplate>::connectAssemblyToAssembly(Assembly_t* sending,
		Assembly_t* receiving) {
	// Don't connect an Assembly to itself
	if (receiving == sending) {
		return;
	}

	Connection *c = new Connection();

	sending->addOutgoingConnection(c);
	receiving->addIncomingConnection(c);
}

/**
 * Iterators over all the Assemblies in our Layer, and averages their
 * output. This serves as a measure of overall activity in the Layer.
 *
 * @returns average of Assembly->getOutput() over the entire Layer
 */
template<class ConnectionTemplate>
float Layer<ConnectionTemplate>::calculateRegionalInhibition() {
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
