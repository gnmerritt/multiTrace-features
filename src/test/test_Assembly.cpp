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
	NoLearning *learningRule = new NoLearning();

	Assembly *assembly = new Assembly(update, learningRule);

	return assembly;
}

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
		fprintf(singleInput_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i, a->getActivation(),
				a->getLTCS(), a->getSTCS(), a->getFatigue(),
				a->getRegionalInhibition());
#endif

		float out = a->getOutput();
		a->tick(0);
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
		fprintf(multipleInputs_f, "%d\t%f\t%f\t%f\t%f\t%f\n", i, a->getActivation(),
				a->getLTCS(), a->getSTCS(), a->getFatigue(),
				a->getRegionalInhibition());
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

	if (assembly_max > 0.6 && assembly_last < 0.2) {
		return true;
	}

	return false;
}

#ifdef DEBUG_OUTPUT
void initializeDebugFiles() {
	noInput_f = fopen("/tmp/noInput.xls", "w");
	fprintf(noInput_f, "Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");

	singleInput_f = fopen("/tmp/singleInput.xls", "w");
	fprintf(singleInput_f, "Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");

	multipleInputs_f = fopen("/tmp/multipleInputs.xls", "w");
	fprintf(multipleInputs_f, "Time\tActivity\tLTCS\tSTCS\tFatigue\tregional_activation\n");
}

void closeDebugFiles() {
	fclose(noInput_f);
	fclose(singleInput_f);
	fclose(multipleInputs_f);
}
#endif

// run the tests!
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

#ifdef DEBUG_OUTPUT
	closeDebugFiles();
#endif

	return 0;
}
