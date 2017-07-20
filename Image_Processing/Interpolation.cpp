#include "stdafx.h"
#include "Interpolation.h"

inline double Interpolate::SinXDivX(double x)
{
	//该函数计算插值曲线sin(x*PI)/(x*PI)的值 //PI=3.1415926535897932385; 
	//下面是它的近似拟合表达式
	const double a = -1; //a还可以取 a=-2,-1,-0.75,-0.5等等，起到调节锐化或模糊程度的作用

	if (x < 0) x = -x; //x=abs(x);
	double x2 = x*x;
	double x3 = x2*x;
	if (x <= 1)
		return (a + 2)*x3 - (a + 3)*x2 + 1;
	else if (x <= 2)
		return a*x3 - (5 * a)*x2 + (8 * a)*x - (4 * a);
	else
		return 0;
}

TRGB32 Interpolate::Pixels_Bound(const BYTE* src, long x, long y, int w, int h)
{
	//assert((pic.width>0)&&(pic.height>0));
	bool IsInPic = true;
	if (x < 0)
	{
		x = 0; IsInPic = false;
	}
	else if (x >= w)
	{
		x = w - 1;
		IsInPic = false;
	}
	if (y < 0)
	{
		y = 0; IsInPic = false;
	}
	else if (y >= h)
	{
		y = h - 1;
		IsInPic = false;
	}

	TRGB32 result;
	result.b = src[y*w * 3 + x * 3 + 0];
	result.g = src[y*w * 3 + x * 3 + 1];
	result.r = src[y*w * 3 + x * 3 + 2];
	return result;
}

inline BYTE Interpolate::border_color(long Color)
{
	if (Color <= 0)
		return 0;
	else if (Color >= 255)
		return 255;
	else
		return static_cast<BYTE>(Color);
}

TRGB32 Interpolate::ThreeOrder0(const BYTE *src, const double fx, const double fy, int w, int h)
{
	long x0 = (long)fx; if (x0 > fx) --x0; //x0=floor(fx);    
	long y0 = (long)fy; if (y0 > fy) --y0; //y0=floor(fy);
	double fu = fx - x0;
	double fv = fy - y0;

	TRGB32 pixel[16];
	long i, j;

	for (i = 0; i < 4; ++i)
	{
		for (j = 0; j < 4; ++j)
		{
			long x = x0 - 1 + j;
			long y = y0 - 1 + i;
			pixel[i * 4 + j] = Pixels_Bound(src, x, y, w, h);
		}
	}

	double afu[4], afv[4];
	//
	afu[0] = SinXDivX(1 + fu);
	afu[1] = SinXDivX(fu);
	afu[2] = SinXDivX(1 - fu);
	afu[3] = SinXDivX(2 - fu);
	afv[0] = SinXDivX(1 + fv);
	afv[1] = SinXDivX(fv);
	afv[2] = SinXDivX(1 - fv);
	afv[3] = SinXDivX(2 - fv);

	double sR = 0, sG = 0, sB = 0;
	for (i = 0; i < 4; ++i)
	{
		double aR = 0, aG = 0, aB = 0;
		for (long j = 0; j < 4; ++j)
		{
			aR += afu[j] * pixel[i * 4 + j].r;
			aG += afu[j] * pixel[i * 4 + j].g;
			aB += afu[j] * pixel[i * 4 + j].b;
		}
		sR += aR*afv[i];
		sG += aG*afv[i];
		sB += aB*afv[i];
	}
	TRGB32 result;
	result.r = border_color((long)(sR + 0.5));
	result.g = border_color((long)(sG + 0.5));
	result.b = border_color((long)(sB + 0.5));
	return result;
}

//

void Interpolate::PicZoom_ThreeOrder0(const BYTE * src, BYTE * dst, int w1, int h1, int w2, int h2)
{
	if (w1 == 0 || h1 == 0 || w2 == 0 || h2 == 0) return;

	for (long y = 0; y < h2; ++y)
	{
		double srcy = (y + 0.4999999)*h1 / h2 - 0.5;
		for (long x = 0; x < w2; ++x)
		{
			double srcx = (x + 0.4999999)*w1 / w2 - 0.5;

			TRGB32 pt = ThreeOrder0(src, srcx, srcy, w1, h1);

			dst[y * w2 * 3 + x * 3 + 0] = pt.b;
			dst[y * w2 * 3 + x * 3 + 1] = pt.g;
			dst[y * w2 * 3 + x * 3 + 2] = pt.r;
		}
	}
}