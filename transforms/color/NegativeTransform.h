#pragma once
#include "../ImageTransformer.h"

class NegativeTransform : public ImageTransformer
{
public:
    virtual ImageTransformer* clone() const override {
        return new NegativeTransform(*this);
    }
	void apply(NetPBMImage& image) const override {
        Matrix <Pixel>& data = image.getData();
        size_t rows = data.getRows();
        size_t cols = data.getCols();

        for (size_t row = 0; row < rows; row++) {
            for (size_t col = 0; col < cols; col++) {
                Pixel& pixel = data[row][col];
                pixel.red = image.getMaxPixelValue() - pixel.red;
                pixel.green = image.getMaxPixelValue() - pixel.green;
                pixel.blue = image.getMaxPixelValue() - pixel.blue;
            }
        }
	}
};

