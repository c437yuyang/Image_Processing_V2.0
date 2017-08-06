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
	//Ϊ�˸������ϣ����贫�������ͨ�����Ѿ��ҶȻ���ͼ����֧��in-place
	static void binThresh(const unsigned char *src, const int w, const int h, unsigned char *dst, const int thresh);
	static void globalBasic(const unsigned char *src, const int w, const int h, unsigned char *dst);
	static void otsu(const unsigned char *src, const int w, const int h, unsigned char *dst);
};