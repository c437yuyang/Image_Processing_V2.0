
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

#pragma region 需要包含的头文件
#include "MainFrm.h"
#pragma endregion


#pragma region 算法类头文件
#include "SaliencyDetection.h"
#include "SLIC.h"
#pragma endregion

#pragma region 参数设置类窗口头文件
#include "DlgSLICParamsSet.h"
#pragma endregion



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
	ON_COMMAND(ID_FILE_SAVE_AS, &CImage_ProcessingView::OnFileSaveAs)
	ON_COMMAND(IDM_SHOWRED, &CImage_ProcessingView::OnShowred)
	//	ON_COMMAND(ID_VIEW_TOOLBAR, &CImage_ProcessingView::OnViewToolbar)
	ON_COMMAND(ID_REVERSE, &CImage_ProcessingView::OnReverse)
	ON_WM_CREATE()
	ON_COMMAND(ID_TOGRAY, &CImage_ProcessingView::OnTogray)
	ON_COMMAND(ID_RETRIEVE, &CImage_ProcessingView::OnRetrieve)
	ON_COMMAND(ID_Test, &CImage_ProcessingView::OnTest)
	ON_COMMAND(ID_EDIT_UNDO, &CImage_ProcessingView::OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &CImage_ProcessingView::OnEditRedo)
	ON_COMMAND(ID_FILTER_AVG, &CImage_ProcessingView::OnFilterAvg)
	ON_COMMAND(ID_SALIENCY_LC, &CImage_ProcessingView::OnSaliencyLc)
	ON_COMMAND(ID_SEGMENT_SLIC, &CImage_ProcessingView::OnSegmentSlic)
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CImage_ProcessingView 构造/析构

CImage_ProcessingView::CImage_ProcessingView()
	: m_strFileNameSave(_T("")), m_imgScaleViewer(1.0)
{
	// TODO: 在此处添加构造代码
	m_strFileNameSave = "";
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
	if (!m_Image.IsNull())
	{

		m_imgScaleViewer.SetImage(m_Image);
		//m_imgScaleViewer.SetScale() //SetScale应该在操作的时候设置，然后状态栏有各显示
		if (m_imgScaleViewer.Draw(pDC->m_hDC) == MyImage_::DRAW_FAIL)
			AfxMessageBox(_T("Error when draw the picture！"));
		ChangeScrollSize();
		UpdateStatusBar(pDC);
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
	if (IDOK == dlg.DoModal())    //调用函数打开一个对话框，并判断是否打开成功
	{
		if (!m_Image.IsNull()) m_Image.Destroy();//判断是否已经有图片，有的话进行清除

		m_strFileNameSave = dlg.GetPathName();
		if (m_Image.Load(m_strFileNameSave) == MyImage_::LOAD_FAIL)
		{
			AfxMessageBox(_T("打开图片出错!"));
			return;
		}
		//获得图片的地址，并且加载图片
		//这里只是加载到数组里，后面的Invalidate(1)再来调用Ondraw函数再来调用MyImage_的Draw方法来画出图片
		//获得图片的大小，并按其大小设置滚动条的初始窗口大小等参数
		UpdateState(true);
	}
}

void CImage_ProcessingView::UpdateState(bool bIsStoreImage)
{
	if (bIsStoreImage)
		m_imgStock.AddImageToStock(m_Image);
	m_nWidth = m_Image.GetWidth();
	m_nHeight = m_Image.GetHeight();
	m_imgScaleViewer.SetNeedToUpdateScaleImage();
	ChangeScrollSize();
	Invalidate(1); //强制调用ONDRAW函数，ONDRAW会绘制图像
}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) {
		MessageBox(_T("你还没有打开一个要保存的图像文件！"));
		return;
	}
	CString strFilter;
	strFilter = "BMP 位图文件|*.bmp|JPEG 图像文件|*.jpg|GIF 图像文件|*.gif|PNG 图像文件|*.png||";   //
	//	strFilter ="所有文件|*.*||";   //

	CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if (IDOK != dlg.DoModal())
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
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	// TODO: 在此处为本机数据添加绘制代码
	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			m_Image.at(j, k, 0) = m_Image.at(j, k, 2);
			m_Image.at(j, k, 1) = m_Image.at(j, k, 2);
		}
	}

	UpdateState(true);
}


int CImage_ProcessingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//打开后自动读入一张图
	CString str("0.jpg");
	m_Image.Load(str);
	UpdateState(true);
	return 0;
}



void CImage_ProcessingView::OnTogray()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull() || m_Image.IsGrayed()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	int avg = 0;
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			/*-------------------------Your Code Here--------------------------*/
			//把图像灰度化
			avg = 0;
			avg += static_cast<int>(m_Image.at(j, k, 0));
			avg += static_cast<int>(m_Image.at(j, k, 1));
			avg += static_cast<int>(m_Image.at(j, k, 2));
			avg /= 3;
			m_Image.at(j, k, 0) = static_cast<BYTE>(avg);
			m_Image.at(j, k, 1) = static_cast<BYTE>(avg);
			m_Image.at(j, k, 2) = static_cast<BYTE>(avg);
			/*-------------------------Your Code Here--------------------------*/
		}
	}
	UpdateState(true);
}


