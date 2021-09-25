#pragma once
#include "Kernel.h"
#include <cstdint>


class MatrixKernel : public Kernel
{
protected:
	uint32_t depth;
	double*** kernel_matrix;

public:
	MatrixKernel(uint32_t height, uint32_t width, uint32_t depth);

	virtual void destroy_kernel_matrix() = 0;

	virtual double** get_kernel_matrix() = 0;

	uint32_t get_depth();
};

