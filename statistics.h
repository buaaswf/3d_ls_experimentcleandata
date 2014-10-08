#ifndef SWF_STATISTICS_H
#define SWF_STATISTICS_H

#include "vol_math_LevelSet.h"
#include <queue>
#include "test.h"
#include "DivideRegion.h"
#include <iostream>
#include <crtdbg.h> 
#include "CImg.h" 
#include "ThreeDim_LevelSet.h"
#include "Filter.h"
#include "WipeNioisePde.h"
#include "string.h"
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#ifdef _WIN32
#include <Windows.h>
#include <strsafe.h>
#else
#include <dirent.h>
#endif
#define output "D:\\swfdata20140420res\\polyp\\" 
//#define output "K:\\20140404\\" 
//#define input1  "L:\\sdfdata2\\edt\\20140409edt\\"	//thickness uint8	//edt		//float
////swf 20140409 delete for float2char
//#define input2  "K:\\sdf\\volume\\clean\\clean\\ep\\20140410\\"//short
////#define input2 "K:\\20140404\\inner\\"
//#define input3  "K:\\skeleton\\"  //skeleton uint8 //unsigned char
//#define input1  "F:\\data\\skeleton-edt\\"				//float
//#define input2	"E:\\volume\\skeletono\\"		//short
//#define input3 "F:\\data\\skeleton-s\\"   //unsigned char
//#define input1 "L:\\sdfdata2\\edt\\20140414edt\\"

