#include "stdafx.h"
#include "AddNoise.h"
#include "Common.h"

//静态变量的初始化
std::default_random_engine AddNoise::e;

void AddNoise::Salt(unsigned char * src, int width, int height, int nCnt)
{
	for (int i = 0; i != nCnt; i++)
	{
		int m = (e() % height), n = (e() % width);
		src[m*width * 3 + n * 3] = 255;
		src[m*width * 3 + n * 3 + 1] = 255;
		src[m*width * 3 + n * 3 + 2] = 255;
	}
}

void AddNoise::Pepper(unsigned char * src, int width, int height, int nCnt)
{
	for (int i = 0; i != nCnt; i++)
	{
		int m = (e() % height), n = (e() % width);
		src[m*width * 3 + n * 3] = 0;
		src[m*width * 3 + n * 3 + 1] = 0;
		src[m*width * 3 + n * 3 + 2] = 0;
	}
}

void AddNoise::Gaussian(unsigned char * src, int width, int height, double mu, double std)
{
	std::normal_distribution<double> dis(mu, std);
	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != 3; ++k)
			{
				double gsval = dis(e);
				src[i*width * 3 + j * 3 + k] = static_cast<unsigned char>
					(Common::saturate_cast<double>(src[i*width * 3 + j * 3 + k] + gsval, 0.0, 255.0));
			}
		}
	}
}

void AddNoise::Rayleigh(unsigned char * src, int width, int height, double mu, double std)
{
	std::uniform_real_distribution<double> dis(0, 1);

	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != 3; ++k)
			{
				double rlval = mu + sqrt(-std * log(1 - dis(e)));//log是e为底，log10才是10为底
				src[i*width * 3 + j * 3 + k] = static_cast<unsigned char>
					(Common::saturate_cast<double>(src[i*width * 3 + j * 3 + k] + rlval, 0.0, 255.0));
			}
		}
	}
}

void AddNoise::Exponential(unsigned char * src, int width, int height, double a)
{
	std::uniform_real_distribution<double> dis(0, 1);

	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != 3; ++k)
			{

				double exval = -1 / a*log(1 - dis(e)) * 255;
				src[i*width * 3 + j * 3 + k] = static_cast<unsigned char>
					(Common::saturate_cast<double>(src[i*width * 3 + j * 3 + k] + exval, 0.0, 255.0));
			}
		}
	}
}

void AddNoise::Uniform(unsigned char * src, int width, int height, double a, double b)
{
	std::uniform_real_distribution<double> dis(0, 1);

	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != 3; ++k)
			{


				double ufval = a + (b - a)*dis(e);
				src[i*width * 3 + j * 3 + k] = static_cast<unsigned char>
					(Common::saturate_cast<double>(src[i*width * 3 + j * 3 + k] + ufval, 0.0, 255.0));
			}
		}
	}

}

void AddNoise::Gamma(unsigned char * src, int width, int height, double a, double b)
{
	std::uniform_real_distribution<double> dis(0, 1);

	for (int i = 0; i != height; ++i)
	{
		for (int j = 0; j != width; ++j)
		{
			for (int k = 0; k != 3; ++k)
			{
				double gmval = 0.0;
				//数字图像处理(Matlab版)上找到的
				for (int i = 0; i != b; ++i)
					gmval += -1 / a*log(1 - dis(e)) * 255;
				src[i*width * 3 + j * 3 + k] = static_cast<unsigned char>
					(Common::saturate_cast<double>(src[i*width * 3 + j * 3 + k] + gmval, 0.0, 255.0));
			}
		}
	}

}
