/**
 * FastGaussian.cpp
 *
 *  @date Apr 14, 2011
 *  @author Nathan Merritt
 */

#include "FastGaussian.hpp"

FastGaussian::FastGaussian(LayerOutput* _data) {
	// TODO Auto-generated constructor stub

}

FastGaussian::~FastGaussian() {
	// TODO Auto-generated destructor stub
}

/**
 *
 * @param timestep timestep of the Layer, so we only have to calculate sums once
 * @param centerX X coord of the center of the sum
 * @param centerY Y coord of the center of the sum
 * @return Gaussian-weighted sum of floats in a box around centerX, centerY
 */
float FastGaussian::getNeighborWeights(int timestep, int centerX, int centerY) {
}

void FastGaussian::sumRow(int row) {
}

float FastGaussian::rowRange(int start, int end) {
}

void FastGaussian::updateRowSums() {
}

