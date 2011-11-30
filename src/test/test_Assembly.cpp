
/*
 * test_Assembly.cpp
 *
 *  @date Feb 3, 2011
 *      @author Nathan Merritt
 */

#include "test_Assembly.hpp"

const float FIRING_THRESH = 0.7f; /** the level at which an Assembly is considered fully active */

static int assemblyCounter = 0;

/** Initialize all the components of an assembly, so we can test it
 *
 * @return a pointer to the configured Assembly
 */
Assembly* initializeAssembly() {
    UpdateModel::ptr update ( new SonntagUpdate() );

    assemblyCounter++;

    Assembly *assembly = new Assembly(assemblyCounter, update, LearningRules::NO_LEARNING);

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
void noInput() {
    Assembly *a = initializeAssembly();
    int i;

    

    for (i = 0; i < 50; ++i) {
	float out = a->getOutput();
	a->tick(0);

	EQ_FLOAT(out, 0);
    }

    PASSED(NO_INPUT_FLAT)
}

/**
 * A single input test case for our Assembly
 *
 * A single connection is initialized (awkwardly) and it's activity is
 * set to maximum. The Assembly is then allowed to run for 1500 ticks, and its
 * maximum output is tracked.
 *
 * @return true if the Assembly becomes highly activated (fires) after this input
 */
void singleInput() {
    Assembly *a = initializeAssembly();
    int i;

    // awkward, I know...
    Connection::vector input;
    Connection::ptr c (new Connection());
    input.push_back(c);

    float fakeActivity = 99.0f;
    float fakeSTCS = 0.0f;

    a->setIncomingConnections(input);
    c->setLTCS(1.0f);
    c->setSTCS(&fakeSTCS);
    c->setActivity(&fakeActivity);

    float assembly_last;
    float assembly_max = 0;

    for (i = 0; i < 1500; ++i) {
	float out = a->getOutput();
	a->tick(0);
	assembly_last = out;

	if (out > assembly_max) {
	    assembly_max = out;
	}

	// shut the activity off after 10 tick
	if (i > 10) {
	    fakeActivity = 0.0f;
	}
    }

    GT(assembly_max, FIRING_THRESH);
    LT(assembly_last, 0.2f);

    PASSED(SINGLE_INPUT_FIRE)
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
void multipleInputs() {
    Assembly *a = initializeAssembly();
    int i;

    const int SECOND_INPUT = 250;

    // awkward, I know...
    Connection::vector input;
    Connection::ptr c (new Connection());
    input.push_back(c);

    float fakeActivity = 99.0f;
    float fakeSTCS = 0.5f;

    a->setIncomingConnections(input);
    c->setActivity(&fakeActivity);
    c->setSTCS(&fakeSTCS);
    c->setLTCS(0.5f);

    float assembly_last;
    float assembly_max = 0;

    for (i = 0; i < 1500; ++i) {
	float out = a->getActivation();

	a->tick(0);
	assembly_last = out;

	if (out > assembly_max) {
	    assembly_max = out;
	}

	if (i > SECOND_INPUT && i < (SECOND_INPUT + 10)) {
	    fakeActivity = 99.0f;
	} else {
	    fakeActivity = 0.0f; // shut the activity off after 1 tick
	}
    }

    GT(assembly_max, FIRING_THRESH);
    LT(assembly_last, 0.2f);

    PASSED(FIRE_TWICE)
}

/**
 * Tests whether maximum regional inhibition can prevent an Assembly
 * from fully firing. Input is left on at maximum, as in regional inhibiton.
 *
 * @return true if the Assembly does not fully fire (some increase in Activity is okay)
 */
void testInhibition() {
    Assembly *a = initializeAssembly();
    int i;

    Connection::ptr c ( new Connection() );

    a->addIncomingConnection(c);

    float fakeActivity = 99.0f;
    c->setActivity(&fakeActivity);

    float assembly_max = 0;

    for (i = 0; i < 500; ++i) {
	float out = a->getOutput();
	a->tick(99); /** inhibition maximized here */

	if (out > assembly_max) {
	    assembly_max = out;
	}
    }

    LT(assembly_max, FIRING_THRESH);

    PASSED(INHIBITION_TEST)
}

int main() {
    noInput();
    singleInput();
    multipleInputs();
    testInhibition();
}
