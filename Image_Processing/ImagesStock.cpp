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
	if (m_nIndexCur != m_nImageCount - 1) //���м�λ��ִ����ӣ���ɾ���м�λ��֮��Ĵ洢
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
	else//�������(β��)
	{
		if (m_nImageCount > m_nStockSize - 1) //�ﵽ�����洢��Ŀ
		{
			m_imgs.pop_front(); //��Ϊ�����õ���pop_front������Ҫ��deque��vector����
			m_imgs.push_back(img);
		}
		else //���������δ�ﵽ�����Ŀ
		{
			m_nImageCount++;
			m_nIndexCur++;
			m_imgs.push_back(img);
		}
	}
	return m_nImageCount;
}