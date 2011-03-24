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

#include "ConnectionPatterns.hpp"

#include "Layer.hpp"

class Cortex {
public:
	typedef std::vector<Layer> LayerVector;
	// NOTE: you will need to declare iterators as 'typename LayerVector::iterator foo'

	typedef boost::shared_ptr<Cortex> ptr;

	enum LayerType {
		DEFAULT_LAYER, INPUT_LAYER,
	};

public:
	Cortex();
	virtual ~Cortex();

	int addLayer(int connectionPattern, int updateModel, int learningRule, int layerType, int rows,
			int cols);
	void connectLayerRange(int layerID, int connectFrom, int connectTo);
	void connectLayerToLayer(int fromID, int toID);

	void tick(); // will tick all the layers

        void setTestName(std::string name) {
            testName = name;
        }

        void setLogLocation(std::string location) {
            logLocation = location;
        }

private:
	LayerVector layers;
	int numberOfStdLayers;

	float averageLayerActivation;

	float arousal;
	float learningRate; // not sure this goes here?

	std::string testName, logLocation;
};

#endif /* CORTEX_H_ */
