/* screate.c - screate, newsem */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sem.h>
#include <stdio.h>

LOCAL int newsem();

/*------------------------------------------------------------------------
 * screate  --  create and initialize a semaphore, returning its id
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL screate(int count)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;

	STATWORD ps;    
	int	sem;

	disable(ps);
	if ( count<0 || (sem=newsem())==SYSERR ) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "screate", endTime-startTime);
		return(SYSERR);
	}
	semaph[sem].semcnt = count;
	/* sqhead and sqtail were initialized at system startup */
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "screate", endTime-startTime);

	return(sem);
}

/*------------------------------------------------------------------------
 * newsem  --  allocate an unused semaphore and return its index
 *------------------------------------------------------------------------
 */
LOCAL int newsem()
{
	int	sem;
	int	i;

	for (i=0 ; i<NSEM ; i++) {
		sem=nextsem--;
		if (nextsem < 0)
			nextsem = NSEM-1;
		if (semaph[sem].sstate==SFREE) {
			semaph[sem].sstate = SUSED;
			return(sem);
		}
	}
	return(SYSERR);
}
