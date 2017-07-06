#pragma once
class MyImage_
{

public:
	enum DrawResult { DRAW_FAIL, DRAW_SUCCESS };
	enum SaveResult { SAVE_FAIL, SAVE_SUCCESS };
	enum LoadResult { LOAD_FAIL, LOAD_SUCCESS };
	enum FillMode { FILL_BLACK, FILL_WHITE, FILL_COPY }; //ö��Ҫд��ǰ�棬���������

public:
	MyImage_(void);
	MyImage_(int w, int h); ////������ɫ
	MyImage_(int w, int h, COLORREF color); //������ɫ
	MyImage_(const MyImage_ &srcImg); //�������캯��
	MyImage_& MyImage_::operator=(MyImage_ &img);//������ֵ�����
	~MyImage_(void);

	LoadResult Load(LPCTSTR pszFileName);//����ͼ�񣬲���pszFileName���ļ�·�����ļ���

	DrawResult  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest,
		_In_ int nDestWidth,
		_In_ int nDestHeight) const;//��ʾͼ��hDestDC�ǻ�ͼ�豸�����ĵľ����xDest,yDest�ǻ�ͼ���Ͻ���ʼ������꣬
									//nDestWidth��Ҫ��Ŀ�ȣ�nDestHeight��Ҫ��ĸ߶�

	DrawResult  Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest) const;//��ʾͼ�񣬲���ͬ��һ��Draw����;�൱����һ��Draw����������Ϊͼ��Ŀ�Ⱥ͸߶ȵ����

	SaveResult Save(
		_In_z_ LPCTSTR pszFileName,
		_In_ REFGUID guidFileType = GUID_NULL) const;//����ͼ��pszFileNameΪ·�������ļ����� guidFileType��û���˽��

	bool IsNull() const;//�ж�ͼ���Ƿ�Ϊ�գ����ǿշ���0
	void Destroy();//����MyImage_����ص��ڴ��е�ͼ��

	void Create(_In_ int width, _In_ int height, _In_ COLORREF color);
	void BorderFillTo(MyImage_ &img1, int nFillPara) const;
	void BorderFillTo(MyImage_ &img1, int nFillPara, FillMode fm) const;
	void RemoveFillTo(MyImage_ &img1, int nFillPara) const;
	void CopyTo(MyImage_ &img) const;

	bool IsGrayed() const;
	void SetGrayed(bool flag = true);
	int GetWidth()const { return m_nWidth; };//����ͼ��Ŀ��
	int GetHeight() const { return m_nHeight; };//����ͼ��ĸ߶�
	void SetWidth(int w) { m_nWidth = w; };
	void SetHeight(int h) { m_nHeight = h; };
	BYTE &at(int i, int j, int ch) { return m_pBits[i*GetWidth() * 3 + j * 3 + ch]; }
	const BYTE at(int i, int j, int ch)const { return m_pBits[i*GetWidth() * 3 + j * 3 + ch]; }

private:
	bool m_bIsGrayed;
	int m_nHeight;
	int m_nWidth;

	BYTE* m_pBits;

	void CreateCImage() const;
	static CImage s_CImage;

};

