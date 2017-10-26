#include "StdAfx.h"
#include "MyImage_.h"
#include <iostream>

CImage MyImage_::s_CImage; //���徲̬����

//���𴴽�һ��CImage������ʾ���߼���
void MyImage_::CreateCImage() const
{
	if (!s_CImage.IsNull()) s_CImage.Destroy();
	int w = GetWidth();
	int h = GetHeight();
	s_CImage.Create(w, h, 24);
}

MyImage_::MyImage_(void)
{
	m_bIsGrayed = false;
	m_pBits = NULL;
	m_nHeight = 0;
	m_nWidth = 0;
}

MyImage_::MyImage_(int w, int h)
{
	m_bIsGrayed = false;
	m_pBits = NULL;
	Create(w, h, RGB(0, 0, 0));
}

MyImage_::MyImage_(int w, int h, COLORREF color)
{
	m_bIsGrayed = false;
	m_pBits = NULL;
	Create(w, h, color);
}

MyImage_::MyImage_(const MyImage_ &img1)
{
	m_pBits = NULL; //���������Ϊnull����Ȼ�ǿ������캯������ô֮ǰ����϶�û�����ɣ���û�����ݣ���˲����ڴ�й©
	img1.CopyTo(*this);
}

MyImage_::MyImage_(BYTE * src, int w, int h) //��uchar�����ʼ��һ��ͼ��
{
	m_pBits = src;
	SetWidth(w);
	SetHeight(h);
	SetGrayed(false);
	src = NULL;
}

bool MyImage_::IsGrayed() const
{
	return m_bIsGrayed;
}

void MyImage_::SetGrayed(bool flag)
{
	m_bIsGrayed = flag;
}

MyImage_::~MyImage_(void)
{
	Destroy();
}

//TODO:enum����δ���
MyImage_::LoadResult MyImage_::Load(LPCTSTR pszFileName)
{
	if (!s_CImage.IsNull()) s_CImage.Destroy();
	s_CImage.Load(pszFileName);
	if (s_CImage.IsNull()) return LOAD_FAIL;
	int w = s_CImage.GetWidth();
	int h = s_CImage.GetHeight();
	int nrow = s_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	Create(w, h, 0);//�������鲿�֣�create����������channel������Ҫ��֮������������channel
	int nChannels = s_CImage.GetBPP() / 8;
	BYTE *psrc = (BYTE *)s_CImage.GetBits();//���m_CImage���һ�е����ص�ַ


	/*��m_CImage���ƽ���ά����*/
	//˵����������1ͨ������3ͨ������4ͨ����ͼƬ���������Ϊ3ͨ����ͼ�񡣲�Ȼ������˵�ͨ����ʾ����(ȫ��)������
	//����ֻ�Ǳ�֤�ܴ���ЩͼƬ
	if (nChannels == 3)
	{
		SetGrayed(false);
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				at(j, k, 0) = psrc[j*nrow + k * 3];//B
				at(j, k, 1) = psrc[j*nrow + k * 3 + 1];//G
				at(j, k, 2) = psrc[j*nrow + k * 3 + 2];//R
			}
		}
	}
	else if (nChannels == 1)
	{
		SetGrayed(true);
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				at(j, k, 0) = psrc[j*nrow + k];//B
				at(j, k, 1) = psrc[j*nrow + k];//G
				at(j, k, 2) = psrc[j*nrow + k];//R
			}
		}
	}
	else if (nChannels == 4)
	{
		SetGrayed(false);
		for (int j = 0; j < h; j++)
		{
			for (int k = 0; k < w; k++)
			{
				at(j, k, 0) = psrc[j*nrow + k * 4];//B
				at(j, k, 1) = psrc[j*nrow + k * 4 + 1];//G
				at(j, k, 2) = psrc[j*nrow + k * 4 + 2];//R
			}
		}
	}
	/*������ά���鲢��m_CImage���ƽ���ά����*/
	return(LOAD_SUCCESS);//Ϊ����CImge���Load��������ֵ��ͬ
}

void MyImage_::Create( //������飬����CImage��Cimageֻ���������ʾ��ʱ��ͼ���ͼƬLoad��ʱ�����
	_In_ int width,
	_In_ int height,
	_In_ COLORREF color)
{
	if (!IsNull()) Destroy();
	SetWidth(width);
	SetHeight(height);
	SetGrayed(false);//����Ϊ���ǲ�ɫͼ��
	int w = GetWidth();
	int h = GetHeight();

	//�޸�Ϊʹ��1ά���飬�ṩ.at�ӿڣ�ʵ��������OpenCV��Ч��
	m_pBits = new BYTE[w * h * 3]();


	int nCb = (color & 0x00ff0000) >> 16;
	int nCg = (color & 0x0000ff00) >> 8;
	int nCr = color & 0x000000ff;

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			at(j, k, 0) = nCb;
			at(j, k, 1) = nCg;
			at(j, k, 2) = nCr;
		}
	}
}


