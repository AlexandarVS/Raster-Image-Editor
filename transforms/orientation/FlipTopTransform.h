#pragma once
#include "../ImageTransformer.h"

class FlipTopTransform : public ImageTransformer {
public:
	virtual ImageTransformer* clone() const override {
		return new FlipTopTransform(*this);
	}
	virtual void apply(NetPBMImage& image) const override {
		Matrix<Pixel>& data = image.getData();
		size_t rows = data.getRows();
		size_t cols = data.getCols();

		for (size_t row = 0; row < rows / 2; row++) {
			for (size_t col = 0; col < cols; col++) {
				std::swap(data[row][col], data[rows - row - 1][col]);
			}
		}
	}
};