#include "stdafx.h"
#include "Thresh.h"
BYTE getMap2rdMax1st(map<BYTE, double> &dmap);
void Threshold::binThresh(const BYTE * src, const int w, const int h, BYTE * dst, const int thresh)
{
	for (int i = 0; i != h; ++i)
	{
		for (int j = 0; j != w; ++j)
		{
			int val = 0;
			if (src[i*w * 3 + j * 3] >= thresh)
				val = 255;
			for (int k = 0; k != 3; ++k)
				dst[i*w * 3 + j * 3 + k] = val;
		}
	}
}

void Threshold::globalBasic(const BYTE * src, const int w, const int h, BYTE * dst)
{

	//先计算全图的均值
	long long sum = 0;
	for (int i = 0; i != h; ++i)
	{
		long long rowSum = 0;
		for (int j = 0; j != w; ++j)
		{
			rowSum += static_cast<long long>(src[i*w * 3 + j * 3]);
		}
		//统计完一行之后先除一下，防止数据太大溢出
		rowSum /= static_cast<long long>(w);
		sum += rowSum;
	}
	sum /= static_cast<long long>(h); //此时均值就是sum了
	BYTE mu = static_cast<BYTE>(sum);

	vector<BYTE> nVecValue1, nVecValue2;
	int nStopThresh = 1;
	int nThresh1 = mu;

	int nAvg1 = 0, nAvg2 = 0;

	for (int i = 0; i != h; ++i)
	{
		for (int j = 0; j != w; ++j)
		{
			if (src[i*w * 3 + j * 3] > nThresh1)
				nVecValue1.push_back(src[i*w * 3 + j * 3]);
			else
				nVecValue2.push_back(src[i*w * 3 + j * 3]);
		}
	}

	nAvg1 = Common::VecMean(nVecValue1);
	nAvg2 = Common::VecMean(nVecValue2);

	int nThresh2 = (nAvg1 + nAvg2) / 2;

	int nCount = 1;
	while (abs(nThresh2 - nThresh1) >= nStopThresh)
	{
		nThresh1 = nThresh2;
		nVecValue1.clear();
		nVecValue2.clear();
		for (int i = 0; i != h; ++i)
		{
			for (int j = 0; j != w; ++j)
			{
				if (src[i*w * 3 + j * 3] > nThresh1)
					nVecValue1.push_back(src[i*w * 3 + j * 3]);
				else
					nVecValue2.push_back(src[i*w * 3 + j * 3]);
			}
		}

		nAvg1 = Common::VecMean(nVecValue1);
		nAvg2 = Common::VecMean(nVecValue2);
		nThresh2 = (nAvg1 + nAvg2) / 2;
		nCount++;
	}

	binThresh(src, w, h, dst, nThresh2);

}

#pragma region 使用map的版本 deprecated
//void Threshold::otsu(const unsigned char * src, const int w, const int h, unsigned char * dst)
//{
//	int nPixTotal = w*h;
//
//	map<BYTE, double> dMapHist;
//	//统计直方图
//	for (int i = 0; i != h; ++i)
//		for (int j = 0; j != w; ++j)
//			dMapHist[src[i * w * 3 + j * 3]]++;
//
//	//得到归一化直方图
//	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
//		it->second /= nPixTotal;
//
//
//	//计算累加和
//	map<BYTE, double> dMapProb;
//	double dProbSum = 0.0;
//	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
//	{
//		dProbSum += it->second;
//		dMapProb[it->first] = dProbSum;
//	}
//
//	//计算累计均值
//	map<BYTE, int> nMapAvg;
//	double dAvgGlobal = 0.0;
//	for (auto it = dMapHist.begin(); it != dMapHist.end(); ++it)
//	{
//		dAvgGlobal += (it->first)*(it->second);
//		nMapAvg[it->first] = static_cast<int>(dAvgGlobal);
//	}
//
//	map<BYTE, double> dMapSigma;
//	double num1 = 0.0, num2 = 0.0;
//	for (auto it = dMapProb.begin(); it != dMapProb.end(); ++it)
//	{
//		if (it->second == 0)
//			continue;
//		if (it->second >= 0.999)
//			break;
//		num1 = pow(dAvgGlobal*(it->second) - nMapAvg[it->first], 2);
//		num2 = (it->second)*(1 - (it->second));
//		dMapSigma[it->first] = num1 / (num2 + eps);
//	}
//	BYTE maxK = getMap2rdMax1st(dMapSigma);
//	binThresh(src, w, h, dst, maxK);
//}
//BYTE getMap2rdMax1st(map<BYTE, double> &dmap)
//{
//	auto it = dmap.begin();
//	BYTE maxIndex = it->first;
//	double maxValue = it->second;
//	it++;
//	for (; it != dmap.end(); ++it)
//	{
//		if (it->second > maxValue)
//		{
//			maxIndex = it->first;
//			maxValue = it->second;
//		}
//	}
//	return maxIndex;
//}  
#pragma endregion


//版本2
void Threshold::otsu(const unsigned char * src, const int w, const int h, unsigned char * dst)
{
	int nPixTotal = w*h;
	double hist[256] = { 0.0 };
	//统计直方图
	for (int i = 0; i != h; ++i)
		for (int j = 0; j != w; ++j)
			++hist[src[i * w * 3 + j * 3]];

	//得到归一化直方图
	std::for_each(std::begin(hist), std::end(hist), [nPixTotal](double &val) {val /= nPixTotal; });


	//计算累加概率和和累加均值
	double prob[256] = { 0.0 };
	double mean[256] = { 0.0 };

	for (int i = 0; i != 256; ++i) 
	{
		prob[i] += i > 0 ? (prob[i - 1] + hist[i]) : hist[i];
		mean[i] = i > 0 ? (mean[i - 1] + i*hist[i]) : i*hist[i];
	}

	double sigma[256] = {0.0};
	for (int i=0;i!=256;++i)
	{
		if (abs(prob[i]) < eps) continue;
		if (prob[i] >= 0.999) break;
		double num1 = pow(mean[255] *(prob[i]) - mean[i], 2);
		double num2 = prob[i]*(1-prob[i]);
		sigma[i] = num1 / (num2 + eps);
	}

	int maxIdx = std::max_element(std::begin(sigma), std::end(sigma)) - std::begin(sigma);

	binThresh(src, w, h, dst, maxIdx);
}
