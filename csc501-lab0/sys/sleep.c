/* sleep.c - sleep */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sleep.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 * sleep  --  delay the calling process n seconds
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL	sleep(int n)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	
	STATWORD ps;    
	if (n<0 || clkruns==0) {
		endTime = ctr1000;
		record_syscall(getpid(), "sleep", endTime-startTime);
		return(SYSERR);
	}
	if (n == 0) {
	        disable(ps);
		resched();
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "sleep", endTime-startTime);
		return(OK);
	}
	while (n >= 1000) {
		sleep10(10000);
		n -= 1000;
	}
	if (n > 0)
		sleep10(10*n);
	endTime = ctr1000;
	record_syscall(getpid(), "sleep", endTime-startTime);
	return(OK);
}
