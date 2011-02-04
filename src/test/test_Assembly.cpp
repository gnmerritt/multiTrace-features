/*
 * test_Assembly.cpp
 *
 *  Created on: Feb 3, 2011
 *      Author: Nathan Merritt
 */

#include "test_Assembly.h"

// initialize all the components of an assembly
Assembly* initializeAssembly() {
	SonntagUpdate *update = new SonntagUpdate();
	HebbianLearning *learningRule = new HebbianLearning();

	Assembly *assembly = new Assembly(update, learningRule);

	return assembly;
}

bool noInput() {
	Assembly *assembly = initializeAssembly();
	int i;

	for (i = 0; i < 20; ++i) {
		float out = assembly->getOutput();
		//printf("  Assembly output at timestep %d: %f\n", i, out);
		assembly->tick(0);

		if (out != (float) 0) {
			return false;
		}
	}

	return true;
}

bool singleInput() {
	Assembly *assembly = initializeAssembly();
	int i;

	// awkward, I know...
	ConnectionVector input;
	Connection *c = new Connection();
	input.push_back(c);

	assembly->setIncomingConnections(&input);
	c->setActivity((float) 99);

	float assembly_last;
	float assembly_max = 0;

	for (i = 0; i < 500; ++i) {
		float out = assembly->getOutput();
		printf("  Assembly output at timestep %d: %f\n", i, out);

		assembly->tick(0);

		assembly_last = out;

		if (out > assembly_max) {
			assembly_max = out;
		}

		c->setActivity(0); // shut the activity off after 1 tick
	}

	if (assembly_max > 0.6 && assembly_last < 0.2) {
		return true;
	}

	return false;
}

// run the tests!
int main() {
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

	return 0;
}
