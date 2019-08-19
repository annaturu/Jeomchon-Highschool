//////////////////////////////////////////////////////////////////////
// EGFace.cpp: implementation of the CEGFace class.
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "EGFace.h"

#include <math.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEGFace::CEGFace()
{
	m_eigenFace   =NULL;
	m_pTrainingSet=NULL;
	m_trainImgNum=0;

	m_distMin=0;
	m_distMax=0;
}

CEGFace::~CEGFace()
{
	if(m_eigenFace){
		delete []m_eigenFace;
		m_eigenFace=NULL;
	}

	if(m_pTrainingSet){
		delete []m_pTrainingSet;
		m_pTrainingSet=NULL;
	}
	m_trainImgNum=0;
}

void CEGFace::ResetAll()
{
	if(m_eigenFace){
		delete []m_eigenFace;
		m_eigenFace=NULL;
	}

	if(m_pTrainingSet){
		delete []m_pTrainingSet;
		m_pTrainingSet=NULL;
	}
	m_trainImgNum=0;
}

BOOL CEGFace::GetTrainingImg(BYTE* buff, int offset)
{
	if(m_trainImgNum==0 || m_trainImgNum<=offset){
		return FALSE;
	}
	
	m_pTrainingSet[offset].GetMatrix(buff);
	return TRUE;
}

BOOL CEGFace::PushTrainingImg(BYTE* buff, int w, int h)
{
	if(m_trainImgNum==0){
		m_pTrainingSet = new Matrix[1];
		m_pTrainingSet->SetMatrix(buff, w, h);
		
		m_trainImgNum++;
	}

	//////////////////////////////////////////////////////
	// ok push training image.
	//////////////////////////////////////////////////////
	else{
		//////////////////////////////////////////////////////
		// training image size must be same..
		//////////////////////////////////////////////////////
		CSize s = m_pTrainingSet[0].Size();

		if(s!=CSize(w, h)){
			TRACE("\nEGFace: training image size not equal\n");
			return FALSE;
		}
		
		//////////////////////////////////////////////////////
		// create new training image buffer
		//////////////////////////////////////////////////////
		Matrix* temp;
		temp = new Matrix[m_trainImgNum+1];

		for(int i=0; i<m_trainImgNum; i++){
			temp[i]=m_pTrainingSet[i];
		}

		temp[m_trainImgNum].SetMatrix(buff, w, h);
		m_trainImgNum++;

		if(m_pTrainingSet){
			delete []m_pTrainingSet;
			m_pTrainingSet=NULL;
		}
		
		//////////////////////////////////////////////////////
		// training image buffer resize.
		//////////////////////////////////////////////////////
		m_pTrainingSet = new Matrix[m_trainImgNum];

		for(i=0; i<m_trainImgNum; i++){
			m_pTrainingSet[i]=temp[i];
		}

		delete []temp;
		temp=NULL;
	}
	return TRUE;
}

