#pragma once
#include "../ImageTransformer.h"
class FlipLeftTransform : public ImageTransformer
{
public:
	virtual ImageTransformer* clone() const override {
		return new FlipLeftTransform(*this);
	}
	virtual void apply(NetPBMImage& image) const override {
		Matrix<Pixel>& data = image.getData();
		size_t rows = data.getRows();
		size_t cols = data.getCols();

		for (size_t row = 0; row < rows; row++) {
			for (size_t col = 0; col < cols / 2; col++) {
				std::swap(data[row][col], data[row][cols - col - 1]);
			}
		}
	}
};

