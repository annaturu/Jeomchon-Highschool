//////////////////////////////////////////////////////////////////////
// PGM.h: interface for the CPGM class.
//////////////////////////////////////////////////////////////////////
#ifndef __PGM_H__
#define __PGM_H__

class CPGM  
{
public:
	CPGM();
	virtual ~CPGM();

	BOOL  Save(CString pathName);
	BOOL  Open(CString pathName);
	BOOL  SetPGM(BYTE* buf, int w, int h);
	CSize GetSize();
	BYTE* GetBuffer();

private:
	BYTE* m_img;
	int	  m_w;
	int   m_h;
};

#endif
