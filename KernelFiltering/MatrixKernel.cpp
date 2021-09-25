#include "MatrixKernel.h"

MatrixKernel::MatrixKernel(uint32_t height, uint32_t width, uint32_t depth) : Kernel(height, width)
{
	this->depth = depth;
	this->kernel_matrix = nullptr;
}

uint32_t MatrixKernel::get_depth()
{
	return this->depth;
}
