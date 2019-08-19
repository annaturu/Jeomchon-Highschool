//////////////////////////////////////////////////////////////////////
// EGFace.h: interface for the CEGFace class.
//////////////////////////////////////////////////////////////////////
#ifndef __EGFACE_H__
#define __EGFACE_H__

#include "Matrix.h"

class CEGFace  
{
public:
	CEGFace();
	virtual ~CEGFace();

	BOOL	GetTrainingImg(BYTE* buff, int offset);
	BOOL	PushTrainingImg(BYTE* buff, int w, int h);
	BOOL	CalculateEigenFace();
	void	ResetAll();
	BOOL	InCommingInputImage(BYTE* data, int w, int h);

public:
	double	m_distMin;
	double	m_distMax;
	Matrix  m_euclidean;

private:
	Matrix* m_eigenFace;
	Matrix*	m_pTrainingSet;	
	Matrix	m_omega;
	int		m_trainImgNum;
};

#endif
