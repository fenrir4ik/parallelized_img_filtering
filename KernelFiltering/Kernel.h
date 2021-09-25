#pragma once
#include <cstdint>
#include <stdexcept>

class Kernel
{
protected:
	uint32_t width;
	uint32_t height;

public:
	Kernel(uint32_t height, uint32_t width);
	virtual void apply_filter(short*** input_matrix, short*** output_matrix, uint32_t corner_y, uint32_t corner_x) = 0;
	uint32_t get_width();
	uint32_t get_height();
};


