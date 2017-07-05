#pragma once
class MyImage_
{
	friend class CCommon;
public:
	MyImage_(void);
	MyImage_(int w, int h); ////产生黑色
	MyImage_(int w, int h, COLORREF color); //产生单色
	MyImage_(const MyImage_ &srcImg); //拷贝构造函数
	MyImage_& MyImage_::operator=(MyImage_ &img);//拷贝赋值运算符
	~MyImage_(void);

	//TODO:有了这个之后，应该把m_pBits设置为private,但是之前写的太多了，没办法，就只能将就继续用了
	BYTE** operator[](int i) { return m_pBits[i]; };  //这里的问题是，返回的二维指针，就算设为const了，还是没法限制不被修改，没办法实现const版本


	int MyImage_::Load(LPCTSTR pszFileName);//加载图像，参数pszFileName是文件路径及文件名

	int  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest,
		_In_ int nDestWidth,
		_In_ int nDestHeight) const;//显示图像，hDestDC是绘图设备上下文的句柄，xDest,yDest是绘图左上角起始点的坐标，
									//nDestWidth是要绘的宽度，nDestHeight是要绘的高度

	int  MyImage_::Draw(
		_In_ HDC hDestDC,
		_In_ int xDest,
		_In_ int yDest) const;//显示图像，参数同上一个Draw函数;相当于上一个Draw后两个参数为图像的宽度和高度的情况

	int MyImage_::Save(
		_In_z_ LPCTSTR pszFileName,
		_In_ REFGUID guidFileType = GUID_NULL) const;//保存图像，pszFileName为路径名和文件名， guidFileType我没有了解过

	bool MyImage_::IsNull() const;//判断图像是否为空，如是空返回0
	void MyImage_::Destroy();//销毁MyImage_类加载到内存中的图像

	void Create(_In_ int width, _In_ int height, _In_ COLORREF color);
	void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara) const;
	void MyImage_::BorderFillTo(MyImage_ &img1, int nFillPara, int FillMode) const;
	void MyImage_::RemoveFillTo(MyImage_ &img1, int nFillPara) const;
	void MyImage_::CopyTo(MyImage_ &img) const;

	bool IsGrayed() const;
	void SetGrayed(bool flag = true);
	int MyImage_::GetWidth()const { return m_nWidth; };//返回图像的宽度
	int MyImage_::GetHeight() const { return m_nHeight; };//返回图像的高度
	void MyImage_::SetWidth(int w) { m_nWidth = w; };//返回图像的宽度
	void MyImage_::SetHeight(int h) { m_nHeight = h; };//返回图像的高度


private:
	bool m_bIsGrayed;
	int m_nHeight;
	int m_nWidth;
	BYTE*** m_pBits;//三维数组，用于保存图像的BGR像素值

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

