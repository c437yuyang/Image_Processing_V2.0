// DlgSLICParamsSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgSLICParamsSet.h"
#include "afxdialogex.h"


// CDlgSLICParamsSet �Ի���

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


// CDlgSLICParamsSet ��Ϣ�������


void CDlgSLICParamsSet::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true);
	CDialogEx::OnOK();
}
