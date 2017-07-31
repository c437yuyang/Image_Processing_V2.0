#include "stdafx.h"
#include "Fourier.h"

void Fourier::FFT(const complex<double>* TD, complex<double>* FD, const int index)
{

	int dotcount = 0;//傅立叶变换点数
	int bfsize = 0, p = 0;//中间变量
	double angle;//角度
	complex<double> *W, *X1, *X2, *X;//定义四个复数

	dotcount = 1 << index;//傅立叶变换总点数。 index相当于log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//分配内存空间
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//计算加权系数Wn
	{
		angle = -i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);//TD拷贝到X1

	for (int k = 0; k < index; k++)//采用蝶形运算进行快速傅立叶变换，共index级
	{
		for (int j = 0; j < 1 << k; j++)//蝶形节排序，从0开始排序
		{
			bfsize = 1 << (index - k);//每个蝶形节的点数
			for (int i = 0; i < bfsize / 2; i++)//排序后蝶形节的一半，运用周期性
			{
				p = j*bfsize;//j个蝶形节的总点数。
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//按频域抽选的基-2 FFT算法，加权系数仅挑选一部分
			}
		}
		X = X1;
		X1 = X2;
		X2 = X; //X1,X2交换值位置
	}

	for (int j = 0; j < dotcount; j++)//倒序变顺序
	{
		p = 0;
		for (int i = 0; i < index; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (index - i - 1);//////////////p=p+1<<(index-i-1)?????
			}
		}
		FD[j] = X1[p];
	}

	delete W;
	delete X1;
	delete X2;

}

void Fourier::IFFT(const complex<double> * TD, complex<double> * FD, const int index)
{
	int dotcount = 0;//傅立叶变换点数
	int bfsize = 0, p = 0;//中间变量
	double angle;//角度
	complex<double> *W, *X1, *X2, *X;//定义四个复数

	dotcount = 1 << index;//傅立叶变换总点数。 index相当于log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//分配内存空间
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//计算加权系数Wn
	{
		angle = i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);////////////////////////////////要标准

	for (int k = 0; k < index; k++)//采用蝶形运算进行快速傅立叶变换，共index级
	{
		for (int j = 0; j < 1 << k; j++)//蝶形节排序，从0开始排序
		{
			bfsize = 1 << (index - k);//每个蝶形节的点数
			for (int i = 0; i < bfsize / 2; i++)//排序后蝶形节的一半，运用周期性
			{
				p = j*bfsize;//j个蝶形节的总点数。
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//按频域抽选的基-2 FFT算法，加权系数仅挑选一部分
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	for (int j = 0; j < dotcount; j++)//倒序  变  顺序
	{
		p = 0;
		for (int i = 0; i < index; i++)
		{
			if (j&(1 << i))
			{
				p += 1 << (index - i - 1);//////////////p=p+1<<(index-i-1)?????
			}
		}

		FD[j]._Val[0] = X1[p]._Val[0] / dotcount;
		FD[j]._Val[1] = X1[p]._Val[1] / dotcount;
	}
	delete W;
	delete X1;
	delete X2;
}


//将图像数据进行二维FFT变换到pFD里
void Fourier::FFT2(const unsigned char * src, const int w, const int h, complex<double>* pFD)
{
	int h_extend = calExLen(h);//图像进行扩展,寻找2的幂次方
	int w_extend = calExLen(w);
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);

	complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	for (int i = 0; i < h; ++i)//把图像的值传给pTD
	{
		for (int j = 0; j < w; ++j)
		{
			//pTD[i*w_extend + j] = src[i*w + j] * (pow(-1.0, i + j));//把频谱搬移到中心，单通道图像 
			pTD[i*w_extend + j] = src[i*w * 3 + j * 3] * (pow(-1.0, i + j));//把频谱搬移到中心 ，3通道图像
		}
	}

	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//对x方向进行快速傅立叶变换,变换到pfd进行存储
	}

	for (int i = 0; i < h_extend; i++)//把pFD进行转置存回pTD,准备再按列进行FFT
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}

	memcpy(pFD, pTD, sizeof(complex<double>)*w_extend*h_extend); //拷贝到pFD

	//到这一步  pTD里存的就是傅里叶变换的全部数据
	delete[] pTD; //删除掉中间数据

}

