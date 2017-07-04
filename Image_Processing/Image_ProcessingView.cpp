
// Image_ProcessingView.cpp : CImage_ProcessingView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Image_Processing.h"
#endif

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CImage_ProcessingView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_OPEN, &CImage_ProcessingView::OnFileOpen)
	ON_COMMAND(IDM_DRAWLINE, &CImage_ProcessingView::OnDrawline)
	ON_COMMAND(ID_FILE_SAVE_AS, &CImage_ProcessingView::OnFileSaveAs)
	ON_COMMAND(IDM_SHOWRED, &CImage_ProcessingView::OnShowred)
//	ON_COMMAND(ID_VIEW_TOOLBAR, &CImage_ProcessingView::OnViewToolbar)
ON_COMMAND(ID_DoubleImage, &CImage_ProcessingView::OnDoubleimage)
ON_COMMAND(ID_REVERSE, &CImage_ProcessingView::OnReverse)
ON_WM_CREATE()
ON_COMMAND(ID_TOGRAY, &CImage_ProcessingView::OnTogray)
ON_COMMAND(ID_RESDWON, &CImage_ProcessingView::OnResdwon)
ON_COMMAND(ID_RETRIEVE, &CImage_ProcessingView::OnRetrieve)
ON_COMMAND(ID_TEST, &CImage_ProcessingView::OnTest)
ON_COMMAND(ID_GrayToHalf, &CImage_ProcessingView::OnGraytohalf)
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构

CImage_ProcessingView::CImage_ProcessingView()
	: m_strFileNameSave(_T(""))
{
	// TODO: 在此处添加构造代码
	m_strFileNameSave = "";
	m_bIsProcessed = FALSE;
	m_nResDownRate = 1;
	m_bIsGrayed = FALSE;
	m_nGrayRate = 1;
}

CImage_ProcessingView::~CImage_ProcessingView()
{
}

BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CImage_ProcessingView 绘制

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	if(m_bIsProcessed == TRUE) 
	{
		if (!m_ImageAfter.IsNull())
		{
			m_ImageAfter.Draw(pDC->m_hDC,0,0);
		}
	}
	else 
	{
		if (!m_Image.IsNull())
		{
			m_Image.Draw(pDC->m_hDC,0,0); 
		}
	}

	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 30; //起始时未打开图片的情况下 View客户区小于300x300就设置滚轮
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImage_ProcessingView 打印


void CImage_ProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CImage_ProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImage_ProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	// 平时单击右键显示剪切菜单
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImage_ProcessingView 诊断

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView 消息处理程序


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if(IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if(!m_Image.IsNull()) m_Image.Destroy();//判断是否已经有图片，有的话进行清除
		
		if(!m_ImageAfter.IsNull()) m_ImageAfter.Destroy();//清除after变量的数据
		m_bIsProcessed = FALSE;
		m_bIsGrayed = FALSE;
		m_strFileNameSave = dlg.GetFileName();
		m_Image.Load(m_strFileNameSave);//获得图片的地址，并且加载图片
		//这里只是加载到数组里，后面的Invalidate(1)再来调用Ondraw函数再来调用MyImage_的Draw方法来画出图片
		//获得图片的大小，并按其大小设置滚动条的初始窗口大小等参数
		CSize sizeTotal;
		int w=m_Image.GetWidth();
		int h=m_Image.GetHeight();
		sizeTotal.cx =w;
		sizeTotal.cy =h;
		SetScrollSizes(MM_TEXT, sizeTotal);

		//Window_Image_w=m_Image.GetWidth();//获得图片的初始大小，为放大缩小功能做准备
		//Window_Image_h=m_Image.GetHeight();//


		Invalidate(1); //强制调用ONDRAW函数
	}
}


void CImage_ProcessingView::OnDrawline()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;
	
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();

	//此方法是直接在客户区画横线，但是窗口的重绘消息调用后会消失
	//CClientDC dc(this);
	//CPen pen(BS_SOLID,3,RGB(255,0,0));
	//dc.SelectObject(&pen);
	//CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	//dc.SelectObject(pBrush);
	//CPoint ptStart,ptEnd;
	//CRect rect;
	//GetClientRect(&rect);
	//ptStart.x = 0;
	//ptStart.y = 10;
	//ptEnd.x = w;
	//ptEnd.y = 10;
	//dc.MoveTo(ptStart);
	//dc.LineTo(ptEnd);
	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	//在第十行画横线 此方法是在读入的图片数组里进行画图，而不是在客户区上画图
	for (int k=0;k<w;k++)
	{
		m_ImageAfter.m_pBits[0][10][k]=0;//B
		m_ImageAfter.m_pBits[1][10][k]=0;//G
		m_ImageAfter.m_pBits[2][10][k]=0;//R
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数
}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) {
		MessageBox(_T("你还没有打开一个要保存的图像文件！"));
		return;
	}
	CString strFilter;
	strFilter ="BMP 位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	//	strFilter ="所有文件|*.*||";   //

	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if ( IDOK != dlg.DoModal()) 
		return;
	// 如果用户没有指定文件扩展名，则为其添加一个
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;   //  获得文件名
	//if(dlg.m_ofn.nFileExtension == 0)     //获得文件扩展名
	//{
	switch (dlg.m_ofn.nFilterIndex)
	{
	case 1:
		strExtension = "bmp"; break;
	case 2:
		strExtension = "jpg"; break;
	case 3:
		strExtension = "gif"; break;
	case 4:
		strExtension = "png"; break;
	default:
		break;
	}
	strFileName = strFileName + _T(".") + strExtension;
	//	//strFileName="C:\\Users\\Lenovo\\Desktop\\QSanguosha-Qingming\\abc.bmp";
	//}
	// 图像保存
	HRESULT hResult = m_Image.Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}

}