void CImage_ProcessingView::OnRetrieve()
{
	// TODO: Add your command handler code here
	m_imgStock.getFirstImage(m_Image);
	UpdateState(false);
}


void CImage_ProcessingView::OnReverse()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误

	int w = m_Image.GetWidth();//获得图像的宽度
	int h = m_Image.GetHeight();//获得图像的高度

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			/*-------------------------Your Code Here--------------------------*/
			//实现图像反像（比如原来是黑色，变成白色）
			m_Image.at(j, k, 0) = 255 - m_Image.at(j, k, 0);
			m_Image.at(j, k, 1) = 255 - m_Image.at(j, k, 1);
			m_Image.at(j, k, 2) = 255 - m_Image.at(j, k, 2);
			/*-------------------------Your Code Here--------------------------*/

		}
	}
	UpdateState(true);
}

void CImage_ProcessingView::OnTest()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;//判断图像是否为空，如果对空图像进行操作会出现未知的错误
	MyImage_ img1;
	m_Image.BorderFillTo(img1, 2, MyImage_::FILL_BLACK);
	img1.CopyTo(m_Image);

	//m_Image.Create(100, 100, RGB(255, 0, 128));

	UpdateState(true);
}


void CImage_ProcessingView::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	if (m_imgStock.getPreImage(m_Image))
		UpdateState(false);
}


void CImage_ProcessingView::OnEditRedo()
{
	// TODO: 在此添加命令处理程序代码
	if (m_imgStock.getNextImage(m_Image))
		UpdateState(false);
}


void CImage_ProcessingView::OnFilterAvg()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;

	int  nPara = 1;

	int nPixCount = (nPara * 2 + 1) * (nPara * 2 + 1);

	MyImage_ imgBackUp, imgFilled;
	m_Image.BorderFillTo(imgBackUp, nPara, MyImage_::FILL_COPY); //备份一下
	m_Image.BorderFillTo(imgFilled, nPara, MyImage_::FILL_COPY);

	int w = imgFilled.GetWidth();
	int h = imgFilled.GetHeight();
	//取均值

	for (int i = nPara; i < h - nPara; ++i) //循环次数只有h-2
	{
		for (int j = nPara; j < w - nPara; ++j)//循环w-2次
		{
			int sum[3] = { 0,0,0 };//三个通道分别的和
								   //应用模板

			for (int m = i - nPara; m <= i + nPara; m++)
			{
				for (int n = j - nPara; n <= j + nPara; n++)
				{
					sum[0] += imgFilled.at(m, n, 0);
					sum[1] += imgFilled.at(m, n, 1);
					sum[2] += imgFilled.at(m, n, 2);
				}
			}
			sum[0] /= nPixCount;
			sum[1] /= nPixCount;
			sum[2] /= nPixCount;
			imgBackUp.at(i, j, 0) = sum[0];
			imgBackUp.at(i, j, 1) = sum[1];
			imgBackUp.at(i, j, 2) = sum[2];
		}
	}
	imgBackUp.RemoveFillTo(m_Image, nPara);
	UpdateState(true);

}



void CImage_ProcessingView::OnSaliencyLc()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;

	/*MyImage_ imgSaliencyLC(m_Image.GetWidth(), m_Image.GetHeight());*/
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	BYTE* SaliencyMap = new BYTE[w * h]();

	SalientRegionDetectionBasedonLC(m_Image.data(), SaliencyMap, w, h, 1);

	for (int i = 0; i != h; ++i)
	{
		for (int j = 0; j != w; ++j)
		{
			m_Image.at(i, j, 0) = SaliencyMap[i*w + j];
			m_Image.at(i, j, 1) = SaliencyMap[i*w + j];
			m_Image.at(i, j, 2) = SaliencyMap[i*w + j];
		}
	}
	UpdateState(true);

}


void CImage_ProcessingView::OnSegmentSlic()
{
	// TODO: 在此添加命令处理程序代码
	if (m_Image.IsNull()) return;
	int nSpNum = 200;
	CDlgSLICParamsSet dlg;
	if (dlg.DoModal() == IDCANCEL) return;
	else
		nSpNum = dlg.m_nSpNum;

	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	UINT* imgBuff = new UINT[w * h * 3]();
	//需要传入一个uint的类型，先将图像类型进行转换
	for (int i = 0; i != h; ++i)
		for (int j = 0; j != w; ++j)
			for (int k = 0; k != 3; ++k)
				imgBuff[i * w * 3 + j * 3 + k] = static_cast<UINT>(m_Image.at(i, j, k));

	SLIC slic;
	int *kLables = new int[w*h]();
	int numLabels(0), weight = 0 /*暂时没用*/, color = 222;
	slic.PerformSLICO_ForGivenK(imgBuff, w, h, kLables, numLabels, nSpNum, weight);

	UINT* SegmentResult = new UINT[w*h];
	slic.DrawContoursAroundSegments(SegmentResult, kLables, w, h, color);

	for (int i = 0; i != h; ++i)
	{
		for (int j = 0; j != w; ++j)
		{
			if (SegmentResult[i*w + j] == color)
			{
				m_Image.at(i, j, 0) = m_Image.at(i, j, 1) = 0;
				m_Image.at(i, j, 2) = 255;
			}
		}
	}
	delete[] imgBuff;
	delete[] SegmentResult;
	delete[] kLables;
	UpdateState(true);
}


