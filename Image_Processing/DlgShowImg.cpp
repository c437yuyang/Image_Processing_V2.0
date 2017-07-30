// DlgShowImg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgShowImg.h"
#include "afxdialogex.h"


// CDlgShowImg �Ի���

IMPLEMENT_DYNAMIC(CDlgShowImg, CDialogEx)


CDlgShowImg::CDlgShowImg(CString WndName,const MyImage_&img, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SHOW_IMG, pParent)
{
	img.CopyTo(m_img);
	m_strWndName = WndName;
}

CDlgShowImg::~CDlgShowImg()
{
}

void CDlgShowImg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgShowImg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgShowImg ��Ϣ�������


int CDlgShowImg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//�����Ի���Ĵ�С����Ӧͼ��
	CRect rcWindow, rcClient;
	int borderWidth, borderHeight;
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	borderWidth = (rcWindow.right - rcWindow.left) - (rcClient.right - rcClient.left);
	borderHeight = (rcWindow.bottom - rcWindow.top) - (rcClient.bottom - rcClient.top);
	this->SetWindowPos(NULL, 0, 0, m_img.GetWidth() + borderWidth, m_img.GetHeight() + borderHeight, SWP_NOMOVE); 
	return 0;
}


BOOL CDlgShowImg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	this->SetWindowTextW(m_strWndName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgShowImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	m_img.Draw(::GetDC(this->m_hWnd), 0, 0);
}


void CDlgShowImg::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	delete this; //��ֹ�ڴ�й¶
	CDialogEx::PostNcDestroy();
}
