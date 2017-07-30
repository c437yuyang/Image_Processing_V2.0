#pragma once
class CvtColor {
	//typedef unsigned char BYTE;


public:
	static void BGR2GRAY(const unsigned char*src,const int w,const int h,unsigned char *dst);//Ö§³Öin-place
private:
	static void RGB2HSI(double R, double G, double B, double &H, double &S, double &I);
	static void HSI2RGB(double H, double S, double I, double &R, double &G, double &B);

};