
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define _pi M_PI

struct Complex
{
	double x;
	double y;
};

struct FFFT
{
	int bits;
	int len_2;
	int len;
	int *exchangeMap;
	struct Complex *e;
	struct Complex *F;
};

struct FFFT* init_FFFT(int bits)
{
	int i,j,k;
	int p;
	struct FFFT *res;
	double ang;
	res=(struct FFFT*)malloc(sizeof(struct FFFT));
	res->bits=bits;
	res->len=1<<(res->bits);
	res->len_2=res->len/2;
	res->exchangeMap=(int*)calloc(res->len,sizeof(int));
	res->e=(struct Complex*)calloc( res->len_2,sizeof(struct Complex));
	res->F=(struct Complex*)calloc(res->len,sizeof(struct Complex));
	for(i=0;i<res->len;i++)
	{
		p=0;
		k=i;
		for(j=0;j<res->bits;j++)
		{
			p=(p<<1)+(k%2);
			k>>=1;
		}
		res->exchangeMap[i]=p;
	}
	for(i=0;i<res->len_2;i++)
	{
		ang=-2*_pi*i/res->len;
		res->e[i].x=cos(ang);
		res->e[i].y=sin(ang);
	}
	return res;
}

int f_FFT(struct FFFT* fft,struct Complex *f)
{
	int i,j,k;
	int unitNum,unitSize,unitSize_2;
	int offset,pPre,pPost;
	struct Complex pre,post,coef,temp;
	for(i=0;i<fft->len;i++)
	{
		//fft->F[i].x=f[fft->exchangeMap[i]].x/fft->len;
		//fft->F[i].y=f[fft->exchangeMap[i]].y/fft->len;
		fft->F[i].x=f[fft->exchangeMap[i]].x;
		fft->F[i].y=f[fft->exchangeMap[i]].y;
		//printf("F[%d]=(%g,%g)\n",i,fft->F[i].x,fft->F[i].y);
	}
	for(i=0;i<fft->bits;i++)
	{

		unitSize_2=1<<i;
		unitSize=unitSize_2<<1;
		unitNum=1<<(fft->bits-i-1);
		//printf("unitNum:%d,unitSize:%d\n",unitNum,unitSize);
		for(j=0;j<unitNum;j++)
		{
			offset=unitSize*j;
			for(k=0;k<unitSize_2;k++)
			{
				coef=fft->e[unitNum*k];
				pPre=offset+k;
				pPost=pPre+unitSize_2;
				pre=fft->F[pPre];
				post=fft->F[pPost];
				//printf("(F1,F2)=(%d,%d)\n",pPre,pPost);
				//printf("before---post=(%g,%g)\n",post.x,post.y);
				temp.x=(post.x*coef.x)-(post.y*coef.y);
				temp.y=(post.x*coef.y)+(post.y*coef.x);
				//printf("coef=(%g,%g)\t",coef.x,coef.y);
				//printf("post=(%g,%g)\n",post.x,post.y);
				fft->F[pPre].x=pre.x+temp.x;
				fft->F[pPre].y=pre.y+temp.y;
				fft->F[pPost].x=pre.x-temp.x;
				fft->F[pPost].y=pre.y-temp.y;
				/*
				F[pPre]=pre+coef*post;
				F[pPost]=pre-coef*post;
				*/
			}
			/*
			for(int m=0;m<fft->len;m++)
			{
				printf("F[%d]=(%g,%g)\n",m,fft->F[m].x,fft->F[m].y);
			}
			*/


		}
	}
	for(i=0;i<fft->len;i++)
	{
		fft->F[i].x/=fft->len;
		fft->F[i].y/=fft->len;
	}
	return 0;
}

int main()
{
	int i,j,k;
	struct FFFT *f;
	struct Complex ff[1024];

	f=init_FFFT(10);
	for(k=0;k<1000*500;k++)
	{
	for(i=0;i<f->len;i++)
	{
		//printf("(%d,%d)\n",i,f->exchangeMap[i]);
		ff[i].x=i;
		ff[i].y=0;
	}
	f_FFT(f,ff);
	for(i=0;i<f->len;i++)
	{
		//printf("(%g,%g)\n",f->F[i].x,f->F[i].y);
	}
	}
	
	return 0;
}