//#define input1 "D:\\swfdata20140420res\\auto\\outer\\"//GAC levelset data
//# define input1 "L:\\sdfdata2\\inner\\"
//#define input2 "E:\\volume\\segmention\\rate\\"
//# define input2 "F:\\data\\dirty\\bone\\"K:\sdf\volume\clean\clean\ep\clean
//#define input2 "K:\\sdf\\volume\\clean\\clean\\ep\\clean\\"
// roc����GAC��FP.TPֵ�õ�
// #define input1 "D:\\swfdata20140420res\\roc3041\\"
//#define input2 "D:\\segdata\\people\\roc3041\\" //people data roc 3041
//#define input3 "K:\\sdf\\volume\\clean\\clean\\ep\\clean\\roc3041\\"
//#define  input3 "L:\\sdfdata2\\edt\\20140414skeleton\\"
//#define input1 "D:\\swfdata20140420res\\auto\\edt\\"
//#define input2 "K:\\sdf\\volume\\clean\\clean\\ep\\clean\\"
//#define input3 "D:\\swfdata20140420res\\auto\\skeleton\\"
/*
35-44divide region dir
#define input1 "L:\\sdfdata2\\edt\\35-44edt\\"
#define input2 "K:\\sdf\\volume\\clean\\clean\\ep\\clean\\"
#define input3 "K:\\skeleton\\35-44\\"
*/
//people divide region dir
//#define input1 "D:\\segdata\\people\\thickness\\edt\\"
//#define input1 "L:\\sdfdata2\\inner\\"
#define input1 "E:\\SEG\\32-3124inner-outer\\edt\\"
//#define input2 "K:\\sdf\\volume\\clean\\clean\\ep\\clean\\"
#define input3 "E:\\SEG\\skeleton\\"
// people /people divide region dir
#define  input2 "D:\\data\\clean\\doctor\\"
using namespace cimg_library;
using namespace std;
//////////////////////////////////////////////////////////////////////////
//��ȡָ��Ŀ¼�������ļ����ļ������������ļ��У���GetFileFromDir��ʹ��
//strDir: ���룬Ŀ¼·��
//vFileDirList�� ������ļ�·���б�
//���أ���
//////////////////////////////////////////////////////////////////////////
void GetFileNameFromDir(string strDir,vector<string>& vFileDirList)
{
#ifdef _WIN32
	WIN32_FIND_DATAA ffd;
	LARGE_INTEGER filesize;
	string szDir;
	//size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	szDir=strDir+"\\*";
	hFind = FindFirstFileA(szDir.c_str(), &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		cout<<"get file name error"<<endl;
		return;
	}
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			string filename=ffd.cFileName;//(const char*)
			string filedir=strDir+"\\"+filename;
			vFileDirList.push_back(filedir);
		}
	}while (FindNextFileA(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		cout<<"FindFirstFile error"<<endl;
		return;
	}
	FindClose(hFind);
#else
	DIR *dir;
	struct dirent *ptr;
	dir = opendir(strDir.c_str());
	while( (ptr = readdir(dir)) != NULL)
	{
		string path = strDir + string("/") + string(ptr->d_name);
		vFileDirList.push_back(path);
	}
	closedir(dir);
	sort(vFileDirList.begin(), vFileDirList.end());
#endif
}
PIXTYPE  biglittleedian(PIXTYPE  val)
{

	PIXTYPE *q = &val;
	unsigned char * p = (unsigned char*)q;
	std::swap(p[0], p[3]);
	std::swap(p[1], p[2]);
	return val;
}
void rocwayinner2people(string dir1, string dir2);
void evaluate(string dir,int l ,int m,int n);
void evaluate(string dir,int l,int m,int n)
{
	//int l=512,m=512,n=570;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]=output;
	strcat(dir2,dst);
	char dir3[300];
	strcpy(dir3,dir2);
	strcat(dir3,"outer5-8_2_20140405.raw");
	float * indata1=test.readStreamfloat(dir3,&l,&m,&n);
	char dir4[300];
	strcpy(dir4,dir2);
	strcat(dir4,"inner5-8_2.raw");
	float * indata2=test.readStreamfloat(dir4,&l,&m,&n);
	for ( int k = 0; k < n; k++ )
	{
		for ( int j = 0; j < m; j++)
		{
			for ( int i = 0; i < l; i++)
			{
				PIXTYPE *val=&indata1[k*m*l+j*l+i];
				//if(i<409 &&i> 107 && j>156 &&j <390)
				//{
				//	if (*val>1)
				//	{
				//		*val=0;

				//	}
				//	else *val=100;
				//}
				//else *val=0;
				*val -= indata2[ k*m*l + j*l + i];
			}
		}
	}
	//for (int i = 0; i < l*m*n; i++)
	//{
	//	indata1[i] -= indata2[i];
	//}
	FILE *p;
	char dir5[300];
	strcpy(dir5,dir2);
	strcat(dir5,"thickness5-8_2_20140405.raw");
	p=fopen(dir5,"wb");
	unsigned char * indata1char=new unsigned char[l*n*m];
	for (int i=0;i< l*m*n;i++)
	{
		indata1char[i]=(unsigned char)indata1[i];
	}
	delete []indata1;
	fwrite(indata1char,sizeof(unsigned char),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1char;
	delete [] indata2;

}

void pvalue( string dir1,string dir2,string dir22)
{
	int l=0,m=0,n=0;
	char dst1[200],dst2[200],dst3[200];
	strcpy(dst1,dir1.c_str()); //string-->char
	strcpy(dst2,dir2.c_str());
	strcpy(dst3,dir22.c_str());
	RawImage test;
	char dir3[200]	=	input1;  //"D:\\sdfdata\\distance\\rocdata\\" ;
	char dir4[200]	=	input2;  //"K:\\sdf\\volume\\clean\\clean\\ep\\rocdatao\\";
	char dir5[200]	=	"K:\\sdf\\volume\\clean\\clean\\ep\\clean\\roc3041\\" ;
	strcat(dir3,dst1);
	strcat(dir4,dst2);
	strcat(dir5,dst3);
	short * indata2=test.readStream(dir5,&l,&m,&n);
	delete [] indata2;
	cout<<m<<endl;
	float * indata1 = test.readStreamfloat(dir3,&l,&m,&n);	//level-set data
	float * indata3 = test.readStreamfloat(dir4,&l,&m,&n);	//robot data

	int TT=0,TF=0,FT=0,FF=0;
	double *res=new double[4];
	for (int i = 100; i < n-100; i++ )
	{
		for (int j = 0; j < l; j++)
		{
			for (int k =0; k < m; k++)
			{

				PIXTYPE val1 = indata1 [i*l*m+j*m+k];
				//cout<<val1<<endl;
				short val2 = indata3 [i*l*m+j*m+k];
				//if (val2 !=0)
				//{
				//	cout << val2 <<endl;
				//} 
				//else
				//{
				//}

				if ( val1 == 100)
				{
					//cout <<val1<<endl;

					if (val2 ==100  ) //or val [-924,-124]
					{
						TT++;
					} 
					else
					{
						FT++;
					}
				} 
				if (val2 ==100 && (( j - 256 ) * ( j - 256 ) + ( k - 256 ) * ( k-256 )) <= 200 *200) //> -938 && val2 <-124
				{

					if (val1==0)
					{
						TF++;

					} 

				} 
				else if (val1==0)
				{
					FF++;
				}



			}
		}
	}
	res [0] = TT; 
	res [1] = FT;
	res [2] = TF;
	res [3] = FF;
	cout <<"TT:"<<TT<<"TF:"<<TF<<"FF:"<<FF<<"FT"<<FT<<endl;
	//FILE *p=fopen("K:\\sdf\\volume\\clean\\clean\\ep\\roc.txt","at+");
	//fwrite(res,sizeof(double),4,p);
	//fclose(p);
	//fflush(stdout);
	ofstream os("K:\\sdf\\volume\\clean\\clean\\ep\\roc.txt",ios::app);
	if (os)
	{
		for (int i=0; i <4; i++)
		{
			os << res[i] <<" ";

		}
		os << endl;
	} 
	else cerr <<"no file" <<endl;
	delete [] res;
	delete [] indata1;
	delete [] indata3;

}
void ddcircle(string dir)
{
	int l=512;
	int m=512; 
	int	n=700;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]="D:\\sdfdata\\";
	strcat(dir2,dst);
	char dir1[200] = "J:\\swfdata\\";
	strcat(dir1,dst);
	float * indata1=test.readStreamfloat(dir2,&l,&m,&n);
	for (int i = 0; i < n; i++)
	{
		for ( int j = 0; j < l; j++)
		{
			for ( int k = 0; k < m; k++)
			{
				PIXTYPE *val=&indata1[i*l*m+j*m+k];
				if ((( j - 256 ) * ( j - 256 ) + ( k - 256 ) * ( k-256 )) >= 40000)
				{

					*val = 0;

				} 
			}
		}
	}
	//Raw *indata=new Raw(l,m,n,indata1);
	FILE *p;
	p=fopen(dir1,"wb");
	fwrite(indata1,sizeof(PIXTYPE),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	//indata->~Raw();
}
void roc()
{
	string dir1(input1);//K:\sdf\volume\clean\clean\ep// D:\sdfdata\distance\rocdata
	string dir2(input2);//K:\sdf\volume\clean\clean\ep\rocdatao
	//cout << dir1 <<endl;
	vector<string> files1;
	vector<string> files2;
	GetFileNameFromDir(dir1,files1);
	GetFileNameFromDir(dir2,files2);
	vector<string>::iterator iterFile1,iterFile2=files2.begin();;
	for (iterFile1=files1.begin();iterFile1!=files1.end();iterFile1++)
	{


		iterFile1->assign(iterFile1->substr(dir1.size()+1));
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		cout<<*iterFile1 <<endl;
		cout<<*iterFile2 <<endl;
		//pvalue(*iterFile1,*iterFile2);

		//rocwayinner2people(*iterFile1,*iterFile2);
		iterFile2++;
		//ddcircle(*iterFile);
		//testcolon(argc,*iterFile);
	}

	cout<<endl;
}

void roc3()
{
	string dir1(input1);//K:\sdf\volume\clean\clean\ep// D:\sdfdata\distance\rocdata
	string dir2(input2);//K:\sdf\volume\clean\clean\ep\rocdatao
	string dir3(input3);
	//cout << dir1 <<endl;
	vector<string> files1;
	vector<string> files2;
	vector<string> files3;
	GetFileNameFromDir(dir1,files1);
	GetFileNameFromDir(dir2,files2);
	GetFileNameFromDir(dir3,files3);
	vector<string>::iterator iterFile1,
		iterFile2 = files2.begin(),
		iterFile3 = files3.begin() ;
	for (iterFile1=files1.begin();iterFile1!=files1.end();iterFile1++)
	{


		iterFile1->assign(iterFile1->substr(dir1.size()+1));
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		iterFile3->assign(iterFile3->substr(dir3.size()+1));
		cout<<*iterFile1 <<endl;
		cout<<*iterFile2 <<endl;
		cout <<*iterFile3 <<endl;
		//pvalue(*iterFile1,*iterFile2,*iterFile3);

		rocwayinner2people(*iterFile1,*iterFile2);
		iterFile2++;
		iterFile3++;
		//ddcircle(*iterFile);
		//testcolon(argc,*iterFile);
	}

	cout<<endl;
}
void float2uchar(int l, int m ,int n,string dir)
{
	//int l=512;
	//int m=512; 
	//int	n=700;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char dir2[200]="K:\\20140404\\inner\\";		//"D:\\sdfdata\\distance\\";
	strcat(dir2,dst);
	char dir1[200] = "K:\\20140404\\inneruint8\\";
	strcat(dir1,dst);
	float * indata1=test.readStreamfloat(dir2,&l,&m,&n);
	unsigned char * outdata=new unsigned char[l*m*n];
	for (int i = 0; i < n; i++)
	{
		for ( int j = 0; j < l; j++)
		{
			for ( int k = 0; k < m; k++)
			{
				//PIXTYPE *val=&indata1[i*l*m+j*m+k];
				outdata [i*l*m+j*m+k]= (unsigned char)indata1[i*l*m+j*m+k];

			}
		}
	}
	//Raw *indata=new Raw(l,m,n,indata1);
	FILE *p;
	p=fopen(dir1,"wb");
	fwrite(outdata,sizeof(unsigned char),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;
	delete [] outdata;

}
void thincknessstd(string dir)
{
	int l=512;
	int m=512; 
	int	n=700;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char readdir[200]="L:\\sdfdata2\\edt\\";
	strcat(readdir,dst);
	//char writedir[200] = "J:\\swfdata\\";
	//strcat(writedir,dst);
	PIXTYPE * indata1=test.readStreamfloat(readdir,&l,&m,&n);
	Raw *src = new Raw(l,m,n,indata1);
	PIXTYPE sum = 0, std = 0, mean = 0, s = 0;
	int countall = 0;
	int countboundary = 0;
	for (int k = 0; k < src->getZsize(); k++)
	{
		for ( int j = 0 ; j < src->getYsize(); j++)
		{
			for ( int i =0 ; i < src->getXsize() ; i ++)
			{
				PIXTYPE  val = src->get(i,j,k);
				float * p= &val;
				unsigned char * bp= (unsigned char *)p;
std:swap(bp[0],bp[3]);
				std::swap(bp[1],bp[2]);
				//cout << val <<endl;
				if (val > 0 && val <100 )
				{

					//cout <<val <<endl;
					sum +=  val;

					s += val*val;		//std simple implementation ,just for data less than 10

					countall++;
					if ( src->get ( i-1, j ,k)== 0|| src->get(i-1 ,j-1,k)==0 || src->get(i-1,j+1,k)==0 \
						|| src->get(i,j-1,k)==0|| src->get(i,j+1,k)==0 || src->get(i+1,j,k) == 0 || src->get(i+1,j-1,k)==0|| src->get(i+1,j+1,k)==0)
					{
						countboundary ++;
					}
				} 
				//else
				//{
				//}
			}
		}

	}
	mean = sqrt(sum) / countboundary;
	std = sqrt(s-countboundary*mean)/ countboundary ;
	PIXTYPE *res =new PIXTYPE [2];
	res[1]=mean;
	res[2]=std;
	cout<<"mean" <<mean <<"std"<< std<<endl;
	ofstream os("K:\\sdf\\volume\\clean\\clean\\ep\\meanstd.txt",ios::app);
	if (os)
	{
		for (int i=0; i <2; i++)
		{
			os << res[i] <<" ";

		}
		os << endl;
	} 
	delete [] res;
	delete [] indata1;

}

void thincknessstdv2(string dir)
{
	int l=512;
	int m=512; 
	int	n=700;
	RawImage test;
	char dst[100];
	strcpy(dst,dir.c_str());
	char readdir[200]="L:\\sdfdata2\\edt\\";
	strcat(readdir,dst);
	//char writedir[200] = "J:\\swfdata\\";
	//strcat(writedir,dst);
	PIXTYPE * indata1=test.readStreamfloat(readdir,&l,&m,&n);
	Raw *src = new Raw(l,m,n,indata1);
	PIXTYPE sum = 0, std = 0, mean = 0, s = 0;
	int countall = 0;
	int countboundary = 0;
	for (int k = 0; k < src->getZsize(); k++)
	{
		for ( int j = 0 ; j < src->getYsize(); j++)
		{
			for ( int i =0 ; i < src->getXsize() ; i ++)
			{
				PIXTYPE  val = src->get(i,j,k);
				float * p= &val;
				unsigned char * bp= (unsigned char *)p;
std:swap(bp[0],bp[3]);
				std::swap(bp[1],bp[2]);
				//cout << val <<endl;
				if (val > 0 && val <100 )
				{

					//cout <<val <<endl;


					countall++;
					if ( src->get ( i-1, j ,k) <= val || src->get(i-1 ,j-1,k) <= val || src->get(i-1,j+1,k) <=val \
						|| src->get(i,j-1,k)<=val || src->get(i,j+1,k)<= val || src->get(i+1,j,k) <= val || src->get(i+1,j-1,k)<= val || src->get(i+1,j+1,k)<= val)
					{
						sum +=  val;

						s += val*val;		//std simple implementation ,just for data less than 10
						countboundary ++;
					}
				} 
				//else
				//{
				//}
			}
		}

	}
	mean = sum / countboundary;
	std = sqrt((s-countboundary*mean*mean)/ countboundary );
	PIXTYPE *res =new PIXTYPE [2];
	res[1]=mean;
	res[2]=std;
	cout<<"mean" <<mean <<"std"<< std<<endl;
	ofstream os("K:\\sdf\\volume\\clean\\clean\\ep\\meanstd.txt",ios::app);
	if (os)
	{
		for (int i=0; i <2; i++)
		{
			os << res[i] <<" ";

		}
		os << endl;
	} 
	delete [] res;
	delete [] indata1;

}
void testcolonway2(int argc,string dir)
{

	char *pt="single_well";
	int l=0,m=0,n=0,l1=0,l2=0,iter_outer=10;
	RawImage test;
	char dirhead[200]=input2;  //K:\\sdf\\volume\\clean\\clean\\ep\\

	char dirbody[100];
	strcpy(dirbody,dir.c_str());
	cout <<"dirbody"<<dirbody<<endl;
	strcat(dirhead,dirbody);
	cout << "dirhead" << dirhead<< endl;
	short * indata=test.readStream(dirhead,&l,&m,&n);
	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];		
	}

	Raw *input=new Raw(l,m,n,inputo);
	Filter *f=new Filter();
	//input=f->guass3DFilter(input,3);
	RawImage *write=new RawImage();
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	ls->initialg(*input);
	for (int i=0; i<input->getXsize(); i++)
	{
		for (int j=0; j<input->getYsize(); j++)
		{
			for (int k=0; k<input->getZsize(); k++)
			{
				if (input->get(i,j,k)>=1)
				{
					initial->put(i,j,k,-2);
				}
				else initial->put(i,j,k,2);

			}
		}

	}
	*initial=ls->minimal_surface(*initial,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	char *outname1="inner.raw";
	char outdir[200]=output;
	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	ls->outerwall(*initial,*input,5.0,0.1,-3,1.5,1,10,pt);
	//*initial -=temp;
	char *outname2="outer5-8_2.raw";
	char outdir2[200]=output;
	strcat(outdir2,dirbody);
	strcat(outdir2,outname2);
	test.writeImageName(*initial,outdir2);
	evaluate(dir,l,m,n);
}

void threshold()
{
	int l=512;
	int m=512; 
	int	n=700;
	RawImage test;

	char dir2[200]="K:\\sdf\\volume\\clean\\clean\\ep\\test3\\WI_3035_P_iso_clean.raw";

	char dir1[200] = "L:\\sdfdata2\\testthreshold\\WI_3035_P_iso_clean_threshold.raw";

	short * indata1=test.readStream(dir2,&l,&m,&n);

	for (int i = 0; i < n; i++)
	{
		for ( int j = 0; j < l; j++)
		{
			for ( int k = 0; k < m; k++)
			{
				short *val=&indata1[i*l*m+j*m+k];
				if (*val >-938 && *val < -124)
				{
					*val =100;
				}
				else *val = 0;
			}
		}
	}
	//Raw *indata=new Raw(l,m,n,indata1);
	FILE *p;
	p=fopen(dir1,"wb");
	fwrite(indata1,sizeof(short),l*m*n,p);
	fclose(p);
	fflush(stdout);
	delete [] indata1;



}
bool sign(PIXTYPE data)
{
	return data< -124 && data > -924;
	//return data!=100;
}

void rocwayinner2people(string dir1, string dir2)
{
	int l=0,m=0,n=0;
	char dst1[200],dst2[200];
	strcpy(dst1,dir1.c_str()); //string-->char
	strcpy(dst2,dir2.c_str());
	RawImage test;
	char dir3[200]="L:\\sdfdata2\\inner\\" ;
	char dir4[200]="K:\\sdf\\volume\\clean\\clean\\ep\\clean\\";
	char dir5[200]="D:\\segdata\\people\\time4\\";
	strcat(dir3,dst1);
	strcat(dir4,dst2);
	strcat(dir5,dst1);
	//short * indata2=test.readStream(dir4,&l,&m,&n);
	//cout<<m<<endl;
	//float * indata1=test.readStreamfloat(dir3,&l,&m,&n);

	//char * file="L:\\sdfdata2\\inner\\",
	//	*file1="K:\\sdf\\volume\\clean\\clean\\ep\\clean\\";
	//int l = 512,
	//	m = 512,
	//	n = 700;
	//int l = 0,
	//	m = 0,
	//	n = 0;
	//RawImage test;
	short*orgiondata=test.readStream(dir4,&l,&m,&n);
	PIXTYPE * innerdata=test.readStreamfloat(dir3,&l,&m,&n);
	Raw src(l,m,n,innerdata);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) orgiondata[i];		
	}
	Raw *orgion = new Raw(l,m,n,inputo);


	int count=0,time=6;
	do 
	{
		//Raw srcnew(src);
		for (int k =1; k< n-1 ;k++)
		{
			for (int j = 1; j < m-1; j++)
			{
				for (int i = 1; i < l-1; i++)
				{
					PIXTYPE val =src.get(i,j,k);
					//unsigned char * p= (unsigned char *)innerdata;
					//std::swap(p[0],p[3]);
					//std::swap(p[1],p[2]);

					if (val==100)
					{		
						PIXTYPE data1=orgion->get ( i-1, j ,k);
						//PIXTYPE *q=&data1;
						//unsigned char * p= (unsigned char*) q;
						//std::swap(p[0],p[3]);
						//std::swap(p[1],p[2]);
						if (sign(data1))
						{
							src.put(i-1 ,j,k,100);
							count++;
						}
						if (sign(orgion->get(i-1 ,j-1,k)))
						{
							src.put(i-1,j-1,k,100);
							count ++;
						}

						if (sign(orgion->get(i-1,j+1,k)))
						{
							src.put(i-1,j+1,k,100);
							count++;
						}
						if (sign(orgion->get(i,j-1,k)))
						{
							src.put(i,j-1,k,100);
							count++;
						}
						if (sign(orgion->get(i,j+1,k)))
						{
							src.put(i,j+1,k,100);
							count++;
						}
						if (sign(orgion->get(i+1,j,k)) )
						{
							src.put(i+1,j,k,100);
							count++;
						}
						if (sign(orgion->get(i+1,j-1,k)))
						{
							src.put(i+1,j-1,k,100);
							count++;
						}
						if (sign(orgion->get(i+1,j+1,k)))
						{
							src.put(i+1,j+1,k,100);
							count++;
						}
					}

				}
			}

		}

		time--;
		cout <<count <<endl;

	} while (time);

	test.writeImageName(src,dir5);
	//delete &src;
	PIXTYPE * innerdata1=test.readStreamfloat(dir3,&l,&m,&n);
	PIXTYPE * outterdata=test.readStreamfloat(dir5,&l,&m,&n);
	int i=0;
	while (i < l*m*n)
	{

		outterdata[i] -= innerdata1[i];
		i++;
	}
	Raw res(l,m,n,outterdata);
	test.writeImageName(res,dir5);
	delete []innerdata1;
}
Raw* thicknessequalHU(Raw *origion,Raw *thickness)
{
	Raw *HU =new Raw(*origion);
	for (int i=0;i<origion->size();i++)
	{
		if(thickness->getXYZ(i)!=0)
			HU->putXYZ(i,origion->getXYZ(i));
		else
			HU->putXYZ(i,0);

	}
	return HU;
}

