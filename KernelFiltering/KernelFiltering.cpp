// KernelFiltering.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <opencv2/opencv.hpp> 
#include "Kernel3d.h"
#include "Image.h"
#include "Kernel.h"
#include "KernelTransformer.h"

using namespace std;
using namespace cv;

template <typename T>
void print_matrix(uint32_t h, uint32_t w, T** matrix)
{
	for (uint32_t i = 0; i < h; i++)
	{
		cout << "  ";
		for (uint32_t j = 0; j < w; j++)
		{
			cout << matrix[i][j] << "\t";
		}
		cout << endl;
	}
	cout << endl;
}

template <typename T>
void print_matrix(uint32_t h, uint32_t w, uint32_t d, T*** matrix)
{
	for (int k = 0; k < d; k++)
	{
		for (int i = 0; i < h; i++)
		{
			cout << "  ";
			for (int j = 0; j < w; j++)
			{
				cout << matrix[i][j][k] << "\t";
			}
			cout << endl;
		}
		cout << endl;
	}
}


int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "rus");

	uint32_t kernel_height = 3;
	uint32_t kernel_width = 3;
	uint32_t kernel_depth = 3;

	double** initial_kernel = new double* [kernel_height];
	

	initial_kernel[0] = new double[kernel_width] {0, -1, 0};
	initial_kernel[1] = new double[kernel_width] {-1, 5, -1};
	initial_kernel[2] = new double[kernel_width] {0, -1, 0};
	
	Kernel3d kernel(3, 3, 3, initial_kernel);
	KernelTransformer trans;
	

	string in = (argc >= 2) ? argv[1] : "";
	string Out = (argc >= 3) ? argv[2] : "";
	int number_of_iterations = (argc >= 4)? stoi(argv[3]) : 1;
	int number_of_threads = (argc >= 5) ? stoi(argv[4]) : -1;

	cout << endl;
	cout << "In: " << in << endl;
	cout << "Out: " << Out << endl;
	cout << "Number of iterations: " << number_of_iterations << endl;
	cout << "Number of threads: " << number_of_threads << endl;

	Image image(argv[1]);
	if (number_of_threads == -1)
	{
		trans.process_image(image, &kernel, number_of_iterations, true);
	}
	else
	{
		trans.process_image(image, &kernel, number_of_iterations, true, MULTITHREAD, number_of_threads);
	}
	image.save_image(argv[2]);

	return 0;
}