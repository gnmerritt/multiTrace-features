/*
 * test_Assembly.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: Nathan Merritt
 */

#include "test_Assembly.h"

const float FIRING_THRESH = 0.7f; /** the level at which an Assembly is considered fully active */

/** Initialize all the components of an assembly, so we can test it
 *
 * @return a pointer to the configured Assembly
 */
Assembly* initializeAssembly() {
	SonntagUpdate *update = new SonntagUpdate();
	NoLearning *learningRule = new NoLearning();

	Assembly *assembly = new Assembly(update, learningRule);

	return assembly;
}

/**
 * A simple test case for our Assembly:
 *
 * If we don't give it any input, it's activation should not ever go
 * above 0
 *
 * @return true if activation stays at 0
 */
bool noInput() {
	Assembly *a = initializeAssembly();
	int i;

	for (i = 0; i < 20; ++i) {
#ifdef DEBUG_OUTPUT
		fprintf(noInput_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i, a->getActivation(),
				a->getLTCS(), a->getSTCS(), a->getFatigue(),
				a->getRegionalInhibition());
#endif

		float out = a->getOutput();
		a->tick(0);

		if (out != (float) 0) {
			return false;
		}
	}

	return true;
}

/**
 * A single input test case for our Assembly
 *
 * A single connection is initialized (awkwardly) and it's activity is
 * set to maximum. The Assembly is then allowed to run for 500 ticks, and its
 * maximum output is tracked.
 *
 * @return true if the Assembly becomes highly activated (fires) after this input
 */
bool singleInput() {
	Assembly *a = initializeAssembly();
	int i;

	// awkward, I know...
	ConnectionVector input;
	Connection *c = new Connection();
	input.push_back(c);

	a->setIncomingConnections(&input);
	c->setActivity((float) 99);

	float assembly_last;
	float assembly_max = 0;

	for (i = 0; i < 500; ++i) {
#ifdef DEBUG_OUTPUT
		fprintf(singleInput_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i,
				a->getActivation(), a->getLTCS(), a->getSTCS(),
				a->getFatigue(), a->getRegionalInhibition());
#endif

		float out = a->getOutput();
		a->tick(0);
		assembly_last = out;

		if (out > assembly_max) {
			assembly_max = out;
		}

		c->setActivity(0); // shut the activity off after 1 tick
	}

	if (assembly_max > FIRING_THRESH && assembly_last < 0.2) {
		return true;
	}

	return false;
}

/**
 * Similarly to singleInput(), this test checks the Assembly's ability
 * to fire again while it is in the middle of a consolidation period.
 * Extremely strong input should be able to make the assembly fire even
 * if it is highly fatigued.
 *
 * @see singleInput()
 * @return true if the Assembly fires twice (2nd time after the second input)
 */
bool multipleInputs() {
	Assembly *a = initializeAssembly();
	int i;

	const int SECOND_INPUT = 250;

	// awkward, I know...
	ConnectionVector input;
	Connection *c = new Connection();
	input.push_back(c);

	a->setIncomingConnections(&input);
	c->setActivity((float) 99);

	float assembly_last;
	float assembly_max = 0;

	for (i = 0; i < 500; ++i) {
#ifdef DEBUG_OUTPUT
		fprintf(multipleInputs_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i,
				a->getActivation(), a->getLTCS(), a->getSTCS(),
				a->getFatigue(), a->getRegionalInhibition());
#endif

		float out = a->getOutput();

		a->tick(0);
		assembly_last = out;

		if (out > assembly_max) {
			assembly_max = out;
		}

		if (i > SECOND_INPUT && i < (SECOND_INPUT + 10)) {
			c->setActivity((float) 99);
		} else {
			c->setActivity(0); // shut the activity off after 1 tick
		}
	}

	if (assembly_max > FIRING_THRESH && assembly_last < 0.2) {
		return true;
	}

	return false;
}

/**
 * Tests whether maximum regional inhibition can prevent an Assembly
 * from fully firing. Input is left on at maximum, as in regional inhibiton.
 *
 * @return true if the Assembly does not fully fire (some increase in Activity is okay)
 */
bool testInhibition() {
	Assembly *a = initializeAssembly();
	int i;

	ConnectionVector input;
	Connection *c = new Connection();
	input.push_back(c);

	a->setIncomingConnections(&input);
	c->setActivity((float) 99);

	float assembly_max = 0;

	for (i = 0; i < 500; ++i) {
#ifdef DEBUG_OUTPUT
		fprintf(testInhibition_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i,
				a->getActivation(), a->getLTCS(), a->getSTCS(),
				a->getFatigue(), a->getRegionalInhibition());
#endif

		float out = a->getOutput();
		a->tick(99); /** inhibition maximized here */

		if (out > assembly_max) {
			assembly_max = out;
		}
	}

	if (assembly_max > FIRING_THRESH) {
		return false;
	}

	return true;
}

#ifdef DEBUG_OUTPUT
void initializeDebugFiles() {
	noInput_f = fopen("/tmp/noInput.xls", "w");
	fprintf(noInput_f,
			"Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");

	singleInput_f = fopen("/tmp/singleInput.xls", "w");
	fprintf(singleInput_f,
			"Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");

	multipleInputs_f = fopen("/tmp/multipleInputs.xls", "w");
	fprintf(multipleInputs_f,
			"Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");

	testInhibition_f = fopen("/tmp/testInhibition.xls", "w");
	fprintf(testInhibition_f,
			"Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");
}

void closeDebugFiles() {
	fclose(noInput_f);
	fclose(singleInput_f);
	fclose(multipleInputs_f);
	fclose(testInhibition_f);
}
#endif

/** Runs the unit tests, reporting results to stdout!
 *
 * @see noInput()
 * @see singleInput()
 * @see multipleInputs()
 * @see testInhibition()
 */
int main() {
#ifdef DEBUG_OUTPUT
	initializeDebugFiles();
#endif

	printf("No Input baseline test...");
	if (noInput()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("Single Input fire then decay test...");
	if (singleInput()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("Double Input, fire & fire then decay test...");
	if (multipleInputs()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("Regional inhbition test: no firing after 1 input...");
	if (testInhibition()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

#ifdef DEBUG_OUTPUT
	closeDebugFiles();
#endif

	return 0;
}
