/* getprio.c - getprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * getprio -- return the scheduling priority of a given process
 *------------------------------------------------------------------------
 */

extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);

SYSCALL getprio(int pid)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;	
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "getprio", endTime-startTime);
		return(SYSERR);
	}
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "getprio", endTime-startTime);
	return(pptr->pprio);
}
