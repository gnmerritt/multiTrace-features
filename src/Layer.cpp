/*
 * Layer.cpp
 *
 *  Created on: Feb 2, 2011
 *      Author: Nathan Merritt
 */

#include "Layer.h"

#ifdef DEBUG_LAYER_OUTPUT
const std::string layer_tick = "%d\t%f\n";
const std::string layer_init = "Timestep\tAverageActivity\n";
#endif

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
 * @param _layerID the UID of this layer within the Cortex
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::Layer(int _rows,
		int _cols, int _layerID) :
	rows(_rows), cols(_cols), layerID(_layerID), lastActivationAverage(0.0f),
			timestep(0), connectionPattern(ConnectionTemplate()) {
	// initialize the update model
	// TODO: make this threaded
	UpdateModel::ptr updateModel ( new UpdateTemplate() );

	assemblies.reserve(rows);

	// build a row by col sized AssemblyLayer (2d vector)
	for (int curRow = 0; curRow < rows; ++curRow) {
		assemblies.push_back(AssemblyVector());
		assemblies.back().reserve(cols);

		for (int curCol = 0; curCol < cols; ++curCol) {
			int id = getAssemblyID(curRow, curCol);

			Assembly_t a(id, updateModel);
			assemblies.back().push_back(a);
		}
	}

	// and wire up our intra-Layer connections
	AssemblyLayer_ID thisLayer = getAssemblyLayer();
	connectLayerToLayer(thisLayer, thisLayer);

	/** create our assemblyOutputBlock, a float[row][col] with each Assembly's output
	 for the GUI	*/
	assemblyOutputBlock = new float*[rows];
	assemblyOutputBlock[0] = new float[rows * cols];
	for (int i = 1; i < rows; ++i) {
		assemblyOutputBlock[i] = assemblyOutputBlock[i - 1] + rows;
	}

#ifdef DEBUG_LAYER_OUTPUT
	std::stringstream out;
	out << "/tmp/layer_" << getId() << ".xls";

	std::string filename = out.str();

	layer_tick_f = fopen(filename.c_str(), "w");

	fprintf(layer_tick_f, "%s", layer_init.c_str());
#endif
}

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::~Layer() {
	// deallocate assemblyOtputBlock
	delete[] assemblyOutputBlock[0];
	delete[] assemblyOutputBlock;

#ifdef DEBUG_LAYER_OUTPUT
	fclose(layer_tick_f);
#endif
}

template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
typename Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::AssemblyLayer_ID Layer<
		ConnectionTemplate, UpdateTemplate, LearningTemplate>::getAssemblyLayer() {
	return typename Layer::AssemblyLayer_ID(&assemblies, layerID);
}

/**
 * The main update method! Calculates regional inhibition, then ticks
 * all the Assemblies within this Layer.
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
float Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::tick() {
	float currentActivation_sum = 0;

	// tick all the Assemblies, and remember their output
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < cols; ++col) {
			float thisAssembly;

			thisAssembly = assemblies[row][col].tick(lastActivationAverage);
			assemblyOutputBlock[row][col] = thisAssembly;
			currentActivation_sum += thisAssembly;
		}
	}

	// print to our debug file
#ifdef DEBUG_LAYER_OUTPUT
	fprintf(layer_tick_f, layer_tick.c_str(), timestep, lastActivationAverage);
#endif

	// update the average activation (used for Regional Inhibition)
	int numAssemblies = rows * cols;
	if (numAssemblies == 0) {
		numAssemblies = 1;
	}

	lastActivationAverage = currentActivation_sum / numAssemblies;

	timestep++;

	return lastActivationAverage;
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
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::connectLayerToLayer(
		AssemblyLayer_ID sendingLayer, AssemblyLayer_ID receivingLayer) {
	AssemblyLayer *projecting = sendingLayer.first;
	int projectingID = sendingLayer.second;

	// do all the connecting
	for (unsigned int row = 0; row < projecting->size(); ++row) {
		for (unsigned int col = 0; col < projecting->at(row).size(); ++col) {
			AssemblyLocation location(row, col, projectingID);

			LocalizedAssembly sendingAssembly(&(projecting->at(row)[col]),
					&location);

			connectAssemblyToLayer(sendingAssembly, receivingLayer);
		}
	}

	// and now recalculate the incoming connection weights in the receivingLayer
	typename AssemblyLayer::iterator row;
	typename AssemblyVector::iterator col;
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
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::connectAssemblyToLayer(
		LocalizedAssembly sender, AssemblyLayer_ID receivingLayer) {
	AssemblyLayer *receiving = receivingLayer.first;
	int receivingID = receivingLayer.second;

	AssemblyLocation sendingLoc = *sender.second;

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
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
void Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::connectAssemblyToAssembly(
		Assembly_t* sending, Assembly_t* receiving) {
	// Don't connect an Assembly to itself
	if (receiving == sending) {
		return;
	}

	Connection *c = new Connection();

	sending->addOutgoingConnection(c);
	receiving->addIncomingConnection(c);
}

/*
 * Calculates the unique identifier for the Assembly at row,col in this Layer.
 * Currently implemented as an integer with three parts: (layerID, row, col) = (___,___,___)
 * Limits us to 100 Layers of 100x100 (1 million Assemblies)
 *
 * @param row Assembly's row in AssemblyLayer
 * @param col Assembly's col in AssemblyLayer
 * @return the unique identification number of the Assembly at (col,row,layerID)
 */
template<class ConnectionTemplate, class UpdateTemplate, class LearningTemplate>
int Layer<ConnectionTemplate, UpdateTemplate, LearningTemplate>::getAssemblyID(
		int row, int col) {
	int layer_field = layerID * 1000000;
	int row_field = row * 1000;

	int id = layer_field + row_field + col;

	return id;
}
