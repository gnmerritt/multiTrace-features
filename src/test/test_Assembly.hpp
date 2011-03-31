/**
 * test_Assembly.h
 *
 *	Unit tests for a single Assembly, to try and ensure that it behaves
 *	as expected.
 *
 *  @date Feb 3, 2011
 *      @author Nathan Merritt
 *
 *  @see Assembly.h
 */

#ifndef TEST_ASSEMBLY_H_
#define TEST_ASSEMBLY_H_

#include "AssemblyIncludes.hpp"

#define DEBUG_ASSEMBLY_OUTPUT

Assembly* initializeAssembly();

// Assembly-level
bool noInput();
bool singleInput();
bool multipleInputs();
bool testInhibition();

#endif /* TEST_ASSEMBLY_H_ */
