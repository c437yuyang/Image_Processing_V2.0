#include "StdAfx.h"
#include "MyImage_.h"
#include <iostream>

CImage MyImage_::s_CImage;

//���𴴽�һ��Cimage������ʾ���߼���
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
int MyImage_::Load(LPCTSTR pszFileName)
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
				m_pBits[0][j][k] = psrc[j*nrow + k * 3];//B
				m_pBits[1][j][k] = psrc[j*nrow + k * 3 + 1];//G
				m_pBits[2][j][k] = psrc[j*nrow + k * 3 + 2];//R
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
				m_pBits[0][j][k] = psrc[j*nrow + k];//B
				m_pBits[1][j][k] = psrc[j*nrow + k];//G
				m_pBits[2][j][k] = psrc[j*nrow + k];//R
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
				m_pBits[0][j][k] = psrc[j*nrow + k * 4];//B
				m_pBits[1][j][k] = psrc[j*nrow + k * 4 + 1];//G
				m_pBits[2][j][k] = psrc[j*nrow + k * 4 + 2];//R
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

	// ͨ�� �� ��,ch,y,x
	m_pBits = (BYTE***)new   BYTE**[3];
	for (int i = 0; i < 3; i++)
	{
		m_pBits[i] = (BYTE**)new  BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			m_pBits[i][j] = new BYTE[w];
		}
	}
	int nCb = (color & 0x00ff0000) >> 16;
	int nCg = (color & 0x0000ff00) >> 8;
	int nCr = color & 0x000000ff;

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			m_pBits[0][j][k] = nCb;//B
			m_pBits[1][j][k] = nCg;//G
			m_pBits[2][j][k] = nCr;//R
		}
	}
}


int  MyImage_::Draw(
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
	int nrow = s_CImage.GetPitch();//���m_CImageÿһ�����ص�RGB��ռ�õĴ洢�ռ�Ĵ�С


	BYTE *psrc = (BYTE *)s_CImage.GetBits();//���m_CImage���һ�е����ص�ַ

	/*����ά���鸴�ƽ�m_CImage������ʾ*/

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			psrc[j*nrow + k * 3] = m_pBits[0][j][k];//B
			psrc[j*nrow + k * 3 + 1] = m_pBits[1][j][k];//G
			psrc[j*nrow + k * 3 + 2] = m_pBits[2][j][k];//R
		}
	}

	/*����ά���鸴�ƽ�m_CImage*/

	if (s_CImage.Draw(hDestDC, xDest, yDest, nDestWidth, nDestHeight))
		return DRAW_SUCCESS;
	return DRAW_FAIL;
}

int  MyImage_::Draw(
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
		int h = GetHeight();
		//for   (int   i=0;   i<2;   i++)   //comment by yxp 2016_12_11
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < h; j++)
			{

				delete[] m_pBits[i][j];
				m_pBits[i][j] = NULL;
			}
		}
		for (int i = 0; i < 3; i++)
		{
			delete[] m_pBits[i];
			m_pBits[i] = NULL;
		}
		delete[]  m_pBits;
		m_pBits = NULL;
	}

}

int MyImage_::Save(
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
			psrc[j*nrow + k * 3] = m_pBits[0][j][k];//B
			psrc[j*nrow + k * 3 + 1] = m_pBits[1][j][k];//G
			psrc[j*nrow + k * 3 + 2] = m_pBits[2][j][k];//R
		}
	}
	if (s_CImage.Save(pszFileName, guidFileType))
		return SAVE_FAIL;
	return SAVE_SUCCESS;
}


void MyImage_::CopyTo(MyImage_ &img1) const
{
	if (!img1.IsNull())
		img1.Destroy();

	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth());
	img1.SetHeight(this->GetHeight());


	int w = GetWidth();
	int h = GetHeight();

	/*������ά���鲢��m_CImage���ƽ���ά����*/

	// ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];
	for (int i = 0; i < 3; i++)
	{
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			img1.m_pBits[i][j] = new BYTE[w];
		}
	}
	//����ͼ����������
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			img1.m_pBits[0][j][k] = m_pBits[0][j][k];//B
			img1.m_pBits[1][j][k] = m_pBits[1][j][k];//G
			img1.m_pBits[2][j][k] = m_pBits[2][j][k];//R
		}
	}

}

void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara) const
{
	BorderFillTo(img1, nFillPara, FILL_BLACK);
}

