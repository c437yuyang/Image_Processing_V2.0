#include "stdafx.h"
#include "Fourier.h"

void Fourier::FFT(const complex<double>* TD, complex<double>* FD, const int index)
{

	int dotcount = 0;//����Ҷ�任����
	int bfsize = 0, p = 0;//�м����
	double angle;//�Ƕ�
	complex<double> *W, *X1, *X2, *X;//�����ĸ�����

	dotcount = 1 << index;//����Ҷ�任�ܵ����� index�൱��log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//�����ڴ�ռ�
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//�����Ȩϵ��Wn
	{
		angle = -i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);//TD������X1

	for (int k = 0; k < index; k++)//���õ���������п��ٸ���Ҷ�任����index��
	{
		for (int j = 0; j < 1 << k; j++)//���ν����򣬴�0��ʼ����
		{
			bfsize = 1 << (index - k);//ÿ�����νڵĵ���
			for (int i = 0; i < bfsize / 2; i++)//�������νڵ�һ�룬����������
			{
				p = j*bfsize;//j�����νڵ��ܵ�����
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//��Ƶ���ѡ�Ļ�-2 FFT�㷨����Ȩϵ������ѡһ����
			}
		}
		X = X1;
		X1 = X2;
		X2 = X; //X1,X2����ֵλ��
	}

	for (int j = 0; j < dotcount; j++)//�����˳��
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
	int dotcount = 0;//����Ҷ�任����
	int bfsize = 0, p = 0;//�м����
	double angle;//�Ƕ�
	complex<double> *W, *X1, *X2, *X;//�����ĸ�����

	dotcount = 1 << index;//����Ҷ�任�ܵ����� index�൱��log2N

	W = new complex<double>[sizeof(complex<double>)*dotcount / 2];//�����ڴ�ռ�
	X1 = new complex<double>[sizeof(complex<double>)*dotcount];
	X2 = new complex<double>[sizeof(complex<double>)*dotcount];

	for (int i = 0; i < dotcount / 2; i++)//�����Ȩϵ��Wn
	{
		angle = i * 2 * pi / dotcount;
		W[i] = complex<double>(cos(angle), sin(angle));
	}

	memcpy(X1, TD, sizeof(complex<double>)*dotcount);////////////////////////////////Ҫ��׼

	for (int k = 0; k < index; k++)//���õ���������п��ٸ���Ҷ�任����index��
	{
		for (int j = 0; j < 1 << k; j++)//���ν����򣬴�0��ʼ����
		{
			bfsize = 1 << (index - k);//ÿ�����νڵĵ���
			for (int i = 0; i < bfsize / 2; i++)//�������νڵ�һ�룬����������
			{
				p = j*bfsize;//j�����νڵ��ܵ�����
				X2[p + i] = X1[p + i] + X1[p + i + bfsize / 2];
				X2[p + i + bfsize / 2] = (X1[p + i] - X1[p + i + bfsize / 2])*W[i*(1 << k)];//��Ƶ���ѡ�Ļ�-2 FFT�㷨����Ȩϵ������ѡһ����
			}
		}
		X = X1;
		X1 = X2;
		X2 = X;
	}

	for (int j = 0; j < dotcount; j++)//����  ��  ˳��
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


//��ͼ�����ݽ��ж�άFFT�任��pFD��
void Fourier::FFT2(const unsigned char * src, const int w, const int h, complex<double>* pFD)
{
	int h_extend = calExLen(h);//ͼ�������չ,Ѱ��2���ݴη�
	int w_extend = calExLen(w);
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);

	complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	for (int i = 0; i < h; ++i)//��ͼ���ֵ����pTD
	{
		for (int j = 0; j < w; ++j)
		{
			//pTD[i*w_extend + j] = src[i*w + j] * (pow(-1.0, i + j));//��Ƶ�װ��Ƶ����ģ���ͨ��ͼ�� 
			pTD[i*w_extend + j] = src[i*w * 3 + j * 3] * (pow(-1.0, i + j));//��Ƶ�װ��Ƶ����� ��3ͨ��ͼ��
		}
	}

	for (int i = 0; i < h_extend; i++) //����Ҳ���ǰ��н���fft
	{
		FFT(&pTD[w_extend*i], &pFD[w_extend*i], w_index);//��x������п��ٸ���Ҷ�任,�任��pfd���д洢
	}

	for (int i = 0; i < h_extend; i++)//��pFD����ת�ô��pTD,׼���ٰ��н���FFT
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[h_extend*j + i] = pFD[w_extend*i + j];
		}
	}

	for (int i = 0; i < w_extend; i++)//����Ҳ���ǰ��н���fft
	{
		FFT(&pTD[h_extend*i], &pFD[h_extend*i], h_index);//��y������п��ٸ���Ҷ�任��ʵ���Ƕ�h������п��ٸ���Ҷ�任
	}

	for (int i = 0; i < h_extend; i++)//�Ա任�������ת�ã����ԭͼ��
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD[w_extend*i + j] = pFD[h_extend*j + i];
		}
	}

	memcpy(pFD, pTD, sizeof(complex<double>)*w_extend*h_extend); //������pFD

	//����һ��  pTD���ľ��Ǹ���Ҷ�任��ȫ������
	delete[] pTD; //ɾ�����м�����

}

