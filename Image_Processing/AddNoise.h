#pragma once
#include <random>
class AddNoise
{

public:
	static void Salt(unsigned char * src, int width, int height, int nCnt);
	static void Pepper(unsigned char * src, int width, int height, int nCnt);
	static void Gaussian(unsigned char * src, int width, int height, double mu,double std);
	static void Rayleigh(unsigned char * src, int width, int height, double mu, double std);
	static void Exponential(unsigned char * src, int width, int height, double a);
	static void Uniform(unsigned char * src, int width, int height, double a,double b);
	static void Gamma(unsigned char * src, int width, int height, double a, double b);

private:
	static std::default_random_engine e;

};