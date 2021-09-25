#pragma once
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp> 

using namespace std;
using namespace cv;

class KernelTransformer;

class Image
{
private:
	uint32_t height;
	uint32_t width;
	uint32_t depth;
	short*** image;

public:
	Image(string path);

	void save_image(string path);
	
	uint32_t get_width();

	uint32_t get_height();

	uint32_t get_depth();

	//preserv unvalid hwd during assigning image->image = matrix
	void set_image(uint32_t height, uint32_t width, uint32_t depth, short*** image);

	void image_free();

	void info();

	~Image();

	friend class KernelTransformer;
};