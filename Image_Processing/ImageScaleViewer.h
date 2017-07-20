#pragma once
#include "stdafx.h"
#include "MyImage_.h"

class ImageScaleViewer {

public:
	ImageScaleViewer(double dScale) :m_dScale(dScale), m_img(NULL), m_bNeedToUpdateScaleImage(false) {}
	void SetImage(const MyImage_& src) { m_img = &src; }
	void SetScale(double scale) { m_dScale = scale; }
	double GetScale() { return m_dScale; }
	void SetNeedToUpdateScaleImage() { m_bNeedToUpdateScaleImage = true; }
	MyImage_::DrawResult Draw(HDC hdc);

private:
	const MyImage_ *m_img;
	MyImage_ m_imgAfterScaling;
	double m_dScale;
	bool m_bNeedToUpdateScaleImage; //��ϵ�����ӷ����仯��ʱ�򣬲������������������Ҫ���´������ŵ�ͼ��
};