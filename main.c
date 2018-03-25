#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "FFT.h"

struct Complex f[1<<20],F[1<<20];
int main()
{
	srand(time(NULL));
	int i;
	for(i=0;i<1<<20;i++)
	{
		//f[i].x=(double)rand()/1000;
		//f[i].y=(double)rand()/1000;
		f[i].x=i;
		f[i].y=0;
	}
	/*
	f[16].x=100;
	f[16].y=100;
	f[1008].x=100;
	f[1008].y=-100;
	*/

	for(i=0;i<1;i++)
	{
	FFT(f,F,10);
	//IFFT(F,f,20);
	}
	for(i=0;i<16;i++)
	{
		printf("(%g,%g)\n",F[i].x,F[i].y);
		//printf("%g\n",F[i].x*F[i].x+F[i].y*F[i].y);
		//printf("(%g,%g)\n",f[i].x,f[i].y);
		//printf("%g\n",f[i].x);
	}
	return 0;
}