BOOL CImage_ProcessingView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//按住ctrl+滚轮实现缩放图像
	if (nFlags == MK_CONTROL && zDelta < 0)
	{
		if (m_imgScaleViewer.GetScale() > 0.2)
		{
			m_imgScaleViewer.SetScale(m_imgScaleViewer.GetScale() - 0.1);
			if (m_imgScaleViewer.GetScale() != 1.0)
				m_imgScaleViewer.SetNeedToUpdateScaleImage();
			UpdateState(false);
		}

	}
	else if (nFlags == MK_CONTROL && zDelta > 0)
	{
		if (m_imgScaleViewer.GetScale() < 2.0)
		{
			m_imgScaleViewer.SetScale(m_imgScaleViewer.GetScale() + 0.1);
			if (m_imgScaleViewer.GetScale() != 1.0)
				m_imgScaleViewer.SetNeedToUpdateScaleImage();
			UpdateState(false);

		}

	}
	//按住shift实现滚轮图像左右滚动
	else if (nFlags == MK_SHIFT && zDelta < 0) //朝右
	{
		//OnHScroll()
		int minpos, maxpos, curpos;
		GetScrollRange(SB_HORZ, &minpos, &maxpos);
		maxpos = GetScrollLimit(SB_HORZ);
		curpos = GetScrollPos(SB_HORZ);
		if (curpos + 10 < maxpos)
		{
			SetScrollPos(SB_HORZ, curpos + 10);
			Invalidate(TRUE); //这里不需updatestate(),但是需要重画一下，在release模式下，更新太快会出现错误
		}
	}
	else if (nFlags == MK_SHIFT && zDelta > 0) //朝左
	{
		int minpos, maxpos, curpos;
		GetScrollRange(SB_HORZ, &minpos, &maxpos);
		maxpos = GetScrollLimit(SB_HORZ);
		curpos = GetScrollPos(SB_HORZ);
		if (curpos - 10 > minpos)
		{
			SetScrollPos(SB_HORZ, curpos - 10);
			Invalidate(TRUE);
		}
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}


void CImage_ProcessingView::ChangeScrollSize()
{
	CSize sizeTotal;
	sizeTotal.cx = static_cast<int>(m_nWidth*m_imgScaleViewer.GetScale()); //这里要设置为实际显示的大小才行
	sizeTotal.cy = static_cast<int>(m_nHeight*m_imgScaleViewer.GetScale()); //起始时未打开图片的情况下 View客户区小于300x300就设置滚轮
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImage_ProcessingView::UpdateStatusBar(CDC *pDC)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCStatusBar *pStatusBar = (CMFCStatusBar *)(pFrame->GetStatusBar());
	CClientDC dc(this);

	//显示图片长宽
	CString strImgSize;
	strImgSize.Format(_T("长:%d,宽:%d"), m_Image.GetHeight(), m_Image.GetWidth());
	CSize sz = dc.GetTextExtent(strImgSize);
	int index = pStatusBar->CommandToIndex(ID_INDICATOR_IMGSIZE);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_IMGSIZE, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strImgSize);

	//显示缩放系数
	CString strScaleFactor;
	strScaleFactor.Format(_T("缩放:%3.0f%%"), m_imgScaleViewer.GetScale()*100.0);
	sz = dc.GetTextExtent(strScaleFactor);
	index = pStatusBar->CommandToIndex(ID_INDICATOR_DISPALYSCALE);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_DISPALYSCALE, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strScaleFactor);

	ReleaseDC(pDC);
}


void CImage_ProcessingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

#pragma region 状态栏信息更新
	//实时显示鼠标坐标值
	CString strMousePos, strMouseRGB, strMouseHSI;
	strMousePos.Format(_T("x:%d,y:%d"), point.x, point.y);
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strMousePos);
	//std::cout << sz.cx << std::endl;
	//CMainFrame *pFrame=(CMainFrame *)AfxGetApp()->m_pMainWnd;  
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCStatusBar *pStatusBar = (CMFCStatusBar *)(pFrame->GetStatusBar());

	int index = pStatusBar->CommandToIndex(ID_INDICATOR_MOUSE_POS);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_MOUSE_POS, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strMousePos);

	//实时显示RGB值
	COLORREF color = dc.GetPixel(point.x, point.y);
	int nCb = (color & 0x00ff0000) >> 16;
	int nCg = (color & 0x0000ff00) >> 8;
	int nCr = color & 0x000000ff;
	strMouseRGB.Format(_T("R:%d,G:%d,B:%d"), nCr, nCg, nCb);
	sz = dc.GetTextExtent(strMouseRGB);
	index = pStatusBar->CommandToIndex(ID_INDICATOR_RGB);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_RGB, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strMouseRGB);
#pragma endregion


	CScrollView::OnMouseMove(nFlags, point);
}