//将图像数据进行二维IFFT变换到pTD里
void Fourier::IFFT2(const complex<double>* pFD, const int w_extend, const int h_extend, complex<double>* pTD)
{
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);
	//开始逆向变换
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间
	complex<double>* temp = pTD;

	memcpy(pTD1, pFD, sizeof(complex<double>)*w_extend*h_extend); //先拷贝到临时内存里

	//让图像均值为0
	/*pTD1[h_extend/2*h_extend+w_extend/2] = 0.0;*/
	for (int i = 0; i < h_extend; i++) //横着也就是按行进行fft，变换到temp里
	{
		IFFT(&pTD1[w_extend*i], &temp[w_extend*i], w_index);//对x方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//转置
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = temp[w_extend*i + j];
		}
	}

	for (int i = 0; i < w_extend; i++)//竖着也就是按列进行fft
	{
		IFFT(&pTD1[h_extend*i], &temp[h_extend*i], h_index);//对y方向进行快速傅立叶变换，实质是对h方向进行快速傅立叶变换
	}

	for (int i = 0; i < h_extend; i++)//对变换结果进行转置，变回原图形
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = temp[h_extend*j + i];
		}
	}

	for (int i = 0; i < h_extend; i++)//对变换结果乘以之前的频域平移因子，不然会出现之前的相位出错
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(i + j)));
		}
	}
	memcpy(pTD, pTD1, sizeof(complex<double>)*w_extend*h_extend);

	//到此pTD1存的就全部都是时域数据
	delete[]pTD1; //一定要去掉，不然多占几十兆内存
}


//进行二维fft，并且返回可显示的图像
void Fourier::FFT2(const unsigned char * src, const int w, const int h, unsigned char * dst)
{
	int h_extend = calExLen(h);//图像进行扩展,寻找2的幂次方
	int w_extend = calExLen(w);

	complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	FFT2(src, w, h, pFD);
	//到这一步  pFD里存的就是傅里叶变换的全部数据

	//返回一个可以用于显示的图像（取变换的幅度谱）
	vector<double> dVecMag; //统计幅度信息
	for (int i = 0; i < h_extend; i++) //显示幅度谱
	{
		for (int j = 0; j < w_extend; j++)
		{
			double mag = sqrt(pFD[i*w_extend + j].real()*pFD[i*w_extend + j].real()
				+ pFD[i*w_extend + j].imag()*pFD[i*w_extend + j].imag());
			double logMag = log(1.0 + mag);
			dVecMag.push_back(logMag);
		}
	}

	Common::Norm(dVecMag, 0.0, 255.0);

	//为了保持一致，返回三通道
	for (int i = 0; i != h_extend; ++i)
		for (int j = 0; j != w_extend; ++j)
			for (int k = 0; k != 3; ++k)
				dst[i*w_extend * 3 + j * 3 + k] = static_cast<unsigned char>(dVecMag[i * w_extend + j]);

	//for (int i = 0; i != dVecMag.size(); ++i)
	//	dst[i] = static_cast<unsigned char>(dVecMag[i]); //单通道图像版本

	delete[] pFD;
}

//注意，这里返回图像的参数得要是扩展后的长宽才行!!!
void Fourier::IFFT2(const complex<double>* pFD, const int w, const int h, unsigned char * dst)
{
	int h_extend = calExLen(h);//图像进行扩展,寻找2的幂次方
	int w_extend = calExLen(w);
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);

	complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//分配内存空间


	//先进行二维逆FFT到pTD里面
	IFFT2(pFD, w_extend, h_extend, pTD);

	//取实部恢复到dst里面
	for (int i = 0; i < h_extend; ++i) //显示反变换得到的图像
	{
		for (int j = 0; j < w_extend; ++j)
		{
			unsigned char val = static_cast<unsigned char>(Common::saturate_cast(pTD[i*w_extend + j].real(), 0.0, 255.0));
			for (int k = 0; k != 3; ++k)
			{
				dst[i*w_extend * 3 + j * 3 + k] = val;
			}
		}
	}
	delete[] pTD;

}


//写一个测试方法测试逆变换对不对
void Fourier::test(const unsigned char * src,int w,int h, unsigned char * dst)
{

	int h_extend = calExLen(h);//图像进行扩展,寻找2的幂次方
	int w_extend = calExLen(w);
	complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	FFT2(src, w, h, pFD);

	IFFT2(pFD, w_extend, h_extend, dst);
	delete[] pFD;

}


