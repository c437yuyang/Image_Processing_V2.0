// DlgThreshParamsSet.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgThreshParamsSet.h"
#include "afxdialogex.h"

#include "Image_ProcessingView.h"
#include "MainFrm.h"
#include "Image_Processing.h"

// CDlgThreshParamsSet 对话框

IMPLEMENT_DYNAMIC(CDlgThreshParamsSet, CDialogEx)

CDlgThreshParamsSet::CDlgThreshParamsSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PARAM_THRESH, pParent), m_nThresh(100)
{

}

CDlgThreshParamsSet::~CDlgThreshParamsSet()
{
}

void CDlgThreshParamsSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_THRESHMODE, m_threshMode);
	DDX_Control(pDX, IDC_SLIDER_THRESH, m_SliderThresh);
}


BEGIN_MESSAGE_MAP(CDlgThreshParamsSet, CDialogEx)
	ON_WM_HSCROLL()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_THRESH, &CDlgThreshParamsSet::OnNMReleasedcaptureSliderThresh)
	ON_CBN_SELCHANGE(IDC_COMBO_THRESHMODE, &CDlgThreshParamsSet::OnCbnSelchangeComboThreshmode)
	ON_BN_CLICKED(IDCANCEL, &CDlgThreshParamsSet::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDlgThreshParamsSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgThreshParamsSet 消息处理程序
BOOL CDlgThreshParamsSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_threshMode.AddString(_T("otsu(最大方差法)"));
	m_threshMode.AddString(_T("基本全局阈值法"));
	m_threshMode.AddString(_T("自定义"));

	m_threshMode.SetCurSel(0);


	GetParent()->EnableWindow(FALSE);
	this->EnableWindow();

	//这里还是只能声明为友元，不然始终访问不到
	CImage_ProcessingView *pView = (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->m_Image.CopyTo(m_img);


	//初始显示
	if (m_threshMode.GetCurSel() == 0)
	{
		m_nThresh = Threshold::getOTSUThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight());
	}
	else if (m_threshMode.GetCurSel() == 1)
	{
		m_nThresh = Threshold::getGlobalBasicThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight());
	}
	else
	{
		m_nThresh = 100;
	}

	Threshold::binThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight(), pView->m_Image.data(), m_nThresh);
	pView->UpdateState(false);
	m_SliderThresh.SetRange(1, 254);
	m_SliderThresh.SetTicFreq(1);
	m_SliderThresh.SetPos(m_nThresh);

	CString str;
	str.Format(_T("阈值:%d"), m_SliderThresh.GetPos());
	SetDlgItemText(IDC_STATIC_THRESHNUM, str);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgThreshParamsSet::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CDlgThreshParamsSet::OnNMReleasedcaptureSliderThresh(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	m_threshMode.SetCurSel(2); //设置为自定义
	CImage_ProcessingView *pView = (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_nThresh = m_SliderThresh.GetPos();
	CString str;
	str.Format(_T("阈值:%d"), m_SliderThresh.GetPos());
	SetDlgItemText(IDC_STATIC_THRESHNUM, str);

	Threshold::binThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight(), pView->m_Image.data(), m_nThresh);
	pView->UpdateState(false);
	*pResult = 0;
}


void CDlgThreshParamsSet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialogEx::PostNcDestroy();
}


void CDlgThreshParamsSet::OnCbnSelchangeComboThreshmode()
{
	// TODO: Add your control notification handler code here

	CImage_ProcessingView *pView = (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	if (m_threshMode.GetCurSel() == 0)
		m_nThresh = Threshold::getOTSUThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight());
	else if (m_threshMode.GetCurSel() == 1)
		m_nThresh = Threshold::getGlobalBasicThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight());
	else
		m_nThresh = m_SliderThresh.GetPos();

	CString str;
	str.Format(_T("阈值:%d"), m_nThresh);
	SetDlgItemText(IDC_STATIC_THRESHNUM, str);
	Threshold::binThresh(m_img.data(), m_img.GetWidth(), m_img.GetHeight(), pView->m_Image.data(), m_nThresh);
	pView->UpdateState(false);
	m_SliderThresh.SetPos(m_nThresh);
}


void CDlgThreshParamsSet::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView = (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	m_img.CopyTo(pView->m_Image);
	pView->UpdateState(false);
	CDialogEx::OnCancel();
}


void CDlgThreshParamsSet::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	GetParent()->EnableWindow(TRUE);
	CImage_ProcessingView *pView = (CImage_ProcessingView*)((CMainFrame*)AfxGetMainWnd())->GetActiveView();
	pView->UpdateState(true);
	CDialogEx::OnOK();
}
