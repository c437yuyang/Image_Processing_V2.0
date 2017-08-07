#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "MyImage_.h"
#include "Thresh.h"

// CDlgThreshParamsSet �Ի���

class CDlgThreshParamsSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgThreshParamsSet)

public:
	CDlgThreshParamsSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgThreshParamsSet();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_PARAM_THRESH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_threshMode;
	CSliderCtrl m_SliderThresh;
	int m_nThresh;
	MyImage_ m_img;

	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNMReleasedcaptureSliderThresh(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PostNcDestroy();
	afx_msg void OnCbnSelchangeComboThreshmode();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
