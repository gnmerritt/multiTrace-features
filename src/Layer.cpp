/**
 * Layer.cpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 */

#include <cstdio>

#include "Layer.hpp"

#ifdef DEBUG_LAYER_OUTPUT
const std::string layer_tick = "%d\t%f\n";
const std::string layer_init = "Timestep\tAverageActivity\n";
#endif

// the size of the sampling of the gaussian kernel
#define GAUSSIAN_SAMPLE_DIMENSIONS 5
#define GAUSSIAN_OFFSET 2
static const int GAUSSIAN_SCALE = 10; // LTCS of Connections = scale*weight[row][col]
// sampled Gaussian kernel filter with sigma = 1
static const float gaussianWeight[GAUSSIAN_SAMPLE_DIMENSIONS][GAUSSIAN_SAMPLE_DIMENSIONS] =
{
     { 0.00366300, 0.01465201, 0.02564102, 0.01465201, 0.00366300 },
     { 0.01465201, 0.05860805, 0.09523809, 0.05860805, 0.01465201 },
     { 0.02564102, 0.09523809, 0.00000000, 0.09523809, 0.02564102 },
     { 0.01465201, 0.05860805, 0.09523809, 0.05860805, 0.01465201 },
     { 0.00366300, 0.01465201, 0.02564102, 0.01465201, 0.00366300 } };

/**
 * Builds a layer that has size rows*cols. This entails the following steps:
 *
 *      Initializing an UpdateModel (1 for now), per-column eventually
 *      Constructing the Assemblies
 *      For each Assembly, use the ConnectionPattern to build input/output ConnectionVectors
 *        Initially these are only intra-Layer
 *
 * @param _connectionPattern which pattern to use in this Layer
 * @param _updateModel passed down to each Assembly, controls state variable updating
 * @param _learningRule passed down to each Assembly, controls how the Assembly learns
 * @param _rows number of rows in the Layer
 * @param _cols number of columns in the layer
 * @param _layerID the UID of this layer within the Cortex
 * @param connectToSelf whether or not this Layer projects onto itself
 */
Layer::Layer(ConnectionPatterns::classes _connectionPattern,
             UpdateModels::classes _updateModel,
             LearningRules::classes _learningRule,
             int _rows, int _cols, int _layerID, bool connectToSelf, bool lateralInhibition) :
     rows(_rows), cols(_cols), layerID(_layerID), lastActivationAverage(0.0f), timestep(0),
     connectionPattern(ConnectionPatterns::instanceOf(_connectionPattern)) {
     // initialize the update model
     // TODO: make this threaded
     UpdateModel::ptr updateModel(UpdateModels::instanceOf(_updateModel));

     assemblies.reserve(rows);
     assemblyOutputBlock.reserve(rows);

     // build a row by col sized AssemblyLayer (2d vector)
     for (int curRow = 0; curRow < rows; ++curRow) {
          assemblies.push_back(AssemblyVector());
          assemblies.back().reserve(cols);

          assemblyOutputBlock.push_back(FloatVec());
          assemblyOutputBlock.back().reserve(cols);

          for (int curCol = 0; curCol < cols; ++curCol) {
               int id = getAssemblyID(curRow, curCol);

               Assembly_t a(id, updateModel, _learningRule);
               assemblies.back().push_back(a);

               float row_ratio = (float) .5 * (rows - curRow) / rows;
               float col_ratio = (float) .5 * (cols - curCol) / cols;

               assemblyOutputBlock.back().push_back(row_ratio + col_ratio);
          }
     }

     // add lateral inhibition connections
     if (lateralInhibition) {
          connectLateralInhibition();
     }

     // and wire up our intra-Layer connections
     if (connectToSelf) {
          connectToLayer(getAssemblyLayer());
     }

#ifdef DEBUG_LAYER_OUTPUT
     std::stringstream out;
     out << "/tmp/layer_" << getId() << ".xls";

     std::string filename = out.str();

     layer_tick_f = fopen(filename.c_str(), "w");

     fprintf(layer_tick_f, "%s", layer_init.c_str());
#endif
}

Layer::~Layer() {
#ifdef DEBUG_LAYER_OUTPUT
     fclose(layer_tick_f);
#endif
}

Layer::AssemblyLayer_ID Layer::getAssemblyLayer() {
     return AssemblyLayer_ID(&assemblies, layerID);
}

/**
 * The main update method! Calculates regional inhibition, then ticks
 * all the Assemblies within this Layer.
 */
