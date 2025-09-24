#pragma once
#include "../images/NetPBMImage.h";
class ImageTransformer
{
public:
	virtual void apply(NetPBMImage& image) const = 0;
	virtual ImageTransformer* clone() const = 0;
	virtual ~ImageTransformer() noexcept = default;
};