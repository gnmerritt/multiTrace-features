/**
 * Layer.h
 *
 *	A Layer manages a 2d array of Assemblies. It constructs them, sets up
 *	their connections (both with each other and with assemblies in different
 *	layers), and calls tick() on each of them as the model runs.
 *
 *	The Layer is also the level where regional inhibition, clarity and arousal
 *	are managed. Clarity measures the level of specificity of response, indicating
 *	a well-learned stimuli. Regional inhibition serves to keep Assembly activity
 *	in check, while arousal serves to non-discriminatingly increase activity.
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <vector>
#include <utility>

#include <boost/shared_ptr.hpp>

#include "ConnectionPatterns.hpp"
#include "UpdateModels.hpp"

#include "Assembly.hpp"

#define DEBUG_LAYER_OUTPUT

class Layer {
public:
	typedef boost::shared_ptr<Layer> ptr;

	typedef Assembly Assembly_t; // possibly useful in the future?

	// holds our Assemblies (of type Assembly_t)
	typedef std::vector<Assembly_t> AssemblyVector;
	typedef std::vector<AssemblyVector> AssemblyLayer; // 2d!

	// used for connecting AssemblyLayers of different Layers
	typedef std::pair<AssemblyLayer*, int> AssemblyLayer_ID;
	typedef std::pair<Assembly_t*, AssemblyLocation*> LocalizedAssembly;

public:
	Layer(int _connectionPattern, int _updateModel, int _learningRule, int _rows, int _cols,
			int _layerID, bool connectToSelf);
	virtual ~Layer();

	float tick();

	void connectToLayer(AssemblyLayer_ID receivingLayer);

	float getLastRegionalActivation() const {
		return lastActivationAverage;
	}

	int getId() const {
		return layerID;
	}

	int getSize() const {
		return rows * cols;
	}

	float** getOutputBlock() const {
		return assemblyOutputBlock;
	}

	AssemblyLayer_ID getAssemblyLayer();

private:
	void connectLayerToLayer(AssemblyLayer_ID sendingLayer, AssemblyLayer_ID receivingLayer);
	void connectAssemblyToLayer(LocalizedAssembly sender, AssemblyLayer_ID receivingLayer);
	void connectAssemblyToAssembly(Assembly_t* sending, Assembly_t* receiving);

	int getAssemblyID(int row, int col);

public:
	int rows, cols;

private:
	AssemblyLayer assemblies;
	int layerID; /** 1 is the input layer */

	float lastActivationAverage;
	float** assemblyOutputBlock;
	int timestep;

	ConnectionPattern connectionPattern;

#ifdef DEBUG_LAYER_OUTPUT
	FILE *layer_tick_f;
#endif
};

#endif /* LAYER_H_ */