float Layer::tick() {
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
     //fprintf(layer_tick_f, layer_tick.c_str(), timestep, lastActivationAverage);
     printf(layer_tick.c_str(), timestep, lastActivationAverage);
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

/**
 * @brief Safe access into the assembly vector, wraps if it would be out of bounds
 *
 * Returns Activity*(1 - Fatigue) for the Assembly located at [row][col] (wraps around
 * if this would be an assembly that is out of bounds)
 *
 * @return float located at assemblyOutputBlock[row][col]
 */
float Layer::safeOutput(int row, int col) {
     const int wrapRow = row % assemblyOutputBlock.size();
     const int wrapCol = col % assemblyOutputBlock.back().size();

     Assembly_t assembly = assemblies.at(wrapRow).at(wrapCol);
     const float a = assembly.getOutput();
     const float f = assembly.getFatigue();

     return a * (1 - f);
}

/**
 * Loop to build up lateral inhibition around each Assemblies.
 */
void Layer::connectLateralInhibition() {
     for (unsigned int row = 0; row < assemblies.size(); ++row) {
          for (unsigned int col = 0; col < assemblies[0].size(); ++col) {
               // for each Assembly, loop again building up inhibition
               // around it
               Assembly_t* inhibited = &(assemblies[row][col]);

               // loop over the weights defined in gaussianWeight
               for (int gRow = -GAUSSIAN_OFFSET; gRow <= GAUSSIAN_OFFSET; ++gRow) {
                    for (int gCol = -GAUSSIAN_OFFSET; gCol <= GAUSSIAN_OFFSET; ++gCol) {
                         const int inhibitingRow = (gRow + row) % assemblies.size();
                         const int inhibitingCol = (gCol + col) % assemblies[0].size();

                         // don't ever add an Assembly as inhibiting itself
                         if (inhibitingRow == (int)row && inhibitingCol == (int)col) {
                              continue;
                         }

                         Assembly_t* inhibitor = &(assemblies[inhibitingRow][inhibitingCol]);

                         Connection::ptr c(new Connection());
                         const float inhibitionMagnitude = GAUSSIAN_SCALE*
                              gaussianWeight[gRow + GAUSSIAN_OFFSET][gCol + GAUSSIAN_OFFSET];

                         //printf("Added inhibition of: %f at (%d,%d)\n", inhibitionMagnitude,
                         //       inhibitingCol, inhibitingRow);
                         c->setLTCS(inhibitionMagnitude);

                         inhibited->addLateralInhibition(c);
                         inhibitor->addOutgoingConnection(c);
                    }
               }
          }
     }
}

/** Iterate over the AssemblyLater and check connectivity between
 *  any two Assemblies. Runs n^2, but we have to do it... This method
 *  implements the first part of the iterator, which runs on every Assembly
 *  in the sending layer
 *
 *  @see connectAssemblyToLayer()
 *  @sideeffect populates output connections of Assemblies in sendingLayer
 *  @param sender Layer&ID we're projecting connections from (efferent)
 *  @param receiver Layer&ID we're connecting to (afferent)
 */
void Layer::connectToLayer(AssemblyLayer_ID receivingLayer) {
     // do all the connecting
     for (int row = 0; row < rows; ++row) {
          for (int col = 0; col < cols; ++col) {
               connectAssemblyToLayer(row, col, receivingLayer);
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
void Layer::connectAssemblyToLayer(int sourceRow, int sourceCol, AssemblyLayer_ID receivingLayer) {
     AssemblyLayer *receiving = receivingLayer.first;
     int receivingID = receivingLayer.second;

     AssemblyLocation sendingLoc(sourceRow, sourceCol, layerID);

     for (unsigned int row = 0; row < receiving->size(); ++row) {
          for (unsigned int col = 0; col < receiving->at(row).size(); ++col) {

               AssemblyLocation receivingLoc(row, col, receivingID);

               float strength = connectionPattern->areConnected(sendingLoc, receivingLoc);

               // initial strength of 0 is allowed
               if (strength >= 0.0f) {
                    connectAssemblyToAssembly(assemblies.at(sourceRow).at(sourceCol),
                                              receiving->at(row).at(col));
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
void Layer::connectAssemblyToAssembly(Assembly_t &sending, Assembly_t &receiving) {
     // Don't connect an Assembly to itself
     if (receiving.getId() == sending.getId()) {
          return;
     }

     Connection::ptr c(new Connection());

     sending.addOutgoingConnection(c);
     receiving.addIncomingConnection(c);

     const float distance = sending.distanceTo(receiving);
     c->setDistance(distance);
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
int Layer::getAssemblyID(int row, int col) {
     int layer_field = layerID * 1000000;
     int row_field = row * 1000;

     int id = layer_field + row_field + col;

     return id;
}
