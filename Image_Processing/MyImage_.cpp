#include "StdAfx.h"
#include "MyImage_.h"
#include <iostream>

CImage MyImage_::s_CImage; //定义静态变量

//负责创建一个CImage用于显示或者加载
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

MyImage_::MyImage_(BYTE * src, int w, int h) //从uchar数组初始化一个图像
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

//TODO:enum该如何处理
MyImage_::LoadResult MyImage_::Load(LPCTSTR pszFileName)
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

	//修改为使用1维数组，提供.at接口，实现类似于OpenCV的效果
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
	//获得m_CImage每一行像素的RGB所占用的存储空间的大小,这里取得的nrow是负值，才能跟后面的getbits是最后一行相配合
	int nrow = s_CImage.GetPitch();

	BYTE *psrc = (BYTE *)s_CImage.GetBits();//获得m_CImage最后一行的像素地址

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
	//这里因为位图是从下到上的数据，所以直接用memcpy实现不了，需要逆向复制的函数才行


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
	int nrow = s_CImage.GetPitch();//获得m_CImage每一行像素的RGB所占用的存储空间的大小
	BYTE *psrc = (BYTE *)s_CImage.GetBits();//获得m_CImage最后一行的像素地址
	//数据复制到CImage里面
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
	if (img1 == *this) return; //处理自赋值

	if (!img1.IsNull())
		img1.Destroy();

	img1.SetGrayed(m_bIsGrayed);
	img1.SetWidth(this->GetWidth());
	img1.SetHeight(this->GetHeight());


	int w = GetWidth();
	int h = GetHeight();
	img1.m_pBits = new BYTE[w*h * 3]();


	////复制图像数据数组
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

//填充模式0代表255填充，1代表0填充，2代表复制边界像素填充
//不支持inplace操作!
void MyImage_::BorderFillTo(MyImage_ &dst, int nFillPara, FillMode fm) const
{
	//TODO:这里应该判断是自身怎么办
	//先更新参数
	if (!dst.IsNull())
		dst.Destroy();
	const int N = nFillPara;
	dst.SetGrayed(m_bIsGrayed);
	dst.SetWidth(this->GetWidth() + 2 * N);
	dst.SetHeight(this->GetHeight() + 2 * N);

	int w = dst.GetWidth();
	int h = dst.GetHeight();

	//设置填充模式
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

	//原图数据进行复制
	for (int j = N; j < h - N; j++) //行
	{
		for (int k = N; k < w - N; k++) //列
		{
			dst.at(j, k, 0) = at(j - N, k - N, 0);//B
			dst.at(j, k, 1) = at(j - N, k - N, 1);//G
			dst.at(j, k, 2) = at(j - N, k - N, 2);//R
		}
	}

	//对边缘进行处理
	//上下边缘
	//其实底下不用填充若默认为0的话，因为新数组里面默认就是0
	for (int k = N; k != 0; --k) //行
	{
		for (int i = 0; i != w; ++i)//列
		{
			for (int j = 0; j != 3; ++j)//通道
			{
				if (nFillColor == 0 || nFillColor == 255)
				{
					dst.at(N - k, i, j) = nFillColor;//上边缘
					dst.at(h - k, i, j) = nFillColor;//下边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					if (i >= N && i <= w - 1 - N)
					{
						dst.at(N - k, i, j) = at(0, i - N, j);//上边缘
						dst.at(h - k, i, j) = at(h - 2 * N - 1, i - N, j);//下边缘
					}
					else if (i < N)
					{
						dst.at(N - k, i, j) = at(0, 0, j);//上边缘
						dst.at(h - k, i, j) = at(h - 2 * N - 1, 0, j);//下边缘
					}
					else if (i > w - 1 - N)
					{
						dst.at(N - k, i, j) = at(0, w - 2 * N - 1, j);//上边缘
						dst.at(h - k, i, j) = at(h - 2 * N - 1, w - 2 * N - 1, j);//下边缘
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
					dst.at(i, k, j) = nFillColor;//左边缘
					dst.at(i, w - k - 1, j) = nFillColor;//右边缘
				}
				else //nFillMode 在复制边缘像素的时候设为-1
				{
					dst.at(i, k, j) = at(i - N, 0, j);//左边缘填充设为复制第一列像素
					dst.at(i, w - k - 1, j) = at(i - N, w - 2 * N - 1, j);//底边缘填充设为复制最后一列像素
				}
			}
		}
	}

}

void MyImage_::RemoveFillTo(MyImage_ &dst, int nFillPara) const
{
	//先更新参数
	if (!dst.IsNull())
		dst.Destroy();
	const int N = nFillPara;
	dst.SetGrayed(m_bIsGrayed);
	dst.SetWidth(this->GetWidth() - 2 * N);
	dst.SetHeight(this->GetHeight() - 2 * N);

	int w = dst.GetWidth();
	int h = dst.GetHeight();

	dst.m_pBits = new BYTE[w * h * 3]();

	//原图数据进行复制
	for (int j = 0; j < h; j++) //行
	{
		for (int k = 0; k < w; k++) //列
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

	//下面代码根据EffecitveC++条款11设计，异常安全且可以处理自赋值
	BYTE* pOrig = m_pBits;
	SetGrayed(img.m_bIsGrayed);
	SetWidth(img.GetWidth());
	SetHeight(img.GetHeight());
	int w = GetWidth();
	int h = GetHeight();
	m_pBits = new BYTE[w * h * 3]();

	//复制图像数据数组
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
