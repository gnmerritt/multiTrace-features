/**
 * HebbianLearning.cpp
 *
 *  @date Feb 2, 2011
 *  @author Nathan Merritt
 *  @see HebbianLearning.hpp
 */

#include <algorithm>
#include <math.h>

#include "HebbianLearning.hpp"

//#define DEBUG_LEARNING
//#define DEBUG_LEARNING_VERBOSE

/** default parameterization, @see setParameters */
static const float DEFAULT_PARAMETERS[] =
{ 0.01f, // learning strength
  0.1f, // receiving learning (start)
  0.6f, // receiving learning (stop)
  0.0f, // sending contrib threshold
};

/**
 * Calls super constructor, and sets default parameters.
 *
 * @param state Pointer to the AssemblyState we read from
 * @param input Pointer to the input Connections we use for calculations
 */
HebbianLearning::HebbianLearning(AssemblyState::ptr state, Connection::vector *input) :
     LearningRule(state, input), storedContributions(SynapseHistory()), hasStoredLearning(false) {
     setParameters(DEFAULT_PARAMETERS);
     resetStoredLearning();
}

HebbianLearning::~HebbianLearning() {
     resetStoredLearning();
}

/**
 *      @brief
 *      Main method for handling Hebbian update of input connections to an Assembly
 *
 *      There are four important states for learning:
 *              - Activity is above the lower learning threshold, but not yet full.
 *                 In this state we take note of what other Assemblies activating us
 *              - Activity becomes fully active (1 tick). Remember this, so that we
 *                 can learn (later) based on which inputs caused us to fire.
 *              - After being fully active, activity has calmed down. Apply learning.
 *              - Activity lower than in the last tick, and we haven't reached full
 *                activation yet. The earlier inputs did not cause us to fire, so
 *                forget about them.
 *
 *      Other states (fully active, weakly active...) aren't important to this model of
 *      learning, so they aren't treated here.
 *
 *      @see LearningRule.hpp
 */
void HebbianLearning::tick(Connection::vector *newInput) {
     incomingConnections = newInput;

     const float last_activity = postSynapticState->output;
     const float activity = postSynapticState->activity;
     const float activity_derivative = activity - last_activity;

     // Assembly is being activated, record input connection contributions
     if (activity > parameters[REC_LEARNING_LOWER] && activity < parameters[REC_LEARNING_UPPER]
         && activity_derivative >= 0) {
          tallyContributions();
#ifdef DEBUG_LEARNING
          printf("Tallying contributions!\n");
#endif
     }

     // crossed the LEARNING_STOP boundary for the first time, mark our learning as stored
     // this should get run when the Assembly has fired fully
     else if (activity > parameters[REC_LEARNING_UPPER] && last_activity
              <= parameters[REC_LEARNING_UPPER]) {
          hasStoredLearning = true;
#ifdef DEBUG_LEARNING
          printf("hasStoredLearning = true\n");
#endif
     }

     // Assembly fired, and now activity has dropped off. Apply stored learning.
     else if (activity < parameters[REC_LEARNING_LOWER] && hasStoredLearning) {
          applyLearningToConnections();
          resetStoredLearning();
#ifdef DEBUG_LEARNING
          printf("applying learning, activity has dropped off\n");
#endif
     }

     // Assembly failed to fire, delete any learning we've stored so far
     else if (activity < parameters[REC_LEARNING_UPPER] && activity_derivative < 0
              && !hasStoredLearning) {
          resetStoredLearning();
#ifdef DEBUG_LEARNING_VERBOSE
          // this will get printed /A LOT/
          printf("Assembly failed to fire, deleting learning\n");
#endif
     }

     // no default case on purpose
}

/**
 * @brief
 * Resets any stored learning, used only when the Assembly adds or deletes Connection s
 *
 */
void HebbianLearning::dropLearning() {
     resetStoredLearning(); // nothing else that needs to happen yet
}

/**
 *      Main method for determining changes in input connection strengths (ie, learning).
 *  First, calculates the %contribution of each connection, then weights it by
 *  receivingCurve() and sendingCurve(). These weighted values are then stored, and
 *  if this Assembly becomes fully active than the incoming connection strengths
 *  of any Assemblies that helped it fire are strengthened.
 *
 * @see receivingCurve()
 * @see sendingCurve()
 * @see applyLearningToConnections()
 */
