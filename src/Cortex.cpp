/*
 * Cortex.cpp
 *
 *  Created on: Feb 15, 2011
 *      Author: Nathan Merritt
 */

#include "Cortex.hpp"

/**
 * @brief Cortex constructor
 *
 * Doesn't do much work, all the fun stuff is done via addLayer and connectLayerRange
 */
Cortex::Cortex() :
        layers(LayerVector()), connectedLayers(ConnectionMap()), numberOfStdLayers(0),
        testName(""), logLocation("") {
}

Cortex::~Cortex() {
	// all memory management handled by lower level objects, /shouldn't/ have any bits of
	// dynamically allocated memory at this level

	// TODO: valgrind and figure out if this is true or not
}

/**
 * Adds a new Layer to our LayerVector
 *
 * @param connectionPattern sets the Layer's connectivity, from Cortex::ConnectionPatterns
 * @param updateModel the Layer's update model, @see Cortex::UpdateModels
 * @param learningRule how the Layer learns, @see Cortex::LearningRules
 * @param layerType for now, DEFAULT or INPUT
 * @param rows number of rows in the new Layer
 * @param cols number of columns in the new Layer
 */
int Cortex::addLayer(int connectionPattern, int updateModel, int learningRule, int layerType,
		int rows, int cols) {
	if (layerType == Cortex::DEFAULT_LAYER) {
		Layer newLayer(connectionPattern, updateModel, learningRule, rows, cols, numberOfStdLayers, false);
		numberOfStdLayers++;

		layers.push_back(newLayer);
	}

	// TODO: INPUT LAYER(s)!

	return numberOfStdLayers - 1;
}

/**
 * Helper method to connectLayerRange, performs the loop so the constructor code looks simpler
 *
 * @param layerID id of projecting Layer
 * @param connectFrom lower bound of connection range
 * @param connectTo upper bound of connection range
 * @see connectLayerRange()
 */
void Cortex::connectLayerRange(int layerID, int connectFrom, int connectTo) {
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
void Cortex::connectLayerToLayer(int fromID, int toID) {
	// sanity checks
	if (fromID < 0 || toID < 0 || fromID > layers.size() || toID > layers.size())
		return;

	// don't double up on connections (although both directions are allowed)
        LayerConnection newConnection(fromID, toID);
        if (connectedLayers.find(newConnection) != connectedLayers.end()) {
            return;
        }

	Layer::AssemblyLayer_ID target = layers[toID].getAssemblyLayer();

	layers[fromID].connectToLayer(target);
        connectedLayers.insert(newConnection);
}

/**
 * Runs tick() on each Layer in layers, and calculates the activation of the entire network
 * as a weighted sum of each Layer's activation multiplied by it's number of Assemblies.
 *
 */
void Cortex::tick() {
	LayerVector::iterator layer;

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

