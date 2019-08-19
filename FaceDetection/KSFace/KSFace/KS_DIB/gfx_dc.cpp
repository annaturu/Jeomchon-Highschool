//////////////////////////////////////////////////////////////////////
// gfx_dc.cpp: implementation of the gfx_dc class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#pragma hdrstop
//////////////////////////////////////////////////////////////////////
#include "gfx_dc.h"
#include "fileIO.h"
#include "Assembler.h"
#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
gfx_dc::gfx_dc()
{
	m_w  =0;
	m_h  =0; 
	m_img.hbm=0;
	m_img.pxs=NULL;

	ZeroMemory(&m_img.bif, sizeof(BITMAPINFO));
}

gfx_dc::gfx_dc(int _w, int _h)
{
	m_img.pxs=NULL;
	m_img.hbm=NULL;
	ZeroMemory(&m_img.bif, sizeof(BITMAPINFO));

	create(_w, _h);
}

gfx_dc::~gfx_dc()
{
	destroy();
}

void gfx_dc::destroy()
{
	m_img.pxs=NULL;

	if(m_img.hbm){
		DeleteObject(m_img.hbm);
		m_img.hbm=NULL;
	}
	m_w=0;
	m_h=0;

	ZeroMemory(&m_img.bif, sizeof(BITMAPINFO));
}

BOOL gfx_dc::create(CString pathName)
{
	fileIO f;
	BOOL bsuccess=f.Open(this, pathName);
	
	return bsuccess;
}

BOOL gfx_dc::create(int _w, int _h)
{
	m_w = _w;
	m_h = _h;

	m_img.bif.bmiHeader.biSize			=sizeof(BITMAPINFOHEADER);
	m_img.bif.bmiHeader.biWidth			=_w;
	m_img.bif.bmiHeader.biHeight		=-_h;
	m_img.bif.bmiHeader.biPlanes		=1;
	m_img.bif.bmiHeader.biBitCount		=32;
	m_img.bif.bmiHeader.biCompression	=BI_RGB;
	m_img.bif.bmiHeader.biSizeImage		=_w*_h*4;

	m_img.hbm=CreateDIBSection(NULL,
		                       &m_img.bif,
							   DIB_RGB_COLORS,
							   (void**)&m_img.pxs,
							   NULL, NULL);
	
	create_font(&m_img.fnt);
	if(!m_img.hbm){
		return FALSE;
	}
	
	return TRUE;
}

void gfx_dc::create_font(LOGFONT* font)
{
	font->lfHeight         = 0;
	font->lfWidth          = 0;
	font->lfEscapement     = 0;
	font->lfOrientation	   = 0;
	font->lfWeight		   = FW_MEDIUM;
	font->lfItalic		   = FALSE;
	font->lfUnderline	   = FALSE;
	font->lfStrikeOut	   = FALSE;
	font->lfCharSet        = DEFAULT_CHARSET;
	font->lfOutPrecision   = OUT_CHARACTER_PRECIS;
	font->lfClipPrecision  = CLIP_CHARACTER_PRECIS;
	font->lfQuality		   = DEFAULT_QUALITY;
	font->lfPitchAndFamily = DEFAULT_PITCH|FF_DONTCARE;
	sprintf( font->lfFaceName, "Verdana" );
}

void gfx_dc::update(CDC* pdc)
{
	SetDIBitsToDevice(pdc->m_hDC,
					  0, 0, 
					  m_w, m_h,
					  0, 0,
					  0, m_h,
					  m_img.pxs,
					  &m_img.bif, 
					  DIB_RGB_COLORS);
}

void gfx_dc::update(CDC* pdc, int x, int y)
{
	SetDIBitsToDevice(pdc->m_hDC,
					  x, y, 
					  m_w, m_h,
					  0, 0,
					  0, m_h,
					  m_img.pxs,
					  &m_img.bif, 
					  DIB_RGB_COLORS);
}

