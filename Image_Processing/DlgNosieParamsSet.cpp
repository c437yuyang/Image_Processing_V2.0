// DlgNosieParamsSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgNosieParamsSet.h"
#include "afxdialogex.h"


// CDlgNosieParamsSet 对话框

IMPLEMENT_DYNAMIC(CDlgNosieParamsSet, CDialogEx)

CDlgNosieParamsSet::CDlgNosieParamsSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PARAM_NOISE, pParent)
	, m_dNoiseParam1(0)
	, m_dNoiseParam2(0)
	, m_nNosieType(0)
{

}

CDlgNosieParamsSet::~CDlgNosieParamsSet()
{
}

void CDlgNosieParamsSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARAM1, m_dNoiseParam1);
	DDX_Text(pDX, IDC_EDIT_PARAM2, m_dNoiseParam2);
	DDX_Radio(pDX, IDC_RADIO_SALT, m_nNosieType);
}


BEGIN_MESSAGE_MAP(CDlgNosieParamsSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgNosieParamsSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_SALT, &CDlgNosieParamsSet::OnBnClickedRadioSalt)
	ON_BN_CLICKED(IDC_RADIO_GAUSS, &CDlgNosieParamsSet::OnBnClickedRadioGauss)
	ON_BN_CLICKED(IDC_RADIO_Rayleigh, &CDlgNosieParamsSet::OnBnClickedRadioRayleigh)
	ON_BN_CLICKED(IDC_RADIO_EXPONENTIAL, &CDlgNosieParamsSet::OnBnClickedRadioExponential)
	ON_BN_CLICKED(IDC_RADIO_UNIFORM, &CDlgNosieParamsSet::OnBnClickedRadioUniform)
	ON_BN_CLICKED(IDC_RADIO_GAMMA, &CDlgNosieParamsSet::OnBnClickedRadioGamma)
	ON_BN_CLICKED(IDC_RADIO_PEPPER, &CDlgNosieParamsSet::OnBnClickedRadioPepper)
END_MESSAGE_MAP()


// CDlgNosieParamsSet 消息处理程序


void CDlgNosieParamsSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void CDlgNosieParamsSet::OnBnClickedRadioSalt()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_TXT_PARAM1, _T("salt prob"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("no use"));
	SetDlgItemTextW(IDC_EDIT_PARAM1, _T("0.001"));
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_PARAM2, -1);

}
void CDlgNosieParamsSet::OnBnClickedRadioPepper()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_TXT_PARAM1, _T("pepper prob"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("no use"));
	SetDlgItemTextW(IDC_EDIT_PARAM1, _T("0.001"));
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_PARAM2, -1);
}


void CDlgNosieParamsSet::OnBnClickedRadioGauss()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_TXT_PARAM1, _T("mu"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("std"));
	SetDlgItemInt(IDC_EDIT_PARAM1, 0);
	SetDlgItemInt(IDC_EDIT_PARAM2, 20);
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(TRUE);

}




void CDlgNosieParamsSet::OnBnClickedRadioRayleigh()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_TXT_PARAM1, _T("mu"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("std"));
	SetDlgItemInt(IDC_EDIT_PARAM1, -10);
	SetDlgItemInt(IDC_EDIT_PARAM2, 300);
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(TRUE);

}


void CDlgNosieParamsSet::OnBnClickedRadioExponential()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemTextW(IDC_TXT_PARAM1, _T("a"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("no use"));
	SetDlgItemInt(IDC_EDIT_PARAM1, 20);
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(FALSE);
	SetDlgItemInt(IDC_EDIT_PARAM2, -1);

}


void CDlgNosieParamsSet::OnBnClickedRadioUniform()
{
	// TODO: 在此添加控件通知处理程序代码
	SetDlgItemTextW(IDC_TXT_PARAM1, _T("a"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("b"));
	SetDlgItemInt(IDC_EDIT_PARAM1, -10);
	SetDlgItemInt(IDC_EDIT_PARAM2, 10);
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(TRUE);
}


void CDlgNosieParamsSet::OnBnClickedRadioGamma()
{
	// TODO: 在此添加控件通知处理程序代码

	SetDlgItemTextW(IDC_TXT_PARAM1, _T("a"));
	SetDlgItemTextW(IDC_TXT_PARAM2, _T("b"));
	SetDlgItemInt(IDC_EDIT_PARAM1, 10);
	SetDlgItemInt(IDC_EDIT_PARAM2, 1);
	GetDlgItem(IDC_EDIT_PARAM2)->EnableWindow(TRUE);

}




BOOL CDlgNosieParamsSet::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	OnBnClickedRadioSalt();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
