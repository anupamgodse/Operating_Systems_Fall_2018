/* chprio.c - chprio */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * chprio  --  change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;	
extern void record_syscall(int, char *, unsigned long);
SYSCALL chprio(int pid, int newprio)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || newprio<=0 ||
	    (pptr = &proctab[pid])->pstate == PRFREE) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(pid, "chprio", endTime - startTime);	
		return(SYSERR);
	}
	pptr->pprio = newprio;
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "chprio", endTime-startTime);
	return(newprio);
}