void gfx_dc::update_stretch(CDC* pdc, int x, int y, int w, int h)
{
	SetStretchBltMode(pdc->m_hDC, COLORONCOLOR);
	StretchDIBits(pdc->m_hDC,
		          0, 0,
				  w, h,
				  0, 0,
				  m_w, m_h,
				  m_img.pxs,
				  &m_img.bif,
				  DIB_RGB_COLORS,
				  SRCCOPY);
}

void* gfx_dc::get_lppixels()
{
	return (void*)m_img.pxs;
}

void gfx_dc::clear(BYTE r, BYTE g, BYTE b, BYTE a)
{
	long	totalSize=(m_w*m_h);
	if (!totalSize) return;

	BYTE	*buf=(BYTE *)m_img.pxs;
	DWORD	color=((0&0xff)<<24)|((r&0xff)<<16)|((g&0xff)<<8)|(b&0xff);

	_asm
	{
		mov	esi, buf
		mov	edi, esi
		mov	ecx, totalSize

	CLEAR_CUSTOM:

		lodsd
		mov		eax,color
		stosd
		loop	CLEAR_CUSTOM
	}
}

void gfx_dc::getpixelXY(int x, int y, BYTE& _r, BYTE& _g, BYTE& _b)
{
	if (x>=m_w|| y>=m_h|| x<0 || y<0){
		_r=0;
		_g=0;
		_b=0;
	}

	int t = m_w << 2;
	BYTE *dst = (BYTE *)m_img.pxs + y * t + ( x << 2 );

	_b=*(dst);
	_g=*(dst+1);
	_r=*(dst+2);
}

void gfx_dc::inverse(int x, int y, int w, int h)
{
	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	int t = (m_w-w) << 2;
	BYTE *dst = (BYTE *)m_img.pxs + ((y*m_w+x)<<2);

	while(h--){
		for(int i=0; i<w; i++){
			dst[0]=~dst[0];
			dst[1]=~dst[1];
			dst[2]=~dst[2];
			dst+=4;
		}
		dst+=t;
	}
}

void gfx_dc::fill(BYTE r, BYTE g, BYTE b)
{
	DWSet(m_img.pxs, 
		  BGR(b, g, r), 
		  m_w*m_h);
}

void gfx_dc::fill_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b)
{
	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	int t = m_w << 2;
	BYTE *dst = (BYTE *)m_img.pxs + y * t + ( x << 2 );
	COLORREF color = BGR ( b, g, r );

	while ( h-- )
	{
		DWSet ( dst, color, w );

		dst += t;
	}
}

void gfx_dc::fill_glass( BYTE r, BYTE g, BYTE b, BYTE a)
{
	BYTE *dst = (BYTE *)m_img.pxs;
	int len = m_w*m_h;

	while(len--){
		dst[0]=(((b-dst[0])*a)+(dst[0]<<8))>>8;
		dst[1]=(((g-dst[1])*a)+(dst[1]<<8))>>8;
		dst[2]=(((r-dst[2])*a)+(dst[2]<<8))>>8;
		dst+=4;
	}
}

void gfx_dc::fill_glass50( BYTE r, BYTE g, BYTE b)
{
	int len = m_w*m_h;
	COLORREF* dst = (COLORREF*)m_img.pxs;
	COLORREF color = BGR(b, g, r);

	color >>= 1;
	color &= 0x007F7F7F;
		
	while(len--){
		*dst=((*dst>>1)&0x007F7F7F)+color;
		dst++;
	}
}

void gfx_dc::fill_glass_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	int t = (m_w-w) << 2;
	BYTE *dst = (BYTE *)m_img.pxs + ((y*m_w+x)<<2);

	while(h--){
		for(int i=0; i<w; i++){
			dst[0]=(((b-dst[0])*a)+(dst[0]<<8))>>8;
			dst[1]=(((g-dst[1])*a)+(dst[1]<<8))>>8;
			dst[2]=(((r-dst[2])*a)+(dst[2]<<8))>>8;
			dst+=4;
		}
		dst+=t;
	}
}