void CImage_ProcessingView::OnShowred()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	// TODO: 在此处为本机数据添加绘制代码


	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_ImageAfter.m_pBits[0][j][k]=0;//B   用循环访问图像的像素值，将它的绿色分量和蓝色分量置为0，图像就只剩下红色分量了
			m_ImageAfter.m_pBits[1][j][k]=0;//G

		}
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


//void CImage_ProcessingView::OnViewToolbar()
//{
//	// TODO: 在此添加命令处理程序代码
//}




void CImage_ProcessingView::OnDoubleimage()
{
	// TODO: 在此添加命令处理程序代码
	if(m_Image.IsNull()) //判断图像是否为空，如果对空图像进行操作会出现未知的错误
	{
		MessageBox(_T("请先打开一幅图像！"));
		 return;
	}
	int w=m_Image.GetWidth();//获得第一幅图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度

	MyImage_ Image_Second;
	CFileDialog dlg(TRUE);    //为打开文件创建一个变量
	if(IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if(!Image_Second.IsNull()) Image_Second.Destroy();//判断是否已经有图片，有的话进行清除

		Image_Second.Load(dlg.GetPathName());//调用对话框的GetPathName获得图片的位置，并且调用Load加载图片
		//m_bIsProcessed = TRUE;

		if(Image_Second.IsNull()) return;
		if (w!=Image_Second.GetWidth()||h!=Image_Second.GetHeight())//判断两幅图像大小是否一致
		{
			MessageBox(_T("两幅图像大小不一致！"));
			//Image_Second.Destroy();
			return;
		}
		for (int j=0; j<h; j++)
			for (int i=0; i<w; i++)//两幅图像做简单相加操作
			{
				m_Image.m_pBits[0][j][i]=m_Image.m_pBits[0][j][i]+Image_Second.m_pBits[0][j][i];
				m_Image.m_pBits[1][j][i]=m_Image.m_pBits[1][j][i]+Image_Second.m_pBits[1][j][i];
				m_Image.m_pBits[2][j][i]=m_Image.m_pBits[2][j][i]+Image_Second.m_pBits[2][j][i];
			}

		//修改为两幅图片大小不一致也可运行
		//int wSmaller,hSmaller;
		//if(Image_Second.GetWidth() >= w) 
		//	wSmaller = w;
		//else wSmaller = Image_Second.GetWidth();

		//if(Image_Second.GetHeight() >= h) 
		//	hSmaller = h;
		//else hSmaller = Image_Second.GetHeight();

		//CString str;
		//str.Format("%d,%d",wSmaller,hSmaller);
		//MessageBox(str);
		
		//m_ImageAfter.Creat(wSmaller,hSmaller,128);
		//for (int j=0; j<hSmaller; j++) 
		//{
		//	for (int i=0; i<wSmaller; i++)//两幅图像做简单相加操作
		//	{
		//		m_ImageAfter.m_pBits[0][j][i]=(m_Image.m_pBits[0][j][i]+Image_Second.m_pBits[0][j][i])/2;
		//		m_ImageAfter.m_pBits[1][j][i]=(m_Image.m_pBits[1][j][i]+Image_Second.m_pBits[1][j][i])/2;
		//		m_ImageAfter.m_pBits[2][j][i]=(m_Image.m_pBits[2][j][i]+Image_Second.m_pBits[2][j][i])/2;
		//	}
		//}

		Invalidate();
	}
}


void CImage_ProcessingView::OnReverse()
{
	// TODO: Add your command handler code here
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度

	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_ImageAfter.m_pBits[0][j][k]=255-m_ImageAfter.m_pBits[0][j][k];//B   用循环访问图像的像素值
			m_ImageAfter.m_pBits[1][j][k]=255-m_ImageAfter.m_pBits[1][j][k];//G
			m_ImageAfter.m_pBits[2][j][k]=255-m_ImageAfter.m_pBits[2][j][k];//R

		}
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


