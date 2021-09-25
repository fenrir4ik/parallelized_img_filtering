#pragma once
#include "MatrixKernel.h"
#include <iostream>
using namespace std;


class Kernel3d : public MatrixKernel
{
public:
	/*
		Kernel of shape height x width x depth
		Filled by values from values_matrix
	*/
	Kernel3d(uint32_t height, uint32_t width, uint32_t depth, double** values_matrix);

	Kernel3d(uint32_t height, uint32_t width, uint32_t depth, double*** values_matrix);

	void destroy_kernel_matrix() override;

	double** get_kernel_matrix() override;

	void set_kernel_matrix(uint32_t height, uint32_t width, uint32_t depth, double** values_matrix);

	void set_kernel_matrix(uint32_t height, uint32_t width, uint32_t depth, double*** values_matrix);

	void apply_filter(short*** input_matrix, short*** output_matrix, uint32_t corner_y, uint32_t corner_x) override;

	~Kernel3d();

	friend ostream& operator << (ostream& os, const Kernel3d& kernel);
};