//支持in-place
#include "stdafx.h"
#include "Filter.h"

bool Filter::imFilter(const MyImage_ &srcImg, const double *pTpl, const int nTplSize,
	MyImage_ &dstImg, const int nFillMode, const bool bIsGrayed)

{
	if (nTplSize % 2 != 1 || nTplSize < 3)
	{
		//AfxMessageBox(_T("滤波器模板必须大于为大于等于3的奇数！"));
		return false;
	}

	int nFtPara = nTplSize / 2; //边缘填充系数

	if (srcImg.IsNull())
		return false;

	srcImg.CopyTo(dstImg); //支持in-lace

	MyImage_ imgGrayed(srcImg);//先灰度化

	if (bIsGrayed)
	{
		CvtColor::BGR2GRAY(srcImg.data(), srcImg.GetWidth(), srcImg.GetHeight(), imgGrayed.data());
		imgGrayed.SetGrayed(true);
	}

	MyImage_ imgFilled;
	imgGrayed.BorderFillTo(imgFilled, nFtPara, MyImage_::FILL_COPY); //边缘填充

	int w = imgFilled.GetWidth();
	int h = imgFilled.GetHeight();

	MyImage_ imgTemp(w, h);

	//应用模板
	for (int i = nFtPara; i != h - nFtPara; ++i) //循环次数只有h-2
	{
		for (int j = nFtPara; j != w - nFtPara; ++j)//循环w-2次
		{
			double sum[3] = { 0,0,0 };//三个通道分别的和
									  //应用模板
			int r = 0, c = 0;
			for (int i1 = i - nFtPara; i1 <= i + nFtPara; ++i1) //行
			{
				for (int j1 = j - nFtPara; j1 <= j + nFtPara; ++j1) //列
				{
					double temp = pTpl[r * nTplSize + c];
					if (bIsGrayed)
						sum[0] += imgFilled.at(i1, j1, 0)* temp;
					else
					{
						sum[0] += imgFilled.at(i1, j1, 0)* temp;
						sum[1] += imgFilled.at(i1, j1, 1)* temp;
						sum[2] += imgFilled.at(i1, j1, 2)* temp;
					}
					c++;
				}
				c = 0;
				r++;
			}

			//取正值
			imgTemp.at(i, j, 0) = static_cast<unsigned char>(Common::saturate_cast(sum[0], 0.0, 255.0));
			if (bIsGrayed)
				imgTemp.at(i, j, 1) = imgTemp.at(i, j, 2) = imgTemp.at(i, j, 0);
			else
			{
				imgTemp.at(i, j, 1) = static_cast<unsigned char>(
					Common::saturate_cast(sum[1], 0.0, 255.0));
				imgTemp.at(i, j, 2) = static_cast<unsigned char>(
					Common::saturate_cast(sum[2], 0.0, 255.0));
			}
		}
	}

	imgTemp.RemoveFillTo(dstImg, nFtPara);

	return true;
}


bool Filter::imFilter(const MyImage_ &srcImg, const vector<double>& tpl,
	MyImage_ &dstImg, const int nFillMode, const bool bIsGrayed)

{
	return imFilter(srcImg, &tpl[0], sqrt(tpl.size()), dstImg, nFillMode, bIsGrayed);
}
