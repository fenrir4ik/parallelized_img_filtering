#pragma once
#include "Image.h"
#include "Kernel.h"
#include "Kernel3d.h"
enum THREADS { SINGLE, MULTITHREAD };

class KernelTransformer
{
public:
	KernelTransformer();

	void process_image(Image &image, Kernel* kernel, int num_of_iterations, bool retain_size = true, THREADS thr = SINGLE, uint32_t threads_number = 1);

	void filter_image(Kernel* kernel, short*** image, short*** buffer, uint32_t h, uint32_t w, uint32_t i_start = 0, uint32_t j_start = 0);

	void filter_image_mulithread(Kernel* kernel, short*** image, short*** buffer, uint32_t h, uint32_t w, uint32_t threads_number);

	short*** add_padding(Image &image, uint32_t padding_vertical, uint32_t padding_horizontal);
};