
// Image_ProcessingView.cpp : CImage_ProcessingView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "Image_Processing.h"
#endif

#include "Image_ProcessingDoc.h"
#include "Image_ProcessingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma region ��Ҫ������ͷ�ļ�
#include "MainFrm.h"
#pragma endregion






// CImage_ProcessingView

IMPLEMENT_DYNCREATE(CImage_ProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImage_ProcessingView, CScrollView)
	// ��׼��ӡ����
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
	ON_COMMAND(ID_ADDNOISE, &CImage_ProcessingView::OnAddnoise)
	ON_COMMAND(ID_FREQ_FFT, &CImage_ProcessingView::OnFreqFft)
	ON_COMMAND(ID_CLOSE_CHILDS, &CImage_ProcessingView::OnCloseChilds)
	ON_COMMAND(ID_FREQ_FILTER, &CImage_ProcessingView::OnFreqFilter)
	ON_COMMAND(ID_SEGEMENT_THRESH, &CImage_ProcessingView::OnSegementThresh)
END_MESSAGE_MAP()

// CImage_ProcessingView ����/����

CImage_ProcessingView::CImage_ProcessingView()
	:m_imgScaleViewer(1.0),m_imgStock(10)
{
	// TODO: �ڴ˴���ӹ������
}

CImage_ProcessingView::~CImage_ProcessingView()
{
}

BOOL CImage_ProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CScrollView::PreCreateWindow(cs);
}

// CImage_ProcessingView ����

void CImage_ProcessingView::OnDraw(CDC* pDC)
{
	//CImage_ProcessingDoc* pDoc = GetDocument();
	//ASSERT_VALID(pDoc);
	//if (!pDoc)
	//	return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (!m_Image.IsNull())
	{

		m_imgScaleViewer.SetImage(m_Image);
		//m_imgScaleViewer.SetScale() //SetScaleӦ���ڲ�����ʱ�����ã�Ȼ��״̬���и���ʾ
		if (m_imgScaleViewer.Draw(pDC->m_hDC) == MyImage_::DRAW_FAIL)
			AfxMessageBox(_T("Error when draw the picture��"));
		ChangeScrollSize();
		UpdateStatusBar(pDC);
	}

	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 300; //��ʼʱδ��ͼƬ������� View�ͻ���С��300x300�����ù���
	SetScrollSizes(MM_TEXT, sizeTotal);
}


// CImage_ProcessingView ��ӡ


void CImage_ProcessingView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CImage_ProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImage_ProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImage_ProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CImage_ProcessingView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CImage_ProcessingView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
	// ƽʱ�����Ҽ���ʾ���в˵�
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CImage_ProcessingView ���

#ifdef _DEBUG
void CImage_ProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImage_ProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

//CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // �ǵ��԰汾��������
//{
//	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
//	return (CImage_ProcessingDoc*)m_pDocument;
//}
#endif //_DEBUG


// CImage_ProcessingView ��Ϣ�������


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE);    //Ϊ���ļ�����һ������
	if (IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if (!m_Image.IsNull()) m_Image.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ��������

		//pathname����ȫ·��
		if (m_Image.Load(dlg.GetPathName()) == MyImage_::LOAD_FAIL)
		{
			AfxMessageBox(_T("��ͼƬ����!"));
			return;
		}
		//���ͼƬ�ĵ�ַ�����Ҽ���ͼƬ
		//����ֻ�Ǽ��ص�����������Invalidate(1)��������Ondraw������������MyImage_��Draw����������ͼƬ
		//���ͼƬ�Ĵ�С���������С���ù������ĳ�ʼ���ڴ�С�Ȳ���
		//��Ҫ�����������ű���Ϊ100%
		m_imgScaleViewer.SetScale(1.0);
		UpdateState(true);
	}
}

void CImage_ProcessingView::UpdateState(bool bIsStoreImage)
{
	if (bIsStoreImage)
		m_imgStock.add(m_Image);
	m_imgScaleViewer.SetNeedToUpdateScaleImage();
	ChangeScrollSize();
	Invalidate(1); //ǿ�Ƶ���ONDRAW������ONDRAW�����ͼ��
}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) {
		MessageBox(_T("�㻹û�д�һ��Ҫ�����ͼ���ļ���"));
		return;
	}
	CString strFilter;
	strFilter = "BMP λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIF ͼ���ļ�|*.gif|PNG ͼ���ļ�|*.png||";   //
	//	strFilter ="�����ļ�|*.*||";   //

	CFileDialog dlg(FALSE, NULL, NULL, NULL, strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if (IDOK != dlg.DoModal())
		return;
	// ����û�û��ָ���ļ���չ������Ϊ�����һ��
	CString strFileName;
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;   //  ����ļ���
	//if(dlg.m_ofn.nFileExtension == 0)     //����ļ���չ��
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
	// ͼ�񱣴�
	HRESULT hResult = m_Image.Save(strFileName);
	if (FAILED(hResult))
	{
		MessageBox(_T("����ͼ���ļ�ʧ�ܣ�"));
	}

}


