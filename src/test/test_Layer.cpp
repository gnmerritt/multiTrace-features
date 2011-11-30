/*
 * test_Layer.cpp
 *
 *  @date Feb 10, 2011
 *      @author Nathan Merritt
 */

#include "test_Layer.hpp"

const static float LAYER_THRESHOLD_ACTIVE = 0.5f;
const static float LAYER_THRESHOLD_SILENT = 0.1f;

/**
 * Tests the corner case of a 1x1 Layer (only 1 Assembly). This test
 * ensures that none of the update or tick algorithms break with the
 * minimum number of inputs
 *
 * @returns True if a 1x1 Layer is constructed and remains silent
 */
void noInputLayer1_1() {
    Layer::ptr layer(new Layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
                               LearningRules::NO_LEARNING, 1, 1, 1, true, true)); // 1x1, layerID=1
    int i;

    for (i = 0; i < 500; ++i) {
        float avgOutput = layer->getLastRegionalActivation();

        LT(avgOutput, 0.1f);

        layer->tick();
    }

    PASSED(NO_INPUT_LAYER_1_1)
}

/**
 * Tests the creation of a large layer. This is helpful to see if any
 * of the tick() or creation algorithms break down when run on a larger
 * number of assemblies.
 *
 * @returns True if a 30x30 layer can be created and ticked
 */
void noInputLayer30_30() {
    Layer::ptr layer(new Layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
                               LearningRules::NO_LEARNING, 30, 30, 2, true, true)); // 30x30, layerID=2

    for (int i = 0; i < 500; ++i) {
        float avgOutput = layer->tick();

        LT(avgOutput, 0.1f);
    }

    PASSED(NO_INPUT_LAYER_30_30)
}

/**
 *      Our first real, activity-driven unit test. This test (using a smaller
 *      network, for speed's sake) adds new connections to every Assembly and
 *      fully activates them for the first 50 ticks. This should be enough to drive
 *      the entire layer to fully activate.
 *
 * @returns True if 20 hyper-active connections to each Assembly can drive
 * activity in the whole layer
 */
void singleInputLayer10_10() {
    Layer layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
                LearningRules::NO_LEARNING, 10, 10, 3, true, true);

    Layer::AssemblyLayer *assemblies = layer.getAssemblyLayer().first;
    Layer::AssemblyLayer::iterator row;
    Layer::AssemblyVector::iterator col;

    const int numNewConnections = 20;

    Connection::vector c;
    c.reserve(numNewConnections);

    float fakeActivity = 99.0f;
    float fakeSTCS = 1.0f;

    for (int i = 0; i < numNewConnections; ++i) {
        Connection::ptr connec(new Connection());
        connec->setActivity(&fakeActivity);
	connec->setSTCS(&fakeSTCS);
        c.push_back(connec);
    }

    // input on all the assemblies in the first row
    for (row = assemblies->begin(); row != assemblies->end(); ++row) {
        for (col = row->begin(); col != row->end(); ++col) {
	    col->setIncomingConnections(c);
        }
    }

    float max = -1;

    for (int i = 0; i < 1000; ++i) {
        if (i == 50) {
	    fakeActivity = 0.0f;
        }

        float avgOutput = layer.tick();

        if (avgOutput > max) {
            max = avgOutput;
        }

        LTE(avgOutput, 1.0f);
    }

    GTE(max, LAYER_THRESHOLD_ACTIVE);
    PASSED(ACTIVE_LAYER_10_10)
}

/**
 * Heavily activate a portion of a 10x10 layer, and insure that at least 20% of assemblies don't activate
 */
void lateralInhibition() {
    Layer layer(ConnectionPatterns::UNR_PATTERN, UpdateModels::SONNTAG_UPDATE,
                LearningRules::NO_LEARNING, 10, 10, 0, false, true);

    PASSED(LATERAL_INHIBITION)
}

int main() {
    noInputLayer1_1();
    noInputLayer30_30();
    singleInputLayer10_10();
    lateralInhibition();
}