MyImage_::DrawResult  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest,
	_In_ int nDestWidth,
	_In_ int nDestHeight) const
{
	CreateCImage();
	if (s_CImage.IsNull() || IsNull()) return DRAW_FAIL;
	int w = s_CImage.GetWidth();
	int h = s_CImage.GetHeight();
	//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С,����ȡ�õ�nrow�Ǹ�ֵ�����ܸ������getbits�����һ�������
	int nrow = s_CImage.GetPitch();

	BYTE *psrc = (BYTE *)s_CImage.GetBits();//���m_CImage���һ�е����ص�ַ

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			psrc[j*nrow + k * 3] = at(j, k, 0);
			psrc[j*nrow + k * 3 + 1] = at(j, k, 1);
			psrc[j*nrow + k * 3 + 2] = at(j, k, 2);
		}
	}
	//memcpy_s(psrc - w * (h - 1) * 3, w * h * 3, m_pBits, w * h * 3); 
	//������Ϊλͼ�Ǵ��µ��ϵ����ݣ�����ֱ����memcpyʵ�ֲ��ˣ���Ҫ�����Ƶĺ�������


	if (s_CImage.Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight))
		return DRAW_SUCCESS;
	return DRAW_FAIL;
}

MyImage_::DrawResult  MyImage_::Draw(
	_In_ HDC hDestDC,
	_In_ int xDest,
	_In_ int yDest) const
{
	return Draw(hDestDC, xDest, yDest, m_nWidth, m_nHeight);
}

bool MyImage_::IsNull() const
{
	return m_pBits == NULL;
}

void MyImage_::Destroy()
{
	if (m_pBits != NULL)
	{
		delete[]  m_pBits;
		m_pBits = NULL;
	}

}

MyImage_::SaveResult MyImage_::Save(
	_In_z_ LPCTSTR pszFileName,
	_In_ REFGUID guidFileType) const
{
	CreateCImage();
	if (IsNull() || s_CImage.IsNull()) return SAVE_FAIL;
	int w = GetWidth();
	int h = GetHeight();
	int nrow = s_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С
	BYTE *psrc = (BYTE *)s_CImage.GetBits();//���m_CImage���һ�е����ص�ַ
	//���ݸ��Ƶ�CImage����
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			psrc[j*nrow + k * 3] = at(j, k, 0);//B
			psrc[j*nrow + k * 3 + 1] = at(j, k, 1);//G
			psrc[j*nrow + k * 3 + 2] = at(j, k, 2);//R
		}
	}
	if (s_CImage.Save(pszFileName, guidFileType))
		return SAVE_FAIL;
	return SAVE_SUCCESS;
}


void MyImage_::CopyTo(MyImage_ &img1) const
{
	if (img1 == *this) return; //�����Ը�ֵ

	if (!img1.IsNull())
		img1.Destroy();

	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth());
	img1.SetHeight(this->GetHeight());


	int w = GetWidth();
	int h = GetHeight();
	img1.m_pBits = new BYTE[w*h * 3]();


	////����ͼ����������
	//for (int j = 0; j < h; j++)
	//{
	//	for (int k = 0; k < w; k++)
	//	{
	//		img1.at(j, k, 0) = at(j, k, 0);//B
	//		img1.at(j, k, 1) = at(j, k, 1);//G
	//		img1.at(j, k, 2) = at(j, k, 2);//R
	//	}
	//}
	memcpy_s(img1.m_pBits, w * h * 3, m_pBits, w * h * 3);

}

void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara) const
{
	BorderFillTo(img1, nFillPara, FILL_BLACK);
}

