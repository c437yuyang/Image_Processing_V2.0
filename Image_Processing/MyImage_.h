#pragma once
class MyImage_
{
	friend class CCommon;
public:
	MyImage_(void);
	MyImage_(int w, int h); ////������ɫ
	MyImage_(int w, int h, COLORREF color); //������ɫ
	MyImage_(const MyImage_ &srcImg); //�������캯��
	MyImage_& MyImage_::operator=(MyImage_ &img);//������ֵ�����
	~MyImage_(void);

	//TODO:�������֮��Ӧ�ð�m_pBits����Ϊprivate,����֮ǰд��̫���ˣ�û�취����ֻ�ܽ��ͼ�������
	BYTE** operator[](int i) { return m_pBits[i]; };  //����������ǣ����صĶ�άָ�룬������Ϊconst�ˣ�����û�����Ʋ����޸ģ�û�취ʵ��const�汾


	int MyImage_::Load(LPCTSTR pszFileName);//����ͼ�񣬲���pszFileName���ļ�·�����ļ���

	int  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest,
		_In_ int nDestWidth,
		_In_ int nDestHeight) const;//��ʾͼ��hDestDC�ǻ�ͼ�豸�����ĵľ����xDest,yDest�ǻ�ͼ���Ͻ���ʼ������꣬
									//nDestWidth��Ҫ��Ŀ�ȣ�nDestHeight��Ҫ��ĸ߶�

	int  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest) const;//��ʾͼ�񣬲���ͬ��һ��Draw����;�൱����һ��Draw����������Ϊͼ��Ŀ�Ⱥ͸߶ȵ����

	int MyImage_::Save(
		_In_z_ LPCTSTR pszFileName,
		_In_ REFGUID guidFileType = GUID_NULL) const;//����ͼ��pszFileNameΪ·�������ļ����� guidFileType��û���˽��

	bool MyImage_::IsNull() const;//�ж�ͼ���Ƿ�Ϊ�գ����ǿշ���0
	void MyImage_::Destroy();//����MyImage_����ص��ڴ��е�ͼ��

	void Create(_In_ int width, _In_ int height, _In_ COLORREF color);
	void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara) const;
	void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara, int FillMode) const;
	void MyImage_::RemoveFillTo(MyImage_ &img1, int nFillPara) const;
	void MyImage_::CopyTo(MyImage_ &img) const;

	bool IsGrayed() const;
	void SetGrayed(bool flag = true);
	int MyImage_::GetWidth()const { return m_nWidth; };//����ͼ��Ŀ��
	int MyImage_::GetHeight() const { return m_nHeight; };//����ͼ��ĸ߶�
	void MyImage_::SetWidth(int w) { m_nWidth = w; };//����ͼ��Ŀ��
	void MyImage_::SetHeight(int h) { m_nHeight = h; };//����ͼ��ĸ߶�


private:
	bool m_bIsGrayed;
	int m_nHeight;
	int m_nWidth;
	BYTE*** m_pBits;//��ά���飬���ڱ���ͼ���BGR����ֵ

	void CreateCImage() const;
	static CImage s_CImage;
public:
	enum DrawResult { DRAW_FAIL, DRAW_SUCCESS };
	enum SaveResult { SAVE_FAIL, SAVE_SUCCESS };
	enum LoadResult { LOAD_FAIL, LOAD_SUCCESS };
	enum FillMode { FILL_BLACK, FILL_WHITE, FILL_COPY };
	//typedef enum DrawResult DrawResult;
	//typedef enum SaveResult SaveResult;
	//typedef enum LoadResult LoadResult;

};

