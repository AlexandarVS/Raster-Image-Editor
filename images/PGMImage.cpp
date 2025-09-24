#include "PGMImage.h"
#include <iostream>
#include <fstream>
#pragma warning (disable : 4996)

PGMImage::PGMImage(const std::string& fileName)
{
    load(fileName);
}

void PGMImage::load(const std::string& fileName)
{
    this->filePath = fileName;

    std::ifstream ifs(fileName);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open PGM file: " + fileName);
    }

    ifs >> magicNumber;
    if (magicNumber != "P2") {
        throw std::runtime_error("Unsupported PGM format: " + magicNumber);
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

void PGMImage::save() const
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

void PGMImage::saveas(const std::string& filename) const
{
    if (getFileExtension(filePath) != ".pgm") {
        throw std::runtime_error("Invalid format ");
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

NetPBMImage* PGMImage::clone() const
{
    return new PGMImage(*this);
}

std::string PGMImage::getFormat() const
{
	return "portable graymap format";
}

unsigned short PGMImage::getMaxPixelValue() const
{
	return maxPixelValue;
}

void PGMImage::readData(std::istream& is)
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            unsigned short val;
            is >> val;
            if (is.fail() || val > maxPixelValue) {
                throw std::runtime_error("Corupted PBM data");
            }
            data[row][col] = Pixel(val);
        }
    }
}

void PGMImage::writeData(std::ostream& os) const
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            const Pixel& pixel = data[row][col];
            unsigned short value = pixel.red;
            os << value << " ";
        }
        os << std::endl;
    }
}
