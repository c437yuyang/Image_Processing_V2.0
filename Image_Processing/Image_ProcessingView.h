
// Image_ProcessingView.h : CImage_ProcessingView ��Ľӿ�
//

#pragma once
#include "MyImage_.h"
#include "ImagesStock.h" //�����ָ�

class CImage_ProcessingView : public CScrollView
{
protected: // �������л�����
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

// ����
public:
	CImage_ProcessingDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImage_ProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
public:
	MyImage_ m_Image;
	CImagesStock m_imgStock; //ͼ����ʷ��Ϣ�洢
	int m_nWidth;
	int m_nHeight;

	afx_msg void OnFileSaveAs();
	afx_msg void OnShowred();
private:
	// save the original file's name
	CString m_strFileNameSave;
public:
	afx_msg void OnReverse();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTogray();
	afx_msg void OnRetrieve();
	afx_msg void OnTest();

public:
	void UpdateState(bool bIsStoreImage);

	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp �еĵ��԰汾
inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
   { return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument); }
#endif

