/* sleep1000.c - sleep1000 */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep1000 --  delay the caller for a time specified in 1/100 of seconds
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL sleep1000(int n)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    

	if (n < 0  || clkruns==0) {
		endTime = ctr1000;
		record_syscall(getpid(), "sleep1000", endTime-startTime);
	        return(SYSERR);
	}
	disable(ps);
	if (n == 0) {		/* sleep1000(0) -> end time slice */
	        ;
	} else {
		insertd(currpid,clockq,n);
		slnempty = TRUE;
		sltop = &q[q[clockq].qnext].qkey;
		proctab[currpid].pstate = PRSLEEP;
	}
	resched();
        restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "sleep1000", endTime-startTime);
	return(OK);
}
