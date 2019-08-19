// fileBMP.h: interface for the fileBMP class.
//////////////////////////////////////////////////////////////////////
#ifndef __FILEBMP_H__
#define __FILEBMP_H__

#include "fileIO.h"

class fileBMP:fileIO
{
public:
	fileBMP();
	virtual ~fileBMP();

	BOOL Open(gfx_dc* pdib, CString pathName);
	BOOL Save(gfx_dc* pdib, CString pathName);
};

#endif
