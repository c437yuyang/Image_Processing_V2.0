
// Image_ProcessingView.h : CImage_ProcessingView 类的接口
//

#pragma once
#include "MyImage_.h"
#include "ImagesStock.h" //撤销恢复
#include "ImageScaleViewer.h" //控制图像缩放以及显示图像的类

class CImage_ProcessingView : public CScrollView
{
protected: // 仅从序列化创建
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// 特性
public:
	CImage_ProcessingDoc* GetDocument() const;

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

private:
	// save the original file's name
	CString m_strFileNameSave;
#pragma region utility里面的一些工具
	MyImage_ m_Image;
	CImagesStock m_imgStock; //图像历史信息存储，用于撤销恢复  
	ImageScaleViewer m_imgScaleViewer; //控制图像显示及缩放
#pragma endregion
	int m_nWidth;
	int m_nHeight;


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
#pragma endregion



};

#ifndef _DEBUG  // Image_ProcessingView.cpp 中的调试版本
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

