/* sleep10.c - sleep10 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep10  --  delay the caller for a time specified in tenths of seconds
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL	sleep10(int n)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    
	if (n < 0  || clkruns==0) {
		endTime = ctr1000;
		record_syscall(getpid(), "sleep10", endTime-startTime);
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep10(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n*100);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "sleep10", endTime-startTime);
	return(OK);
}
