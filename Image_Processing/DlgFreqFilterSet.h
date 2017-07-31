#pragma once


// DlgFreqFilterSet 对话框

class DlgFreqFilterSet : public CDialogEx
{
	DECLARE_DYNAMIC(DlgFreqFilterSet)

public:
	DlgFreqFilterSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgFreqFilterSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_FILTER_FREQ_SET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nFilterType;
	int m_nRadius;
	int m_nBpfOrder;
	afx_msg void OnBnClickedOk();
	double m_dEnforceWeight;
	double m_dOriginWeight;
	afx_msg void OnBnClickedRadioIlpf();
	afx_msg void OnBnClickedRadioBlpf();
	afx_msg void OnBnClickedRadioGlpf();
	afx_msg void OnBnClickedRadioIhpf();
	afx_msg void OnBnClickedRadioBhpf();
	afx_msg void OnBnClickedRadioGhpf();
};