void HUandThickness()
{
	//thickness data
	string dir1(input1);//K:\sdf\volume\clean\clean\ep// D:\sdfdata\distance\rocdata
	//origion data
	string dir2(input2);//K:\sdf\volume\clean\clean\ep\rocdatao
	//skeleton data
	string dirskelton(input3);//K:\sdf\volume\clean\clean\ep\rocdatao
	//cout << dir1 <<endl;
	vector<string> files1;
	vector<string> files2;
	vector<string> files3;
	GetFileNameFromDir(dir1,files1);
	GetFileNameFromDir(dir2,files2);
	GetFileNameFromDir(dirskelton,files3);
	vector<string>::iterator iterFile1,iterFile2=files2.begin(),iterFile3=files3.begin();
	for (iterFile1=files1.begin();iterFile1!=files1.end();iterFile1++)
	{


		iterFile1->assign(iterFile1->substr(dir1.size()+1));
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		iterFile3->assign(iterFile3->substr(dirskelton.size()+1));
		cout<<*iterFile1 <<endl;
		cout<<*iterFile2 <<endl;
		cout <<*iterFile3 <<endl;
		//iterFile2++;
		int l=0,m=0,n=0;
		char dst1[200],dst2[200],dst3[200];
		strcpy(dst1,(*iterFile1).c_str()); //string-->char
		strcpy(dst2,(*iterFile2).c_str());
		strcpy(dst3,(*iterFile3).c_str());
		RawImage test;
		char dir3[200]=input1;
		char dir4[200]=input2;
		char dir5[200]=input1;
		char dir6[200]=input3;
		strcat(dir3,dst1);
		strcat(dir4,dst2);
		strcat(dir5,dst1);
		strcat(dir6,dst3);
		short*orgiondata=test.readStream(dir4,&l,&m,&n);
		PIXTYPE * innerdata=new PIXTYPE [l*m*n];
		test.readImage2(innerdata,dir3,l*m*n);
		unsigned char * skeletondata =new unsigned char[l*m*n];
		test.readImage(skeletondata,dir6,l*m*n);

		float *inputo=new float[l*m*n];
		PIXTYPE *skeletondataF=new PIXTYPE[l*m*n];
		PIXTYPE *innerdataF= new PIXTYPE[l*m*n];
		PIXTYPE max=0,min=1000;
		for (int i = 0; i < l*m*n; i++)
		{

			PIXTYPE a=inputo[i]=(float) orgiondata[i]+1020;	
			max > a?max=max:max=a; 
			min < a? min =min:min=a;
			PIXTYPE b=skeletondataF[i]=(float(skeletondata[i]));
			//if (PIXTYPE c=innerdataF[i]=biglittleedian((float)innerdata[i]))
			//{
			//	cout <<"aaa"<<endl;
			//}

			PIXTYPE c=innerdataF[i]=biglittleedian((float)innerdata[i]);
			//if (c !=0)
			//{
			//	cout <<c<<endl;
			//}
		}
		cout <<max<<endl;
		cout <<min <<endl;
		delete []skeletondata;
		delete []innerdata;
		delete [] orgiondata;
		Raw *thickness=new Raw(l,m,n,innerdataF,true);
		Raw *orgion = new Raw(l,m,n,inputo);
		Raw *skeleton =new Raw(l,m,n,skeletondataF); 
		Raw *hu=thicknessequalHU(orgion,thickness);
		queue<Point>q;
		vector<Point> c;
		DivideRegion *dr=new DivideRegion(q,skeleton,c);
		//dr->putskletoninorders1();//region2
		//dr->putskeletoninorders3s2(dr->center.front(),dr->center.back());
		//dr->wirtecenterline((int)skeleton->getXsize(),(int)skeleton->getYsize(),(int)skeleton->getZsize());
		//dr->DivideRegionv2(skeleton,hu);//region 2
		//dr->DivideRegionthicknessv2(skeleton,thickness);//region2
		//dr->DivideRegionv1(skeleton,hu);//compute hu statistic
		dr->DivideRegionthickness(skeleton,thickness);
		iterFile2++;
		iterFile3++;
		delete thickness;
		delete orgion;
		delete skeleton;
		delete hu;
	}

}
void directdivideregion()
{

	//thickness data
	string dir1(input1);//K:\sdf\volume\clean\clean\ep// D:\sdfdata\distance\rocdata
	//origion data
	string dir2(input2);//K:\sdf\volume\clean\clean\ep\rocdatao
	//skeleton data
	string dirskelton(input3);//K:\sdf\volume\clean\clean\ep\rocdatao
	//cout << dir1 <<endl;
	vector<string> files1;
	vector<string> files2;
	vector<string> files3;
	GetFileNameFromDir(dir1,files1);
	GetFileNameFromDir(dir2,files2);
	GetFileNameFromDir(dirskelton,files3);
	vector<string>::iterator iterFile1,iterFile2=files2.begin(),iterFile3=files3.begin();
	for (iterFile1=files1.begin();iterFile1!=files1.end();iterFile1++)
	{


		iterFile1->assign(iterFile1->substr(dir1.size()+1));
		iterFile2->assign(iterFile2->substr(dir2.size()+1));
		iterFile3->assign(iterFile3->substr(dirskelton.size()+1));
		cout<<*iterFile1 <<endl;
		cout<<*iterFile2 <<endl;
		cout <<*iterFile3 <<endl;
		//iterFile2++;
		int l=0,m=0,n=0;
		char dst1[200],dst2[200],dst3[200];
		strcpy(dst1,(*iterFile1).c_str()); //string-->char
		strcpy(dst2,(*iterFile2).c_str());
		strcpy(dst3,(*iterFile3).c_str());
		RawImage test;
		char dir3[200]=input1;
		char dir4[200]=input2;
		char dir5[200]=input1;
		char dir6[200]=input3;
		strcat(dir3,dst1);
		strcat(dir4,dst2);
		strcat(dir5,dst1);
		strcat(dir6,dst3);
		short*orgiondata=test.readStream(dir4,&l,&m,&n);
		delete [] orgiondata;
		PIXTYPE * innerdata=new PIXTYPE [l*m*n];
		test.readImage2(innerdata,dir3,l*m*n);
		PIXTYPE *innerdataF= new PIXTYPE[l*m*n];
		PIXTYPE max=0,min=1000;
		for (int i = 0; i < l*m*n; i++)
		{

			//PIXTYPE a=inputo[i]=(float) orgiondata[i]+1020;	
			//max > a?max=max:max=a; 
			//min < a? min =min:min=a;
			//PIXTYPE b=skeletondataF[i]=(float(skeletondata[i]));
			PIXTYPE c=innerdataF[i]=biglittleedian((float)innerdata[i]);
		}
		cout <<max<<endl;
		cout <<min <<endl;
		delete []innerdata;
		Raw *thickness=new Raw(l,m,n,innerdataF,true);
		int count=0;
		for (int i=0;i<thickness->getXsize();i++)
		{
			for (int j=0 ;j <thickness->getYsize() ;j++)
			{
				for (int k=0;k < thickness->getZsize() ;k++)
				{
					if (thickness->get(i,j,k)>0)
					{
						count++;
					}
				}
			}
		}

		int histgram[4][20]={0};
		int i=0;
		for (int j = 1; j < thickness->size(); j++)
		{
			
			PIXTYPE val=thickness->getXYZ(j);
			if (thickness->getXYZ(j)>0)
			{
				if (i <= count/4)
				{
					i++;

					histgram[0][(int)val]++;


				}
				else if (i<= 2*count/4)
				{
					i++;


					histgram[1][(int)val]++;


				} 
				else if (i<= 3*count/4)
				{
					i++;

					histgram[2][(int)val]++;;

				} 
				else
				{

					histgram[3][(int)val]++;

				}



			}
		}
		for (int i = 0; i < 4; i++)
		{
			//for (int j = 0; j < 255; j++)
			//{
			ofstream os("thicknesshistgram.txt",ios::app);
			for (int j=0;j<20;j++)
			{
				os<<histgram[i][j]<<" ";
			}
			os <<endl;


		}
		delete thickness;	
		iterFile2++;
		iterFile3++;

		}
		

	
} 


