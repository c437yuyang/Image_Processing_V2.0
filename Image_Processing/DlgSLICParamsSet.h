#pragma once


// CDlgSLICParamsSet �Ի���

class CDlgSLICParamsSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSLICParamsSet)

public:
	CDlgSLICParamsSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSLICParamsSet();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLGSLICPARAM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nSpNum;
	afx_msg void OnBnClickedOk();
};
