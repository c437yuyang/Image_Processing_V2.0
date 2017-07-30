// DlgShowImg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image_Processing.h"
#include "DlgShowImg.h"
#include "afxdialogex.h"


// CDlgShowImg 对话框

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


// CDlgShowImg 消息处理程序


int CDlgShowImg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//调整对话框的大小来适应图像
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

	// TODO:  在此添加额外的初始化
	this->SetWindowTextW(m_strWndName);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgShowImg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	m_img.Draw(::GetDC(this->m_hWnd), 0, 0);
}


void CDlgShowImg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	delete this; //防止内存泄露
	CDialogEx::PostNcDestroy();
}
