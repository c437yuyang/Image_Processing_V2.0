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
	/// ʵ�ֹ��ܣ� ����SPATIAL ATTENTION MODEL��ͼ�������Լ��
	///    �ο����ģ� Visual Attention Detection in Video Sequences Using Spatiotemporal Cues�� Yun Zhai and Mubarak Shah.  Page 4-5��
	///    ����ʱ�䣺 2014.8.2
	/// </summary>
	/// <param name="Src">��Ҫ���м���ͼ�����ݣ�ֻ֧��24λͼ��</param>
	/// <param name="SaliencyMap">�����������ͼ��Ҳ��24λ�ġ�</param>
	/// <param name="Width">����Ĳ�ɫ���ݵĶ�Ӧ�ĻҶ����ݡ�</param>
	/// <param name="Height">����ͼ�����ݵĸ߶ȡ�</param>
	/// <param name="Stride">ͼ���ɨ���д�С��</param>
	/// <remarks> �������ػҶ�ֵ���е�ͳ�ơ�</remarks>
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

		//����Ҷȼ�ֱ��ͼ
		CurIndex = 0;
		for (Y = 0; Y < Height; Y++)
		{
			for (X = 0; X < Width; X++)
			{
				Index = Y*Width * 3 + X * 3;
				Value = (Src[Index] + Src[Index + 1] * 2 + Src[Index + 2]) / 4.0;//�����Ҷ�ֵ���Ա㲻��Ҫ�ظ�����
				HistGram[Value] ++;
				Gray[CurIndex] = Value;
				//Index += 3;
				CurIndex++;
			}
		}

		//����������ֵ
		for (Y = 0; Y < 256; Y++)
		{
			Value = 0;
			for (X = 0; X < 256; X++)
				Value += abs(Y - X) * HistGram[X];//    ���Ĺ�ʽ��9�����Ҷȵľ���ֻ�о���ֵ��������ʵ�����Ż��ٶȣ�������������û��Ҫ��
			Dist[Y] = Value;
		}

		for (Y = 0; Y < Height; Y++)
		{
			CurIndex = Y * Width;
			for (X = 0; X < Width; X++)
			{
				DistMap[CurIndex] = Dist[Gray[CurIndex]];//����ȫͼÿ�����ص�������
				CurIndex++;
			}
		}

		Normalize(DistMap, SaliencyMap, Width, Height, Stride); //��һ��ͼ������

		free(Gray);
		free(DistMap);
	}


	//void  SalientRegionDetectionBasedonAC(unsigned char *Src, unsigned char *SaliencyMap, int Width,
	//	int Height, int Stride, int R1, int MinR2, int MaxR2, int Scale)
	//{
	//	int X, Y, Z, Index, CurIndex;
	//	//Ϊʲô��ÿһ��һ��ƽ��ֵ�����������˲����Ǿ��ε���ѡ�ĸ���Χ�����˲���
	//	unsigned char *MeanR1 = (unsigned char *)malloc(Height); 
	//	unsigned char *MeanR2 = (unsigned char *)malloc(Height);
	//	unsigned char *Lab = (unsigned char *)malloc(Height);
	//	float *DistMap = (float *)malloc(Height * Width * sizeof(float));
	//
	//	//InitRGBLAB();
	//	for (Y = 0; Y < Height; Y++)
	//		RGBToLAB(Src + Y, Lab + Y, Width);   //    ע��Ҳ����Lab�ռ���е�
	//
	//	memcpy(MeanR1, Lab, Height);
	//	if (R1 > 0)            //    ���R1==0�����ʾ��ȡԭʼ����
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
	//			for (X = 0; X < Width; X++)                    //    ����ȫͼÿ�����ص�������
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
	//	Normalize(DistMap, SaliencyMap, Width, Height, Stride);        //    ��һ��ͼ������
	//
	//	free(MeanR1);
	//	free(MeanR2);
	//	free(DistMap);
	//	free(Lab);
	//	//FreeRGBLAB();
	//}


	/// <summary>
	/// ʵ�ֹ��ܣ� ����ȫ�ֶԱȶȵ�ͼ�������Լ��
	///    �ο����ģ� 2011 CVPR Global Contrast based salient region detection  Ming-Ming Cheng
	///               http://mmcheng.net/salobj/
	///    ����ʱ�䣺 2014.8.3
	/// </summary>
	/// <param name="Src">��Ҫ���м���ͼ�����ݣ�ֻ֧��24λͼ��</param>
	/// <param name="SaliencyMap">�����������ͼ��Ҳ��24λ�ġ�</param>
	/// <param name="Width">����Ĳ�ɫ���ݵĶ�Ӧ�ĻҶ����ݡ�</param>
	/// <param name="Height">����ͼ�����ݵĸ߶ȡ�</param>
	/// <param name="Stride">ͼ���ɨ���д�С��</param>
	///    <remarks> ��Lab�ռ���еĴ���ʹ�������ε�LABת�������ö���������ͼ����ɫ����������Ϊ256�֣�������ֱ��ͼ����������Բ��ұ������ø�˹ģ������������Ŀ����С�</remarks>

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
	//	Resample(Lab, Width, Height, Stride, Sample, FitWidth, FitHeight, FitWidth * 3, 0);    //    ����ڲ�ֵ
	//
	//	RGBQUAD *Palette = (RGBQUAD *)malloc(256 * sizeof(RGBQUAD));
	//
	//	GetOptimalPalette(Sample, FitWidth, FitHeight, FitWidth * 3, 256, Palette);
	//
	//	ErrorDiffusionFloydSteinberg(Lab, Mask, Width, Height, Stride, Palette, true);            //    �Ȱ�ͼ����Ϣ���������ٵķ�Χ�ڣ�����������256�ֲ�ɫ
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
	//	for (Y = 0; Y < 256; Y++)                                // ��������LC�ķ�ʽ���������Լ���
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
	//	Normalize(DistMap, SaliencyMap, Width, Height, Stride);                //    ��һ��ͼ������
	//
	//	GuassBlur(SaliencyMap, Width, Height, Stride, 1);                    //    �������ģ���������ֲ������
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
