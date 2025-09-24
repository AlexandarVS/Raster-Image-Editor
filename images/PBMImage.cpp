#include "PBMImage.h"
#include <fstream>
#pragma warning(disable : 4996)

PBMImage::PBMImage(const std::string& filePath)
{
    load(filePath);
}

void PBMImage::load(const std::string& filePath)
{
    this->filePath = filePath;
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        throw std::runtime_error("Could not open PBM file: " + filePath);
    }

    if (!isValidExtension()) {
        throw std::runtime_error("Invalid format ");
    }

    ifs >> magicNumber;
    if (magicNumber != "P1") {
        throw std::runtime_error("Unsupported PBM format: " + magicNumber);
    }

    skipComments(ifs);
    readResolution(ifs);
    readData(ifs);
    checkForJunk(ifs);
}

void PBMImage::save() const
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
    writeData(ofs);
}

void PBMImage::saveas(const std::string& filePath) const
{
    if (getFileExtension(filePath) != ".pbm") {
        throw std::runtime_error("Invalid format ");
    }

    std::ofstream ofs(filePath);
    if (!ofs.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }

    ofs << magicNumber << std::endl;
    ofs << resolution.width << " " << resolution.height << std::endl;
    writeData(ofs);
}

NetPBMImage* PBMImage::clone() const
{
    return new PBMImage(*this);
}

std::string PBMImage::getFormat() const
{
    return "portable bitmap format";
}

unsigned short PBMImage::getMaxPixelValue() const
{
    return 1;
}

void PBMImage::readData(std::istream& is)
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            unsigned short val;
            is >> val;
            if (val != 0 && val != 1) {
                throw std::runtime_error("Corupted PBM data");
            }
            data[row][col] = (val == 0) ? Pixel(0) : Pixel(1);
        }
    }
}

void PBMImage::writeData(std::ostream& os) const
{
    for (size_t row = 0; row < resolution.height; row++) {
        for (size_t col = 0; col < resolution.width; col++) {
            const Pixel& pixel = data[row][col];
            unsigned short val = pixel.red;
            os << val << " ";
        }
        os << std::endl;
    }
}