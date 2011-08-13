#include "srk_app_base.h"
#pragma hdrstop
#include <stdio.h>
#include <unistd.h>

void t_sleep(unsigned int n)
{
	printf("t_sleep(%u)...",n);
	fflush(stdout);
	usleep(n);
	printf("ok\n");
}

void main()
{
	TSonorkClock	c1,c2,c3;
	UINT u;
	for(u=0;u<10000000;u+=250000)
	{
	c1.LoadCurrent();
	printf("C1: %f\n",c1.GetValue());
	t_sleep(u);	
	c2.LoadCurrent();
	printf("C2: %f\n",c2.GetValue());
	c2.LoadInterval(c1);
	printf("C2-C1: %f\n",c2.GetValue());
	}
}

void sonork_puts(const char *msg)
{
	printf("[Sonork] %s\n",msg);
}
