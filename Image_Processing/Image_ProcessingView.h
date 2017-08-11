
// Image_ProcessingView.h : CImage_ProcessingView ��Ľӿ�
//

#pragma once
#pragma region ������ͷ�ļ�
#include "MyImage_.h"
#include "Stock.h" //�����ָ�
#include "ImageScaleViewer.h" //����ͼ�������Լ���ʾͼ�����  
#include "DlgShowImg.h"
#pragma endregion

#pragma region �㷨��ͷ�ļ�
#include "SaliencyDetection.h"
#include "SLIC.h"
#include "AddNoise.h"
#include "Fourier.h"
#include "CvtColor.h"
#include "Thresh.h"
#pragma endregion

#pragma region ���������ര��ͷ�ļ�
#include "DlgSLICParamsSet.h"
#include "DlgNosieParamsSet.h"
#include "DlgFreqFilterSet.h"
#include "DlgThreshParamsSet.h"
#pragma endregion

class CImage_ProcessingView : public CScrollView
{
	friend class CDlgThreshParamsSet;

protected: // �������л�����
	CImage_ProcessingView();
	DECLARE_DYNCREATE(CImage_ProcessingView)

	// ����
public:
	/*CImage_ProcessingDoc* GetDocument() const;*/
	//CImage_ProcessingDoc* GetDocument() const;


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
#pragma region һЩϵͳ��Ϣ����
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
#pragma region ״̬���º���
	void UpdateState(bool bIsStoreImage);
	void ChangeScrollSize();
	void UpdateStatusBar(CDC *pDC);
#pragma endregion
#pragma region ������˵���Ӧ����
	afx_msg void OnEditUndo();
	afx_msg void OnEditRedo();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveAs();
#pragma endregion



#pragma region �Լ�ʵ��ͼ������Ӧ����
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
#pragma region һЩ���߳�Ա������
	MyImage_ m_Image;
	Stock<MyImage_> m_imgStock; //ͼ����ʷ��Ϣ�洢�����ڳ����ָ�  
	ImageScaleViewer m_imgScaleViewer; //����ͼ����ʾ������
	void ShowImgInDlg(CString strWindowName, const MyImage_ &srcImg); //��һ����ģ̬�Ի�����ʾָ��ͼ��
	afx_msg void OnCloseChilds(); //�ر������Ӵ���
#pragma endregion
	
	
public:
	afx_msg void OnSegementThresh();
};

#ifndef _DEBUG  // Image_ProcessingView.cpp �еĵ��԰汾
//inline CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const
//{
//	return reinterpret_cast<CImage_ProcessingDoc*>(m_pDocument);
//}
#endif