//��ͼ�����ݽ��ж�άIFFT�任��pTD��
void Fourier::IFFT2(const complex<double>* pFD, const int w_extend, const int h_extend, complex<double>* pTD)
{
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);
	//��ʼ����任
	complex<double>* pTD1 = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//�����ڴ�ռ�
	complex<double>* temp = pTD;

	memcpy(pTD1, pFD, sizeof(complex<double>)*w_extend*h_extend); //�ȿ�������ʱ�ڴ���

	//��ͼ���ֵΪ0
	/*pTD1[h_extend/2*h_extend+w_extend/2] = 0.0;*/
	for (int i = 0; i < h_extend; i++) //����Ҳ���ǰ��н���fft���任��temp��
	{
		IFFT(&pTD1[w_extend*i], &temp[w_extend*i], w_index);//��x������п��ٸ���Ҷ�任
	}

	for (int i = 0; i < h_extend; i++)//ת��
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[h_extend*j + i] = temp[w_extend*i + j];
		}
	}

	for (int i = 0; i < w_extend; i++)//����Ҳ���ǰ��н���fft
	{
		IFFT(&pTD1[h_extend*i], &temp[h_extend*i], h_index);//��y������п��ٸ���Ҷ�任��ʵ���Ƕ�h������п��ٸ���Ҷ�任
	}

	for (int i = 0; i < h_extend; i++)//�Ա任�������ת�ã����ԭͼ��
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = temp[h_extend*j + i];
		}
	}

	for (int i = 0; i < h_extend; i++)//�Ա任�������֮ǰ��Ƶ��ƽ�����ӣ���Ȼ�����֮ǰ����λ����
	{
		for (int j = 0; j < w_extend; j++)
		{
			pTD1[w_extend*i + j] = pTD1[w_extend*i + j] * (pow(-1.0, -(i + j)));
		}
	}
	memcpy(pTD, pTD1, sizeof(complex<double>)*w_extend*h_extend);

	//����pTD1��ľ�ȫ������ʱ������
	delete[]pTD1; //һ��Ҫȥ������Ȼ��ռ��ʮ���ڴ�
}


//���ж�άfft�����ҷ��ؿ���ʾ��ͼ��
void Fourier::FFT2(const unsigned char * src, const int w, const int h, unsigned char * dst)
{
	int h_extend = calExLen(h);//ͼ�������չ,Ѱ��2���ݴη�
	int w_extend = calExLen(w);

	complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	FFT2(src, w, h, pFD);
	//����һ��  pFD���ľ��Ǹ���Ҷ�任��ȫ������

	//����һ������������ʾ��ͼ��ȡ�任�ķ����ף�
	vector<double> dVecMag; //ͳ�Ʒ�����Ϣ
	for (int i = 0; i < h_extend; i++) //��ʾ������
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

	//Ϊ�˱���һ�£�������ͨ��
	for (int i = 0; i != h_extend; ++i)
		for (int j = 0; j != w_extend; ++j)
			for (int k = 0; k != 3; ++k)
				dst[i*w_extend * 3 + j * 3 + k] = static_cast<unsigned char>(dVecMag[i * w_extend + j]);

	//for (int i = 0; i != dVecMag.size(); ++i)
	//	dst[i] = static_cast<unsigned char>(dVecMag[i]); //��ͨ��ͼ��汾

	delete[] pFD;
}

//ע�⣬���ﷵ��ͼ��Ĳ�����Ҫ����չ��ĳ������!!!
void Fourier::IFFT2(const complex<double>* pFD, const int w, const int h, unsigned char * dst)
{
	int h_extend = calExLen(h);//ͼ�������չ,Ѱ��2���ݴη�
	int w_extend = calExLen(w);
	int h_index = log2(h_extend);
	int w_index = log2(w_extend);

	complex<double>* pTD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();//�����ڴ�ռ�


	//�Ƚ��ж�ά��FFT��pTD����
	IFFT2(pFD, w_extend, h_extend, pTD);

	//ȡʵ���ָ���dst����
	for (int i = 0; i < h_extend; ++i) //��ʾ���任�õ���ͼ��
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


//дһ�����Է���������任�Բ���
void Fourier::test(const unsigned char * src,int w,int h, unsigned char * dst)
{

	int h_extend = calExLen(h);//ͼ�������չ,Ѱ��2���ݴη�
	int w_extend = calExLen(w);
	complex<double>* pFD = new complex<double>[sizeof(complex<double>)*w_extend*h_extend]();

	FFT2(src, w, h, pFD);

	IFFT2(pFD, w_extend, h_extend, dst);
	delete[] pFD;

}


