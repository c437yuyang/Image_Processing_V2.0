// DlgSLICParamsSet.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgSLICParamsSet.h"
#include "afxdialogex.h"


// CDlgSLICParamsSet 对话框

IMPLEMENT_DYNAMIC(CDlgSLICParamsSet, CDialogEx)

CDlgSLICParamsSet::CDlgSLICParamsSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLGSLICPARAM, pParent)
	, m_nSpNum(200)
{

}

CDlgSLICParamsSet::~CDlgSLICParamsSet()
{
}

void CDlgSLICParamsSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SPNUM, m_nSpNum);
}


BEGIN_MESSAGE_MAP(CDlgSLICParamsSet, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgSLICParamsSet::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgSLICParamsSet 消息处理程序


void CDlgSLICParamsSet::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	CDialogEx::OnOK();
}
