#pragma once
#include "../ImageTransformer.h"

class GrayscaleTransform : public ImageTransformer
{
public:
	virtual ImageTransformer* clone() const override {
		return new GrayscaleTransform(*this);
	}

	virtual void apply(NetPBMImage& image) const override {
		if (image.getMagicNumber() == "P3" && image.isTrulyColor()) {
			Matrix<Pixel>& data = image.getData();
			size_t rows = data.getRows();
			size_t cols = data.getCols();

			for (size_t row = 0; row < rows; row++) {
				for (size_t col = 0; col < cols; col++) {
					Pixel& pixel = data[row][col];
					unsigned short gray = (0.299 * pixel.red + 0.587 * pixel.green + 0.114 * pixel.blue);
					pixel = Pixel(gray);
				}
			}
		}
	}
};

