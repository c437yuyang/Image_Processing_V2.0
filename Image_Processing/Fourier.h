#pragma once
#include "Common.h" //pi等常量
#include <vector>
using std::vector;
#include <complex>
using std::complex;

class Fourier //提供傅里叶变换及其相关处理
{

private:
	//1维fft
	static void FFT(const complex<double> * TD, complex<double> * FD, const int index);
	//1维逆fft
	static void IFFT(const complex<double> * TD, complex<double> * FD, const int index);

	//二维FFT，dst里面存变换后数据，不是真正返回，所以用作中间函数，为private
	static void FFT2(const unsigned char* src, const int w, const int h, complex<double>* pFD);

	//二维IFFT，dst里面存返变换后数据
	static void IFFT2(const complex<double> * TD, const int w_extend, const int h_extend, complex<double>* pTD);
public:
	static int calExLen(const int l) { return static_cast<int>(pow(2, ceil(log2(l)))); };

	//2维图像的fft，假设输入进的是3通道图像数据(没办法，为了配合框架，框架有点不合理)
	static void FFT2(const unsigned char* src, const int w, const int h, unsigned char *dst);

	//2维图像的ifft
	static void IFFT2(const complex<double>* pFD, const int w, const int h, unsigned char *dst);

	static void test(const unsigned char * src, unsigned char * dst);

};