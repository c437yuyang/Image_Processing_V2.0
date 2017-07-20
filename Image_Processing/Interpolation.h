#pragma once

typedef unsigned char BYTE;

struct TRGB32      //32 bit color
{
	BYTE  b, g, r;          //a is alpha
};

class Interpolate {

public:
	//http://blog.csdn.net/housisong/article/details/1452249
	//���ξ����ֵ�����㷨��һ���ο�ʵ�֣�PicZoom_ThreeOrder0
	//	�ú�����û����������Ż���ֻ��һ���򵥵ĸ���ʵ�ְ汾;
	static void PicZoom_ThreeOrder0(const BYTE * src, BYTE * dst, int w1, int h1,int w2,int h2);
	

private:
	static TRGB32 ThreeOrder0(const BYTE* src, const double fx, const double fy,int w, int h);
	static TRGB32 Pixels_Bound(const BYTE* src, long x, long y, int w, int h);
	static BYTE Interpolate::border_color(long Color);
	static inline double SinXDivX(double x);

};