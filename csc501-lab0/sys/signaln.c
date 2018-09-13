/* signaln.c - signaln */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  signaln -- signal a semaphore n times
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL signaln(int sem, int count)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    
	struct	sentry	*sptr;

	disable(ps);
	if (isbadsem(sem) || semaph[sem].sstate==SFREE || count<=0) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "signaln", endTime-startTime);
		return(SYSERR);
	}
	sptr = &semaph[sem];
	for (; count > 0  ; count--)
		if ((sptr->semcnt++) < 0)
			ready(getfirst(sptr->sqhead), RESCHNO);
	resched();
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "signaln", endTime-startTime);
	return(OK);
}
