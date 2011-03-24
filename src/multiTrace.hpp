/**
 * Main header file, to be included in the GUI or elsewhere when you
 * want to run multiTrace code.
 *
 * @date March 7, 2011
 * @author Nathan Merritt
 */

#ifndef MULTI_TRACE_H
#define MULTI_TRACE_H

#include "Cortex.hpp"

// connection models
#include "ConnectionPattern.hpp"
#include "UNR.hpp"

// update models
#include "UpdateModel.hpp"
#include "SonntagUpdate.hpp"

// learning rules
#include "LearningRule.hpp"
#include "NoLearning.hpp"
#include "HebbianLearning.hpp"

#endif
