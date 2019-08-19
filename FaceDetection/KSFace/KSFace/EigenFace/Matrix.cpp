//////////////////////////////////////////////////////////////////////
// Matrix.cpp: implementation of the Matrix class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Matrix.h"

#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Matrix::Matrix()
{
	matrix = NULL;
	w =0;
	h =0;
}

Matrix::~Matrix()
{
	if(matrix){
		delete []matrix;
		matrix = NULL;
	}
	w =0;
	h =0;
}

void Matrix::Normalize()
{
	ASSERT(CheckVisible());
	
	double sum=0;

	for(int x=0; x<w*h; x++){
		sum = sum+ (matrix[x]*matrix[x]);
	}

	if(sum){
		double temp = sqrt(sum);

		for(x=0; x<w*h; x++){
			matrix[x]/=temp;
		}
	}
}

void Matrix::DeleteMatrix()
{
	if(matrix){
		delete []matrix;
		matrix = NULL;
	}
	w =0;
	h =0;
}

void Matrix::GetMinMax(double* pMin, double* pMax)
{
	ASSERT(CheckVisible());

	double min = matrix[0];
	double max = matrix[0];

	for(int x=0; x<w*h; x++){
		if(min > matrix[x]) min = matrix[x];
		if(max < matrix[x]) max = matrix[x];
	}

	*pMin = min;
	*pMax = max;
}

void Matrix::SortIdx(int* index)
{
	int s = w*h;

	//init index
	for(int i=0; i < s; i++)
		index[i] = i;

	//sort
	for (i=0; i < s; i++) {
		double max = matrix[index[i]];
		int maxIndex = i;

		for(int j=i+1; j<s; j++){
			if(max < matrix[index[j]]){
				maxIndex = j;
				max = matrix[index[j]];
			}
		}

		if(maxIndex != i){
			j = index[i];
			index[i] = index[maxIndex];
			index[maxIndex] = j;
		}
	}
}

void Matrix::TransposeQ()
{
	ASSERT(CheckEqualWH());
	
	for(int i=0; i < w; i++) { 
		for(int j=i+1; j < w; j++) {
			double tmp = matrix[i*w+j];
			matrix[i*w+j] = matrix[j*w+i];
			matrix[j*w+i] = tmp;
		}
	}
}

void Matrix::Transpose(const Matrix& m)
{
	SetMatrix(m.h, m.w);
	
	double	*dst = matrix;
	double  *src = m.matrix;

	for(int i=0; i<m.h; i++){
		for(int j=0; j<m.w; j++){
			
			dst[j*w+i] = src[i*m.w+j];
		}
	}
}

