#include "NetPBMImage.h"

void NetPBMImage::setResolution(Resolution resolution)
{
	this->resolution = resolution;
}

void NetPBMImage::setData(const Matrix<Pixel>& newData)
{
	data = newData;
	resolution.width = newData.getCols();
	resolution.height = newData.getRows();
}

void NetPBMImage::setMagicNumber(const std::string& magic)
{
	this->magicNumber = magic;
}

size_t NetPBMImage::getWidth() const
{
	return resolution.width;
}

size_t NetPBMImage::getHeight() const
{
	return resolution.height;
}

std::string NetPBMImage::getMagicNumber() const
{
	return magicNumber;
}

std::string NetPBMImage::getFilePath() const
{
	return filePath;
}

const Matrix<Pixel>& NetPBMImage::getData() const
{
	return data;
}

Matrix<Pixel>& NetPBMImage::getData()
{
	return data;
}

bool NetPBMImage::isColorImage() const
{
	return magicNumber == "P3";
}

bool NetPBMImage::isTrulyColor() const {
	for (size_t i = 0; i < data.getRows(); i++) {
		for (size_t j = 0; j < data.getCols(); j++) {
			const Pixel& pixel = data[i][j];
			if (pixel.red != pixel.green || pixel.green != pixel.blue) {
				return true;
			}
		}
	}
	return false;
}


bool NetPBMImage::isMonochrome() const {
	size_t max = getMaxPixelValue();

	for (size_t i = 0; i < data.getRows(); i++) {
		for (size_t j = 0; j < data.getCols(); j++) {
			const Pixel& pixel = data[i][j];

			bool isBlack = (pixel.red == 0 && pixel.green == 0 && pixel.blue == 0);
			bool isWhite = (pixel.red == max && pixel.green == max && pixel.blue == max);

			if (!isBlack && !isWhite) {
				return false;
			}
		}
	}
	return true;
}

bool NetPBMImage::isValidExtension() const
{
	for (size_t i = 0; i < CONSTANTS::VALID_FILE_FORMATS.size(); i++) {
		std::string extension = getFileExtension(filePath);
		if (extension == CONSTANTS::VALID_FILE_FORMATS[i]) {
			return true;
		}
	}
	return false;
}


void NetPBMImage::skipComments(std::istream& is) const
{
	char ch;
	while (is >> std::ws && is.peek() == '#') {
		std::string comment;
		std::getline(is, comment);
	}
}

void NetPBMImage::readResolution(std::istream& is)
{
	is >> resolution.width >> resolution.height;
	if (resolution.width == 0 || resolution.height == 0) {
		throw std::runtime_error("Invalid resolution: width and height must be positive.");
	}
	data = Matrix<Pixel>(resolution.height, resolution.width);
}

void NetPBMImage::checkForJunk(std::istream& is)
{
	char junk;
	if (is >> junk) {
		throw std::runtime_error("Corrupted file, extra data found in PBM file!");
	}
}

std::string NetPBMImage::getFileExtension(const std::string& filePath)
{
	int position = filePath.find_last_of('.');
	if (position == std::string::npos || position == filePath.size() - 1) {
		throw std::runtime_error("Invalid format file");
	}
	return filePath.substr(position);
}

std::string NetPBMImage::getFileName(const std::string filePath)
{
	int position = filePath.find_last_of(".");
	if (position == std::string::npos || position == filePath.size() - 1) {
		throw std::runtime_error("Invalid format file");
	}
	return filePath.substr(0, position);
}
