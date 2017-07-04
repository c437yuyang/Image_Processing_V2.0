
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

// CImage_ProcessingView ����/����

CImage_ProcessingView::CImage_ProcessingView()
	: m_strFileNameSave(_T(""))
{
	// TODO: �ڴ˴���ӹ������
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
	if(IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if(!m_Image.IsNull()) m_Image.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ��������
		
		if(!m_ImageAfter.IsNull()) m_ImageAfter.Destroy();//���after����������
		m_bIsProcessed = FALSE;
		m_bIsGrayed = FALSE;
		m_strFileNameSave = dlg.GetFileName();
		m_Image.Load(m_strFileNameSave);//���ͼƬ�ĵ�ַ�����Ҽ���ͼƬ
		//����ֻ�Ǽ��ص�����������Invalidate(1)��������Ondraw������������MyImage_��Draw����������ͼƬ
		//���ͼƬ�Ĵ�С���������С���ù������ĳ�ʼ���ڴ�С�Ȳ���
		CSize sizeTotal;
		int w=m_Image.GetWidth();
		int h=m_Image.GetHeight();
		sizeTotal.cx =w;
		sizeTotal.cy =h;
		SetScrollSizes(MM_TEXT, sizeTotal);

		//Window_Image_w=m_Image.GetWidth();//���ͼƬ�ĳ�ʼ��С��Ϊ�Ŵ���С������׼��
		//Window_Image_h=m_Image.GetHeight();//


		Invalidate(1); //ǿ�Ƶ���ONDRAW����
	}
}


void CImage_ProcessingView::OnDrawline()
{
	// TODO: �ڴ���������������
	if(m_Image.IsNull()) return;
	
	int w=m_Image.GetWidth();
	int h=m_Image.GetHeight();

	//�˷�����ֱ���ڿͻ��������ߣ����Ǵ��ڵ��ػ���Ϣ���ú����ʧ
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

	//�ڵ�ʮ�л����� �˷������ڶ����ͼƬ��������л�ͼ���������ڿͻ����ϻ�ͼ
	for (int k=0;k<w;k++)
	{
		m_ImageAfter.m_pBits[0][10][k]=0;//B
		m_ImageAfter.m_pBits[1][10][k]=0;//G
		m_ImageAfter.m_pBits[2][10][k]=0;//R
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //ǿ�Ƶ���ONDRAW����
}


void CImage_ProcessingView::OnFileSaveAs()
{
	// TODO: �ڴ���������������
	if (m_Image.IsNull()) {
		MessageBox(_T("�㻹û�д�һ��Ҫ�����ͼ���ļ���"));
		return;
	}
	CString strFilter;
	strFilter ="BMP λͼ�ļ�|*.bmp|JPEG ͼ���ļ�|*.jpg|GIF ͼ���ļ�|*.gif|PNG ͼ���ļ�|*.png||";   //
	//	strFilter ="�����ļ�|*.*||";   //

	CFileDialog dlg(FALSE,NULL,NULL,NULL,strFilter);
	//CFileDialog dlg(FALSE,NULL,NULL,NULL);

	if ( IDOK != dlg.DoModal()) 
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
	if(m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���


	int w=m_Image.GetWidth();//���ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�
	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_ImageAfter.m_pBits[0][j][k]=0;//B   ��ѭ������ͼ�������ֵ����������ɫ��������ɫ������Ϊ0��ͼ���ֻʣ�º�ɫ������
			m_ImageAfter.m_pBits[1][j][k]=0;//G

		}
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //ǿ�Ƶ���ONDRAW������ONDRAW�����ͼ��
}


//void CImage_ProcessingView::OnViewToolbar()
//{
//	// TODO: �ڴ���������������
//}




void CImage_ProcessingView::OnDoubleimage()
{
	// TODO: �ڴ���������������
	if(m_Image.IsNull()) //�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���
	{
		MessageBox(_T("���ȴ�һ��ͼ��"));
		 return;
	}
	int w=m_Image.GetWidth();//��õ�һ��ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�

	MyImage_ Image_Second;
	CFileDialog dlg(TRUE);    //Ϊ���ļ�����һ������
	if(IDOK == dlg.DoModal())    //���ú�����һ���Ի��򣬲��ж��Ƿ�򿪳ɹ�
	{
		if(!Image_Second.IsNull()) Image_Second.Destroy();//�ж��Ƿ��Ѿ���ͼƬ���еĻ��������

		Image_Second.Load(dlg.GetPathName());//���öԻ����GetPathName���ͼƬ��λ�ã����ҵ���Load����ͼƬ
		//m_bIsProcessed = TRUE;

		if(Image_Second.IsNull()) return;
		if (w!=Image_Second.GetWidth()||h!=Image_Second.GetHeight())//�ж�����ͼ���С�Ƿ�һ��
		{
			MessageBox(_T("����ͼ���С��һ�£�"));
			//Image_Second.Destroy();
			return;
		}
		for (int j=0; j<h; j++)
			for (int i=0; i<w; i++)//����ͼ��������Ӳ���
			{
				m_Image.m_pBits[0][j][i]=m_Image.m_pBits[0][j][i]+Image_Second.m_pBits[0][j][i];
				m_Image.m_pBits[1][j][i]=m_Image.m_pBits[1][j][i]+Image_Second.m_pBits[1][j][i];
				m_Image.m_pBits[2][j][i]=m_Image.m_pBits[2][j][i]+Image_Second.m_pBits[2][j][i];
			}

		//�޸�Ϊ����ͼƬ��С��һ��Ҳ������
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
		//	for (int i=0; i<wSmaller; i++)//����ͼ��������Ӳ���
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
	if(m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	int w=m_Image.GetWidth();//���ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�

	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);

	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			m_ImageAfter.m_pBits[0][j][k]=255-m_ImageAfter.m_pBits[0][j][k];//B   ��ѭ������ͼ�������ֵ
			m_ImageAfter.m_pBits[1][j][k]=255-m_ImageAfter.m_pBits[1][j][k];//G
			m_ImageAfter.m_pBits[2][j][k]=255-m_ImageAfter.m_pBits[2][j][k];//R

		}
	}
	m_bIsProcessed = TRUE;
	Invalidate(1); //ǿ�Ƶ���ONDRAW������ONDRAW�����ͼ��
}


