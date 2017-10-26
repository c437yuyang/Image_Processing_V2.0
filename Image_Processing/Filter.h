#pragma once

#include "MyImage_.h"  //��������ı�Ե��亯��������Ӧ�ÿ����滻��uchar *
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

