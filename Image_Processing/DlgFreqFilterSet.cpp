// DlgFreqFilterSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgFreqFilterSet.h"
#include "afxdialogex.h"


// DlgFreqFilterSet 对话框

IMPLEMENT_DYNAMIC(DlgFreqFilterSet, CDialogEx)

DlgFreqFilterSet::DlgFreqFilterSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_FILTER_FREQ_SET, pParent)
	, m_nFilterType(0)
	, m_nRadius(200)
	, m_nBpfOrder(2)
	, m_dEnforceWeight(0.5)
	, m_dOriginWeight(1.0)
{

}

DlgFreqFilterSet::~DlgFreqFilterSet()
{
}

void DlgFreqFilterSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ILPF, m_nFilterType);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_nRadius);
	DDX_Text(pDX, IDC_EDIT_BPF_ORDER, m_nBpfOrder);
	DDX_Text(pDX, IDC_EDIT_HPF_ENFORCE_WEIGHT, m_dEnforceWeight);
	DDX_Text(pDX, IDC_EDIT_HPF_ORIGIN_WEIGHT, m_dOriginWeight);
}


BEGIN_MESSAGE_MAP(DlgFreqFilterSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &DlgFreqFilterSet::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_ILPF, &DlgFreqFilterSet::OnBnClickedRadioIlpf)
	ON_BN_CLICKED(IDC_RADIO_BLPF, &DlgFreqFilterSet::OnBnClickedRadioBlpf)
	ON_BN_CLICKED(IDC_RADIO_GLPF, &DlgFreqFilterSet::OnBnClickedRadioGlpf)
	ON_BN_CLICKED(IDC_RADIO_IHPF, &DlgFreqFilterSet::OnBnClickedRadioIhpf)
	ON_BN_CLICKED(IDC_RADIO_BHPF, &DlgFreqFilterSet::OnBnClickedRadioBhpf)
	ON_BN_CLICKED(IDC_RADIO_GHPF, &DlgFreqFilterSet::OnBnClickedRadioGhpf)
END_MESSAGE_MAP()


// DlgFreqFilterSet 消息处理程序


void DlgFreqFilterSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//TODO:可以加上数据校验，先不做了
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


void DlgFreqFilterSet::OnBnClickedRadioIlpf()
{
	// TODO: 在此添加控件通知处理程序代码

	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);

}


void DlgFreqFilterSet::OnBnClickedRadioBlpf()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
}


void DlgFreqFilterSet::OnBnClickedRadioGlpf()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
}


void DlgFreqFilterSet::OnBnClickedRadioIhpf()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
}


void DlgFreqFilterSet::OnBnClickedRadioBhpf()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
}


void DlgFreqFilterSet::OnBnClickedRadioGhpf()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_BPF_ORDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_HPF_ENFORCE_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_HPF_ORIGIN_WEIGHT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_RADIUS)->EnableWindow(TRUE);
}