void gfx_dc::fill_glass50_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	COLORREF* dst = (COLORREF*)m_img.pxs+y*m_w+x;
	COLORREF color = BGR(b, g, r);

	color >>= 1;
	color &= 0x007F7F7F;

	while(h--){
		for(int i=0; i<w; i++){
			dst[i]=((dst[i]>>1)&0x007F7F7F)+color;
		}
		dst+=m_w;
	}
}

void gfx_dc::rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	line(x,   y,   x+w, y,   r, g, b, a);
	line(x+w, y,   x+w, y+h, r, g, b, a);
	line(x+w, y+h,  x, y+h,  r, g, b, a);
	line(x,   y+h,  x, y,    r, g, b, a);
}

void gfx_dc::rect(CRect rect, BYTE r, BYTE g, BYTE b, BYTE a)
{
	int x=rect.left;
	int y=rect.top;
	
	if(x>rect.right ) x=rect.right; 
	if(y>rect.bottom) y=rect.bottom; 

	int w=abs(rect.Width());
	int h=abs(rect.Height());

	if ( !clip_xy_wh( x, y, w, h ) )
		return;

	line(x,   y,   x+w, y,   r, g, b, a);
	line(x+w, y,   x+w, y+h, r, g, b, a);
	line(x+w, y+h,  x, y+h,  r, g, b, a);
	line(x,   y+h,  x, y,    r, g, b, a);
}

void gfx_dc::copy_to_bitmap(CDC* pdc, CBitmap* pbitmap)
{
	SetDIBits(pdc->m_hDC,
		      (HBITMAP)(*pbitmap),
			  0,
			  m_h,
			  m_img.pxs,
			  &m_img.bif,
			  DIB_RGB_COLORS);
}

void gfx_dc::copy_from_bitmap(CDC* pdc, CBitmap* pbitmap)
{
	GetDIBits(pdc->m_hDC,
		      (HBITMAP)(*pbitmap),
			  0,
			  m_h,
			  m_img.pxs,
			  &m_img.bif,
			  DIB_RGB_COLORS);
}

void gfx_dc::copyFrom_image(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));
	
	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = m_w*m_h;

	while(len--){
		dst[0] = (BYTE)(src[0]);
		dst[1] = (BYTE)(src[1]);
		dst[2] = (BYTE)(src[2]);

		dst += 4;
		src += 3;
	}
}

void gfx_dc::copyFrom_grayImage(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));
	
	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = m_w*m_h;

	while(len--){
		dst[0] = (BYTE)(src[0]);
		dst[1] = (BYTE)(src[0]);
		dst[2] = (BYTE)(src[0]);

		dst += 4;
		src ++;
	}
}


void gfx_dc::copy_to_image_rect(BYTE* img, int x, int y, int w, int h)
{
	ASSERT(clip_xy_wh( x, y, w, h));

	int t = (m_w-w) << 2;
	BYTE *dst = (BYTE *)img;
	BYTE *src = (BYTE *)m_img.pxs+((y*m_w+x)<<2);
	
	while(h--){
		for(int i=0; i<w; i++){
			dst[0] = (BYTE)(src[0]);
			dst[1] = (BYTE)(src[1]);
			dst[2] = (BYTE)(src[2]);
			dst += 3;
			src += 4;
		}
		src += t;
	}
}


void gfx_dc::copy_to_image_rect_gray(BYTE* img, int x, int y, int w, int h)
{
	ASSERT(clip_xy_wh( x, y, w, h));

	int t = (m_w-w) << 2;
	BYTE *dst = (BYTE *)img;
	BYTE *src = (BYTE *)m_img.pxs+((y*m_w+x)<<2);
	
	while(h--){
		for(int i=0; i<w; i++){
			*dst= (BYTE)((src[0]+src[1]+src[2])/3.f);
			dst++;
			src += 4;
		}
		src += t;
	}
}


