#include "StdAfx.h"
#include "MyImage_.h"
#include <iostream>

CImage MyImage_::s_CImage;

//负责创建一个Cimage用于显示或者加载
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
	m_pBits = NULL; //这里可以先为null，既然是拷贝构造函数，那么之前对象肯定没有生成，就没有数据，因此不会内存泄漏
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

//TODO:enum该如何处理
int MyImage_::Load(LPCTSTR pszFileName)
{
	if (!s_CImage.IsNull()) s_CImage.Destroy();
	s_CImage.Load(pszFileName);
	if (s_CImage.IsNull()) return LOAD_FAIL;
	int w = s_CImage.GetWidth();
	int h = s_CImage.GetHeight();
	int nrow = s_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	Create(w, h, 0);//创建数组部分，create里面设置了channel，所以要在之后再重新设置channel
	int nChannels = s_CImage.GetBPP() / 8;
	BYTE *psrc = (BYTE *)s_CImage.GetBits();//获得m_CImage最后一行的像素地址


	/*将m_CImage复制进三维数组*/
	//说明：不管是1通道还是3通道还是4通道的图片，读入后都作为3通道的图像。不然解决不了单通道显示出错(全黑)的问题
	//这里只是保证能打开这些图片
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
	/*创建三维数组并将m_CImage复制进三维数组*/
	return(LOAD_SUCCESS);//为了与CImge类的Load函数返回值相同
}

void MyImage_::Create( //填充数组，不管CImage，Cimage只负责最后显示的时候和加载图片Load的时候调用
	_In_ int width,
	_In_ int height,
	_In_ COLORREF color)
{
	if (!IsNull()) Destroy();
	SetWidth(width);
	SetHeight(height);
	SetGrayed(false);//先设为都是彩色图像
	int w = GetWidth();
	int h = GetHeight();

	// 通道 行 列,ch,y,x
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
	int nrow = s_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小


	BYTE *psrc = (BYTE *)s_CImage.GetBits();//获得m_CImage最后一行的像素地址

	/*将三维数组复制进m_CImage进行显示*/

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			psrc[j*nrow + k * 3] = m_pBits[0][j][k];//B
			psrc[j*nrow + k * 3 + 1] = m_pBits[1][j][k];//G
			psrc[j*nrow + k * 3 + 2] = m_pBits[2][j][k];//R
		}
	}

	/*将三维数组复制进m_CImage*/

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
	int nrow = s_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	BYTE *psrc = (BYTE *)s_CImage.GetBits();//获得m_CImage最后一行的像素地址
	//数据复制到CImage里面
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

	/*创建三维数组并将m_CImage复制进三维数组*/

	// 通道 行 列
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
	//复制图像数据数组
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

//填充模式0代表255填充，1代表0填充，2代表复制边界像素填充
void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara, int FillMode) const
{
	//先更新参数
	if (!img1.IsNull())
		img1.Destroy();
	const int N = nFillPara;
	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth() + 2 * N);
	img1.SetHeight(this->GetHeight() + 2 * N);


	int w = img1.GetWidth();
	int h = img1.GetHeight();

	//设置填充模式
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

	//创建数组 通道 行 列
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


	//原图数据进行复制
	for (int j = N; j < h - N; j++) //行
	{
		for (int k = N; k < w - N; k++) //列
		{
			img1[0][j][k] = m_pBits[0][j - N][k - N];//B
			img1[1][j][k] = m_pBits[1][j - N][k - N];//G
			img1[2][j][k] = m_pBits[2][j - N][k - N];//R
		}
	}

	//对边缘进行处理
	//上下边缘
	//其实底下不用填充若默认为0的话，因为新数组里面默认就是0
	for (int k = 0; k != N; ++k) //行
	{
		for (int i = 0; i != w; ++i)//列
		{
			for (int j = 0; j != 3; ++j)
			{
				if (nFillColor == 0 || nFillColor == 255)
				{
					img1.m_pBits[j][k][i] = nFillColor;//上边缘
					img1.m_pBits[j][h - k - N][i] = nFillColor;//下边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					if (i >= N && i <= w - 1 - N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][i - N];//上边缘
						img1.m_pBits[j][h - 1 - k][i] = m_pBits[j][h - 2 * N - 1][i - N];//下边缘
					}
					else if (i < N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][0];//上边缘
						img1.m_pBits[j][h - k - 1][i] = m_pBits[j][h - 2 * N - 1][0];//下边缘
					}
					else if (i > w - 1 - N)
					{
						img1.m_pBits[j][k][i] = m_pBits[j][0][w - 2 * N - 1];//上边缘
						img1.m_pBits[j][h - k - 1][i] = m_pBits[j][h - 2 * N - 1][w - 2 * N - 1];//下边缘
					}
					else {
						std::cout << "else" << std::endl;
					}
				}

			}
		}
	}

	//边缘的填充必须在这里面处理才行
	for (int k = 0; k != N; ++k) //列
	{
		for (int i = N; i != h - N; ++i) //行
		{
			for (int j = 0; j != 3; ++j)
			{

				if (nFillColor == 0 || nFillColor == 255)
				{
					img1.m_pBits[j][i][k] = nFillColor;//左边缘
					img1.m_pBits[j][i][w - k - 1] = nFillColor;//右边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					img1.m_pBits[j][i][k] = m_pBits[j][i - N][0]; //左边缘填充设为复制第一列像素
					img1.m_pBits[j][i][w - k - 1] = m_pBits[j][i - N][w - 2 * N - 1];  //底边缘填充设为复制最后一列像素
				}
			}
		}
	}

}

void MyImage_::RemoveFillTo(MyImage_ &img1, int nFillPara) const
{
	//先更新参数
	if (!img1.IsNull())
		img1.Destroy();
	const int N = nFillPara;
	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth() - 2 * N);
	img1.SetHeight(this->GetHeight() - 2 * N);


	int w = img1.GetWidth();
	int h = img1.GetHeight();

	// 先创建数组(通道 行 列)
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
	//原图数据进行复制
	for (int j = 0; j < h; j++) //行
	{
		for (int k = 0; k < w; k++) //列
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