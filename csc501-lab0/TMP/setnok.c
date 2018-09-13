/* setnok.c - setnok */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  setnok  -  set next-of-kin (notified at death) for a given process
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL	setnok(int nok, int pid)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;

	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid)) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "setnok", endTime-startTime);
		return(SYSERR);
	}
	pptr = &proctab[pid];
	pptr->pnxtkin = nok;
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "setnok", endTime-startTime);

	return(OK);
}
