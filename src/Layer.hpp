/**
 * Layer.hpp
 *
 *  @created Feb 2, 2011
 *  @author Nathan Merritt
 */

#ifndef LAYER_H_
#define LAYER_H_

#include <vector>
#include <utility>

#include <boost/shared_ptr.hpp>

#include "ConnectionPatterns.hpp"
#include "UpdateModels.hpp"

#include "Assembly.hpp"

//#define DEBUG_LAYER_OUTPUT

/**
 *      @brief Assembly container, with both lateral and regional inhibition
 *
 *      A Layer manages a 2d array of Assemblies. It constructs them, sets up
 *      their connections (both with each other and with assemblies in different
 *      layers), and calls tick() on each of them as the model runs.
 *
 *      The Layer is also the level where regional inhibition, clarity and arousal
 *      are managed. Clarity measures the level of specificity of response, indicating
 *      a well-learned stimuli. Regional inhibition serves to keep Assembly activity
 *      in check, while arousal serves to non-discriminatingly increase activity.
 */
class Layer {
public:
     typedef boost::shared_ptr<Layer> ptr;
     typedef std::vector<Layer::ptr> vector;

     typedef Assembly Assembly_t; // possibly useful in the future?

     // holds our Assemblies (of type Assembly_t)
     typedef std::vector<Assembly_t> AssemblyVector;
     typedef std::vector<AssemblyVector> AssemblyLayer; // 2d!

     // used for connecting AssemblyLayers of different Layers
     typedef std::pair<AssemblyLayer*, int> AssemblyLayer_ID;
     typedef std::pair<Assembly_t*, AssemblyLocation*> LocalizedAssembly;

     // Layer output block, for easy visualization by a GUI
     typedef std::vector<float> FloatVec;
     typedef std::vector<FloatVec> LayerOutput;

public:
     Layer(ConnectionPatterns::classes _connectionPattern,
           UpdateModels::classes _updateModel,
           LearningRules::classes _learningRule,
           int _rows, int _cols, int _layerID, bool connectToSelf, bool lateralInhibition);
     virtual ~Layer();

     float tick();

     void connectToLayer(AssemblyLayer_ID receivingLayer);

     float getLastRegionalActivation() const {
          return lastActivationAverage;
     }

     Assembly_t getAssembly(int row, int col) {
          return assemblies[row][col];
     }

     int getId() const {
          return layerID;
     }

     int getSize() const {
          return rows * cols;
     }

     LayerOutput* getOutputBlock() {
          return &(assemblyOutputBlock);
     }

     void triggerAssembly(int row, int col, float inputStrength) {
          if (inputStrength > 1) {
               inputStrength = 1;
          } else if (inputStrength < 0) {
               inputStrength = 0;
          }

          getAssembly(row, col).setActivation(inputStrength);
     }

     void printOutputBlock(); ///@todo

     AssemblyLayer_ID getAssemblyLayer();

private:
     void connectAssemblyToLayer(int row, int col, AssemblyLayer_ID receivingLayer);
     void connectAssemblyToAssembly(Assembly_t &sending, Assembly_t &receiving);

     void connectLateralInhibition();

     float safeOutput(int row, int col);

     int getAssemblyID(int row, int col);

public:
     int rows, cols;

private:
     AssemblyLayer assemblies;
     int layerID;

     float lastActivationAverage;
     LayerOutput assemblyOutputBlock;
     int timestep;

     ConnectionPattern::ptr connectionPattern;

#ifdef DEBUG_LAYER_OUTPUT
     FILE *layer_tick_f;
#endif
};

#endif /* LAYER_H_ */
