#pragma once
#include <iostream>
#include <string>
#include "utils/Matrix.h"
#include "utils/Resolution.h"
#include "utils/Pixel.h"

namespace CONSTANTS {
	const std::vector<std::string> VALID_FILE_FORMATS = {".ppm", ".pbm", ".pgm" };
}

class NetPBMImage
{
public:
	virtual void load(const std::string& filePath) = 0;
	virtual void save() const = 0;
	virtual void saveas(const std::string& filePath) const = 0;
	virtual unsigned short getMaxPixelValue() const = 0;
	virtual NetPBMImage* clone() const = 0;
	virtual ~NetPBMImage() noexcept = default;
	virtual std::string getFormat() const = 0;

	void setResolution(Resolution resolution);
	void setData(const Matrix<Pixel>& newData);
	void setMagicNumber(const std::string& magic);

	size_t getWidth() const;
	size_t getHeight() const;

	std::string getMagicNumber() const;
	std::string getFilePath() const;

	const Matrix<Pixel>& getData() const;
	Matrix<Pixel>& getData();

	bool isColorImage() const;
	bool isTrulyColor() const;
	bool isMonochrome() const;
	bool isValidExtension() const;

	static std::string getFileExtension(const std::string& filePath);
	static std::string getFileName(const std::string filePath);

protected:
	void skipComments(std::istream& is) const;
	void readResolution(std::istream& is);
	void checkForJunk(std::istream& is);
	virtual void readData(std::istream& is) = 0;
	virtual void writeData(std::ostream& os) const = 0;

protected:
	std::string filePath;
	std::string magicNumber;
	Resolution resolution;
	Matrix<Pixel> data;
};