//////////////////////////////////////////////////////////////////////
// fileRAW.h: interface for the fileRAW class.
//////////////////////////////////////////////////////////////////////
#ifndef __FILERAW_H__
#define __FILERAW_H__

#include "fileIO.h"

class fileRAW:fileIO  
{
public:
	fileRAW();
	virtual ~fileRAW();

	BOOL Open(gfx_dc* pdib, CString pathName);
	BOOL Save(gfx_dc* pdib, CString pathName);
};
#endif
