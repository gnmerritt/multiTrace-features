/**
 * Assembly.hpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 */

#ifndef ASSEMBLY_H_
#define ASSEMBLY_H_

#include <string>
#include <cstring>
#include <sstream>

#include "AssemblyState.hpp"
#include "LearningRules.hpp"
#include "UpdateModel.hpp"
#include "Connection.hpp"

static const int MINIMUM_FULL_ID = 1000000;
static const int ROW_ID = 100000; // row is middle 3 digits nnn,___,nnn
static const int COLUMN_ID = 100; // column is last three digits

//#define DEBUG_ASSEMBLY_OUTPUT

/**
 *      @brief Models a connected, well-learned network of neurons capable of self-sustaining activity
 *
 *      The unit of basic computation in a multiTrace network. Each assembly
 *      models a well-defined neurological Cell Assembly, and can sustain
 *      its own activity.
 */
class Assembly {
public:
    Assembly(int _id, UpdateModel::ptr _model, int _learningRule);
    virtual ~Assembly();

    void addIncomingConnection(Connection::ptr newInput);
    void addOutgoingConnection(Connection::ptr newOutput);
    void addLateralInhibition(Connection::ptr newInhibition);

    void setIncomingConnections(Connection::vector in);
    void setOutgoingConnections(Connection::vector out);

    float distanceTo(const Assembly &other);

    float tick(float regional_activation);

    void initializeIncConnectionStrengths(); // danger: will destroy all learning

    void setActivation(float strength);

    int getId() const {
        return id;
    }

    int getLayer() const;
    int getRow() const;
    int getCol() const;

    float getActivation() const { // A(t)
        return state->activity;
    }
    float getOutput() const { // A(t-1)
        return state->output;
    }
    float getLTCS() const { // L(t) (constant)
        return state->ltcs;
    }
    float getSTCS() const { //S(t)
        return state->stcs;
    }
    float getFatigue() const { // F(t)
        return state->fatigue;
    }
    float getRegionalInhibition() const {
        return state->regional_activation; // set from our Layer by tick()
    }

private:
    void initializeLearningRule();
    float calculateInhibition();

private:
    int id; /** unique in a network, contains row,col,layerId @see getAssemblyID */
    int ruleId;

    int timestep; /** how many times tick() has been called */

    AssemblyState::ptr state;
    UpdateModel::ptr updateModel;
    Connection::vector input, output, lateralInhibition;
    LearningRule::ptr learningRule;

#ifdef DEBUG_ASSEMBLY_OUTPUT
    FILE *tick_f;
#endif
};

#endif /* ASSEMBLY_H_ */
