/* getpid.c - getpid */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * getpid  --  get the process id of currently executing process
 *------------------------------------------------------------------------
 */

extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL getpid()
{
	unsigned long startTime = ctr1000, endTime;
	endTime = ctr1000;
	record_syscall(currpid, "getpid", endTime-startTime);
	
	return(currpid);

}
