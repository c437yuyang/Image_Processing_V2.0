#pragma once

#include "MyImage_.h"
#include <deque>
#include <vector>
#include <iostream>
using namespace std;

class CImagesStock
{
public:
	CImagesStock(void);
	~CImagesStock(void);
	//int addImage(MyImage_ &img);
	int AddImageToStock(MyImage_ &img);
	int getCurIndex() { return m_nIndexCur; }
	int getStockCount() { return m_nImageCount; }
	bool getPreImage(MyImage_&);
	bool getNextImage(MyImage_ &);
	void clear() { m_imgs.clear(); m_nIndexCur = -1; m_nImageCount = 0; }
private:
	deque<MyImage_> m_queImages;
	deque<MyImage_> m_imgs;
	//vector<MyImage_> m_imgs;
	int m_nIndexCur;
	int m_nImageCount;
	int m_nStockSize;
};