void gfx_dc::copy_to_DC_rect(CDC* pdc, int x, int y)
{
	SetDIBitsToDevice(pdc->m_hDC,
		              x, y,
					  m_w, m_h,
					  0, 0, 
					  0, m_h,
					  m_img.pxs,
					  &m_img.bif,
					  DIB_RGB_COLORS);
}

void gfx_dc::copy_from_DC_rect(CDC* pdc, int x, int y, int w, int h)
{
	ASSERT(check_wh(w, h));

	CDC bufferDC;
	CBitmap  bufferBitmap;
	CBitmap* oldBitmap;

	bufferDC.CreateCompatibleDC(pdc);
	bufferBitmap.CreateCompatibleBitmap(pdc, w, h);

	oldBitmap=bufferDC.SelectObject(&bufferBitmap);

	bufferDC.FillSolidRect(0, 0, m_w, m_h, 0);
	bufferDC.BitBlt(0, 0, m_w, m_h, pdc, x, y, SRCCOPY);

	GetDIBits(pdc->m_hDC,
		      (HBITMAP)(bufferBitmap),
			  0,
			  m_h,
			  m_img.pxs,
			  &m_img.bif,
			  DIB_RGB_COLORS);
	
	bufferDC.SelectObject(oldBitmap);
}

void gfx_dc::copy_from_DC(CDC* pdc, int w, int h)
{
	ASSERT(check_wh(w, h));

	CDC bufferDC;
	CBitmap  bufferBitmap;
	CBitmap* oldBitmap;

	bufferDC.CreateCompatibleDC(pdc);
	bufferBitmap.CreateCompatibleBitmap(pdc, w, h);

	oldBitmap=bufferDC.SelectObject(&bufferBitmap);

	bufferDC.FillSolidRect(0, 0, m_w, m_h, 0);
	bufferDC.BitBlt(0, 0, m_w, m_h, pdc, 0, 0, SRCCOPY);

	GetDIBits(pdc->m_hDC,
		      (HBITMAP)(bufferBitmap),
			  0,
			  m_h,
			  m_img.pxs,
			  &m_img.bif,
			  DIB_RGB_COLORS);
	
	bufferDC.SelectObject(oldBitmap);
}

void gfx_dc::copy_to_this(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));

	DWCopy(pdib->m_img.pxs, 
		   m_img.pxs, 
		   m_w*m_h);
}


void gfx_dc::copy_from_this(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));

	DWCopy(m_img.pxs, 
		   pdib->m_img.pxs, 
		   m_w*m_h);
}
/*

void gfx_dc::copy_from_this(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));
	
	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)pdib->m_img.pxs;
	int len = m_w*m_h;

	while(len--){
		dst[0] = (BYTE)(src[0]);
		dst[1] = (BYTE)(src[1]);
		dst[2] = (BYTE)(src[2]);

		dst += 4;
		src += 4;
	}
}
*/

void gfx_dc::copy_from_this_rect(gfx_dc* pdib, int x, int y)
{
	if(!clip_xy_wh(x, y, pdib->m_w, pdib->m_h))		return;

	int t = (m_w - pdib->m_w) << 2;
	BYTE *dst = (BYTE *)m_img.pxs + ( ( y * m_w + x ) << 2 );
	BYTE *src = (BYTE *)pdib->m_img.pxs;
	
	int th = pdib->m_h;

	while(th--){
		for(int i=0; i<pdib->m_w; i++){
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
			dst += 4;
			src += 4;
		}
		dst += t;
	}
}

void gfx_dc::copy_form_thisCK(gfx_dc* pdib, BYTE r, BYTE g, BYTE b)
{
	ASSERT(check_wh(pdib));

	DWCopyCK(m_img.pxs,
		     pdib->m_img.pxs,
			 m_w*m_h,
			 BGR(b,g,r));
}

void gfx_dc::copy_form_thisAlpha(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));
	ASSERT(FALSE);
}