void data2SegFormat(string clean, string seg, string tobecondata);
void condata()
{
	string dir1("D:\\data\\clean\\cleangac\\");
	//string dir2("F:\\cleanres\\inner\\");
	string dir2("M:\\3000GAC\\inner\\convertdata\\");
	string dir3("D:\\data\\segmention\\gac\\");
	vector<string> files1;
	vector<string> files2;
	vector<string> files3;
	GetFileNameFromDir(dir1, files1);
	GetFileNameFromDir(dir2, files2);
	GetFileNameFromDir(dir3, files3);
	vector<string>::iterator iterFile1 = files1.begin();
	vector<string>::iterator iterFile2 = files2.begin();
	vector<string>::iterator iterFile3 = files3.begin();
	for (iterFile2 = files2.begin() ; iterFile2 != files2.end(); iterFile2++)
	{

		

		iterFile2->assign(iterFile2->substr(dir2.size() + 1));
		cout << *iterFile2 << endl;
		//ddcircle(*iterFile);
		iterFile1->assign(iterFile1->substr(dir1.size() + 1));
		iterFile3->assign(iterFile3->substr(dir3.size() + 1));
		cout << *iterFile1 << endl;
		cout << *iterFile3 << endl;
		data2SegFormat(*iterFile1,*iterFile3,*iterFile2);
		iterFile1++;
		iterFile3++;
		//float2uchar(512,512,700,*iterFile2);
		//testsesmic();
		//thincknessstdv2(*iterFile2);
		//roc(*iterFile2);
		//rate(argc,*iterFile2);
	}
}
void data2SegFormat(string clean,string seg,string tobecondata)
{


	char cleandir[100] ="D:\\data\\clean\\cleangac\\";
	char segdir[100] ="D:\\data\\segmention\\gac\\";
	char tobecondatadir[100] ="M:\\3000GAC\\inner\\convertdata\\";
	char dirbody1[100];
	strcpy(dirbody1, clean.c_str());
	strcat(cleandir, dirbody1);
	char dirbody2[100];
	strcpy(dirbody2,seg.c_str());
	strcat(segdir, dirbody2);
	char dirbody3[100];
	strcpy(dirbody3, tobecondata.c_str());
	strcat(tobecondatadir, dirbody3);
	int lx = 0, ly = 0, lz = 0;
	float rate = 0;
	ifstream file;
	file.open(cleandir, ios::out | ios::app | ios::binary);
	if (!file.is_open()) {
		cout << "The file open failed, Please check it and try again" << endl;
		exit(0);
	}
	file.read(reinterpret_cast<char *>(&lx), sizeof(int));
	file.read(reinterpret_cast<char *>(&ly), sizeof(int));
	file.read(reinterpret_cast<char *>(&lz), sizeof(int));

	int size = lx*ly*lz;
	file.close();
	//read seg data
	FILE * op = fopen(segdir, "r");
	if (op == NULL)
	{
		printf("open fail");
	}
	char *buf = new char[16];
	fread((char *)buf, sizeof(char), 16, op);
	fclose(op);
	//��seg��ͷ�ļ�д��
	char outputdir[100]="F:\\SEG\\";
	strcat(outputdir,dirbody3);
	FILE *w1 = fopen(outputdir,"wb");
	fwrite(buf,sizeof(char),16,w1);
	fclose(w1);
	FILE *p = fopen(tobecondatadir, "r");
	
	if (p == NULL)
	{
		printf("open fail");
	}
	int datatype = 2;
	if (datatype = 1)
	{
		float *data = new float[lx*ly*lz];
		//float2uchar()

		fread((char *)data, sizeof(float), size, p);
		unsigned char *uchardata = new unsigned char[size];
		for (size_t i = 0; i < size; i++)
		{
			float val = (data[i]);
			//if (val != 0)
			//	cout << val << endl;
			uchardata[i] = (unsigned char)(val);

		}
		delete[] data;

		fclose(p);
		printf("data to be converted read is ok\n");

		FILE *w2 = fopen(outputdir, "a+");
		fwrite(uchardata, sizeof(unsigned char), size, w2);

	}
	else if (datatype==2)
	{
		//float *data = new float[lx*ly*lz];
		//float2uchar()
		unsigned char *data = new unsigned char[size];
		fread((char *)data, sizeof(unsigned char), size, p);
		
		//for (size_t i = 0; i < size; i++)
		//{
		//	float val = (data[i]);
		//	//if (val != 0)
		//	//	cout << val << endl;
		//	uchardata[i] = (unsigned char)(val);

		//}
		//delete[] data;

		fclose(p);
		printf("data to be converted read is ok\n");

		FILE *w2 = fopen(outputdir, "a+");
		fwrite(data, sizeof(unsigned char), size, w2);

	}
	












	fclose(op);
	printf("read is ok\n");

}
Point* chekneghbor(Point *p,Raw *src)
{
	Point *res = new Point();
	bool flag = true;


	for (size_t l = p->x - 1; l <= p->x + 1 && flag; l++)
	{
		for (size_t m = p->y - 1; m <= p->y + 1 && flag; m++)
		{
			for (size_t n = p->z - 1; n <= p->z + 1 && flag; n++)
			{
				if ((int)l<src->getXsize() && l>0 && (int)m<src->getYsize()&&m>0 && n>0 && (int)n<src->getZsize())
				{
					if (src->get(l, m, n) == 100)
					{

						res->x = l;
						res->y = m;
						res->z = n;
						res->value = 200;
						src->put(l, m, n, 200);
						flag = false;

					}

				}

				


			}

		}
	}
	if (flag == true)
	{
 		res->value = 0;

	}
	return  res;
}
void removeOutliers(Raw * origincolon,Raw *res)
{
	int first = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	vector< vector<Point> > list(1000);
	int num = 0;
	while (i <= origincolon->getXsize() && j <= origincolon->getYsize() - 1 && k <= origincolon->getZsize() - 1)
	{
		bool flag = true;
		//find the first point
		for (i = 0; i < origincolon->getXsize() && flag; i++)
		{
			for (j = 0; j < origincolon->getYsize() && flag; j++)
			{
				for (k = 0; k < origincolon->getZsize() && flag; k++)
				{
					if (origincolon->get(i, j, k) == 100)

					{
						origincolon->put(i, j, k, 200);
						flag = false;
						//cout << origincolon->get(i, j, k) << endl;
						//break;
					}
				}
			}
		}
		if (flag == false)
		{
			std::stack<Point> path;
			path.push(Point(i, j, k, 200));
			//path.push(*chekneghbor(&Point(i, j, k, 100), origincolon));
			while (!path.empty())
			{
				PIXTYPE temp = 1;
				while (temp != 0)
				{
					Point *val = chekneghbor(&path.top(), origincolon);
					temp = val->value;
					if (temp != 0)
					{
						path.push(*val);
						list[num].push_back(path.top());

					}

					//path.push(*val, origincolon));
				}
				path.pop();
			}


			cout << num++ << endl;


		}
		else break;

	}
	int maxlistnum = 0;
	int num2maxnum = -1;
	int max = -1;
	for (size_t i = 0; i < res->getXsize(); i++)
	{
		for (size_t j = 0; j < res->getYsize(); j++)
		{
			for (size_t k = 0; k < res->getZsize(); k++)
			{
				res->put(i, j, k, 0);

			}

		}

	}
	for (size_t kk = 0; kk < num; kk++)
	{


		if (max < list[kk].size())
		{
			maxlistnum = kk;

		}
		else if (list[kk].size() > 1600)
		{
			num2maxnum = kk;
			for (vector<Point>::iterator it = list[kk].begin(); it != list[kk].end(); ++it)
			{

				res->put(it->x, it->y, it->z, it->value);
			}
		}


	}

	cout << list[maxlistnum].size() << endl;
	cout << list[num2maxnum].size() << endl;


	if (maxlistnum >= 0)
	{
		for (vector<Point>::iterator it = list[maxlistnum].begin(); it != list[maxlistnum].end(); ++it)
		{

			res->put(it->x, it->y, it->z, it->value);
		}

	}

	






}
void removeOutliersv2(Raw * origincolon, Raw *res)
{
	int first = 0;
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	vector< vector<Point> > list(1000);
	int num = 0;
	while (i <= origincolon->getXsize() && j <= origincolon->getYsize() && k <= origincolon->getZsize())
	{
		bool flag = true;
		//find the first point
		for (i = 0; i < origincolon->getXsize() && flag; i++)
		{
			for (j = 0; j < origincolon->getYsize() && flag; j++)
			{
				for (k = 0; k < origincolon->getZsize() && flag; k++)
				{
					if (origincolon->get(i, j, k) == 100)

					{
						origincolon->put(i, j, k, 200);
						flag = false;
						//cout << origincolon->get(i, j, k) << endl;
						//break;
					}
				}
			}
		}
		if (flag == false)
		{
			std::stack<Point> path;
			path.push(Point(i, j, k, 200));
			//path.push(*chekneghbor(&Point(i, j, k, 100), origincolon));
			while (!path.empty())
			{
				PIXTYPE temp = 1;
				while (temp != 0)
				{
					Point *val = chekneghbor(&path.top(), origincolon);
					temp = val->value;
					if (temp != 0)
					{
						path.push(*val);
						list[num].push_back(path.top());

					}

					//path.push(*val, origincolon));
				}
				path.pop();
			}


			cout << num++ << endl;


		}
		else break;

	}
	int maxlistnum = 0;
	int num2maxnum = -1;
	int max = -1;
	for (size_t i = 0; i < res->getXsize(); i++)
	{
		for (size_t j = 0; j < res->getYsize(); j++)
		{
			for (size_t k = 0; k < res->getZsize(); k++)
			{
				res->put(i, j, k, 0);

			}

		}

	}
	for (size_t kk = 0; kk < num; kk++)
	{



		if (list[kk].size() > 46000)
		{
			num2maxnum = kk;
			for (vector<Point>::iterator it = list[kk].begin(); it != list[kk].end(); ++it)
			{

				res->put(it->x, it->y, it->z, it->value);
			}
		}


	}

	cout << list[maxlistnum].size() << endl;
	cout << list[num2maxnum].size() << endl;


	//if (maxlistnum >= 0)
	//{
	//	for (vector<Point>::iterator it = list[maxlistnum].begin(); it != list[maxlistnum].end(); ++it)
	//	{

	//		res->put(it->x, it->y, it->z, it->value);
	//	}

	//}








}
void cleanraw()
{
	//if data is unsigned char -->2
	//if data is float --->1
	// input clean data for  size
	//input data need to be cleaned
	string cleandir("D:\\data\\clean\\cleangac\\");
	//string noisydir("F:\\resdata\\optm\\inner\\");
	string noisydir("M:\\3000GAC\\inner\\convertdata\\");
	vector<string> files1;
	vector<string> files2;
	GetFileNameFromDir(cleandir,files1);
	GetFileNameFromDir(noisydir,files2);
	//int a[6] = {4,20,45,46,58,61};
	//int a[2] = { 43,50 };
	for (size_t i = 0; i < 8; i++)
	{
		//vector<string>::iterator itclean = files1.begin() + 1+a[i];
		//vector<string>::iterator itnoisy = files2.begin() + a[i];
		vector<string>::iterator itclean = files1.begin() + i;
		vector<string>::iterator itnoisy = files2.begin() + i;

		
			int datatype = 1;
			int l, m, n;
			cout << *itclean << endl;
			cout << *itnoisy << endl;
			//char dirclean[100];
			//strcpy(dirclean,itclean._c_str());
			//char dirnoisy[100];
			//strcpy(dirnoisy,itnosiy._c_str());
			/*
			1\float
			2\unsigned char
			*/
			Raw *origincolon = NULL;
			Raw *res = NULL;
			RawImage *test = new RawImage();
			switch (datatype)
			{
			case 1:
			{
				short * indata = test->readStream(itclean->c_str(), &l, &m, &n);
				delete[] indata;
				float * buf = new float[l*m*n];

				FILE *p = fopen(itnoisy->c_str(), "r");
				fread(buf, sizeof(float), l*m*n, p);
				origincolon = new Raw(l, m, n, buf);
				//for (size_t i = 0; i < l*m*n; i++)
				//{
				//	if (origincolon->getXYZ(i) != 0)
				//	{
				//		cout << buf[i] << endl;

				//	}

				//}
				res = new Raw(l, m, n);

			}
				break;
			case 2:
			{
				short * indata = test->readStream(itclean->c_str(), &l, &m, &n);
				delete[] indata;
				unsigned char * buf = new unsigned char[l*m*n];

				FILE *p = fopen(itnoisy->c_str(), "r");
				fread(buf, sizeof(unsigned char), l*m*n, p);
				PIXTYPE *data = new PIXTYPE[l*m*n];
				for (size_t i = 0; i < l*m*n; i++)
				{
					data[i] = (PIXTYPE)buf[i];

				}
				origincolon = new Raw(l, m, n, data);
				res = new Raw(l, m, n);
			}
				break;
			default:
			{


			}
				break;
			}


			removeOutliersv2(origincolon, res);
			char outdir[100] = "F:\\cleanres\\";
			string filename(itnoisy->substr(noisydir.size() + 1));
			strcat(outdir, filename.c_str());
			cout << outdir << endl;
			test->writeImageName(*res, outdir);

		//}

	}

	
}
void computeHistgram(Raw *polyp)
{
	PIXTYPE max = -1000;
	PIXTYPE min = 1000;
	for (size_t i = 0; i < polyp->size(); i++)
	{
		PIXTYPE val = polyp->getXYZ(i);
		max < val ? max = val : max = max;
		min > val ? min = val : min = min;


	}
	int delta = (max - min) / 10;
	int *a = new int[10];
	for (size_t i = 0; i < 10; i++)
	{
		a[i] = 0;

	}
	for (size_t i = 0; i < polyp->size(); i++)
	{
		PIXTYPE val = polyp->getXYZ(i);
		a[(int)(val - min)/delta-1]++;

	}
	ofstream os("E:\\statistics\\hist.txt",ios::app);
	os << max << " ";
	os << min << " ";
	for (size_t i = 0; i < 10; i++)
	{
		os << a[i] << " ";

	}
	os << endl;
	

}
//take the polyp into consideration
void computeHistgramreal(Raw *polyp,Raw *real)
{
	PIXTYPE max = -1000;
	PIXTYPE min = 1000;
	for (size_t i = 0; i < polyp->size(); i++)
	{
		PIXTYPE val = polyp->getXYZ(i);
		max < val ? max = val : max = max;
		min > val ? min = val : min = min;


	}
	int delta = (max - min) / 10;
	int *a = new int[10];
	for (size_t i = 0; i < 10; i++)
	{
		a[i] = 0;

	}
	for (size_t i = 0; i < polyp->size(); i++)
	{
		if (real->getXYZ(i)!=0)
		{
			PIXTYPE val = polyp->getXYZ(i);
			a[(int)(val - min) / delta - 1]++;

		}


	}
	ofstream os("E:\\statistics\\hist.txt", ios::app);
	os << max << " ";
	os << min << " ";
	for (size_t i = 0; i < 10; i++)
	{
		os << a[i] << " ";

	}
	os << endl;


}
void testpolyphist()
{

	string dir2("E:\\statistics\\1\\");//D:\swfdata20140420res\polyp\regiongrowingfillnull
	string dir3("D:\\swfdata20140420res\\polyp\\0731\\");
	vector<string> files2;
	vector<string> files3;
	GetFileNameFromDir(dir2, files2);
	GetFileNameFromDir(dir3,files3);
	vector<string>::iterator iterFile2;
	vector<string>::iterator iterFile3=files3.begin();
	int i = 0;
	for (iterFile2 = files2.begin(); iterFile2 != files2.end(); iterFile2++)
	{
		
		iterFile2->assign(iterFile2->substr(dir2.size() + 1));
		cout << *iterFile2 << endl;
		cout << *iterFile3 << endl;
		char *pt = "single_well";
		int l = 0, m = 0, n = 0, l1 = 0, l2 = 0, iter_outer = 50;
		RawImage test;
		char dirhead[200] = "E:\\statistics\\1\\";
		char dirhead2[200] = "D:\\swfdata20140420res\\polyp\\0731\\";
		char dirbody[100];
		char dirbody2[100];
		strcpy(dirbody, iterFile2->c_str());
		strcpy(dirbody2,iterFile3->c_str());
 		cout << "dirbody" << dirbody << endl;
		strcat(dirhead, dirbody);
		strcat(dirhead2,dirbody2);
		cout << "dirhead" << dirhead << endl;
		//RawImage test;
		PIXTYPE * buf = new PIXTYPE[512*512*50];
		PIXTYPE *buf2 = new PIXTYPE[512*512*50];
		test.readImage2(buf, dirhead, 512 * 512 * 50);
		test.readImage2(buf2, dirhead, 512 * 512 * 50);
		Raw *polyp = new Raw(512,512,50,buf);
		Raw *realpolyp = new Raw(512,512,50,buf2);
		computeHistgramreal(polyp, realpolyp);
		iterFile3++;

	}
}
#endif