#pragma once


// CDlgNosieParamsSet 对话框

class CDlgNosieParamsSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgNosieParamsSet)

public:
	CDlgNosieParamsSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgNosieParamsSet();
	virtual BOOL OnInitDialog();
	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PARAM_NOISE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	enum NoiseType {
		Salt, Pepper, Gaussian, Rayleigh, Exponential, Uniform, Gamma
	};

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedRadioSalt();
	afx_msg void OnBnClickedRadioGauss();
	afx_msg void OnBnClickedRadioRayleigh();
	afx_msg void OnBnClickedRadioExponential();
	afx_msg void OnBnClickedRadioUniform();
	afx_msg void OnBnClickedRadioGamma();
	afx_msg void OnBnClickedRadioPepper();
	double m_dNoiseParam1;
	double m_dNoiseParam2;
	int m_nNosieType;
};
