/*
 * test_Assembly.h
 *
 *	Unit tests for a single Assembly, to try and ensure that it behaves
 *	as expected.
 *
 *  Created on: Feb 3, 2011
 *      Author: Nathan Merritt
 */

#ifndef TEST_ASSEMBLY_H_
#define TEST_ASSEMBLY_H_

#include "AssemblyIncludes.h"

#include <stdio.h>
#include <iostream>

Assembly* initializeAssembly();

bool noInput();
bool singleInput();
bool multipleInputs;

#endif /* TEST_ASSEMBLY_H_ */
