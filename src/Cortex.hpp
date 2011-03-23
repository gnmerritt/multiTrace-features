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
#include <iostream>

#include <boost/shared_ptr.hpp>

#include "Layer.h"

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
class Cortex {
public:
	typedef std::vector<Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate> > LayerVector;
	// NOTE: you will need to declare iterators as 'typename LayerVector::iterator foo'

	typedef boost::shared_ptr<Cortex> ptr;

private:
	enum LayerType {
		DEFAULT_LAYER, INPUT_LAYER,
	};

public:
	Cortex(int _numStdLayers, int rowsPerLayer, int colsPerLayer, int connectTo, int connectFrom,
			std::string _testName, std::string _logLocation);
	virtual ~Cortex();

	void addLayer(int layerType, int rows, int cols);
	void connectLayerRange(int layerID, int connectFrom, int connectTo);
	void connectLayerToLayer(int fromID, int toID);

	void tick(); // will tick all the layers

private:
	LayerVector layers;
	int numberOfStdLayers;

	bool** connectedLayer;

	float averageLayerActivation;

	float arousal;
	float learningRate; // not sure this goes here?

	std::string testName, logLocation;
};

#endif /* CORTEX_H_ */
