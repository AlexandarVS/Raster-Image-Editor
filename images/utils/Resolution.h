#pragma once
struct Resolution{
	size_t width;
	size_t height;

	Resolution() : width(0), height(0) {};
	Resolution(size_t width, size_t height) : width(width), height(height) {}
};