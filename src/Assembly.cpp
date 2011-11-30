/**
 * Assembly.cpp
 *
 *  @date Feb 2, 2011
 *      @author Nathan Merritt
 */

#include <cstdio>
#include <math.h>

#include "Assembly.hpp"

const float INITIAL_TOTAL_LTCS = 0.4f; // how much of an Assembly's LTCS is intra-Assembly
const float STCS_GAIN = 0.2f; // controls spread of activity via Connection::stcs

// used by debug printf's
#ifdef DEBUG_ASSEMBLY_OUTPUT
const std::string assembly_tick = "%d\t%f\t%f\t%f\t%f\t%f\n";
const std::string assembly_init = "Timestep\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n";
#endif

Assembly::Assembly(int _id, UpdateModel::ptr _model, int _learningRule) :
    id(_id), ruleId(_learningRule), timestep(0), state(new AssemblyState()), updateModel(_model),
    input(Connection::vector()), output(Connection::vector()), lateralInhibition(Connection::vector())
{
    initializeLearningRule();

#ifdef DEBUG_ASSEMBLY_OUTPUT
    std::stringstream out;
    out << "/tmp/assembly_" << getId() << ".xls";

    std::string filename = out.str();

    tick_f = fopen(filename.c_str(), "w");

    fprintf(tick_f, "%s", assembly_init.c_str());
#endif
}

Assembly::~Assembly() {
#ifdef DEBUG_ASSEMBLY_OUTPUT
    fclose(tick_f);
#endif
}

/**
 * Accessors for Row, Column & Layer
 */
int Assembly::getLayer() const {
    if (id < MINIMUM_FULL_ID) {
        return 0;
    }
    const int bigLayer = id - getRow() - getCol();

    return bigLayer / MINIMUM_FULL_ID;
}

int Assembly::getRow() const {
    if (id < MINIMUM_FULL_ID) {
        return 0;
    }
    return ((id % ROW_ID) - getCol()) / 1000;
}

int Assembly::getCol() const {
    if (id < MINIMUM_FULL_ID) {
        return 0;
    }
    return id % COLUMN_ID;
}

/**
 * Used to wire up inter-Assembly connections
 *
 * @see connectAssemblyToAssembly()
 * @param newInput Connection added to our input vector
 * @sideeffect reinitializes the initial connection strengths (undoes learning)
 */
void Assembly::addIncomingConnection(Connection::ptr newInput) {
    input.push_back(newInput);
}

/**
 * Used to wire up inter-Assembly connections
 *
 * @see connectAssemblyToAssembly()
 * @param newOutput Connection added to our output vector
 */
void Assembly::addOutgoingConnection(Connection::ptr newOutput) {
    output.push_back(newOutput);

    newOutput->setSTCS(&(state->stcs));
    newOutput->setActivity(&(state->activity));
    newOutput->setLastActivity(&(state->output));
}

/**
 * Adds a lateral inhibition connection to this Assembly. This approach is used
 * instead of something in Layer::tick() because it's a lot faster and cleaner.
 */
void Assembly::addLateralInhibition(Connection::ptr newInhibition) {
    lateralInhibition.push_back(newInhibition);
}

/**
 * Run the learning rule constructor
 */
void Assembly::initializeLearningRule() {
    learningRule = LearningRule::ptr(LearningRules::instanceOf(ruleId, state, &input));
}

/**
 * Passes our state & connections to the UpdateModel, where they are updated.
 * Next, updates incoming connections by our LearningRule
 * Finally, updates outgoing connections to reflect our current activity
 *
 * @param regional_activation sum of A(t) for all Assemblies in this layer
 * @see UpdateModel.h
 * @see LearningRule.h
 */
float Assembly::tick(float regional_activation) {
    // update our internal state with layer data
    state->regional_activation = regional_activation;
    state->lateral_inhibition = calculateInhibition();

    // tick the internal state
    updateModel->tick(state, &input);

    // adjust incoming connections
    learningRule->tick(&input);

    // log timestep activity to our debug file in /tmp
#ifdef DEBUG_ASSEMBLY_OUTPUT
    fprintf(tick_f, assembly_tick.c_str(), ++timestep, getActivation(), getLTCS(), getSTCS(),
	    getFatigue(), getRegionalInhibition());
#endif

    // return our activation to our Layer
    return state->output;
}

/**
 * Simple iterator that sums input of any connections marked as inhibitory
 *
 */
float Assembly::calculateInhibition() {
    float sum = 0;
    Connection::vector::iterator c;

    for (c = lateralInhibition.begin(); c != lateralInhibition.end(); ++c) {
	Connection::ptr connection = *c;
	sum += connection->getOutput();
    }

    return sum;
}

/**
 * @brief
 * Sets the input of this Assembly, to be applied next time tick() is called. THIS WILL BREAK LEARNING
 *
 * This setter will manually replace the term that is normally calculated by
 * summing all of the incoming connections. This means that Assembly activation
 * will be uncoupled from input its receiving, which is not a problem unless
 * you're trying to model learning somehow. YOU HAVE BEEN WARNED.
 *
 * @param strength amount of activation, valid ranges on [0, 1]
 */
void Assembly::setActivation(float strength) {
    if (strength > 1) {
	strength = 1.0f;
    }
    if (strength < 0) {
	strength = -1.0f;
    }

    state->manual_input = strength;
}

/**
 * Sets up a pre-configured array of input Connections, and assigns them
 * an initial LTCS based on in.size()
 *
 * @param in a pointer to a ConnectionVector which we will use as input
 */
void Assembly::setIncomingConnections(Connection::vector in) {
    input = in;

    initializeLearningRule();
    initializeIncConnectionStrengths();
}

/**
 * Processing of weights is handled post-synaptically, so we don't need
 * to do anything to these connections except update their activity
 * level when we tick().
 *
 * @param out a pointer to a ConnectionVector which we will keep updated with our activity
 */
void Assembly::setOutgoingConnections(Connection::vector out) {
    output = out;
}

/**
 * Returns the distance squared between two Assemblies
 *
 * @param other the Assembly we're finding distance to
 * @return square of euclidean distance between two Assemblys
 */
float Assembly::distanceTo(const Assembly &other) {
    float diff_x = pow((getCol() - other.getCol()), 2);
    float diff_y = pow((getRow() - other.getRow()), 2);
    float diff_z = pow((getLayer() - other.getLayer()), 2);

    return diff_x + diff_y + diff_z;
}

/**
 * Intra-unit LTCS doesn't change, so we have (1-LTCS) to spread among
 * each of our n incoming connections.
 *
 * @see updateOutgoingConnections()
 */
void Assembly::initializeIncConnectionStrengths() {
    const int numConnections = input.empty() ? 0 : input.size() ;

    float perConnection = (1 - INITIAL_TOTAL_LTCS) / numConnections;

    Connection::vector::iterator in;
    for (in = input.begin(); in != input.end(); ++in) {
	//(*in)->setInitialLTCS(perConnection);
	(*in)->setLTCS(perConnection);
    }

    learningRule->dropLearning();
}