void CImage_ProcessingView::OnShowred()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	int w = m_Image.GetWidth();//���ͼ��Ŀ��
	int h = m_Image.GetHeight();//���ͼ��ĸ߶�

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
	//�򿪺��Զ�����һ��ͼ
	CString str("0.jpg");
	m_Image.Load(str);
	UpdateState(true);
	return 0;
}



void CImage_ProcessingView::OnTogray()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull() || m_Image.IsGrayed()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	int w = m_Image.GetWidth();//���ͼ��Ŀ��
	int h = m_Image.GetHeight();//���ͼ��ĸ߶�

	//for (int i = 0; i != h;++i)
	//{
	//	for (int j = 0; j != w; ++j)
	//	{
	//		/*-------------------------Your Code Here--------------------------*/
	//		//��ͼ��ҶȻ�
	//		BYTE val = static_cast<BYTE>((m_Image.at(i, j, 0) + m_Image.at(i, j, 1) + m_Image.at(i, j, 2)) / 3.0);
	//		for (int k = 0; k != 3; ++k)
	//			m_Image.at(i, j, k) = val;
	//		/*-------------------------Your Code Here--------------------------*/
	//	}
	//}

	CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
	m_Image.SetGrayed(true);
	UpdateState(true);
}


void CImage_ProcessingView::OnRetrieve()
{
	// TODO: Add your command handler code here
	m_imgStock.first(m_Image);
	UpdateState(false);
}


