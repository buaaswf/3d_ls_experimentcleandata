#include <iostream>
#include <fstream>
#include <string>
#include "Polyp.h"
#include <queue>
using namespace std;
void seedGrowing(Raw * src,vector<Seed> seedlist);
Polyp::Polyp(void)
{
}


Polyp::~Polyp(void)
{
}
int CountLines(char *filename)
{
	ifstream ReadFile;
	int n=0;
	string tmp;
	ReadFile.open(filename,ios::in);//ios::in :readonly
	if(ReadFile.fail())// open fails
	{
		return 0;
	}
	else//if exit
	{
		while(getline(ReadFile,tmp,'\n'))
		{
			n++;
		}
		ReadFile.close();
		return n;
	}
}
vector<Seed> readSeedFromTXT(char *txtfilepath)
{
		ifstream file;
		int LINES;
		char filename[512]="inFile.txt";
		file.open(filename,ios::in);
		if(file.fail())
		{
			cout<<"not exit."<<endl;
			file.close();
		}
		else//if exit
		{
			LINES=CountLines(filename);
			int *tempInt=new int[LINES];
			char *tempChar=new char[LINES];
			int i=0;
			while(!file.eof()) //read data to array
			{

				file>>tempInt[i];
				file>>tempChar[i];
				i++;
			}
			file.close(); //close file
			for(i=0;i<LINES;i++)
				cout<<tempInt[i]<<"\t"<<tempChar[i]<<endl;
			delete []tempInt;
			delete []tempChar;
		}

		vector<Seed> seed;
		return seed;

}
Raw* Polyp::initialRegion(vector<Seed> seedlist,int size,int l, int m, int n)
{
	Raw *initialRegion=new Raw(l,m,n);
	//memset(initialRegion->getdata(),2,initialRegion->size());
	for (int i = 0; i < l*m*n; i++)
	{
		initialRegion->putXYZ(i,-2);
	}
	for (vector<Seed>::iterator it = seedlist.begin();it!=seedlist.end();++it)
	{
		
		//Seed *seed=new Seed(it->x,it->y,it->z);
		for (int i = it->x - size; i < it->x + size; ++i )
		{
			for (int j = it->y - size; j < it->y + size; ++j)
			{
				for (int k = it->z - size; k< it->z + size; ++k)
				{
					initialRegion->put(i,j,k,2);
				}
				
			}
			
		}
		
	}
	return initialRegion;
}
void Polyp::polypDetect(string dir)
{
	char *pt="single_well";
	int l=0,m=0,n=0,l1=0,l2=0,iter_outer=20;
	RawImage test;
	char dirhead[200]=input2;  //K:\\sdf\\volume\\clean\\clean\\ep\\

	char dirbody[100];
	strcpy(dirbody,dir.c_str());
	cout <<"dirbody"<<dirbody<<endl;
	strcat(dirhead,dirbody);
	cout << "dirhead" << dirhead<< endl;
	short * indata=test.readStream(dirhead,&l,&m,&n);
	unsigned char * thicknessdata=test.readStreamuchar(dirhead,l,m,n,212);
	n=50;
	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) indata[i];		
	}

	Raw *input=new Raw(l,m,n,inputo);
	/*Filter *f=new Filter();*/
	//input=f->guass3DFilter(input,3);
	RawImage *write=new RawImage();
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	//20140405 delete because of the existance of 
	ls->initialg(*input);
	//for (int i=0; i<input->getXsize(); i++)
	//{
	//	for (int j=0; j<input->getYsize(); j++)
	//	{
	//		for (int k=0; k<input->getZsize(); k++)
	//		{
	//			if (input->get(i,j,k)>=1)
	//			{
	//				initial->put(i,j,k,-2);
	//			}
	//			else initial->put(i,j,k,2);

	//		}
	//	}

	//}
	vector<Seed> seedlist;
	Seed *seed=new Seed(164, 373, 20);
	seedlist.push_back(*seed);
	Raw *initialdata=initialRegion(seedlist,20,l,m,n);
	*initial=ls->minimal_surface(*initialdata,*input,5.0,0.1,3,1.5,1,iter_outer,pt);
	char *outname1="inner5-8_2.raw";
	char outdir[200]=output;

	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	//test.readImage2(initial->getdata(),outdir,l*m*n);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	//ls->outerwallauto(*initial,*input,10,0.1,-6,1.5,1,10,pt);
	//*initial -=temp;
	//char *outname2="outer5-8_2_20140405.raw";
	//char outdir2[200]=output;
	//strcat(outdir2,dirbody);
	//strcat(outdir2,outname2);
	//test.writeImageName(*initial,outdir2);
	//evaluate(dir,l,m,n);
}
void Polyp::polypDetect(string dir,string dirthickness,int method0)
{
	char *pt="single_well";
	int l=0,m=0,n=0,l1=0,l2=0,iter_outer=20;
	RawImage test;
	char dirhead[200]=input2;  //K:\\sdf\\volume\\clean\\clean\\ep\\
	
	char dirheadt[200]=inputt; 
	char dirbodyt[100];
	char dirbody[100];
	strcpy(dirbody,dir.c_str());
	strcpy(dirbodyt,dirthickness.c_str());
	cout <<"dirbody"<<dirbody<<endl;
	cout <<"dirbody thickness"<<dirbodyt<<endl;
	strcat(dirhead,dirbody);
	strcat(dirheadt,dirbodyt);
	cout << "dirhead" << dirhead<< endl;
	short * indata=test.readStream(dirhead,&l,&m,&n);
	delete [] indata;
	unsigned char * thicknessdata=test.readStreamuchar(dirheadt,l,m,n,212);
	n=50;
	Raw *initial=new Raw(l,m,n);
	float *inputo=new float[l*m*n];
	for (int i = 0; i < l*m*n; i++)
	{
		inputo[i]=(float) thicknessdata[i];		
	}
	delete [] thicknessdata;
	Raw *input=new Raw(l,m,n,inputo);
	/*Filter *f=new Filter();*/
	//input=f->guass3DFilter(input,3);
	RawImage *write=new RawImage();
	ThreeDim_LevelSet *ls=new ThreeDim_LevelSet();
	//20140405 delete because of the existance of 
	ls->initialg(*input);
	//for (int i=0; i<input->getXsize(); i++)
	//{
	//	for (int j=0; j<input->getYsize(); j++)
	//	{
	//		for (int k=0; k<input->getZsize(); k++)
	//		{
	//			if (input->get(i,j,k)>=1)
	//			{
	//				initial->put(i,j,k,-2);
	//			}
	//			else initial->put(i,j,k,2);

	//		}
	//	}

	//}
	vector<Seed> seedlist;
	Seed *seed=new Seed(145, 379, 25);
	seedlist.push_back(*seed);
	int method=method0;
	if (method==1)
	{ 
	Raw *initialdata=initialRegion(seedlist,5,l,m,n);
	*initial=ls->minimal_surface(*initialdata,*input,5.0,0.1,-3,1.5,1,iter_outer,pt);
	char *outname1="inner5-8_2.raw";
	char outdir[200]=output;

	strcat(outdir,dirbody);
	strcat(outdir,outname1);
	//test.readImage2(initial->getdata(),outdir,l*m*n);
	test.writeImageName(*initial,outdir);
	//Raw temp(*initial);
	//ls->outerwallauto(*initial,*input,10,0.1,-6,1.5,1,10,pt);
	//*initial -=temp;
	//char *outname2="outer5-8_2_20140405.raw";
	//char outdir2[200]=output;
	//strcat(outdir2,dirbody);
	//strcat(outdir2,outname2);
	//test.writeImageName(*initial,outdir2);
	//evaluate(dir,l,m,n);
	}
	else
	{
		seedGrowing(input,seedlist);

		char *outname1="inner5-8_2.raw";
		char outdir[200]=output;
		strcat(outdir,dirbody);
		strcat(outdir,outname1);
		for (int i=0;i<input->size();i++)
		{
			if (input->getXYZ(i)!=200)
			{
				input->putXYZ(i,0);
			}
		}
		test.writeImageName(*input,outdir);
	}

}
void seedGrowing(Raw * src,vector<Seed> seedlist)
{
	queue<Seed> seedqueue;
	//Seed seedl=;
	seedqueue.push(seedlist.back());
	seedlist.pop_back();

	while (!seedqueue.empty())
	{
		
		Seed start=seedqueue.front();
		seedqueue.pop();
		//int size=1;
		//for (int i=start.x-1; i <start.x+1;i++)
		//{
		//	for (int j=start.y-1; j<start.y+1;j++)
		//	{
		//		for (int k=start.z-1; k<start.z+1; k++)
		//		{
		//			if (src->get(i,j,k)==1)
		//			{
		//				Seed *adj=new Seed(i,j,k);
		//				seedqueue.push(*adj);
		//				src->put(i,j,k,200);
		//			}
		//		}
		//	}
		//}
		int i=start.x;
		int j=start.y;
		int k=start.z;
		//for (int i = 0; i < src->size(); i++)
		//{
		//	src->putXYZ(i,0);

		//}
		if (i>1&&i<src->getXsize()-1&&j>1 && j<src->getYsize()-1 && k > 1&& k<src->getZsize()-1)
		{
			int up=src->get(i-1,j,k)==1;
			int down=src->get(i+1,j,k)==1;
			int left=src->get(i,j-1,k)==1;
			int right=src->get(i,j+1,k)==1;
			int front =src->get(i,j,k-1)==1;
			int back=src->get(i,j,k+1)==1;
			if (src->get(i,j,k)==1)
			{
				int count=0;
				if (up)
				{
					Seed *adj=new Seed(i-1,j,k);
					seedqueue.push(*adj);
					//src->put(i-1,j,k,200);
					count++;
				}
				if (down)
				{
					Seed *adj=new Seed(i+1,j,k);
					seedqueue.push(*adj);
						count++;
					//src->put(i+1,j,k,200);
				}
				if (left)
				{
					Seed *adj=new Seed(i,j-1,k);
					seedqueue.push(*adj);
						count++;
					//src->put(i,j-1,k,200);
				}
				if (right)
				{
					Seed *adj=new Seed(i,j+1,k);
					seedqueue.push(*adj);
						count++;
					//src->put(i,j+1,k,200);
				}
				if (front)
				{
					Seed *adj=new Seed(i,j,k-1);
					seedqueue.push(*adj);
						count++;
					//src->put(i,j,k-1,200);
				}
				if (back)
				{
					Seed *adj=new Seed(i,j,k+1);
					seedqueue.push(*adj);
						count++;
					//src->put(i,j,k+1,200);
				}
				if (count>0)
				{
					src->put(i,j,k,200);
				}
				else src->put(i,j,k,300);
			}
		}


	}//...while


}