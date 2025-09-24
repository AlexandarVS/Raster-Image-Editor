#pragma once
struct Pixel {
	unsigned short red;
    unsigned short green;
    unsigned short blue;

    Pixel() : red(0), green(0), blue(0) {}
    Pixel(unsigned short value) : red(value), green(value), blue(value) {}
    Pixel(unsigned short red, unsigned short green, unsigned short blue)
        : red(red), green(green), blue(blue) {}
};