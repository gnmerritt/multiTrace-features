/**
 * Cortex.h
 *
 *      @brief Layer container, implements global inhibition, manages wiring
 *
 * Manages a group of connection Layers, coordinating their construction,
 * wiring and interacting with a GUI to characterize their activity.
 *
 *
 *  @date Feb 15, 2011
 *      @author Nathan Merritt
 */

#ifndef CORTEX_H_
#define CORTEX_H_

#include <vector>
#include <iostream>
#include <set>
#include <utility>

#include <boost/shared_ptr.hpp>

#include "ConnectionPatterns.hpp"

#include "Layer.hpp"

class Cortex {
public:
     typedef boost::shared_ptr<Cortex> ptr;

     typedef std::pair<int, int> LayerConnection;
     typedef std::set<LayerConnection> ConnectionMap;

     enum LayerType {
          DEFAULT_LAYER, INPUT_LAYER,
     };

public:
     Cortex();
     virtual ~Cortex();

     int addLayer(ConnectionPatterns::classes _connectionPattern,
                  UpdateModels::classes _updateModel,
                  LearningRules::classes _learningRule,
                  LayerType layerType, int rows, int cols, bool selfConnected, bool lateralInhibition);
     void connectLayerRange(int layerID, int connectFrom, int connectTo);
     void connectLayerToLayer(int fromID, int toID);

     void tick(); // will tick all the layers

     void triggerAssembly(int layerID, int row, int col, float inputStrength);

     Layer::vector getLayers() {
          return layers;
     }

     void setTestName(std::string name) {
          testName = name;
     }

     void setLogLocation(std::string location) {
          logLocation = location;
     }

private:
     Layer::vector layers;
     ConnectionMap connectedLayers;
     int numberOfStdLayers;

     float averageLayerActivation;

     float arousal;
     float learningRate; // not sure this goes here?

     std::string testName, logLocation;
};

#endif /* CORTEX_H_ */
