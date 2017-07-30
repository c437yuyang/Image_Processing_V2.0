#pragma once

#include <math.h>
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;
typedef unsigned char uchar;

class Salient 
{
	static void Normalize(float * DistMap, uchar * SaliencyMap, int Width, int Height, int Stride = 1)
	{
		auto it_max = max_element(DistMap, DistMap + Width*Height);
		auto it_min = min_element(DistMap, DistMap + Width*Height);

		float span = *it_max - *it_min;
		float min = *it_min;
		for_each(DistMap, DistMap + Width*Height,
			[&min, &span](float &f)
		{
			f = (f - min) / span * 255;
		});

		for (int i = 0; i != Width*Height; ++i)
			SaliencyMap[i] = static_cast<uchar>(DistMap[i]);

	}


public:
	/// <summary>
	/// 实现功能： 基于SPATIAL ATTENTION MODEL的图像显著性检测
	///    参考论文： Visual Attention Detection in Video Sequences Using Spatiotemporal Cues。 Yun Zhai and Mubarak Shah.  Page 4-5。
	///    整理时间： 2014.8.2
	/// </summary>
	/// <param name="Src">需要进行检测的图像数据，只支持24位图像。</param>
	/// <param name="SaliencyMap">输出的显著性图像，也是24位的。</param>
	/// <param name="Width">输入的彩色数据的对应的灰度数据。</param>
	/// <param name="Height">输入图像数据的高度。</param>
	/// <param name="Stride">图像的扫描行大小。</param>
	/// <remarks> 基于像素灰度值进行的统计。</remarks>
	static void SalientRegionDetectionBasedonLC(
		unsigned char *Src,
		unsigned char *SaliencyMap,
		int Width, int Height,
		int Stride)
	{
		int X, Y, Index, CurIndex, Value;
		int Dist[256] = { 0 };
		int HistGram[256] = { 0 };
		unsigned char *Gray = (unsigned char*)malloc(Width * Height);
		float *DistMap = (float *)malloc(Height * Width * sizeof(float));

		memset(HistGram, 0, 256 * sizeof(int));

		//计算灰度及直方图
		CurIndex = 0;
		for (Y = 0; Y < Height; Y++)
		{
			for (X = 0; X < Width; X++)
			{
				Index = Y*Width * 3 + X * 3;
				Value = (Src[Index] + Src[Index + 1] * 2 + Src[Index + 2]) / 4.0;//保留灰度值，以便不需要重复计算
				HistGram[Value] ++;
				Gray[CurIndex] = Value;
				//Index += 3;
				CurIndex++;
			}
		}

		//计算显著性值
		for (Y = 0; Y < 256; Y++)
		{
			Value = 0;
			for (X = 0; X < 256; X++)
				Value += abs(Y - X) * HistGram[X];//    论文公式（9），灰度的距离只有绝对值，这里其实可以优化速度，但计算量不大，没必要了
			Dist[Y] = Value;
		}

		for (Y = 0; Y < Height; Y++)
		{
			CurIndex = Y * Width;
			for (X = 0; X < Width; X++)
			{
				DistMap[CurIndex] = Dist[Gray[CurIndex]];//计算全图每个像素的显著性
				CurIndex++;
			}
		}

		Normalize(DistMap, SaliencyMap, Width, Height, Stride); //归一化图像数据

		free(Gray);
		free(DistMap);
	}


	//void  SalientRegionDetectionBasedonAC(unsigned char *Src, unsigned char *SaliencyMap, int Width,
	//	int Height, int Stride, int R1, int MinR2, int MaxR2, int Scale)
	//{
	//	int X, Y, Z, Index, CurIndex;
	//	//为什么是每一行一个平均值向量？方框滤波不是矩形的吗，选哪个范围进行滤波？
	//	unsigned char *MeanR1 = (unsigned char *)malloc(Height); 
	//	unsigned char *MeanR2 = (unsigned char *)malloc(Height);
	//	unsigned char *Lab = (unsigned char *)malloc(Height);
	//	float *DistMap = (float *)malloc(Height * Width * sizeof(float));
	//
	//	//InitRGBLAB();
	//	for (Y = 0; Y < Height; Y++)
	//		RGBToLAB(Src + Y, Lab + Y, Width);   //    注意也是在Lab空间进行的
	//
	//	memcpy(MeanR1, Lab, Height);
	//	if (R1 > 0)            //    如果R1==0，则表示就取原始像素
	//		BoxBlur(MeanR1, Width, Height, Stride, R1);
	//
	//	memset(DistMap, 0, Height * Width * sizeof(float));
	//
	//	for (Z = 0; Z < Scale; Z++)
	//	{
	//		memcpy(MeanR2, Lab, Height);
	//		BoxBlur(MeanR2, Width, Height, Stride, (MaxR2 - MinR2) * Z / (Scale - 1) + MinR2);
	//		for (Y = 0; Y < Height; Y++)
	//		{
	//			Index = Y;
	//			CurIndex = Y * Width;
	//			for (X = 0; X < Width; X++)                    //    计算全图每个像素的显著性
	//			{
	//				DistMap[CurIndex] += sqrt(
	//					(MeanR2[Index] - MeanR1[Index]) * (MeanR2[Index] - MeanR1[Index]) +
	//					(MeanR2[Index + 1] - MeanR1[Index + 1]) * (MeanR2[Index + 1] - MeanR1[Index + 1]) +
	//					(MeanR2[Index + 2] - MeanR1[Index + 2]) * (MeanR2[Index + 2] - MeanR1[Index + 2]) + 0.0);
	//				CurIndex++;
	//				Index += 3;
	//			}
	//		}
	//	}
	//
	//	Normalize(DistMap, SaliencyMap, Width, Height, Stride);        //    归一化图像数据
	//
	//	free(MeanR1);
	//	free(MeanR2);
	//	free(DistMap);
	//	free(Lab);
	//	//FreeRGBLAB();
	//}


