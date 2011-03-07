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

#include "Assembly.h"
#include "Assembly.cpp" // for template class functions
#include "UNR.h"
#include "SonntagUpdate.h"

// types of LearningRules
#include "NoLearning.h"
#include "HebbianLearning.h"

#define DEBUG_LAYER_OUTPUT

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
class Layer {
public:
	typedef Assembly<LearningTemplate> Assembly_t;

	// holds our Assemblies (of type Assembly_t)
	typedef std::vector<Assembly_t> AssemblyVector;
	typedef std::vector<AssemblyVector> AssemblyLayer; // 2d!

	// used for connecting AssemblyLayers of different Layers
	typedef std::pair<AssemblyLayer*, int> AssemblyLayer_ID;
	typedef std::pair<Assembly_t*, AssemblyLocation*> LocalizedAssembly;

public:
	Layer(int rows, int cols, int layerID);
	virtual ~Layer();

	float tick();

	float getLastRegionalActivation() const {
		return lastActivationAverage;
	}

	int getId() const {
		return layerID;
	}

	float** getOutputBlock() const {
		return assemblyOutputBlock;
	}

	AssemblyLayer_ID getAssemblyLayer();

private:
	void connectLayerToLayer(AssemblyLayer_ID sendingLayer,
			AssemblyLayer_ID receivingLayer);
	void connectAssemblyToLayer(LocalizedAssembly sender,
			AssemblyLayer_ID receivingLayer);
	void connectAssemblyToAssembly(Assembly_t* sending, Assembly_t* receiving);

	int getAssemblyID(int row, int col);

public:
	const int rows, cols;

private:
	AssemblyLayer assemblies;
	const int layerID; /** 1 is the input layer */

	float lastActivationAverage;
	float** assemblyOutputBlock;
	int timestep;

	ConnectionTemplate connectionPattern;

#ifdef DEBUG_LAYER_OUTPUT
	FILE *layer_tick_f;
#endif
};

#endif /* LAYER_H_ */