void Matrix::Multiply(const Matrix& m1, const Matrix& m2)
{
	SetMatrix(m1.h, m2.w);

	double	*s1 = m1.matrix;
	double  *s2 = m2.matrix;

	for(int i=0; i < w; i++) {
		for(int j=0; j < h; j++) {
			matrix[i*w+j] = 0;

			for(int k=0; k < m2.h; k++){
				matrix[i*w+j] += s1[i*m1.w+k] * s2[k*m2.w+j];
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////
// computes eigenvalues and vectors of a symmetric matrix 
// columns of matVectors contain the eigenvectors
//
// a - matrix
// n - dimension of matrix
// d - array for eigenvvalues
// v - array for eigenvectors (each vector occupies a column)
//////////////////////////////////////////////////////////////////////////////////////
int Matrix::MatEigenSD(int n, double* d, double* v)
{
	int j,iq,ip,ip_times_n,i ;
	double tresh,theta,tau,t,sm,s,h,g,c,*b,*z;
	int nrotint, *nrot = &nrotint;
	
	b = (double *)malloc(n*sizeof(double));
	z = (double *)malloc(n*sizeof(double));
	
	for(ip_times_n=0, ip=0; ip<n; ++ip, ip_times_n+=n)  
	{
		
		/* Initialize the identity matrix */
		for(iq=0; iq<n; ++iq)v[ip_times_n + iq] = 0.0 ;
		v[ip_times_n + ip] = 1.0 ;
		
		/* Initailize b and d to be diagonal of a */
		b[ip] = d[ip] = matrix[ip_times_n + ip];
		z[ip] = 0.0 ;
	}
	
	*nrot = 0 ;
	for(i=0;i<50;++i)
	{
		/* Sum off-diagonal elements */
		sm=0.0 ;
		
		for(ip_times_n=0,ip=0;ip<n-1;ip++,ip_times_n+=n)
			for(iq=ip+1;iq<n;iq++)
				sm += (double)fabs(matrix[ip_times_n + iq]);
			
			/*  If we have converged,  free the working vectors and return. */
			if(sm == 0.0)
			{
				free(b);
				free(z);
				return -1;
			}
			
			/* tresh is different on first three iterations...*/
			tresh=(i<3) ? 0.2f*sm/(n*n) : 0.0f ;
			
			for(ip_times_n=0,ip=0;ip<n-1;ip++,ip_times_n+=n){
				for(iq=ip+1;iq<n;++iq){
					g=100.0f*(double)fabs(matrix[ip_times_n + iq]);
					
					/* After four sweeps, skip the rotation if the off-diagonal element is small */
					/* This test is taken directly from the text and looks a little suspect to me... */
					
					if(i > 3 && g < EPS)
						matrix[ip_times_n + iq] = 0.0 ;
					
					else if(fabs(matrix[ip_times_n+iq]) > tresh){
						h=d[iq]-d[ip];
						if(g < EPS)
							t = (fabs(matrix[ip_times_n+iq]) > EPS) ? (matrix[ip_times_n+iq])/h : 0.0f ; 
						
						else{ 
							theta=(fabs(h) < EPS) ? 0.0f : 0.5f*h/(matrix[ip_times_n+iq]);
							t=1.0f/((double)fabs(theta)+(double)sqrt(1.0f+theta*theta));
							if(theta < 0.0f)
								t = -t ; 
						} 

						c=1.0f/(double)sqrt(1.0f+t*t);
						s=t*c;
						tau=s/(1.0f+c);
						
						h=t*matrix[ip_times_n+iq];
						z[ip] -= h;
						z[iq] += h;
						d[ip] -= h;
						d[iq] += h;
						matrix[ip_times_n+iq]=0.0;
						
						for(j=0;j<ip;j++){
							ROTATE(matrix,j,ip,j,iq,n);
						}

						for(j=ip+1;j<iq;j++){
							ROTATE(matrix,ip,j,j,iq,n);
						}

						for(j=iq+1;j<n;j++){
							ROTATE(matrix,ip,j,iq,j,n);
						}

						for(j=0;j<n;j++){
							ROTATE(v,j,ip,j,iq,n);
						}
						++(*nrot);
					}
				}
			}
			for(ip=0;ip<n;++ip)
			{
				b[ip] += z[ip];
				d[ip]=b[ip];
				z[ip]=0.0;
			}
	}
	
	/* Failed to converge!! What to do ??? */
	/* Well, let's at least free up memory and return without a murmur */
	
	free(b);
	free(z);
	return 0;
}

/* computes m x n * n x p matrix multiply */
/////////////////////////////////////////////////
// must be A(w,h)  B(w,h) ----> A.w == B.h
/////////////////////////////////////////////////
Matrix &Matrix::operator *= (const Matrix& m)
{
	ASSERT(CheckWH_HW(&m));
	
	Matrix temp;
	temp.SetMatrix(w, m.h);

	for(int i=0; i< w; i++){
		for(int j=0; j< w; j++){
			
			for(int k=0; k< h; k++){
				temp.matrix[i*w+j]+=matrix[i*w+j]*m.matrix[k*w+j];
			}
		}
	}
	SetMatrix(w, m.h);
	*this=temp;

	temp.DeleteMatrix();
	return *this;
}

Matrix &Matrix::operator += (const Matrix& m)
{
	ASSERT(CheckWH(&m));

	double	*dst = matrix;
	double  *src = m.matrix;
	
	for(int i=0; i<w*h; i++){
		dst[i] = dst[i] + src[i];
	}

	return *this;	
}

Matrix &Matrix::operator = (const Matrix& m)
{
	SetMatrix(m.w, m.h);

	memcpy(matrix, 
		   m.matrix, 
		   sizeof(double)*w*h);

	return *this;	
}

Matrix &Matrix::operator -= (const Matrix& m)
{
	ASSERT(CheckWH(&m));

	double	*dst = matrix;
	double  *src = m.matrix;
	
	for(int i=0; i<w*h; i++){
		dst[i] = dst[i] - src[i];
	}

	return *this;	
}

Matrix &Matrix::operator /= (double f)
{
	for(int i=0; i<w*h; i++){
		matrix[i] = matrix[i] / f;
	}

	return *this;	
}

Matrix &Matrix::operator *= (double f)
{
	for(int i=0; i<w*h; i++){
		matrix[i] = matrix[i] * f;
	}

	return *this;	
}

void Matrix::GetMatrix(BYTE* data)
{
	ASSERT(CheckVisible());

	for(int i=0; i<w*h; i++){
		data[i] = (BYTE)matrix[i];
	}
}

void Matrix::GetMatrix(double* data)
{
	ASSERT(CheckVisible());

	memcpy(data, 
		   matrix, 
		   sizeof(double)*w*h);
}

Matrix &Matrix::GetMatrix()
{
	return *this;
}

void Matrix::SetMatrix(BYTE* data, int _w, int _h)
{
	DeleteMatrix();

	w = _w;
	h = _h;

	matrix = new double[w*h];
	
	for(int i=0; i<w*h; i++){
		matrix[i]=(BYTE)data[i];
	}
}

void Matrix::SetMatrix(double* data, int _w, int _h)
{
	DeleteMatrix();

	w = _w;
	h = _h;

	matrix = new double[w*h];
	
	memcpy(matrix, 
		   data, 
		   sizeof(double)*w*h);
}

void Matrix::SetMatrix(int _w, int _h)
{
	DeleteMatrix();

	w = _w;
	h = _h;

	matrix = new double[w*h];
	
	memset(matrix, 
		   0, 
		   sizeof(double)*w*h);
}

void Matrix::SaveTXT(CString fname)
{
	FILE* fp;
	
	fp = fopen(fname, "w");

	for(int y=0; y<h; y++){
		for(int x=0; x<w; x++){
			fprintf(fp, "\t%.3f ", (float)matrix[y*w+x]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

void Matrix::SaveRAW(CString fname)
{
	FILE* fp;
	
	if((fp = fopen(fname, "wb"))==FALSE){
		AfxMessageBox("Matrix: file or folder not founded");
		fclose(fp);
		return;
	}

	fprintf(fp, "%s\n", "P5");
	fprintf(fp, "%d\n", w);
	fprintf(fp, "%d\n", h);
	fprintf(fp, "%d\n", 255);
	
	double max = -99999999999;
	double min =  99999999999;

	for(int y=0; y<w*h; y++){
		if(matrix[y] > max) max = matrix[y];
		if(matrix[y] < min) min = matrix[y];
	}

	double t = max-min;
	
	if(t<0) t *=-1;

	BYTE* temp;
	temp = new BYTE[w*h];

	for(y=0; y<w*h; y++){
		double k = matrix[y]-min;
		double r = k / t * 255.f;
		
		temp[y] = (BYTE)r;
	}

	fwrite(temp, 1, w*h, fp);
	delete []temp;

	fclose(fp);
}
