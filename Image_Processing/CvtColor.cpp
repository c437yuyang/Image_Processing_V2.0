#include "stdafx.h"
#include "CvtColor.h"


//Ö§³Öin-place
void CvtColor::BGR2GRAY(const unsigned char * src, const int w, const int h, unsigned char * dst)
{
	for (int i = 0; i != h; ++i)
	{
		for (int j = 0; j != w; ++j)
		{
			unsigned char val = static_cast<unsigned char>(
				(src[i * w * 3 + j * 3] + src[i * w * 3 + j * 3 + 1] + src[i * w * 3 + j * 3 + 2]) / 3.0);
			for (int k = 0; k != 3; ++k)
				dst[i*w * 3 + j * 3 + k] = val;
		}
	}

}

void CvtColor::RGB2HSI(double R, double G, double B,
	double & H, double & S, double & I)
{

}

void CvtColor::HSI2RGB(double H, double S, double I, double & R, double & G, double & B)
{
}
