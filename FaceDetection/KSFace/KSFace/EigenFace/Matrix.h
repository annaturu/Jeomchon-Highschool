//////////////////////////////////////////////////////////////////////
// Matrix.h: interface for the Matrix class.
//////////////////////////////////////////////////////////////////////
#ifndef __MATRIX_H__
#define __MATRIX_H__

#define EPS 0.000000001f
#define ROTATE(a, i, j, k, l, n) g=a[i*n+j]; h=a[k*n+l]; a[i*n+j] = g-s*(h+g*tau); \
  a[k*n+l] = h+s*(g-h*tau)

class AFX_EXT_CLASS Matrix  
{
public:
	Matrix();
	virtual ~Matrix();

	int		MatEigenSD(int n, double* d, double* v);
	void	Normalize();
	void	SortIdx(int* index);
	void	TransposeQ();
	void	Multiply(const Matrix& m1, const Matrix& m2);
	void	Transpose(const Matrix& m);
	void	SetMatrix(BYTE* data, int _w, int _h);
	void	SetMatrix(double* data, int _w, int _h);
	void	SetMatrix(int _w, int _h);
	void	GetMatrix(BYTE*   data);
	void	GetMatrix(double* data);
	void    GetMinMax(double* pMin, double* pMax);
	Matrix&	GetMatrix();
	void	DeleteMatrix();
	void	CopyTo(Matrix m);

	void	SaveTXT(CString fname);
	void	SaveRAW(CString fname);

	Matrix &operator *= (const Matrix&);

	// assignment operators
	Matrix &operator =  (const Matrix&);
	Matrix &operator += (const Matrix&);
	Matrix &operator -= (const Matrix&);
	Matrix &operator /= (double);
	Matrix &operator *= (double);

	//BOOL    operator == (const float f) const;
	//BOOL    operator != (const float f) const;

	// inline helper..
public:
	inline BOOL CheckVisible();
	inline CSize Size();

private:
	inline BOOL CheckEqualWH();
	inline BOOL CheckWH(const Matrix *pMatrix);
	inline BOOL CheckMulWH(const Matrix *pM1, const Matrix *pM2);
	inline BOOL CheckWH(int w, int h);
	inline BOOL CheckWH_HW(const Matrix *pMatrix);
	
public:
	double*  matrix;

private:
	int		 w;
	int		 h;
};

inline CSize Matrix::Size() 
{ 
	return CSize(w, h); 
}

inline BOOL Matrix::CheckVisible() 
{ 
	return ((w!=0) && (h!=0)); 
}

inline BOOL Matrix::CheckEqualWH() 
{ 
	return (h==w);
}

inline BOOL Matrix::CheckWH_HW(const Matrix *pMatrix) 
{ 
	return ((w==pMatrix->h)); 
}

inline BOOL Matrix::CheckWH(const Matrix *pMatrix) 
{ 
	return (( w== pMatrix->w) && (h== pMatrix->h)); 
}

inline BOOL Matrix::CheckMulWH(const Matrix *pM1, const Matrix *pM2) 
{ 
	return (( pM1->h== pM2->w) || (pM2->h==pM1->w)); 
}

inline BOOL Matrix::CheckWH(int w, int h)
{ 
	return ((this->w==w) && (this->h==h)); 
}
#endif