void gfx_dc::add_this(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));

	BYTE* dst=(BYTE*)m_img.pxs;
	BYTE* src=(BYTE*)pdib->m_img.pxs;
	int len = m_w*m_h;
	WORD sum;

	while(len--){
		sum=src[0]+dst[0];
		dst[0]=(BYTE)(sum>255 ? 255:sum);

		sum=src[1]+dst[1];
		dst[1]=(BYTE)(sum>255 ? 255:sum);

		sum=src[2]+dst[2];
		dst[2]=(BYTE)(sum>255 ? 255:sum);

		src+=4;
		dst+=4;
	}
}

void gfx_dc::blend_this(gfx_dc *pdib, BYTE a )
{
	ASSERT (check_wh(pdib));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)pdib->m_img.pxs;
	int len = m_w*m_h;
		
	while(len--){
		dst[0] = (BYTE)(( ( src[0] - dst[0] ) * a + ( dst[0] << 8 ) ) >> 8);
		dst[1] = (BYTE)(( ( src[1] - dst[1] ) * a + ( dst[1] << 8 ) ) >> 8);
		dst[2] = (BYTE)(( ( src[2] - dst[2] ) * a + ( dst[2] << 8 ) ) >> 8);

		dst += 4;
		src += 4;
	}
}

void gfx_dc::blend50_this(gfx_dc *pdib )
{
	ASSERT(check_wh(pdib));

	COLORREF *dst = (COLORREF*)m_img.pxs;
	COLORREF *src = (COLORREF*)pdib->m_img.pxs;
	int len = m_w*m_h;
		
	while ( len--){
		*dst = ( ( *dst >> 1 ) & 0x007F7F7F ) + ( ( *src >> 1 ) & 0x007F7F7F );	
		dst++;
		src++;
	}
}

void gfx_dc::darken_this(gfx_dc* pdib)
{
	ASSERT(check_wh(pdib));

	BYTE* dst=(BYTE*)m_img.pxs;
	BYTE* src=(BYTE*)pdib->m_img.pxs;
	long  len = m_w*m_h;

	while(len--){
		if(src[0]<dst[0]) dst[0]=src[0];
		if(src[1]<dst[1]) dst[1]=src[1];
		if(src[2]<dst[2]) dst[2]=src[2];

		dst+=4;
		src+=4;
	}
}

void gfx_dc::resample(int tw, int th)
{
	gfx_dc temp;
	temp.create(tw, th);

	float dx=(float)(m_w/tw);
	float dy=(float)(m_h/th);

	for(int y=0; y<th; y++){
		for(int x=0; x<tw; x++){
			float sumr=0.f;
			float sumg=0.f;
			float sumb=0.f;

			int sx = (int)(x*dx-dx);
			int ex = (int)(x*dx+dx);
			int sy = (int)(y*dy-dy);
			int ey = (int)(y*dy+dy);
			
			int div =0;

			for(int y2=sy; y2<=ey; y2++){
				for(int x2=sx; x2<=ex; x2++){
					if (x2>=0&&x2<m_w&&y2>=0&&y2<m_h){
					sumr+=m_img.pxs[x2+y2*m_w].r;
					sumg+=m_img.pxs[x2+y2*m_w].g;
					sumb+=m_img.pxs[x2+y2*m_w].b;
					div++;
					}
				}
			}
			if(div==0){
				sumr=m_img.pxs[sx+sy*m_w].r;
				sumg=m_img.pxs[sx+sy*m_w].g;
				sumb=m_img.pxs[sx+sy*m_w].b;
				div=1;
			}
			temp.m_img.pxs[x+y*tw].r=(BYTE)(sumr/div);
			temp.m_img.pxs[x+y*tw].g=(BYTE)(sumg/div);
			temp.m_img.pxs[x+y*tw].b=(BYTE)(sumb/div);
		}
	}

	create(tw, th);
	copy_from_this(&temp);
	temp.destroy();
}

