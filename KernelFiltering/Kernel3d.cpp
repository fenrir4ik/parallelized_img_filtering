#include "Kernel3d.h"


Kernel3d::Kernel3d(uint32_t height, uint32_t width, uint32_t depth, double** values_matrix) : MatrixKernel(height, width, depth)
{
	this->depth = depth;
	try
	{
		this->kernel_matrix = new double* *[height];
		for (int i = 0; i < height; i++)
		{
			this->kernel_matrix[i] = new double* [width];
			for (int j = 0; j < width; j++)
			{
				this->kernel_matrix[i][j] = new double[depth];
				for (int k = 0; k < depth; k++)
				{
					this->kernel_matrix[i][j][k] = values_matrix[i][j];
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Constructor error occurred during parsing value_matrix to kernel_matrix:" << endl;
		cout << ex.what() << endl;
	}
}

Kernel3d::Kernel3d(uint32_t height, uint32_t width, uint32_t depth, double*** values_matrix) : MatrixKernel(height, width, depth)
{
	this->depth = depth;
	try
	{
		this->kernel_matrix = new double* *[height];
		for (int i = 0; i < height; i++)
		{
			this->kernel_matrix[i] = new double* [width];
			for (int j = 0; j < width; j++)
			{
				this->kernel_matrix[i][j] = new double[depth];
				for (int k = 0; k < depth; k++)
				{
					this->kernel_matrix[i][j][k] = values_matrix[i][j][k];
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Constructor error occurred during parsing value_matrix to kernel_matrix:" << endl;
		cout << ex.what() << endl;
	}
}

void Kernel3d::destroy_kernel_matrix()
{
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			delete this->kernel_matrix[i][j];
		}
		delete this->kernel_matrix[i];
	}
	delete this->kernel_matrix;
}

double** Kernel3d::get_kernel_matrix()
{
	double** temporary_matrix = new double* [this->height];
	for (int i = 0; i < this->height; i++)
	{
		temporary_matrix[i] = new double[this->width];
		for (int j = 0; j < this->width; j++)
		{
			temporary_matrix[i][j] = this->kernel_matrix[i][j][0];
		}
	}

	return temporary_matrix;
}

void Kernel3d::set_kernel_matrix(uint32_t height, uint32_t width, uint32_t depth, double** values_matrix)
{
	destroy_kernel_matrix();
	this->height = height;
	this->width = width;
	this->depth = depth;
	if (height % 2 == 0 || width % 2 == 0)
		throw std::runtime_error("Even filter w or h");
	try
	{
		this->kernel_matrix = new double* *[height];
		for (int i = 0; i < height; i++)
		{
			this->kernel_matrix[i] = new double* [width];
			for (int j = 0; j < width; j++)
			{
				this->kernel_matrix[i][j] = new double[depth];
				for (int k = 0; k < depth; k++)
				{
					this->kernel_matrix[i][j][k] = values_matrix[i][j];
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Set_kernel_matrix error occurred during parsing value_matrix to kernel_matrix:" << endl;
		cout << ex.what() << endl;
	}
}

void Kernel3d::set_kernel_matrix(uint32_t height, uint32_t width, uint32_t depth, double*** values_matrix)
{
	destroy_kernel_matrix();
	this->height = height;
	this->width = width;
	this->depth = depth;
	if (height % 2 == 0 || width % 2 == 0)
		throw std::runtime_error("Even filter w or h");
	try
	{
		this->kernel_matrix = new double* *[height];
		for (int i = 0; i < height; i++)
		{
			this->kernel_matrix[i] = new double* [width];
			for (int j = 0; j < width; j++)
			{
				this->kernel_matrix[i][j] = new double[depth];
				for (int k = 0; k < depth; k++)
				{
					this->kernel_matrix[i][j][k] = values_matrix[i][j][k];
				}
			}
		}
	}
	catch (const std::exception& ex)
	{
		cout << "Set_kernel_matrix error occurred during parsing value_matrix to kernel_matrix:" << endl;
		cout << ex.what() << endl;
	}
}

void Kernel3d::apply_filter(short*** input_matrix, short*** output_matrix, uint32_t corner_y, uint32_t corner_x)
{
	double dot_product_sum;
	for (int k = 0; k < this->depth; k++)
	{
		dot_product_sum = 0;
		for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width; j++)
			{
				dot_product_sum += input_matrix[i + corner_y][j + corner_x][k] * this->kernel_matrix[i][j][k];
			}
		}
		
		// RGB NORMALIZATION 

		if (dot_product_sum > 255)
		{
			output_matrix[corner_y][corner_x][k] = 255;
		}
		else if (dot_product_sum < 0)
		{
			output_matrix[corner_y][corner_x][k] = 0;
		}
		else
		{
			output_matrix[corner_y][corner_x][k] = round(dot_product_sum);
		}
	}
}

Kernel3d::~Kernel3d()
{
	destroy_kernel_matrix();
}

ostream& operator<<(ostream& os, const Kernel3d& kernel)
{
	uint32_t h = kernel.height;
	uint32_t w = kernel.width;
	uint32_t d = kernel.depth;

	os << "Kernel of shape (" << h << ", " << w << ", " << d << ")\n";
	for (int k = 0; k < d; k++)
	{
		for (int i = 0; i < h; i++)
		{
			os << "  ";
			for (int j = 0; j < w; j++)
			{
				os << kernel.kernel_matrix[i][j][k] << "  ";
			}
			os << endl;
		}
		os << endl;
	}
	return os;
}