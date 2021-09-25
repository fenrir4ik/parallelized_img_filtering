#include "KernelTransformer.h"
#include "Kernel3d.h"
#include <chrono>
#include <thread>

enum THREADS;

KernelTransformer::KernelTransformer()
{
}

void KernelTransformer::process_image(Image &image, Kernel* kernel, int num_of_iterations, bool retain_size, THREADS thr, uint32_t threads_number)
{
	uint32_t image_h = image.height;
	uint32_t image_w = image.width;
	uint32_t image_d = image.depth;

	uint32_t kernel_h = kernel->get_height();
	uint32_t kernel_w = kernel->get_width();
	uint32_t kernel_d = 3; // RGB hardcode

	if (image_h < kernel_h || image_w < kernel_w)
		throw runtime_error("Image less then filter");

	uint32_t padding_vertical = retain_size ? kernel_h / 2 : 0;
	uint32_t padding_horizontal = retain_size ? kernel_w / 2 : 0;

	uint32_t output_h = (image_h + 2 * padding_vertical - kernel_h) + 1;
	uint32_t output_w = (image_w + 2 * padding_horizontal - kernel_w) + 1;
	uint32_t output_d = 3; // RGB hardcode
	
	short*** matrix;
	if (retain_size)
	{
		matrix = add_padding(image, padding_vertical, padding_horizontal);
	}
	else
	{
		matrix = image.image;
	}

	short*** output_matrix = new short** [output_h];
	for (int i = 0; i < output_h; i++)
	{
		output_matrix[i] = new short* [output_w];
		for (int j = 0; j < output_w; j++)
		{
			output_matrix[i][j] = new short[output_d];
		}
	}

	long long time = 0;
	for (int i = 0; i < num_of_iterations; i++)
	{
		std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

		if (thr == SINGLE)
		{
			this->filter_image(kernel, matrix, output_matrix, output_h, output_w);
		}
		else
		{
			this->filter_image_mulithread(kernel, matrix, output_matrix, output_h, output_w, threads_number);
		}

		std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		time += std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
		std::cout << i+1 << " loop time = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000000.0 << " s." << std::endl;
	}
	std::cout << "Average image processing time = " << time / num_of_iterations / 1000000.0 << " s." << std::endl;
	image.set_image(output_h, output_w, output_d, output_matrix);
}

//result is stored in buffer
void KernelTransformer::filter_image(Kernel* kernel, short*** image, short*** buffer, uint32_t h, uint32_t w, uint32_t i_start, uint32_t j_start)
{
	for (int i = i_start; i < i_start + h; i++)
	{
		for (int j = j_start; j < j_start + w; j++)
		{
			kernel->apply_filter(image, buffer, i, j);
		}
	}
}

void KernelTransformer::filter_image_mulithread(Kernel* kernel, short*** image, short*** buffer, uint32_t h, uint32_t w, uint32_t threads_number)
{
	vector<std::thread> threads;
	if (h > w)
	{
		uint32_t line_width = w / threads_number;
		uint32_t i_start = 0;
		uint32_t j_start = 0;
		uint32_t temp = 0;
		uint32_t temp_j_start = 0;
		for (int i = 0; i < threads_number; i++)
		{
			temp = line_width;
			temp_j_start = j_start;
			if (threads_number > 1)
			{
				if (i == 0)
				{
					line_width += 1;
				}
				else if (i == threads_number - 1)
				{
					line_width = w / threads_number + w % threads_number + 1;
					--j_start;
				}
				else
				{
					line_width += 2;
					--j_start;
				}
			}
			threads.emplace_back(thread(&KernelTransformer::filter_image, *this, kernel, image, buffer, h, line_width, i_start, j_start));
			line_width = temp;
			j_start = temp_j_start;
			j_start += line_width;
		}
	}
	else
	{
		uint32_t line_height = h / threads_number;
		uint32_t i_start = 0;
		uint32_t j_start = 0;
		uint32_t temp = 0;
		uint32_t temp_i_start = 0;

		for (int i = 0; i < threads_number; i++)
		{
			temp = line_height;
			temp_i_start = i_start;
			if (threads_number > 1)
			{
				if (i == 0)
				{
					line_height += 1;
				}
				else if (i == threads_number - 1)
				{
					line_height = h / threads_number + h % threads_number + 1;
					--i_start;
				}
				else
				{
					line_height += 2;
					--i_start;
				}
			}
			threads.emplace_back(thread(&KernelTransformer::filter_image, this, kernel, image, buffer, line_height, w, i_start, j_start));
			line_height = temp;
			i_start = temp_i_start;
			i_start += line_height;
		}
	}

	for (auto& th : threads)
		th.join();
}

short*** KernelTransformer::add_padding(Image &image, uint32_t padding_vertical, uint32_t padding_horizontal)
{
	uint32_t new_h = image.height + 2 * padding_vertical;
	uint32_t new_w = image.width + 2 * padding_horizontal;
	short*** matrix = new short** [new_h];

	for (int i = 0; i < new_h; i++)
	{
		matrix[i] = new short* [new_w];
		for (int j = 0; j < new_w; j++)
		{
			matrix[i][j] = new short[image.depth];
			for (int k = 0; k < image.depth; k++)
			{
				if (j >= padding_vertical && j < new_w - padding_vertical - 1
					&& i >= padding_horizontal && i < new_h - padding_horizontal - 1)
				{
					matrix[i][j][k] = image.image[i - padding_vertical][j - padding_horizontal][k];
				}
				else
				{
					matrix[i][j][k] = 0;
				}
			}
		}
	}
	//cout << "Padding added | Horizontal padding: " << padding_horizontal <<" Vertical padding: " << padding_vertical << endl;
	return matrix;
}
