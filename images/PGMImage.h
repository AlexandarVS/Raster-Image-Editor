#include <iostream>
#include <string>
#include "NetPBMImage.h"

class PGMImage : public NetPBMImage {
public:
	PGMImage(const std::string& filePath);

	virtual void load(const std::string& filePath) override;
	virtual void save() const override;
	virtual void saveas(const std::string& filePath) const override;
	virtual NetPBMImage* clone() const override;
	virtual std::string getFormat() const override;
	virtual unsigned short getMaxPixelValue() const override;
private:
	virtual void readData(std::istream& is) override;
	virtual void writeData(std::ostream& os) const override;
private:
	unsigned short maxPixelValue;
};