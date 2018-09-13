/* gettime.c - gettime */

#include <conf.h>
#include <kernel.h>
#include <date.h>

extern int getutim(unsigned long *);

/*------------------------------------------------------------------------
 *  gettime  -  get local time in seconds past Jan 1, 1970
 *------------------------------------------------------------------------
 */

extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);

SYSCALL	gettime(long *timvar)
{
    /* long	now; */

	/* FIXME -- no getutim */

    unsigned long startTime, endTime;
    startTime = ctr1000;
    endTime = ctr1000;
    
	
    record_syscall(getpid(), "gettime", endTime-startTime);
    return OK;
}