void gfx_dc::line(int x1, int y1, int x2, int y2, int R, int G, int B, int A)
{
	int d, x, y, ax, ay, sx, sy, dx, dy;
	COLORREF Color=RGB ( B, G, R );
	BYTE *dst=(BYTE *)m_img.pxs;

	dx=x2-x1;
	ax=abs ( dx )<<1;
	sx=(dx<0) ? -1 : 1;
	dy=y2-y1;
	ay=abs ( dy )<<1;
	sy=(dy<0) ? -1 : 1;
	x=x1;
	y=y1;

	if ( ax>ay )
	{
		d=ay-(ax>>1);
		while ( x!=x2 )
		{
			if (x>=0 && x<m_w && y>=0 && y<m_h)
			{
				dst[ (y*m_w+x)<<2   ]=(BYTE)(((B-dst[ (y*m_w+x)<<2   ])*A+(dst[ (y*m_w+x)<<2   ]<<8))>>8);
				dst[((y*m_w+x)<<2)+1]=(BYTE)(((G-dst[((y*m_w+x)<<2)+1])*A+(dst[((y*m_w+x)<<2)+1]<<8))>>8);
				dst[((y*m_w+x)<<2)+2]=(BYTE)(((R-dst[((y*m_w+x)<<2)+2])*A+(dst[((y*m_w+x)<<2)+2]<<8))>>8);
			}
			if ( d>=0 )
			{
				y+=sy;
				d-=ax;
			}
			x+=sx;
			d+=ay;
		}
	}

	else
	{
		d=ax-(ay>>1);
		while ( y!=y2 )
		{
			if (x>=0 && x<m_w && y>=0 && y<m_h)
			{
				dst[(y*m_w+x)<<2]=(BYTE)(((B-dst[(y*m_w+x)<<2])*A+(dst[(y*m_w+x)<<2]<<8))>>8);
				dst[((y*m_w+x)<<2)+1]=(BYTE)(((G-dst[((y*m_w+x)<<2)+1])*A+(dst[((y*m_w+x)<<2)+1]<<8))>>8);
				dst[((y*m_w+x)<<2)+2]=(BYTE)(((R-dst[((y*m_w+x)<<2)+2])*A+(dst[((y*m_w+x)<<2)+2]<<8))>>8);
			}
			if ( d>=0 )
			{
				x+=sx;
				d-=ay;
			}
			y+=sy;
			d+=ax;
		}
	}
}

void gfx_dc::grayTo32BitColor(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++)
	{	
		dst[0] = img[offset];
		dst[1] = img[offset];
		dst[2] = img[offset];
		dst += 4;
	}
}

void gfx_dc::setRBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[2]=src[0];

		dst += 4;
		src++;
	}
}

void gfx_dc::setGBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[1]=src[0];

		dst += 4;
		src++;
	}
}

void gfx_dc::setBBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[0]=src[0];

		dst += 4;
		src++;
	}
}

void gfx_dc::filterRBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[0]=0;
		dst[1]=0;
		dst[2]=src[0];

		dst += 4;
		src++;
	}
}

void gfx_dc::filterGBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[0]=0;
		dst[1]=src[0];
		dst[2]=0;

		dst += 4;
		src++;
	}
}

void gfx_dc::filterBBuffer(BYTE* img, int w, int h)
{
	ASSERT(check_wh(w, h));

	BYTE *dst = (BYTE *)m_img.pxs;
	BYTE *src = (BYTE *)img;
	int len = w*h;
		
	for(long offset=0; offset<len; offset++){	
		dst[0]=src[0];
		dst[1]=0;
		dst[2]=0;

		dst += 4;
		src++;
	}
}

