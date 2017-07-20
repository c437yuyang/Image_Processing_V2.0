#include "stdafx.h"
#include "ImageScaleViewer.h"
#include "Interpolation.h" //插值算法所在头文件
MyImage_::DrawResult ImageScaleViewer::Draw(HDC hdc)
{
	if (m_img == NULL) return MyImage_::DRAW_FAIL;
	if (!m_bNeedToUpdateImage) return MyImage_::DRAW_SUCCESS;
	m_bNeedToUpdateImage = false;
	if (m_dScale != 1.0)
	{
		//先将图像调整到m_imgAfterScaling中
		m_imgAfterScaling.Create(
			static_cast<int>(m_img->GetWidth() * m_dScale), 
			static_cast<int>(m_img->GetHeight()*m_dScale),
			0);
		Interpolate::PicZoom_ThreeOrder0
		(m_img->data(),
			m_imgAfterScaling.data(),
			m_img->GetWidth(), m_img->GetHeight(),
			m_imgAfterScaling.GetWidth(),
			m_imgAfterScaling.GetHeight());
		return m_imgAfterScaling.Draw(hdc, 0, 0);
	}
	else
	{
		return m_img->Draw(hdc, 0, 0);
	}
	


}