BOOL CEGFace::CalculateEigenFace()
{
	CSize s = m_pTrainingSet[0].Size();
	
	int	pixels = s.cx *s.cy;
	int	imageN = m_trainImgNum;

	Matrix matA; 
	Matrix matT;
	Matrix matL;
	Matrix eigVL;
	Matrix eigVV;
	Matrix avrM;

	matA.SetMatrix(pixels, imageN);
	matT.SetMatrix(imageN, pixels);
	eigVL.SetMatrix(imageN, 1);
	eigVV.SetMatrix(imageN, imageN);

	avrM.SetMatrix(s.cx, s.cy);

	for(int index=0; index<imageN; index++){
		//////////////////////////////////////////////
		// cal average.
		//////////////////////////////////////////////
		double m=0;

		for(int k=0; k<pixels; k++){
			m = m + m_pTrainingSet[index].matrix[k];
		}
		m = m/pixels; 

		//////////////////////////////////////////////
		// cal Variance & Standard deviation.
		//////////////////////////////////////////////
		double st  =0; //Variance
		double stdd=0; //Standard deviation.
		for(k=0; k<pixels; k++){
			st = st + (m_pTrainingSet[index].matrix[k]-m)*
				      (m_pTrainingSet[index].matrix[k]-m);
		}
		st  = st/pixels; 
		stdd = sqrt(st); 

		//////////////////////////////////////////////
		// normalization.
		//////////////////////////////////////////////
		for(k=0; k<pixels; k++){
			double temp = (m_pTrainingSet[index].matrix[k]-m)*80.f/stdd+100.f;

			if(temp <   0) temp = 0;
			if(temp > 255) temp = 255;

			m_pTrainingSet[index].matrix[k] = temp;
			matA.matrix[index*pixels+k] = temp;
			avrM.matrix[k]+=temp;
		}
	}

	/////////////////////////////////////////////////////
	// cal average matrix.
	/////////////////////////////////////////////////////
	for(int i=0; i<pixels; i++){
		avrM.matrix[i] = avrM.matrix[i]/(float)imageN;
	}
	//////////////////////////////////////////////////////
	// now calculate covarance matrix.  (A^t and then L=A^t*A)
	//////////////////////////////////////////////////////
	matT.Transpose(matA);
	matL.Multiply(matA, matT);
	
	//////////////////////////////////////////////////////
	// calculate eigen vector and value.
	//////////////////////////////////////////////////////
	matL.MatEigenSD(imageN,        // matrix size (w == h)
				    eigVL.matrix,  // eigen value.
				    eigVV.matrix); // eigen vector.

	eigVV.TransposeQ();
	
	//////////////////////////////////////////////////////
	// sorting eigenvalue.
	//////////////////////////////////////////////////////
	int* idx;
	idx = new int[imageN];
	eigVL.SortIdx(idx);

	//////////////////////////////////////////////////////
	// calculate eigenvectors of C matrix.
	//////////////////////////////////////////////////////
	if(m_eigenFace){
		delete []m_eigenFace;
		m_eigenFace=NULL;
	}
	m_eigenFace = new Matrix[imageN];
	
	for( i=0; i <imageN; i++) {
		m_eigenFace[i].SetMatrix(s.cx, s.cy);

		Matrix temp;
		temp.SetMatrix(imageN, 1);

		for(int j =0; j<imageN; j++){
			temp.matrix[j] = eigVV.matrix[idx[i]*imageN+j];
		}

		Matrix r;
		r.SetMatrix(s.cx, s.cy);
		r.Multiply(temp, matA);

		for(j =0; j<pixels; j++){
			m_eigenFace[i].matrix[j]= r.matrix[j];
		}
		m_eigenFace[i].Normalize();

		///////////////////////////////////////////////////
		// save eigenface.
		///////////////////////////////////////////////////
		//CString fname;
		//fname.Format("EigenFaces\\eigenFace_%d.pgm", i);
		//m_eigenFace[i].SaveRAW(fname);
	}
	delete []idx;
	idx=NULL;

	//////////////////////////////////////////////////////???
	// calculate weight by dotproduct.
	// FinalData = RowFeatureVector * RowDataAdjust(matA)
	//////////////////////////////////////////////////////
	m_omega.SetMatrix(imageN, imageN);

	for(int y=0; y<imageN; y++){
		for(int x=0; x<imageN; x++){
			double weight=0;

			for(int k=0; k<pixels; k++){
				weight += matA.matrix[y*pixels+k]*m_eigenFace[x].matrix[k];
			}
			m_omega.matrix[y*imageN+x]=weight;
		}
	}
	return TRUE; 
}

