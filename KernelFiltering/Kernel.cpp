#include "Kernel.h"

Kernel::Kernel(uint32_t height, uint32_t width)
{
	if (height % 2 == 0 || width % 2 == 0)
		throw std::runtime_error("Even filter w or h");
	this->width = width;
	this->height = height;
}

uint32_t Kernel::get_width()
{
	return this->width;
}

uint32_t Kernel::get_height()
{
	return this->height;
}