void HebbianLearning::tallyContributions() {
     // pass 1: determine total activation among all inputs
     double totalInput = 0;
     Connection::vector::iterator c;
     for (c = incomingConnections->begin(); c != incomingConnections->end(); ++c) {
          // double dereferenced b/c Connection::vector is of pointers
          Connection::ptr connection = *c;
          totalInput += connection->getOutput();
     }

     // pass 2: for each input, scale contribution by receivingCurve() and sendingCurve(),
     // and add it to storedContributions
     float receivingWeight = receivingCurve(postSynapticState->activity);
     for (unsigned int i = 0; i < incomingConnections->size(); ++i) {
          Connection::ptr input = incomingConnections->at(i);
          const float percentContribution = input->getOutput() / totalInput;
          const float weightedContribution = percentContribution * sendingCurve(percentContribution)
               * receivingWeight;

          storedContributions[i] += weightedContribution;
     }
}

/**
 * @brief Change incoming connection strengths (learn) based on our SynapseHistory
 *
 * Applies changes in connection strengths that have been already calculated. This
 * method determines the changes in the strength of each connection based on that
 * connection's recent activity.
 *
 * The change in connection strength is directly related to Assembly fatigue and
 * parameters[LEARNING_STRENGTH], and dependent on the contributionCurve function.
 *
 * @see tallyContributions()
 */
void HebbianLearning::applyLearningToConnections() {
     // sort each Connection by its total contribution
     ContributionVector contributions;
     contributions.reserve(storedContributions.size());

     SynapseHistory::iterator stimulation;
     for (stimulation = storedContributions.begin(); stimulation != storedContributions.end(); ++stimulation) {
          ConnectionContribution c;
          c.index = stimulation->first;
          c.contribution = stimulation->second;
          contributions.push_back(c);
     }

     sort(contributions.begin(), contributions.end());

     // now, decide what happens to each Connection, using contributionCurve
     for (unsigned int i = 0; i < contributions.size(); ++i) {
          Connection::ptr thisConnection = incomingConnections->at(contributions[i].index);
          const float connectionRank = static_cast<float>(i) /contributions.size();
          const float thisLTCS = thisConnection->getLTCS();
          const float connectionChange = contributionCurve(connectionRank, thisLTCS);

          const float scaledConnectionChange = connectionChange * parameters[LEARNING_STRENGTH]
               * postSynapticState->fatigue;
          thisConnection->setLTCS(thisLTCS + scaledConnectionChange);

#ifdef DEBUG_LEARNING
          printf("rank: %f fatigue: %f contribution curve: %f\n", connectionRank,
                 postSynapticState->fatigue, connectionChange);
          printf("changed connection weight from %f to %f\n", thisLTCS, thisConnection->getLTCS());
#endif
     }
}

/**
 * @brief
 * Resets any learning that has been stored.
 */
void HebbianLearning::resetStoredLearning() {
     hasStoredLearning = false;
     storedContributions.clear();
}

/**
 * @brief
 * Implements phase-dependent learning
 *
 * @param receivingActivity Current activity of the Assembly receiving input
 * @return learning coefficient for this point in the firing cycle
 */
float HebbianLearning::receivingCurve(float receivingActivity) {
     (void) receivingActivity; // unused
     return 1.0f;
}

/**
 * @brief
 * Implements contribution-dependent learning, probably much more important
 * than @see receivingCurve
 *
 * @param sendingContribution activity(thisConnection) / SUM (All connections activity)
 * @return how important this sending connection is, a function of its contribution
 */
float HebbianLearning::sendingCurve(float sendingContribution) {
     (void) sendingContribution;
     return 1.0f;
}

/**
 *      @brief Changes connection strength using x^3 on [-1,1]
 *
 *      @todo Implement actual RRH here
 *
 * @param contributionRank a float on [-1,1] indicating where in the sorted array each
 *      Connection lies
 */
double HebbianLearning::contributionCurve(float contributionRank, float currentStrength) {
     // we don't use current strength yet
     (void) currentStrength;

     // rank is from 0->1, we want -1 to 1
     contributionRank -= 0.5f;

     // safety first
     if (contributionRank < -1)
          contributionRank = -1;
     else if (contributionRank > 1)
          contributionRank = 1;

     return pow(contributionRank, 3.0f);
}

/**
 * Sets the parameters to a new array of values
 *
 * @param newParams an array of length HebbianLearning::PARAMETER_COUNT
 */
void HebbianLearning::setParameters(const float newParams[]) {
     for (int i = 0; i < PARAMETER_COUNT; ++i) {
          parameters[i] = newParams[i];
     }
}

/**
 * Sets a specific parameter to the desired value. Use HebbianLearning::PARAMETER_COUNT
 * to access the parameter you want to change. Fails silently if you specify an
 * index that's out of range.
 *
 * @param index position in parameters to update
 * @param value new value to be set
 */
void HebbianLearning::setParameter(int index, float value) {
     if (index < 0 || index >= PARAMETER_COUNT)
          return;
     parameters[index] = value;
}