int CImage_ProcessingView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	//�򿪺��Զ�����һ��ͼ
	CString str("1.jpg");
	m_Image.Load(str);
	Invalidate(TRUE);
	return 0;
}


void CImage_ProcessingView::OnTogray()
{
	// TODO: Add your command handler code here
	if(m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	int w=m_Image.GetWidth();//���ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�
	if(m_ImageAfter.IsNull())
		m_Image.CopyTo(m_ImageAfter);
	UINT average = 0;
	for (int j=0;j<h;j++)
	{
		for (int k=0;k<w;k++)
		{
			average = (m_ImageAfter.m_pBits[0][j][k]+m_ImageAfter.m_pBits[1][j][k]+m_ImageAfter.m_pBits[2][j][k])/3;
			m_ImageAfter.m_pBits[0][j][k]=average;//B   ��ѭ������ͼ�������ֵ
			m_ImageAfter.m_pBits[1][j][k]=average;//G
			m_ImageAfter.m_pBits[2][j][k]=average;//R

		}
	}
	m_bIsGrayed = TRUE;
	m_bIsProcessed = TRUE;
	Invalidate(1); //ǿ�Ƶ���ONDRAW������ONDRAW�����ͼ��
}


void CImage_ProcessingView::OnResdwon()
{
	// TODO: Add your command handler code here
	if(m_Image.IsNull()) return;//�ж�ͼ���Ƿ�Ϊ�գ�����Կ�ͼ����в��������δ֪�Ĵ���

	int w=m_Image.GetWidth();//���ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�

	m_nResDownRate *=2;

	int w1 = w / m_nResDownRate;
	int h1 = h / m_nResDownRate;

	UINT rate = m_nResDownRate;

	//���ַ�����1.����w1,h1��ͼ������������������ټӱ������ж�
	//			2.ֱ�ӽ���w,h��ͼ������ɫ��255��䣬���������㷨�޷�����

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
	//		m_Image.m_pBits[0][j][k]=average;//B   ��ѭ������ͼ�������ֵ
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
		CString str("����ִ�лҶȻ���");
		MessageBox(str);
		return;
	}
	if (m_ImageAfter.IsNull())
		return;
	int w=m_Image.GetWidth();//���ͼ��Ŀ��
	int h=m_Image.GetHeight();//���ͼ��ĸ߶�

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
	Invalidate(1); //ǿ�Ƶ���ONDRAW������ONDRAW�����ͼ��

	// TODO: Add your command handler code here
}
