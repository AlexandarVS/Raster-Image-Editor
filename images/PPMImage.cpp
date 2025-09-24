#include "PPMImage.h"
#include <iostream>
#include <fstream>
#pragma warning (disable : 4996)

PPMImage::PPMImage(const std::string& filePath)
{
    load(filePath);
}

void PPMImage::load(const std::string& filePath)
{
    this->filePath = filePath;

    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open PPM file: " + filePath);
    }

    ifs >> magicNumber;
    if (magicNumber != "P3") {
        throw std::runtime_error("Unsupported PPM format: " + magicNumber);
    }

    if (!isValidExtension()) {
        throw std::runtime_error("Invalid format");
    }


    skipComments(ifs);
    readResolution(ifs);
    ifs >> maxPixelValue;
    if (maxPixelValue > USHRT_MAX || maxPixelValue == 0) {
        throw std::runtime_error("Corupted PGM file: Invalid maximum pixel value");
    }
    readData(ifs);
    checkForJunk(ifs);
}

void PPMImage::save() const
{
    std::time_t time = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M%S", std::localtime(&time));
    std::string timeStr(buffer);

    std::string modifiedName = getFileName(filePath) + "_" + timeStr + getFileExtension(filePath);

    std::ofstream ofs(modifiedName);
    if (!ofs) {
        throw std::runtime_error("Could not open file for writing: " + modifiedName);
    }

    ofs << magicNumber << std::endl;
    ofs << resolution.width << " " << resolution.height << std::endl;
    ofs << maxPixelValue << std::endl;
    writeData(ofs);
}

void PPMImage::saveas(const std::string& filePath) const
{
    if (getFileExtension(filePath) != ".ppm") {
        throw std::runtime_error("Invalid format");
    }


    std::ofstream ofs(filePath);
    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }

    ofs << magicNumber << std::endl;
    ofs << resolution.width << " " << resolution.height << std::endl;
    ofs << maxPixelValue << std::endl;
    writeData(ofs);
}

NetPBMImage* PPMImage::clone() const
{
    return new PPMImage(*this);
}

std::string PPMImage::getFormat() const
{
    return "portable pixmap format";
}

unsigned short PPMImage::getMaxPixelValue() const
{
	return maxPixelValue;
}

void PPMImage::readData(std::istream& is)
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            unsigned short red;
            unsigned short green;
            unsigned short blue;

            is >> red >> green >> blue;

            if (is.fail() || red > maxPixelValue || blue > maxPixelValue || green > maxPixelValue) {
                throw std::runtime_error("Corupted PPM data");
            }
            data[row][col] = Pixel(red, green, blue);
        }
    }
}

void PPMImage::writeData(std::ostream& os) const
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            const Pixel& pixel = data[row][col];
            os << pixel.red << " " << pixel.green << " " << pixel.blue << " ";
        }
        os << std::endl;
    }
}
