#pragma once
#include <fstream>
#include "../transforms/color/GrayscaleTransform.h"
#include "../transforms/color/MonochromeTransform.h"
#include "NetPBMImage.h"
class ImageFactory
{
public:
	static NetPBMImage* createImage(const std::string& filename) {
		std::ifstream ifs(filename);
		if (!ifs.is_open()) {
			throw std::runtime_error("Could not open file: " + filename);
		}

		std::string magic;
		ifs >> magic;
		ifs.close();

		if (magic == "P1") return new PBMImage(filename);
		if (magic == "P2") return new PGMImage(filename);
		if (magic == "P3") return new PPMImage(filename);

		throw std::runtime_error("Unsupported format ");
	}
};