//���ģʽ0����255��䣬1����0��䣬2�����Ʊ߽��������
//��֧��inplace����!
void MyImage_::BorderFillTo(MyImage_ &dst, int nFillPara, FillMode fm) const
{
	//TODO:����Ӧ���ж���������ô��
	//�ȸ��²���
	if (!dst.IsNull())
		dst.Destroy();
	const int N = nFillPara;
	dst.SetGrayed(m_bIsGrayed);
	dst.SetWidth(this->GetWidth() + 2 * N);
	dst.SetHeight(this->GetHeight() + 2 * N);

	int w = dst.GetWidth();
	int h = dst.GetHeight();

	//�������ģʽ
	int nFillColor = 0;
	if (fm == FILL_WHITE)
	{
		nFillColor = 255;
	}
	else if (fm == FILL_BLACK)
	{
		nFillColor = 0;
	}
	else // ==2
	{
		nFillColor = -1;
	}

	dst.m_pBits = new BYTE[w*h * 3]();

	//ԭͼ���ݽ��и���
	for (int j = N; j < h - N; j++) //��
	{
		for (int k = N; k < w - N; k++) //��
		{
			dst.at(j, k, 0) = at(j - N, k - N, 0);//B
			dst.at(j, k, 1) = at(j - N, k - N, 1);//G
			dst.at(j, k, 2) = at(j - N, k - N, 2);//R
		}
	}

	//�Ա�Ե���д���
	//���±�Ե
	//��ʵ���²��������Ĭ��Ϊ0�Ļ�����Ϊ����������Ĭ�Ͼ���0
	for (int k = N; k != 0; --k) //��
	{
		for (int i = 0; i != w; ++i)//��
		{
			for (int j = 0; j != 3; ++j)//ͨ��
			{
				if (nFillColor == 0 || nFillColor == 255)
				{
					dst.at(N - k, i, j) = nFillColor;//�ϱ�Ե
					dst.at(h - k, i, j) = nFillColor;//�±�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					if (i >= N && i <= w - 1 - N)
					{
						dst.at(N - k, i, j) = at(0, i - N, j);//�ϱ�Ե
						dst.at(h - k, i, j) = at(h - 2 * N - 1, i - N, j);//�±�Ե
					}
					else if (i < N)
					{
						dst.at(N - k, i, j) = at(0, 0, j);//�ϱ�Ե
						dst.at(h - k, i, j) = at(h - 2 * N - 1, 0, j);//�±�Ե
					}
					else if (i > w - 1 - N)
					{
						dst.at(N - k, i, j) = at(0, w - 2 * N - 1, j);//�ϱ�Ե
						dst.at(h - k, i, j) = at(h - 2 * N - 1, w - 2 * N - 1, j);//�±�Ե
					}
				}
			}
		}
	}

	//��Ե���������������洦�����
	for (int k = 0; k != N; ++k) //��
	{
		for (int i = N; i != h - N; ++i) //��
		{
			for (int j = 0; j != 3; ++j)
			{
				if (nFillColor == 0 || nFillColor == 255)
				{
					dst.at(i, k, j) = nFillColor;//���Ե
					dst.at(i, w - k - 1, j) = nFillColor;//�ұ�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					dst.at(i, k, j) = at(i - N, 0, j);//���Ե�����Ϊ���Ƶ�һ������
					dst.at(i, w - k - 1, j) = at(i - N, w - 2 * N - 1, j);//�ױ�Ե�����Ϊ�������һ������
				}
			}
		}
	}

}

void MyImage_::RemoveFillTo(MyImage_ &dst, int nFillPara) const
{
	//�ȸ��²���
	if (!dst.IsNull())
		dst.Destroy();
	const int N = nFillPara;
	dst.SetGrayed(m_bIsGrayed);
	dst.SetWidth(this->GetWidth() - 2 * N);
	dst.SetHeight(this->GetHeight() - 2 * N);

	int w = dst.GetWidth();
	int h = dst.GetHeight();

	dst.m_pBits = new BYTE[w * h * 3]();

	//ԭͼ���ݽ��и���
	for (int j = 0; j < h; j++) //��
	{
		for (int k = 0; k < w; k++) //��
		{
			dst.at(j, k, 0) = at(j + N, k + N, 0);//B
			dst.at(j, k, 1) = at(j + N, k + N, 1);//B
			dst.at(j, k, 2) = at(j + N, k + N, 2);//B
		}
	}
}

MyImage_& MyImage_::operator=(const MyImage_ &img)
{
	//if (img == *this) { return *this; }
	//img.CopyTo(*this);
	//return *this;

	//����������EffecitveC++����11��ƣ��쳣��ȫ�ҿ��Դ����Ը�ֵ
	BYTE* pOrig = m_pBits;
	SetGrayed(img.m_bIsGrayed);
	SetWidth(img.GetWidth());
	SetHeight(img.GetHeight());
	int w = GetWidth();
	int h = GetHeight();
	m_pBits = new BYTE[w * h * 3]();

	//����ͼ����������
	//for (int j = 0; j < h; j++)
	//{
	//	for (int k = 0; k < w; k++)
	//	{
	//		at(j, k, 0) = img.at(j, k, 0);//B
	//		at(j, k, 1) = img.at(j, k, 1);//G
	//		at(j, k, 2) = img.at(j, k, 2);//R
	//	}
	//}

	memcpy_s(m_pBits, w * h * 3, img.data(), w * h * 3);
	delete[] pOrig;
	return *this;
}

bool MyImage_::operator==(const MyImage_ & rhs)
{
	return m_pBits == rhs.data() && m_bIsGrayed==rhs.IsGrayed() 
			&& m_nWidth==rhs.GetWidth() && m_nHeight==rhs.GetHeight();
}
