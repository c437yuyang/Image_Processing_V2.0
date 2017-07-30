#pragma once
#include "Common.h" //pi�ȳ���
#include <vector>
using std::vector;
#include <complex>
using std::complex;

class Fourier //�ṩ����Ҷ�任������ش���
{

private:
	//1άfft
	static void FFT(const complex<double> * TD, complex<double> * FD, const int index);
	//1ά��fft
	static void IFFT(const complex<double> * TD, complex<double> * FD, const int index);

	//��άFFT��dst�����任�����ݣ������������أ����������м亯����Ϊprivate
	static void FFT2(const unsigned char* src, const int w, const int h, complex<double>* pFD);

	//��άIFFT��dst����淵�任������
	static void IFFT2(const complex<double> * TD, const int w_extend, const int h_extend, complex<double>* pTD);
public:
	static int calExLen(const int l) { return static_cast<int>(pow(2, ceil(log2(l)))); };

	//2άͼ���fft���������������3ͨ��ͼ������(û�취��Ϊ����Ͽ�ܣ�����е㲻����)
	static void FFT2(const unsigned char* src, const int w, const int h, unsigned char *dst);

	//2άͼ���ifft
	static void IFFT2(const complex<double>* pFD, const int w, const int h, unsigned char *dst);

	static void test(const unsigned char * src, unsigned char * dst);

};