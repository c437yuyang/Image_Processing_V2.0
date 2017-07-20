#pragma once


// CDlgSLICParamsSet 对话框

class CDlgSLICParamsSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSLICParamsSet)

public:
	CDlgSLICParamsSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSLICParamsSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSLICPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nSpNum;
	afx_msg void OnBnClickedOk();
};
