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

#include "Assembly.h"
#include "Assembly.cpp" // for template class functions
#include "ConnectionPattern.h"

// types of LearningRules
#include "NoLearning.h"
#include "HebbianLearning.h"

typedef Assembly<NoLearning> NonLearningAssembly;
typedef Assembly<HebbianLearning> HebbianAssembly;

typedef NonLearningAssembly Assembly_t; // which Assembly are we currently using

typedef std::vector<std::vector<Assembly_t> > AssemblyLayer;

class Layer {
public:
	Layer(int rows, int cols);
	virtual ~Layer();

	void tick();

private:
	float calculateRegionalInhibition();

private:
	AssemblyLayer assemblies;
};

#endif /* LAYER_H_ */
