#pragma once
#include "../ImageTransformer.h"

class MonochromeTransform : public ImageTransformer {
public:

	virtual ImageTransformer* clone() const override {
		return new MonochromeTransform(*this);
	}
	void apply(NetPBMImage& image) const override {
		if (!image.isMonochrome()) {
			Matrix<Pixel>& data = image.getData();
			size_t rows = data.getRows();
			size_t cols = data.getCols();

			for (size_t row = 0; row < rows; row++) {
				for (size_t col = 0; col < cols; col++) {
					Pixel& pixel = data[row][col];
					double gray = 0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue;
					pixel = (gray > (image.getMaxPixelValue() / 2)) ? Pixel(image.getMaxPixelValue()) : Pixel(0);
				}
			}
		}
	}
};