BOOL CEGFace::InCommingInputImage(BYTE* data, int w, int h)
{
	m_distMin=0;
	m_distMax=0;

	if(m_trainImgNum==0){
		TRACE("\nTraining image number zero\n");
		return FALSE;
	}

	CSize s = m_pTrainingSet[0].Size();

	if(s.cx!=w || s.cy!=h){
		TRACE("\nTraining image and Input image must be same size\n");
		return FALSE;
	}
	
	int	pixels = s.cx *s.cy;
	int	imageN = m_trainImgNum;

	Matrix newImage;
	Matrix pare;

	pare.SetMatrix(imageN, 1);
	newImage.SetMatrix(data, w, h);
	
	///////////////////////////////////////////////////
	// cal average.
	///////////////////////////////////////////////////
	double m=0;

	for(int k=0; k<pixels; k++){
		m = m+newImage.matrix[k];
	}
	m = m/pixels;

	//////////////////////////////////////////////
	// cal Variance & Standard deviation.
	//////////////////////////////////////////////
	double st  =0; //Variance
	double stdd=0; //Standard deviation.

	for(k=0; k<pixels; k++){
		st = st + (newImage.matrix[k]-m)*(newImage.matrix[k]-m);
	}
	st = st/pixels;
	stdd = sqrt(st);

	//////////////////////////////////////////////
	// normalize input image.
	//////////////////////////////////////////////
	for(k=0; k<pixels; k++){
		newImage.matrix[k] = (newImage.matrix[k]-m)*80.f/stdd+100.f;
	}

	//////////////////////////////////////////////////////
	// calculate weight by dotproduct.
	// FinalData = RowFeatureVector * RowDataAdjust(eigenface)
	//////////////////////////////////////////////////////
	Matrix averageM;
	averageM.SetMatrix(s.cx, s.cy);

	for(int y=0; y<imageN; y++){
		double sum=0;

		for(int x=0; x<pixels; x++){
			sum = sum + (newImage.matrix[x] * m_eigenFace[y].matrix[x]);
			averageM.matrix[x] += m_pTrainingSet[y].matrix[x];
		}
		pare.matrix[y] = sum;
	}
	
	///////////////////////////////////////////////////////////////////////////
	// generate matrix A [pixels X imageN]
	// cal reconstructed image [ must be training set same size. ] matA * pare.
	///////////////////////////////////////////////////////////////////////////
	Matrix temp;
	temp.SetMatrix(imageN, pixels);

	Matrix reConstruct;
	reConstruct.SetMatrix(s.cx, s.cy);

	for(int i=0; i<pixels; i++){	
		for(int j=0; j<imageN; j++){
			temp.matrix[i*imageN+j] = m_eigenFace[j].matrix[i];
			reConstruct.matrix[i]  += temp.matrix[i*imageN+j] * pare.matrix[j];
		}
		averageM.matrix[i]=averageM.matrix[i]/(float)imageN;
	}
	
	//////////////////////////////////////////////////////////////////////
	// cal difference..
	//////////////////////////////////////////////////////////////////////
	Matrix difference;
	difference = newImage;
	difference-= averageM;

	////////////////////////////////////////////////////////////////////
	// cal input image of weight.
	////////////////////////////////////////////////////////////////////
	Matrix wInputImage;
	wInputImage.SetMatrix(imageN, 1);

	for(int j=0; j<imageN; j++){
		double sum =0;

		for(int i=0; i<pixels; i++){
			sum = sum+(m_eigenFace[j].matrix[i]*difference.matrix[i]);
		}
		wInputImage.matrix[j] = sum;
	}
	
	////////////////////////////////////////////////////////////////////
	// euclidean distance.
	////////////////////////////////////////////////////////////////////
	m_euclidean.SetMatrix(imageN, 1);

	for( j=0; j<imageN; j++){
		double temp=0;

		for(int i=0; i<imageN; i++){
			temp += (wInputImage.matrix[i]-m_omega.matrix[j*imageN+i])*
				    (wInputImage.matrix[i]-m_omega.matrix[j*imageN+i]);
		}
		m_euclidean.matrix[j] = sqrt(temp);
	}

	m_euclidean.GetMinMax(&m_distMin, &m_distMax);
	return TRUE;
}
