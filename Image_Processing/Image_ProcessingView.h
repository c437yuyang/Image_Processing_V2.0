
// Image_ProcessingView.h : CImage_ProcessingView 类的接口
//

#pragma once
#pragma region 工具类头文件
#include "MyImage_.h"
#include "Stock.h" //撤销恢复
#include "ImageScaleViewer.h" //控制图像缩放以及显示图像的类  
#include "DlgShowImg.h"
#pragma endregion

#pragma region 算法类头文件
#include "SaliencyDetection.h"
#include "SLIC.h"
#include "AddNoise.h"
#include "Fourier.h"
#include "CvtColor.h"
#include "Thresh.h"
#pragma endregion

#pragma region 参数设置类窗口头文件
#include "DlgSLICParamsSet.h"
#include "DlgNosieParamsSet.h"
#include "DlgFreqFilterSet.h"
#include "DlgThreshParamsSet.h"
#pragma endregion

class CImage_ProcessingView : public CScrollView
{
	friend class CDlgThreshParamsSet;

protected: // 仅从序列化创建
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

	// 特性
public:
	/*CImage_ProcessingDoc* GetDocument() const;*/
	//CImage_ProcessingDoc* GetDocument() const;


	// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
#pragma region 一些系统消息函数
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
#pragma endregion

	DECLARE_MESSAGE_MAP()
public:




public:
#pragma region 状态更新函数
	void UpdateState(bool bIsStoreImage);
	void ChangeScrollSize();
	void UpdateStatusBar(CDC *pDC);
#pragma endregion
#pragma region 工具类菜单响应函数
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
#pragma endregion



#pragma region 自己实现图像处理相应函数
	afx_msg void OnFilterAvg();
	afx_msg void OnSaliencyLc();
	afx_msg void OnSegmentSlic();
	afx_msg void OnShowred();
	afx_msg void OnReverse();
	afx_msg void OnTogray();
	afx_msg void OnRetrieve();
	afx_msg void OnTest();
	afx_msg void OnAddnoise();
	afx_msg void OnFreqFft();
	afx_msg void OnFreqFilter();
#pragma endregion




private:
	CString m_strFileNameSave;
	int m_nWidth;
	int m_nHeight;
	std::vector<CDlgShowImg *> m_dlgs;
#pragma region 一些工具成员及函数
	MyImage_ m_Image;
	Stock<MyImage_> m_imgStock; //图像历史信息存储，用于撤销恢复  
	ImageScaleViewer m_imgScaleViewer; //控制图像显示及缩放
	void ShowImgInDlg(CString strWindowName, const MyImage_ &srcImg); //在一个非模态对话框显示指定图像
	afx_msg void OnCloseChilds(); //关闭所有子窗口
#pragma endregion
	
	
public:
	afx_msg void OnSegementThresh();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp 中的调试版本
//inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
//{
//	return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument);
//}
#endif

