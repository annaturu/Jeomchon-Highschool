//////////////////////////////////////////////////////////////////////
// fileTGA.h: interface for the fileTGA class.
//////////////////////////////////////////////////////////////////////
#ifndef __FILETGA_H__
#define __FILETGA_H__

#include "fileIO.h"

class fileTGA:fileIO
{
public:
	fileTGA();
	virtual ~fileTGA();

	BOOL Open(gfx_dc* pdib, CString pathName);
	BOOL Save(gfx_dc* pdib, CString pathName);
};

#endif
