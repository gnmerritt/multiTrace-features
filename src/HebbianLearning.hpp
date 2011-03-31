/**
 * HebbianLearning.hpp
 *
 *  @brief implements a threshold-based fire together, wire together learning scheme
 *
 *	Changes long term connection strength for each connection according
 *	to compensatory Hebbian learning. The update model deals with modifying
 *	the incoming connections of a given Assembly, according to it's current
 *	levels of Fatigue, etc.
 *
 *	More specifically, learning is "stored" for future application as an Assembly
 *	approaches maximum activation. Any input connections that are highly active
 *	during this time are marked (fire together, wire together). Later, after the
 *	Assembly's activity has died down, input connections that greatly contributed
 *	to activating it are strengthened while other connections are weakened.
 *
 *	Explanation of the parameters:
 *	  learningStrength : directly related to how rapidly connection strengths change
 *	  recLearningStart : the point at which the receiving Assembly begins tracking input
 *	  recLearningStop : "" "" stops tracking input (pos derivative) and when we learn
 *	  		(neg deriv)
 *	  sendingThreshold : only connections with activity >= this threshold are considered
 *	  	 	as firing
 *	  receivingCurve : for varying amount of learning depending on the receiving Assembly's
 *	  		 activity
 *	  sendingCurve : similarly, but a function of the % contribution of the sending Assembly
 *
 *	  NOTE: both receivingCurve and sendingCurve are constant for now, may end up as
 *	  Assemblies of their own eventually.
 *
 *  Created on: Feb 2, 2011
 *  @author Nathan Merritt
 */

#ifndef HEBBIANLEARNING_H_
#define HEBBIANLEARNING_H_

#include "LearningRule.hpp"

class HebbianLearning: public LearningRule {
public:
	HebbianLearning(AssemblyState *state, Connection::vector *input);
	virtual ~HebbianLearning();

	void tick();

	void setParameters(const float newParams[]);
	void setParameter(int index, float value);

private:
	float receivingCurve(); // function of receiving activity
	float sendingCurve(float sendingContribution); // function of sending's % contribution

	// functions to tally future learning at each tick, apply it
	void tallyContributions();
	void applyLearningToConnections();

public:
	enum parameters {
		LEARNING_STRENGTH,
		REC_LEARNING_START,
		REC_LEARNING_STOP,
		SEND_CONTRIB_THRESHOLD,
		PARAMETER_COUNT,
	};

private:
	float parameters[PARAMETER_COUNT]; // for easy optimization via PSO

	bool storedLearning;
};

#endif /* HEBBIANLEARNING_H_ */