	/// <summary>
	/// 实现功能： 基于全局对比度的图像显著性检测
	///    参考论文： 2011 CVPR Global Contrast based salient region detection  Ming-Ming Cheng
	///               http://mmcheng.net/salobj/
	///    整理时间： 2014.8.3
	/// </summary>
	/// <param name="Src">需要进行检测的图像数据，只支持24位图像。</param>
	/// <param name="SaliencyMap">输出的显著性图像，也是24位的。</param>
	/// <param name="Width">输入的彩色数据的对应的灰度数据。</param>
	/// <param name="Height">输入图像数据的高度。</param>
	/// <param name="Stride">图像的扫描行大小。</param>
	///    <remarks> 在Lab空间进行的处理，使用了整形的LAB转换，采用抖动技术将图像颜色总数量降低为256种，在利用直方图计算出显著性查找表，最后采用高斯模糊降低量化后的颗粒感。</remarks>

	//void __stdcall SalientRegionDetectionBasedonHC(unsigned char *Src, unsigned char *SaliencyMap, int Width, int Height, int Stride)
	//{
	//	int X, Y, XX, YY, Index, Fast, CurIndex;
	//	int FitX, FitY, FitWidth, FitHeight;
	//	float Value;
	//	unsigned char *Lab = (unsigned char *)malloc(Height * Stride);
	//	unsigned char *Mask = (unsigned char *)malloc(Height * Width);
	//	float *DistMap = (float *)malloc(Height * Width * sizeof(float));
	//	float *Dist = (float *)malloc(256 * sizeof(float));
	//	int *HistGram = (int *)malloc(256 * sizeof(int));
	//
	//	GetBestFitInfoEx(Width, Height, 256, 256, FitX, FitY, FitWidth, FitHeight);
	//	unsigned char *Sample = (unsigned char *)malloc(FitWidth * FitHeight * 3);
	//
	//	InitRGBLAB();
	//	for (Y = 0; Y < Height; Y++)
	//		RGBToLAB(Src + Y * Stride, Lab + Y * Stride, Width);
	//
	//	Resample(Lab, Width, Height, Stride, Sample, FitWidth, FitHeight, FitWidth * 3, 0);    //    最近邻插值
	//
	//	RGBQUAD *Palette = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));
	//
	//	GetOptimalPalette(Sample, FitWidth, FitHeight, FitWidth * 3, 256, Palette);
	//
	//	ErrorDiffusionFloydSteinberg(Lab, Mask, Width, Height, Stride, Palette, true);            //    先把图像信息量化到较少的范围内，这里量化到256种彩色
	//
	//	memset(HistGram, 0, 256 * sizeof(int));
	//
	//	for (Y = 0; Y < Height; Y++)
	//	{
	//		CurIndex = Y * Width;
	//		for (X = 0; X < Width; X++)
	//		{
	//			HistGram[Mask[CurIndex]] ++;
	//			CurIndex++;
	//		}
	//	}
	//
	//	for (Y = 0; Y < 256; Y++)                                // 采用类似LC的方式进行显著性计算
	//	{
	//		Value = 0;
	//		for (X = 0; X < 256; X++)
	//			Value += sqrt((Palette[Y].rgbBlue - Palette[X].rgbBlue)*(Palette[Y].rgbBlue - Palette[X].rgbBlue) + (Palette[Y].rgbGreen - Palette[X].rgbGreen)*(Palette[Y].rgbGreen - Palette[X].rgbGreen) + (Palette[Y].rgbRed - Palette[X].rgbRed)*(Palette[Y].rgbRed - Palette[X].rgbRed) + 0.0)  * HistGram[X];
	//		Dist[Y] = Value;
	//	}
	//
	//	for (Y = 0; Y < Height; Y++)
	//	{
	//		CurIndex = Y * Width;
	//		for (X = 0; X < Width; X++)
	//		{
	//			DistMap[CurIndex] = Dist[Mask[CurIndex]];
	//			CurIndex++;
	//		}
	//	}
	//
	//	Normalize(DistMap, SaliencyMap, Width, Height, Stride);                //    归一化图像数据
	//
	//	GuassBlur(SaliencyMap, Width, Height, Stride, 1);                    //    最后做个模糊以消除分层的现象
	//
	//	free(Dist);
	//	free(HistGram);
	//	free(Lab);
	//	free(Palette);
	//	free(Mask);
	//	free(DistMap);
	//	free(Sample);
	//	FreeRGBLAB();
	//}

};