CSize gfx_dc::Text(CString fontType, CDC *pDC, CString text, int x, int y, long r, long g, long b, long size, bool flagOutline, int angle)
{
	CSize area;
	CDC dc;
	dc.CreateCompatibleDC(pDC);

	HBITMAP *old=(HBITMAP*)dc.SelectObject(m_img.hbm);

	if (fontType=="") sprintf( m_img.fnt.lfFaceName, "Verdana" ); 
	else              sprintf( m_img.fnt.lfFaceName, fontType  );

	m_img.fnt.lfHeight=size;

	CFont font;

	m_img.fnt.lfEscapement=angle;
	
	font.CreateFontIndirect(&m_img.fnt);
	CFont *oldFont=dc.SelectObject(&font);
	dc.SetBkMode(TRANSPARENT);
	
	if (flagOutline){
		dc.SetTextColor( RGB(0,0,0) );
		dc.TextOut( x-1,y-1, text );
		dc.TextOut( x  ,y-1, text );
		dc.TextOut( x+1,y-1, text );
		dc.TextOut( x-1,y  , text );
		dc.TextOut( x+1,y  , text );
		dc.TextOut( x-1,y+1, text );
		dc.TextOut( x  ,y+1, text );
		dc.TextOut( x+1,y+1, text );
	}

	dc.SetTextColor(RGB(r,g,b) );
	dc.TextOut( x,y, text );
	area = dc.GetTextExtent(text);

	dc.SelectObject(oldFont);
	font.DeleteObject();
	dc.SelectObject(old);

	//sprintf( m_image.FONT.lfFaceName, "Verdana" );
	return area;
}

void gfx_dc::glassRect_Target(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh( x, y, w, h ) )		return;

	int t = ( m_w - w ) << 2;
	BYTE *dst = (BYTE *)m_img.pxs + ( ( y * m_w + x ) << 2 );
	
	int th = h;

	while ( h-- )
	{
		for ( int i = 0; i < w; i++ )
		{
			dst[0] = (BYTE)(( ( b- dst[0] ) * a + ( dst[0] << 8 ) ) >> 8);
			dst[1] = (BYTE)(( ( g- dst[1] ) * a + ( dst[1] << 8 ) ) >> 8);
			dst[2] = (BYTE)(( ( r- dst[2] ) * a + ( dst[2] << 8 ) ) >> 8);
			dst += 4;
		}
		dst += t;
	}
	
	int wr = r+90; if(wr>255) wr=255; // lighten color R
	int wg = g+90; if(wg>255) wg=255; // lighten color G
	int wb = b+90; if(wb>255) wb=255; // lighten color B
	
	int dr = r-90; if(dr<0) dr=0;     // darken color R
	int dg = g-90; if(dg<0) dg=0;     // darken color G
	int db = b-90; if(db<0) db=0;     // darken color B

	int ddr = dr-10; if(ddr<0) ddr=0; // double darken color R
	int ddg = dg-10; if(ddg<0) ddg=0; // double darken color G
	int ddb = db-10; if(ddb<0) ddb=0; // double darken color B

	int da = a+50; if(da > 255) da=255; // more alpha value

	//////////////////////// rect /////////////////////////////////////////
	line(x,   y,   x+w, y,     (BYTE)wr, (BYTE)wg, (BYTE)wb, da);
	line(x+w, y,   x+w, y+th,  (BYTE)dr, (BYTE)dg, (BYTE)db, da);
	line(x+w, y+th,  x, y+th,  (BYTE)dr, (BYTE)dg, (BYTE)db, da);
	line(x,   y+th,  x, y,     (BYTE)wr, (BYTE)wg, (BYTE)wb, da);

	////////////////////////  boundary /////////////////////////////////////
	line(x-1,   y-1, x+6  , y-1, ddr, ddg, ddb, 255);	line(x-1,   y-1, x-1,  y+6, ddr, ddg, ddb, 255);
	line(x-2,   y-2, x+5  , y-2, r, g, b, 255);	line(x-2,   y-2, x-2,  y+5, r, g, b, 255);
	
	line(x+w+2, y-1, x+w-5, y-1, ddr, ddg, ddb, 255);	line(x+w+2, y-1, x+w+2,y+6, ddr, ddg, ddb, 255);
	line(x+w+1, y-2, x+w-6, y-2, r, g, b, 255);	line(x+w+1, y-2, x+w+1,y+5, r, g, b, 255);

	line(x-1,   y+th+2, x+6  , y+th+2, ddr, ddg, ddb, 255);	line(x-1,   y+th+2, x-1,  y+th-5, ddr, ddg, ddb, 255);
	line(x-2,   y+th+1, x+5  , y+th+1, r, g, b, 255);	line(x-2,   y+th+1, x-2,  y+th-6, r, g, b, 255);

	line(x+w+2, y+th+2, x+w-5, y+th+2, ddr, ddg, ddb, 255);	line(x+w+2, y+th+2, x+w+2,y+th-5, ddr, ddg, ddb, 255);
	line(x+w+1, y+th+1, x+w-6, y+th+1, r, g, b, 255);	line(x+w+1, y+th+1, x+w+1,y+th-6, r, g, b, 255);

	//////////////////////// cross coord ///////////////////////////////////
	line(x+(w/2)+1, y+(th/2)-4, x+(w/2)+1, y+(th/2)+6, ddr, ddg, ddb, 255);
	line(x+(w/2)-4, y+(th/2)+1, x+(w/2)+6, y+(th/2)+1, ddr, ddg, ddb, 255);
	
	line(x+(w/2), y+(th/2)-5, x+(w/2), y+(th/2)+5, r, g, b, 255);
	line(x+(w/2)-5, y+(th/2), x+(w/2)+5, y+(th/2), r, g, b, 255);
}

