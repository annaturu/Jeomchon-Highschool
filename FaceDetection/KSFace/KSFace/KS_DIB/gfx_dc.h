//////////////////////////////////////////////////////////////////////
// gfx_dc.h: interface for the gfx_dc class.
//////////////////////////////////////////////////////////////////////
#ifndef __GFX_DC__
#define __GFX_DC__

#define PI 3.14159265358979323846
#define RADIAN(deg)			   (((float)deg)/180.0f*PI)
#define BGR(b, g ,r)  ((DWORD) (((BYTE) (b) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (r)) << 16)))

typedef struct _PIXEL{
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
}PIXEL;

typedef struct _IMAGE{
	PIXEL*		pxs;
	HBITMAP		hbm;
	BITMAPINFO	bif;
	LOGFONT		fnt;
}IMAGE;

class AFX_EXT_CLASS gfx_dc  
{
public:
	gfx_dc();
	gfx_dc(int _w, int _h);
	virtual ~gfx_dc();;	
	void	destroy();

	void	update(CDC* pdc);
	void	update(CDC* pdc, int x, int y);
	void	update_stretch(CDC* pdc, int x, int y, int w, int h);
	BOOL	create(int _w, int _h);
	BOOL	create(CString pathName);

//painter
	void	clear(BYTE r, BYTE g, BYTE b, BYTE a);
	void	inverse(int x, int y, int w, int h);
	void	circle(int cx, int cy, int offset, int rl, BYTE r, BYTE g, BYTE b, BYTE a);
	void	rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a);
	void	rect(CRect rect, BYTE r, BYTE g, BYTE b, BYTE a);
	void	fill(BYTE r, BYTE g, BYTE b);
	void	fill_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b);
	void	fill_glass( BYTE r, BYTE g, BYTE b, BYTE a);
	void	fill_glass50( BYTE r, BYTE g, BYTE b);
	void	fill_glass_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a);
	void	fill_glass50_rect(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a);
	void	fill_glassRect_emboss(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a);
	void	line(int x1, int y1, int x2, int y2, int R, int G, int B, int A);
	void	glassRect_Target(int x, int y, int w, int h, BYTE r, BYTE g, BYTE b, BYTE a);

	void	copyFrom_grayImage(BYTE* img, int w, int h);
	void	copy_from_this_rect(gfx_dc* pdib, int x, int y);
	void	copy_to_image_rect_gray(BYTE* img, int x, int y, int w, int h);
	void	copy_to_image_rect(BYTE* img, int x, int y, int w, int h);
	void	copyFrom_image(BYTE* img, int w, int h);
	void	copy_to_bitmap(CDC* pdc, CBitmap* pbitmap);
	void	copy_from_bitmap(CDC* pdc, CBitmap* pbitmap);
	void	copy_to_DC_rect(CDC* pdc, int x, int y);
	void	copy_from_DC(CDC* pdc, int w, int h);
	void	copy_from_DC_rect(CDC* pdc, int x, int y, int w, int h);
	void	copy_to_this(gfx_dc* pdib);
	void	copy_from_this(gfx_dc* pdib);
	void	copy_form_thisCK(gfx_dc* pdib, BYTE r, BYTE g, BYTE b);
	void	copy_form_thisAlpha(gfx_dc* pdib);
	void	add_this(gfx_dc* pdib);

	void	blend_this(gfx_dc *pdib, BYTE a);
	void	blend50_this(gfx_dc *pdib);
	void	darken_this(gfx_dc* pdib);

	void	resample(int w, int h);
	void	grayTo32BitColor(BYTE* img, int w, int h);

	void	setRBuffer(BYTE* img, int w, int h);
	void	setGBuffer(BYTE* img, int w, int h);
	void	setBBuffer(BYTE* img, int w, int h);

	void	filterRBuffer(BYTE* img, int w, int h);
	void	filterGBuffer(BYTE* img, int w, int h);
	void	filterBBuffer(BYTE* img, int w, int h);
	CSize	Text(CString fontType, 
		         CDC *pDC, 
				 CString text, 
				 int x, int y, 
				 long r, long g, long b, 
				 long size, 
				 bool flagOutline, int angle);

