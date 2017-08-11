#include "StdAfx.h"
#include "ImagesStock.h"


CImagesStock::CImagesStock(void)
{
	m_nStockSize = 10;
	m_nImageCount = 0;
	m_nIndexCur = -1;
}


CImagesStock::~CImagesStock(void)
{
}

bool CImagesStock::getPreImage(MyImage_ &img)
{
	if (m_nIndexCur <= m_nImageCount && m_nIndexCur > 0)
	{
		m_nIndexCur--;
		m_imgs[m_nIndexCur].CopyTo(img);
		return true;
	}
	return false;

}

bool CImagesStock::getNextImage(MyImage_ &img)
{
	if (m_nIndexCur < m_nImageCount - 1)
	{
		m_nIndexCur++;
		m_imgs[m_nIndexCur].CopyTo(img);
		return true;
	}
	return false;

}

bool CImagesStock::getFirstImage(MyImage_ &dst)
{
	if (m_nImageCount > 0)
	{
		m_imgs[0].CopyTo(dst);
	}
	return false;
}

int CImagesStock::AddImageToStock(MyImage_ &img)
{
	if (m_nIndexCur != m_nImageCount - 1) //在中间位置执行添加，先删除中间位置之后的存储
	{
		for (int i = 0; i != m_nImageCount - m_nIndexCur - 1; ++i)
		{
			m_imgs.pop_back();
		}
		m_nImageCount -= (m_nImageCount - m_nIndexCur - 1);
		m_nImageCount++;
		m_nIndexCur++;
		m_imgs.push_back(img);
	}
	else//常规添加(尾部)
	{
		if (m_nImageCount > m_nStockSize - 1) //达到了最大存储数目
		{
			m_imgs.pop_front(); //因为这里用到了pop_front所以需要用deque，vector不行
			m_imgs.push_back(img);
		}
		else //常规添加且未达到最大数目
		{
			m_nImageCount++;
			m_nIndexCur++;
			m_imgs.push_back(img);
		}
	}
	return m_nImageCount;
}