//���ģʽ0����255��䣬1����0��䣬2�����Ʊ߽��������
void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara, int FillMode) const
{
	//�ȸ��²���
	if (!img1.IsNull())
		img1.Destroy();
	const int N = nFillPara;
	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth() + 2 * N);
	img1.SetHeight(this->GetHeight() + 2 * N);


	int w = img1.GetWidth();
	int h = img1.GetHeight();

	//�������ģʽ
	int nFillColor = 0;
	if (FillMode == FILL_WHITE)
	{
		nFillColor = 255;
	}
	else if (FillMode == FILL_BLACK)
	{
		nFillColor = 0;
	}
	else // ==2
	{
		//std::cout << FillMode <<std::endl;
		nFillColor = -1;
	}

	//�������� ͨ�� �� ��
	img1.m_pBits = (BYTE***)new   BYTE**[3];
	for (int i = 0; i < 3; i++)
	{
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			img1.m_pBits[i][j] = new BYTE[w];
		}
	}


	//ԭͼ���ݽ��и���
	for (int j = N; j < h - N; j++) //��
	{
		for (int k = N; k < w - N; k++) //��
		{
			img1[0][j][k] = m_pBits[0][j - N][k - N];//B
			img1[1][j][k] = m_pBits[1][j - N][k - N];//G
			img1[2][j][k] = m_pBits[2][j - N][k - N];//R
		}
	}

	//�Ա�Ե���д���
	//���±�Ե
	//��ʵ���²��������Ĭ��Ϊ0�Ļ�����Ϊ����������Ĭ�Ͼ���0
	for (int k = 0; k != N; ++k) //��
	{
		for (int i = 0; i != w; ++i)//��
		{
			for (int j = 0; j != 3; ++j)
			{
				if (nFillColor == 0 || nFillColor == 255)
				{
					img1.m_pBits[j][k][i] = nFillColor;//�ϱ�Ե
					img1.m_pBits[j][h - k - N][i] = nFillColor;//�±�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					if (i >= N && i <= w - 1 - N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][i - N];//�ϱ�Ե
						img1.m_pBits[j][h - 1 - k][i] = m_pBits[j][h - 2 * N - 1][i - N];//�±�Ե
					}
					else if (i < N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][0];//�ϱ�Ե
						img1.m_pBits[j][h - k - 1][i] = m_pBits[j][h - 2 * N - 1][0];//�±�Ե
					}
					else if (i > w - 1 - N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][w - 2 * N - 1];//�ϱ�Ե
						img1.m_pBits[j][h - k - 1][i] = m_pBits[j][h - 2 * N - 1][w - 2 * N - 1];//�±�Ե
					}
					else {
						std::cout << "else" << std::endl;
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
					img1.m_pBits[j][i][k] = nFillColor;//���Ե
					img1.m_pBits[j][i][w - k - 1] = nFillColor;//�ұ�Ե
				}
				else //nFillMode �ڸ��Ʊ�Ե���ص�ʱ����Ϊ-1
				{
					img1.m_pBits[j][i][k] = m_pBits[j][i - N][0]; //���Ե�����Ϊ���Ƶ�һ������
					img1.m_pBits[j][i][w - k - 1] = m_pBits[j][i - N][w - 2 * N - 1];  //�ױ�Ե�����Ϊ�������һ������
				}
			}
		}
	}

}

void MyImage_::RemoveFillTo(MyImage_ &img1, int nFillPara) const
{
	//�ȸ��²���
	if (!img1.IsNull())
		img1.Destroy();
	const int N = nFillPara;
	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth() - 2 * N);
	img1.SetHeight(this->GetHeight() - 2 * N);


	int w = img1.GetWidth();
	int h = img1.GetHeight();

	// �ȴ�������(ͨ�� �� ��)
	img1.m_pBits = (BYTE***)new   BYTE**[3];
	for (int i = 0; i < 3; i++)
	{
		img1.m_pBits[i] = (BYTE**)new  BYTE*[h];
	}
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < h; j++)
		{
			img1.m_pBits[i][j] = new BYTE[w];
		}
	}
	//ԭͼ���ݽ��и���
	for (int j = 0; j < h; j++) //��
	{
		for (int k = 0; k < w; k++) //��
		{
			img1.m_pBits[0][j][k] = m_pBits[0][j + nFillPara][k + nFillPara];//B
			img1.m_pBits[1][j][k] = m_pBits[1][j + nFillPara][k + nFillPara];//G
			img1.m_pBits[2][j][k] = m_pBits[2][j + nFillPara][k + nFillPara];//R
		}
	}
}

MyImage_& MyImage_::operator=(MyImage_ &img)
{
	img.CopyTo(*this);
	return *this;
}