void CImage_ProcessingView::OnReverse()
{
	// TODO: Add your command handler code here
	if (m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	int w = m_Image.GetWidth();//���ͼ��Ŀ��
	int h = m_Image.GetHeight();//���ͼ��ĸ߶�

	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			/*-------------------------Your Code Here--------------------------*/
			//ʵ��ͼ���񣨱���ԭ���Ǻ�ɫ����ɰ�ɫ��
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
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	//MyImage_ img1;
	//m_Image.BorderFillTo(img1, 2, MyImage_::FILL_BLACK);
	//img1.CopyTo(m_Image);

	//m_Image.Create(100, 100, RGB(255, 0, 128));



#pragma region FT����
	//if (m_Image.IsNull()) return;
	//int w = m_Image.GetWidth();
	//int h = m_Image.GetHeight();

	//if (!m_Image.IsGrayed())
	//{

	//	CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
	//	UpdateState(true);
	//}

	//MyImage_ FT(Fourier::calExLen(w), Fourier::calExLen(h));
	//Fourier::test(m_Image.data(), w, h, FT.data());
	//FT.CopyTo(m_Image);
#pragma endregion

#pragma region ��ֵ����Test
	//if (m_Image.IsNull()) return;
	//int w = m_Image.GetWidth();
	//int h = m_Image.GetHeight();

	//if (!m_Image.IsGrayed())
	//{
	//	CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
	//	m_Image.SetGrayed(true);
	//}

	////Threshold::binThresh(m_Image.data(), w, h, m_Image.data(), 100);
	////Threshold::globalBasic(m_Image.data(), w, h, m_Image.data());
	////Threshold::otsu(m_Image.data(), w, h, m_Image.data());
	//Threshold::OTSU(m_Image.data(), w, h, m_Image.data());
#pragma endregion

	if (m_Image.IsNull()) return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	//double tpl[25] = { 0.1111,0.1111 ,0.1111 ,0.1111 ,0.1111 ,0.1111 ,0.1111 ,0.1111 ,0.1111 };

	vector<double> tpl(25, 1 / 25.0);

	Filter::imFilter(m_Image, tpl, m_Image, MyImage_::FILL_COPY, false);


	UpdateState(true);
}


void CImage_ProcessingView::OnEditUndo()
{
	// TODO: �ڴ���������������
	if (m_imgStock.pre(m_Image))
		UpdateState(false);
}


void CImage_ProcessingView::OnEditRedo()
{
	// TODO: �ڴ���������������
	if (m_imgStock.next(m_Image))
		UpdateState(false);
}


void CImage_ProcessingView::OnFilterAvg()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;

	int  nPara = 1;

	int nPixCount = (nPara * 2 + 1) * (nPara * 2 + 1);

	MyImage_ imgBackUp, imgFilled;
	m_Image.BorderFillTo(imgBackUp, nPara, MyImage_::FILL_COPY); //����һ��
	m_Image.BorderFillTo(imgFilled, nPara, MyImage_::FILL_COPY);

	int w = imgFilled.GetWidth();
	int h = imgFilled.GetHeight();
	//ȡ��ֵ

	for (int i = nPara; i < h - nPara; ++i) //ѭ������ֻ��h-2
	{
		for (int j = nPara; j < w - nPara; ++j)//ѭ��w-2��
		{
			int sum[3] = { 0,0,0 };//����ͨ���ֱ�ĺ�
								   //Ӧ��ģ��

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
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;

	/*MyImage_ imgSaliencyLC(m_Image.GetWidth(), m_Image.GetHeight());*/
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	BYTE* SaliencyMap = new BYTE[w * h]();

	Salient::SalientRegionDetectionBasedonLC(m_Image.data(), SaliencyMap, w, h, 1);

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
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;
	int nSpNum = 200;
	CDlgSLICParamsSet dlg;
	if (dlg.DoModal() == IDCANCEL) return;
	else
		nSpNum = dlg.m_nSpNum;

	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	UINT* imgBuff = new UINT[w * h * 3]();
	//��Ҫ����һ��uint�����ͣ��Ƚ�ͼ�����ͽ���ת��
	for (int i = 0; i != h; ++i)
		for (int j = 0; j != w; ++j)
			for (int k = 0; k != 3; ++k)
				imgBuff[i * w * 3 + j * 3 + k] = static_cast<UINT>(m_Image.at(i, j, k));

	SLIC slic;
	int *kLables = new int[w*h]();
	int numLabels(0), weight = 0 /*��ʱû��*/, color = 222;
	slic.PerformSLICO_ForGivenK(imgBuff, w, h, kLables, numLabels, nSpNum, weight);

	CString str; str.Format(_T("���г�����%d��"), numLabels);
	AfxMessageBox(str);

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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//��סctrl+����ʵ������ͼ��
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
	//��סshiftʵ�ֹ���ͼ�����ҹ���
	else if (nFlags == MK_SHIFT && zDelta < 0) //����
	{
		//OnHScroll()
		int minpos, maxpos, curpos;
		GetScrollRange(SB_HORZ, &minpos, &maxpos);
		maxpos = GetScrollLimit(SB_HORZ);
		curpos = GetScrollPos(SB_HORZ);
		if (curpos + 10 < maxpos)
		{
			SetScrollPos(SB_HORZ, curpos + 10);
			Invalidate(TRUE); //���ﲻ��updatestate(),������Ҫ�ػ�һ�£���releaseģʽ�£�����̫�����ִ���
		}
	}
	else if (nFlags == MK_SHIFT && zDelta > 0) //����
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
	if (!m_Image.IsNull())
	{
		sizeTotal.cx = static_cast<int>(m_Image.GetWidth()*m_imgScaleViewer.GetScale()); //����Ҫ����Ϊʵ����ʾ�Ĵ�С����
		sizeTotal.cy = static_cast<int>(m_Image.GetHeight()*m_imgScaleViewer.GetScale()); //��ʼʱδ��ͼƬ������� View�ͻ���С��300x300�����ù���
	}
	else
	{
		sizeTotal.cx = 300;
		sizeTotal.cy = 300;
	}
	
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImage_ProcessingView::UpdateStatusBar(CDC *pDC)
{
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	CMFCStatusBar *pStatusBar = (CMFCStatusBar *)(pFrame->GetStatusBar());
	CClientDC dc(this);

	//��ʾͼƬ����
	CString strImgSize;
	strImgSize.Format(_T("��:%d,��:%d"), m_Image.GetHeight(), m_Image.GetWidth());
	CSize sz = dc.GetTextExtent(strImgSize);
	int index = pStatusBar->CommandToIndex(ID_INDICATOR_IMGSIZE);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_IMGSIZE, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strImgSize);

	//��ʾ����ϵ��
	CString strScaleFactor;
	strScaleFactor.Format(_T("����:%3.0f%%"), m_imgScaleViewer.GetScale()*100.0);
	sz = dc.GetTextExtent(strScaleFactor);
	index = pStatusBar->CommandToIndex(ID_INDICATOR_DISPALYSCALE);
	pStatusBar->SetPaneInfo(index, ID_INDICATOR_DISPALYSCALE, SBPS_NORMAL, sz.cx);
	pStatusBar->SetPaneText(index, strScaleFactor);

	ReleaseDC(pDC);
}

void CImage_ProcessingView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

#pragma region ״̬����Ϣ����
	//ʵʱ��ʾ�������ֵ
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

	//ʵʱ��ʾRGBֵ
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


void CImage_ProcessingView::OnAddnoise()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;

	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	CDlgNosieParamsSet dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	int nNosieType = dlg.m_nNosieType;
	double para1 = dlg.m_dNoiseParam1;
	double para2 = dlg.m_dNoiseParam2;
	switch (nNosieType)
	{
	case CDlgNosieParamsSet::Salt:
		AddNoise::Salt(m_Image.data(), w, h, static_cast<int>(para1*w*h)); break;
	case CDlgNosieParamsSet::Pepper:
		AddNoise::Pepper(m_Image.data(), w, h, static_cast<int>(para1*w*h)); break;
	case CDlgNosieParamsSet::Gaussian:
		AddNoise::Gaussian(m_Image.data(), w, h, para1, para2); break;
	case CDlgNosieParamsSet::Rayleigh:
		AddNoise::Rayleigh(m_Image.data(), w, h, para1, para2); break;
	case CDlgNosieParamsSet::Exponential:
		AddNoise::Exponential(m_Image.data(), w, h, para1); break;
	case CDlgNosieParamsSet::Uniform:
		AddNoise::Uniform(m_Image.data(), w, h, para1, para2); break;
	case CDlgNosieParamsSet::Gamma:
		AddNoise::Gamma(m_Image.data(), w, h, para1, para2); break;
	default:
		break;
	}

	UpdateState(true);

}


void CImage_ProcessingView::OnFreqFft()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();

	if (!m_Image.IsGrayed()) 
	{
	
		CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
		m_Image.SetGrayed(true);
		UpdateState(true);
	}

	MyImage_ FT(Fourier::calExLen(w),Fourier::calExLen(h));
	Fourier::FFT2(m_Image.data(), w, h, FT.data());
	ShowImgInDlg(_T("����ҶƵ��"), FT);
	
}

