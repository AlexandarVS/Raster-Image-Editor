#pragma once
#include "../ImageTransformer.h"

class RotateRightTransform : public ImageTransformer
{
public:
	virtual ImageTransformer* clone() const override {
		return new RotateRightTransform(*this);
	}
	virtual void apply(NetPBMImage& image) const override {
		Matrix<Pixel>& data = image.getData();
		size_t rows = data.getRows();
		size_t cols = data.getCols();

		Matrix<Pixel> rotated(cols, rows);
		for (size_t row = 0; row < rows; row++) {
			for (size_t col = 0; col < cols; col++) {
				rotated[cols - col - 1][row] = data[row][col];
			}
		}

		image.setData(rotated);
		image.setResolution(Resolution{ rows, cols });
	}
};

