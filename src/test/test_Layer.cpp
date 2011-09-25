/*
 * test_Layer.cpp
 *
 *  @date Feb 10, 2011
 *      @author Nathan Merritt
 */

#include "test_Layer.hpp"

const static float LAYER_THRESHOLD_ACTIVE = 0.7f;
const static float LAYER_THRESHOLD_SILENT = 0.1f;

/**
 * Tests the corner case of a 1x1 Layer (only 1 Assembly). This test
 * ensures that none of the update or tick algorithms break with the
 * minimum number of inputs
 *
 * @returns True if a 1x1 Layer is constructed and remains silent
 */
bool noInputLayer1_1() {
	Layer::ptr layer(new Layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
				   LearningRules::NO_LEARNING, 1, 1, 1, true, true)); // 1x1, layerID=1
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

/**
 * Tests the creation of a large layer. This is helpful to see if any
 * of the tick() or creation algorithms break down when run on a larger
 * number of assemblies.
 *
 * @returns True if a 30x30 layer can be created and ticked
 */
bool noInputLayer30_30() {
	Layer::ptr layer(new Layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
				   LearningRules::NO_LEARNING, 30, 30, 2, true, true)); // 30x30, layerID=2

	for (int i = 0; i < 500; ++i) {
		float avgOutput = layer->tick();

		if (avgOutput > (float) 0.1f) {
			return false;
		}
	}

	return true;
}

/**
 *	Our first real, activity-driven unit test. This test (using a smaller
 *	network, for speed's sake) adds new connections to every Assembly and
 *	fully activates them for the first 50 ticks. This should be enough to drive
 *	the entire layer to fully activate.
 *
 * @returns True if 20 hyper-active connections to each Assembly can drive
 * activity in the whole layer
 */
bool singleInputLayer10_10() {
	Layer layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
		    LearningRules::NO_LEARNING, 10, 10, 3, true, true);

	Layer::AssemblyLayer *assemblies = layer.getAssemblyLayer().first;
	Layer::AssemblyLayer::iterator row;
	Layer::AssemblyVector::iterator col;

	const int numNewConnections = 20;

	Connection::vector c;
	c.reserve(numNewConnections);

	for (int i = 0; i < numNewConnections; ++i) {
		Connection::ptr connec(new Connection());
		c.push_back(connec);
		c[i]->setActivity(99);
	}

	// input on all the assemblies in the first row
	for (row = assemblies->begin(); row != assemblies->end(); ++row) {
		for (col = row->begin(); col != row->end(); ++col) {
			for (int i = 0; i < numNewConnections; ++i) {
				col->addIncomingConnection(c[i]);
			}
			col->initializeIncConnectionStrengths();
		}
	}

	float max = -1;

	for (int i = 0; i < 1000; ++i) {
		if (i == 50) {
			for (int i = 0; i < numNewConnections; ++i) {
				c[i] = Connection::ptr(new Connection());
				c[i]->setActivity(0);
			}
		}

		float avgOutput = layer.tick();

		if (avgOutput > max) {
			max = avgOutput;
		}

		if (avgOutput > 1) {
			printf("output exceeded 1, failing");
			return false;
		}
	}

	if (max >= LAYER_THRESHOLD_ACTIVE) {
		return true;
	}

	printf("layer did not fully activate, failing");
	return false;
}
