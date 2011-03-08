/*
 * Cortex.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#include "Cortex.h"

/**
 * @brief Cortex constructor
 * Constructor for the Cortex. Builds and wires layers up according to parameters, after
 * a Cortex object has been constructed it is ready to run, via tick()
 *
 * @param _numStdLayers how many Layers will be created (more can be added later)
 * @param _rowsPerLayer number of rows in each Layer (homogenous for now)
 * @param _colsPerLayer number of columns in each Layer (homogenous for now)
 * @param connectTo how far "below" each layer sends connections, e.g. -1 indicates a connection to the layer immediately below
 * @param connectFrom how far "above" each layer sends connections, e.g. +2 indicates connections to the next two layers
 * @param _testName optional name for this test run
 * @param _logLocation optional, absolute path to place log files in
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::Cortex(int _numStdLayers,
		int rowsPerLayer, int colsPerLayer, int connectFrom, int connectTo, std::string _testName,
		std::string _logLocation) :
	numberOfStdLayers(0), testName(_testName), logLocation(_logLocation) {

	layers.reserve(_numStdLayers);

	// create all the layers
	for (int i = 0; i < numberOfStdLayers; i++) {
		addLayer(Cortex::DEFAULT_LAYER, rowsPerLayer, colsPerLayer);
	}

	// mark all Layers as "not connected"
	connectedLayer = new bool*[_numStdLayers];
	connectedLayer[0] = new bool[_numStdLayers * _numStdLayers];
	for (int i = 1; i < _numStdLayers; ++i) {
		connectedLayer[i] = connectedLayer[i - 1] + _numStdLayers;
	}

	// connect each Layer to other layers (after they're created)
	for (int layerID = 0; layerID < layers.size(); ++layerID) {
		connectLayerRange(layerID, connectFrom, connectTo);
	}

	// TODO: testName, logLocation don't do anything yet
}

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::~Cortex() {
	delete[] connectedLayer[0];
	delete[] connectedLayer;

	// all memory management handled by lower level objects, /shouldn't/ have any dynamically
	// other bits of allocated memory at this level

	// TODO: valgrind and figure out if this is true or not
}

/**
 * Adds a new Layer to our LayerVector
 *
 * @param layerType for now, DEFAULT or INPUT
 * @param rows number of rows in the new Layer
 * @param cols number of columns in the new Layer
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::addLayer(int layerType,
		int rows, int cols) {

	if (layerType == Cortex::DEFAULT_LAYER) {
		Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate> newLayer(rows, cols,
				numberOfStdLayers, false);
		numberOfStdLayers++;

		layers.push_back(newLayer);
	}

	// TODO: INPUT LAYER(s)!
}

/**
 * Helper method to connectLayerRange, performs the loop so the constructor code looks simpler
 *
 * @param layerID id of projecting Layer
 * @param connectFrom lower bound of connection range
 * @param connectTo upper bound of connection range
 * @see connectLayerRange()
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::connectLayerRange(int layerID,
		int connectFrom, int connectTo) {
	int bottom = layerID + connectFrom;
	int top = layerID + connectTo;

	for (int target = bottom; target <= top; ++target) {
		if (target >= 0)
			connectLayerToLayer(layerID, target);
	}
}

/**
 * Performs basic sanity checks on the requested IDs, then calls a method on the "fromID" Layer
 * that connects the two Layers. Finally, marks connectedLayer[fromID][toID] as true so we don't
 * duplicate Layer-Layer connections (not against any of the rules of the model, however)
 *
 * @param fromID Layer that connections originate from
 * @param toID Layer connections "synapse on"
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::connectLayerToLayer(int fromID,
		int toID) {
	// sanity checks
	if (fromID < 0 || toID < 0 || fromID > layers.size() || toID > layers.size())
		return;

	// don't double up on connections (although both directions are allowed)
	if (connectedLayer[fromID][toID])
		return;

	typename Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::AssemblyLayer_ID target =
			layers[toID].getAssemblyLayer();

	layers[fromID].connectToLayer(target);
	connectedLayer[fromID][toID] = true;
}

/**
 * Runs tick() on each Layer in layers, and calculates the activation of the entire network
 * as a weighted sum of each Layer's activation multiplied by it's number of Assemblies.
 *
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Cortex<ConnectionTemplate, UpdateTemplate, LearningTemplate>::tick() {
	// c++ is stupid. this won't compile w/out the 'typename' preface
	typename LayerVector::iterator layer;

	float activation_sum = 0;
	float assembly_sum = 0;

	for (layer = layers.begin(); layer != layers.end(); ++layer) {
		activation_sum += layer->tick();
		assembly_sum += layer->getSize();
	}

	if (layers.empty()) {
		averageLayerActivation = 0;
	} else {
		averageLayerActivation = activation_sum / layers.size();
	}
}

