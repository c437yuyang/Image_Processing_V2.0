
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


#pragma region �㷨��ͷ�ļ�
#include "SaliencyDetection.h"

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
END_MESSAGE_MAP()

// CImage_ProcessingView ����/����

CImage_ProcessingView::CImage_ProcessingView()
	: m_strFileNameSave(_T(""))
{
	// TODO: �ڴ˴���ӹ������
	m_strFileNameSave = "";
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
	CImage_ProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (!m_Image.IsNull())
	{
		if (m_Image.Draw(pDC->m_hDC, 0, 0) == MyImage_::DRAW_FAIL)
			AfxMessageBox(_T("Error when draw the picture��"));
	}

	return;
}

void CImage_ProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: �������ͼ�ĺϼƴ�С
	sizeTotal.cx = sizeTotal.cy = 30; //��ʼʱδ��ͼƬ������� View�ͻ���С��300x300�����ù���
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

CImage_ProcessingDoc* CImage_ProcessingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage_ProcessingDoc)));
	return (CImage_ProcessingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImage_ProcessingView ��Ϣ�������


void CImage_ProcessingView::OnFileOpen()
{
	// TODO: �ڴ���������������
	CFileDialog dlg(TRUE);    //Ϊ���ļ�����һ������
	if (IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if (!m_Image.IsNull()) m_Image.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ��������

		m_strFileNameSave = dlg.GetPathName();
		if (m_Image.Load(m_strFileNameSave) == MyImage_::LOAD_FAIL)
		{
			AfxMessageBox(_T("��ͼƬ����!"));
			return;
		}
		//���ͼƬ�ĵ�ַ�����Ҽ���ͼƬ
		//����ֻ�Ǽ��ص�����������Invalidate(1)��������Ondraw������������MyImage_��Draw����������ͼƬ
		//���ͼƬ�Ĵ�С���������С���ù������ĳ�ʼ���ڴ�С�Ȳ���
		CSize sizeTotal;
		int w = m_Image.GetWidth();
		int h = m_Image.GetHeight();
		sizeTotal.cx = w;
		sizeTotal.cy = h;
		SetScrollSizes(MM_TEXT, sizeTotal);
		UpdateState(true);
	}
}

void CImage_ProcessingView::UpdateState(bool bIsStoreImage)
{
	if (bIsStoreImage)
		m_imgStock.AddImageToStock(m_Image);
	m_nWidth = m_Image.GetWidth();
	m_nHeight = m_Image.GetHeight();
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

	int avg = 0;
	for (int j = 0; j < h; j++)
	{
		for (int k = 0; k < w; k++)
		{
			/*-------------------------Your Code Here--------------------------*/
			//��ͼ��ҶȻ�
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
	MyImage_ img1;
	m_Image.BorderFillTo(img1, 2, MyImage_::FILL_BLACK);
	img1.CopyTo(m_Image);
	UpdateState(true);
}


void CImage_ProcessingView::OnEditUndo()
{
	// TODO: �ڴ���������������
	if (m_imgStock.getPreImage(m_Image))
		UpdateState(false);
}


void CImage_ProcessingView::OnEditRedo()
{
	// TODO: �ڴ���������������
	if (m_imgStock.getNextImage(m_Image))
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

	SalientRegionDetectionBasedonLC(m_Image.data(), SaliencyMap, w, h, 1);

	for (int i=0;i!=h;++i)
	{
		for (int j=0;j!=w;++j)
		{
			m_Image.at(i, j, 0) = SaliencyMap[i*w + j];
			m_Image.at(i, j, 1) = SaliencyMap[i*w + j];
			m_Image.at(i, j, 2) = SaliencyMap[i*w + j];
		}
	}
	UpdateState(true);

}
