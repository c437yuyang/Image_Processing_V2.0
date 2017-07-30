#pragma once
#include "MyImage_.h"

// CDlgShowImg 对话框

class CDlgShowImg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgShowImg)

public:
	//CDlgShowImg(CWnd* pParent = NULL);   // 标准构造函数
	CDlgShowImg(CString,const MyImage_&,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgShowImg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SHOW_IMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


private:
	MyImage_ m_img;
	CString m_strWndName;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
};
