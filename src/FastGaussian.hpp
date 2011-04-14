/**
 * FastGaussian.h
 *
 *	@brief Calculates a Gaussian-weighted sum around a point in a 2d array of floats
 *
 *	This class is very fast because it performs a majority of its calculations only
 *	once per Layer timestep. By summing each row, and recording the sums as we go where
 *	R(0) = data[i][0], R(1) = R(0) + data[i][1]... We can later go back and figure out the
 *	sum of a range (i,j) by R(j) - R(i-1). This makes calculating a known pattern around
 *	a single data point very fast.
 *
 *  @date Apr 14, 2011
 *  @author Nathan Merritt
 */

#ifndef FASTGUASSIAN_H_
#define FASTGUASSIAN_H_

#include <vector>

class FastGaussian {
public:
	// Layer output block, for easy visualization by a GUI
	typedef std::vector<float> FloatVec;
	typedef std::vector<FloatVec> LayerOutput;

public:
	FastGaussian(LayerOutput *data);
	virtual ~FastGaussian();

	float getNeighborWeights(int timestep, int centerRow, int centerCol);

private:
	void updateRowSums();
	void sumRow(int row);

	float rowRange(int start, int end);

private:
	LayerOutput *data;
	int timestep;

};

#endif /* FASTGUASSIAN_H_ */