void CImage_ProcessingView::ShowImgInDlg(CString strWindowName, const MyImage_ &srcImg)
{
	CDlgShowImg *pDlg = new CDlgShowImg(strWindowName, srcImg);
	pDlg->Create(IDD_DLG_SHOW_IMG, this);
	pDlg->SetWindowTextW(strWindowName);
	pDlg->ShowWindow(SW_SHOW);
	m_dlgs.push_back(pDlg);
}

void CImage_ProcessingView::OnCloseChilds()
{
	// TODO: �ڴ���������������
	if (!m_dlgs.empty())
	{
		for_each(m_dlgs.begin(), m_dlgs.end(), [](CDlgShowImg *p) {p->DestroyWindow(); });
		m_dlgs.clear();
	}
}


void CImage_ProcessingView::OnFreqFilter()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	int w_extend = Fourier::calExLen(w);
	int h_extend = Fourier::calExLen(h);

	DlgFreqFilterSet dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	if (!m_Image.IsGrayed())
	{
		CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
		m_Image.SetGrayed(true);
		UpdateState(true);
	}

	double *pFilter = new double[w_extend*h_extend]();
	int radius = dlg.m_nRadius;
	int order = dlg.m_nBpfOrder;
	double K1 = dlg.m_dOriginWeight;
	double K2 = dlg.m_dEnforceWeight;
	Fourier::Filter_Type type = static_cast<Fourier::Filter_Type>(dlg.m_nFilterType);
	
	Fourier::GetFilter(pFilter, w_extend, h_extend, type, radius, order,K1, K2);
	MyImage_ dst(w_extend,h_extend); //һ��Ҫ��extend���
	Fourier::Filter(m_Image.data(), w, h, pFilter, dst.data());
	dst.CopyTo(m_Image);
	UpdateState(true);
	delete[] pFilter;
}


void CImage_ProcessingView::OnSegementThresh()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) return;
	int w = m_Image.GetWidth();
	int h = m_Image.GetHeight();
	if (!m_Image.IsGrayed()) 
	{
		CvtColor::BGR2GRAY(m_Image.data(), w, h, m_Image.data());
		m_Image.SetGrayed(true);
		UpdateState(TRUE);
	}


	CDlgThreshParamsSet *pDlg = new CDlgThreshParamsSet;
	pDlg->Create(IDD_DLG_PARAM_THRESH, NULL);
	pDlg->ShowWindow(SW_SHOW);

	//�����updatestate��pDlg�������

}