int CImage_ProcessingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//打开后自动读入一张图
	CString str("1.jpg");
	m_Image.Load(str);
	Invalidate(TRUE);
	return 0;
}


void CImage_ProcessingView::OnTogray()
{
	// TODO: Add your command handler code here
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度
	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	UINT average = 0;
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			average = (m_ImageAfter.m_pBits[0][j][k]+m_ImageAfter.m_pBits[1][j][k]+m_ImageAfter.m_pBits[2][j][k])/3;
			m_ImageAfter.m_pBits[0][j][k]=average;//B   用循环访问图像的像素值
			m_ImageAfter.m_pBits[1][j][k]=average;//G
			m_ImageAfter.m_pBits[2][j][k]=average;//R

		}
	}
	m_bIsGrayed = TRUE;
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::OnResdwon()
{
	// TODO: Add your command handler code here
	if(m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度

	m_nResDownRate *=2;

	int w1 = w / m_nResDownRate;
	int h1 = h / m_nResDownRate;

	UINT rate = m_nResDownRate;

	//两种方法：1.建立w1,h1的图，但是其他程序里得再加变量来判断
	//			2.直接建立w,h的图，背景色用255填充，但是其他算法无法叠加

	m_ImageAfter.Creat(w,h,255);

	for (int j=0;j!=h1;j++)
	{
		for (int i=0;i!=w1;i++)
		{
			m_ImageAfter.m_pBits[0][j][i] = m_Image.m_pBits[0][j*rate][i*rate];
			m_ImageAfter.m_pBits[1][j][i] = m_Image.m_pBits[1][j*rate][i*rate];
			m_ImageAfter.m_pBits[2][j][i] = m_Image.m_pBits[2][j*rate][i*rate];
		}
	}
	m_bIsProcessed = TRUE;
	Invalidate();

	//for (int j=0;j!=h;j++)
	//{
	//	for (int i=0;i!=w;i++)
	//	{
	//		if(j<h1 && i<w1)
	//		{
	//			m_Image.m_pBits[0][j][i] = m_Image.m_pBits[0][j*2][i*2];
	//			m_Image.m_pBits[1][j][i] = m_Image.m_pBits[1][j*2][i*2];
	//			m_Image.m_pBits[2][j][i] = m_Image.m_pBits[2][j*2][i*2];
	//		}
	//		else
	//		{
	//			m_Image.m_pBits[0][j][i] = 255;
	//			m_Image.m_pBits[1][j][i] = 255;
	//			m_Image.m_pBits[2][j][i] = 255;	
	//		}

	//	}
	//}
	//Invalidate();
	
	
	//m_Image.CopyTo(m_ImageAfter);
	//m_bIsProcessed = TRUE;
	//m_ImageAfter.Creat(200,200,120);
	//Invalidate();
	//UINT average = 0;
	//for (int j=0;j<h;j++)
	//{
	//	for (int k=0;k<w;k++)
	//	{
	//		average = (m_Image.m_pBits[0][j][k]+m_Image.m_pBits[1][j][k]+m_Image.m_pBits[2][j][k])/3;
	//		m_Image.m_pBits[0][j][k]=average;//B   用循环访问图像的像素值
	//		m_Image.m_pBits[1][j][k]=average;//G
	//		m_Image.m_pBits[2][j][k]=average;//R

	//	}
	//}
}


void CImage_ProcessingView::OnRetrieve()
{
	// TODO: Add your command handler code here
	if(m_bIsProcessed)
	{
		m_bIsProcessed = FALSE;
		m_bIsGrayed = FALSE;
		m_ImageAfter.Destroy();
		Invalidate();
	}
	else return;
}


void CImage_ProcessingView::OnTest()
{
	// TODO: Add your command handler code here
	m_ImageAfter.Creat(200,200,0);
	m_bIsProcessed = TRUE;
	Invalidate();
}


void CImage_ProcessingView::OnGraytohalf()
{
	if(!m_bIsGrayed) 
	{
		CString str("请先执行灰度化！");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return;
	int w=m_Image.GetWidth();//获得图像的宽度
	int h=m_Image.GetHeight();//获得图像的高度

	m_nGrayRate *=2;
	double temp;
	UINT nTrehshold = 256/2 -1;
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			temp = (double)m_ImageAfter.m_pBits[0][j][k]/m_nGrayRate;
			if(m_ImageAfter.m_pBits[0][j][k] > 127)
				if(temp > (UINT)temp) 
					temp = (UINT)temp + 1;
			temp *= m_nGrayRate;
			m_ImageAfter.m_pBits[0][j][k] = temp ;
			m_ImageAfter.m_pBits[1][j][k] = temp ;
			m_ImageAfter.m_pBits[2][j][k] = temp ;
		}
	}
	//m_bIsGrayed = TRUE;
	m_bIsProcessed = TRUE;
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像

	// TODO: Add your command handler code here
}