//helper
	void	getpixelXY(int x, int y, BYTE& _r, BYTE& _g, BYTE& _b);
	void*	get_lppixels();
	inline	CSize get_size();
	inline	BOOL check_w ( int w ); 
	inline	BOOL check_w ( gfx_dc *pdib ); 
	inline	BOOL check_h ( int h );
	inline	BOOL check_h ( gfx_dc *pdib ); 
	inline	BOOL check_wh ( int w, int h ); 
	inline	BOOL check_wh ( gfx_dc *pdib ); 
	inline	void resize ( int w, int h ); 
	inline	void resize ( gfx_dc *pdib ); 
	inline	BOOL is_valid_x ( int x );
	inline	BOOL is_valid_y ( int y );
	inline	void clip_x ( int &x );
	inline	void clip_y ( int &y );
	inline	BOOL clip_xy_wh ( int &x, int &y, int &w, int &h );
	inline	BOOL clip_x1_y1_x2 ( int &x1, int &y, int &x2 );
	inline	BOOL clip_x_y1_y2 ( int &x, int &y1, int &y2 );

public:
	IMAGE	m_img;
	int		m_w;
	int		m_h;

private:
	void create_font(LOGFONT* font);
};

inline CSize gfx_dc::get_size()
{
	return CSize(m_w, m_h);
}

// Clipping
inline BOOL gfx_dc::check_w ( int w ) 
{ 
	return ( m_w == w ); 
};

inline BOOL gfx_dc::check_w ( gfx_dc *pdib ) 
{ 
	return ( m_w == pdib->m_w); 
};

inline BOOL gfx_dc::check_h ( int h ) 
{ 
	return ( m_h == h ); 
};

inline BOOL gfx_dc::check_h ( gfx_dc *pdib ) 
{ 
	return ( m_h == pdib->m_h ); 
};

inline BOOL gfx_dc::check_wh ( int w, int h ) 
{ 
	return ( ( m_w == w ) && ( m_h == h ) ); 
};

inline BOOL gfx_dc::check_wh ( gfx_dc *pdib ) 
{ 
	return ( ( m_w == pdib->m_w ) && ( m_h == pdib->m_h ) ); 
};

inline void gfx_dc::resize ( int w, int h ) 
{ 
	if ( !check_wh ( w, h ) )
		VERIFY ( create ( w, h ) );
};

inline void gfx_dc::resize ( gfx_dc *pdib ) 
{ 
	if ( !check_wh ( pdib ) ) 
		VERIFY ( create ( pdib->m_w, pdib->m_h ) ); 
};

inline BOOL gfx_dc::is_valid_x ( int x )
{
	return ( ( x >= 0 ) && ( x < m_w ) );
}

inline BOOL gfx_dc::is_valid_y ( int y )
{
	return ( ( y >= 0 ) && ( y < m_h ) );
}

inline void gfx_dc::clip_x ( int &x )
{
	if ( x < 0 )
		x = 0;

	if ( x >= m_w )
		x = m_w - 1;
}

inline void gfx_dc::clip_y ( int &y )
{
	if ( y < 0 )
		y = 0;

	if ( y >= m_h )
		y = m_h - 1;
}

inline BOOL gfx_dc::clip_xy_wh ( int &x, int &y, int &w, int &h )
{
	if ( ( x + w ) >= m_w ) 
		w = m_w - x;

	if ( x < 0 ) 
	{
		w += x;
		x = 0;
	}

	if ( ( y + h ) >= m_h )
		h = m_h - y;

	if ( y < 0 )
	{
		h += y;
		y = 0;
	}

	return ( ( w > 0 ) && ( h > 0 ) );
}

inline BOOL gfx_dc::clip_x1_y1_x2 ( int &x1, int &y, int &x2 )
{
	if ( x2 > m_w )
		x2 = m_w - 1;

	if ( x1 < 0 )
		x1 = 0;

	return ( ( x1 != x2 ) && is_valid_y ( y ) );
}

inline BOOL gfx_dc::clip_x_y1_y2 ( int &x, int &y1, int &y2 )
{
	if ( y2 > m_h )
		y2 = m_h - 1;

	if ( y1 < 0 )
		y1 = 0;

	return ( ( y1 != y2 ) && is_valid_x ( x ) );
}

#endif 