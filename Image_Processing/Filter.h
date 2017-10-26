#pragma once

#include "MyImage_.h"  //利用里面的边缘填充函数，后期应该考虑替换成uchar *
#include "CvtColor.h"
#include "Common.h"
#include <vector>
using std::vector;

class Filter
{
public:

	static bool imFilter(const MyImage_ &srcImg, const double *pTpl, const int nTplSize,
		MyImage_ &dstImg, const int nFillMode, const bool bIsGrayed);

	static bool    imFilter(const MyImage_ &srcImg, const vector<double>& tpl,
		MyImage_ &dstImg, const int nFillMode, const bool bIsGrayed);
};

