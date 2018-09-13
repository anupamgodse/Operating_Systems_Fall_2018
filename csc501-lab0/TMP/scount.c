/* scount.c - scount */

#include <conf.h>
#include <kernel.h>
#include <sem.h>

/*------------------------------------------------------------------------
 *  scount  --  return a semaphore count
 *------------------------------------------------------------------------
 */

extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);

SYSCALL scount(int sem)
{
extern	struct	sentry	semaph[];
	unsigned long startTime, endTime;
	startTime = ctr1000;
	if (isbadsem(sem) || semaph[sem].sstate==SFREE) {
		endTime = ctr1000;
		record_syscall(getpid(), "scount", endTime-startTime);	
		return(SYSERR);
	}
	endTime = ctr1000;
	record_syscall(getpid(), "scount", endTime-startTime);

	return(semaph[sem].semcnt);
}
