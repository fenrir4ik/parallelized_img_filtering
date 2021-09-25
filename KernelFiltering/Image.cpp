#include "Image.h"

Image::Image(string path)
{
	//cout << "Reading image from path: " << path << endl;
	Mat img = imread(path, IMREAD_COLOR);

	this->height = img.rows;
	this->width = img.cols;
	this->depth = 3; // RGB hardcode

	this->image = new short** [this->height];
	for (int i = 0; i < this->height; i++)
	{
		this->image[i] = new short* [this->width];
		for (int j = 0; j < this->width; j++)
		{
			this->image[i][j] = new short[this->depth];
			for (int k = 0; k < this->depth; k++)
			{
				this->image[i][j][k] = (short)img.at<Vec3b>(i, j)[k];
			}
		}
	}
	img.release();
	cout << "Image of size (" << this->height << ", " << this->width << ", " << this->depth << ") loaded" << endl;
}


void Image::save_image(string path)
{
	cout << "Saving image to file with path: " << path << endl;
	Mat exit_img = Mat(this->height, this->width, CV_8UC3); // CV_8UC3 for rgb, CV16UC1 for 1 channel
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			for (int k = 0; k < this->depth; k++)
			{
				exit_img.at<Vec3b>(i, j)[k] = this->image[i][j][k];
			}
		}
	}

	imwrite(path, exit_img);
	//cout << "Image of size (" << this->height << ", " << this->width << ", " << this->depth << ") saved" << endl;
	exit_img.release();
}

uint32_t Image::get_width()
{
	return this->width;
}

uint32_t Image::get_height()
{
	return this->height;
}

uint32_t Image::get_depth()
{
	return this->depth;
}

void Image::set_image(uint32_t height, uint32_t width, uint32_t depth, short*** image)
{
	image_free();
	this->height = height;
	this->width = width;
	this->depth = depth;
	this->image = image;
}

void Image::image_free()
{
	for (int i = 0; i < this->height; i++)
	{
		for (int j = 0; j < this->width; j++)
		{
			delete this->image[i][j];
		}
		delete this->image[i];
	}
	delete this->image;
}

void Image::info()
{
	cout << "Image size: (" << this->height << ", " << this->width << ", " << this->depth << ")" << endl;
}

Image::~Image()
{
	image_free();
}