void gfx_dc::fill_glassRect_emboss(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh ( x, y, w, h ) )		return;

	int t = ( m_w - w ) << 2;
	BYTE *dst = (BYTE *)m_img.pxs + ( ( y * m_w + x ) << 2 );
	
	int th = h;

	while ( h-- )
	{
		for ( int i = 0; i < w; i++ )
		{
			dst[0] = (BYTE)(( ( b- dst[0] ) * a + ( dst[0] << 8 ) ) >> 8);
			dst[1] = (BYTE)(( ( g- dst[1] ) * a + ( dst[1] << 8 ) ) >> 8);
			dst[2] = (BYTE)(( ( r- dst[2] ) * a + ( dst[2] << 8 ) ) >> 8);
			dst += 4;
		}
		dst += t;
	}
	
	int wr = r+90; if(wr>255) wr=255; // lighten color R
	int wg = g+90; if(wg>255) wg=255; // lighten color G
	int wb = b+90; if(wb>255) wb=255; // lighten color B
	
	int dr = r-90; if(dr<0) dr=0;     // darken color R
	int dg = g-90; if(dg<0) dg=0;     // darken color G
	int db = b-90; if(db<0) db=0;     // darken color B

	int ddr = dr-10; if(ddr<0) ddr=0; // double darken color R
	int ddg = dg-10; if(ddg<0) ddg=0; // double darken color G
	int ddb = db-10; if(ddb<0) ddb=0; // double darken color B

	int da = a+50; if(da > 255) da=255; // more alpha value

	//////////////////////// rect /////////////////////////////////////////
	line(x,   y,   x+w, y,     (BYTE)wr, (BYTE)wg, (BYTE)wb, da);
	line(x+w, y,   x+w, y+th,  (BYTE)dr, (BYTE)dg, (BYTE)db, da);
	line(x+w, y+th,  x, y+th,  (BYTE)dr, (BYTE)dg, (BYTE)db, da);
	line(x,   y+th,  x, y,     (BYTE)wr, (BYTE)wg, (BYTE)wb, da);
}

void gfx_dc::circle(int cx, int cy, int offset, int rl, BYTE r, BYTE g, BYTE b, BYTE a)
{
	if ( !clip_xy_wh( cx, cy, rl, rl))
		return;

	int theta = int(360.f/offset);
	
	int oldx = (int)(cx+sin(RADIAN(0))*rl);
	int oldy = (int)(cy+cos(RADIAN(0))*rl);

	for(int angle=theta; angle<360; angle+=theta){
		int x=(int)(cx+sin(RADIAN(angle))*rl);
		int y=(int)(cy+cos(RADIAN(angle))*rl);

		line(oldx, oldy, x, y, r, g, b, a);
		oldx = x;
		oldy = y;
	}
}