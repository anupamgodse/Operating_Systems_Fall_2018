/* sleep100.c - sleep100 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep100  --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL sleep100(int n)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0) {
		endTime = ctr1000;
		record_syscall(getpid(), "sleep100", endTime-startTime);
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep100(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*10);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "sleep100", endTime-startTime);
	return(OK);
}
