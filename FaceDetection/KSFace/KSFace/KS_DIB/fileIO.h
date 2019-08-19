//////////////////////////////////////////////////////////////////////
// fileIO.h: interface for the fileIO class.
//////////////////////////////////////////////////////////////////////
#ifndef __FILEIO_H__
#define __FILEIO_H__

#define COLOR_24	24
#define COLOR_32	32
#define COLOR_256	 8
#define COLOR_MONO	 8

#include "gfx_dc.h"

class AFX_EXT_CLASS fileIO
{
public:
	fileIO();
	virtual ~fileIO();

	virtual BOOL Open(gfx_dc* pdib, CString pathName);
	virtual BOOL Save(gfx_dc* pdib, CString pathName);
};
#endif
