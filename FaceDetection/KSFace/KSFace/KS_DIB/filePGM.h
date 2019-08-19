//////////////////////////////////////////////////////////////////////
// filePGM.h: interface for the filePGM class.
//////////////////////////////////////////////////////////////////////
#ifndef __FILEPGM_H__
#define __FILEPGM_H__

#include "fileIO.h"

class filePGM:fileIO
{
public:
	filePGM();
	virtual ~filePGM();

	BOOL Open(gfx_dc* pdib, CString pathName);
	BOOL Save(gfx_dc* pdib, CString pathName);
};

#endif
