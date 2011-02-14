/*
 * runTests.cpp
 *
 *  Created on: Feb 10, 2011
 *      Author: Nathan Merritt
 */

#include "test_Assembly.h"
#include "test_Layer.h"

/** Runs the unit tests, reporting results to stdout!
 *
 * @see noInput()
 * @see singleInput()
 * @see multipleInputs()
 * @see testInhibition()
 */
int main() {
	printf("Assembly-level tests:\n\n");

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

	printf("Regional inhbition test: no firing, constant max input...");
	if (testInhibition()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("\nLayer-level tests:\n\n");

	printf("1 x 1 Layer, no input...");
	if (noInputLayer1_1()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("30 x 30 Layer, no input...");
	fflush(stdout);
	if (noInputLayer30_30()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	printf("10 x 10 Layer, partial input at t=0 to t=10...");
	fflush(stdout);
	if (singleInputLayer10_10()) {
		printf("passed!\n");
	} else {
		printf("failed!\n");
	}

	return 0;
}