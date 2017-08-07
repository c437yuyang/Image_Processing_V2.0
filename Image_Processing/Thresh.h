#pragma once

#include <vector>
using std::vector;
#include <map>
using std::map;
#include "Common.h"
class Threshold 
{

private:


public:
	//为了跟框架配合，假设传入的是三通道的已经灰度化的图，都支持in-place
	static void binThresh(const unsigned char *src, const int w, const int h, unsigned char *dst, const int thresh);
	static int getGlobalBasicThresh(const unsigned char *src, const int w, const int h);
	static int getOTSUThresh(const unsigned char *src, const int w, const int h);
	static void globalBasic(const unsigned char *src, const int w, const int h, unsigned char *dst);
	static void OTSU(const unsigned char *src, const int w, const int h, unsigned char *dst);
};