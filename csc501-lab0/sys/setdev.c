/* setdev.c - setdev */

#include <conf.h>
#include <kernel.h>
#include <proc.h>

extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
/*----------------------------------------------------------------------n
 *  setdev  -  set the two device entries in the process table entry
 *------------------------------------------------------------------------
 */
SYSCALL	setdev(int pid, int dev1, int dev2)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	short	*nxtdev;

	if (isbadpid(pid)) {
		endTime = ctr1000;
		record_syscall(getpid(), "setdev", endTime-startTime);	
		return(SYSERR);
	}
	nxtdev = (short *) proctab[pid].pdevs;
	*nxtdev++ = dev1;
	*nxtdev = dev2;

	endTime = ctr1000;
	record_syscall(getpid(), "setdev", endTime-startTime);

	return(OK);
}
