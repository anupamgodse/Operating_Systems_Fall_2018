/* send.c - send */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  send  --  send a message to another process
 *------------------------------------------------------------------------
 */
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);
SYSCALL	send(int pid, WORD msg)
{
	unsigned long startTime, endTime;
	startTime = ctr1000;
	STATWORD ps;    
	struct	pentry	*pptr;

	disable(ps);
	if (isbadpid(pid) || ( (pptr= &proctab[pid])->pstate == PRFREE)
	   || pptr->phasmsg != 0) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(getpid(), "send", endTime-startTime);	
		return(SYSERR);
	}
	pptr->pmsg = msg;
	pptr->phasmsg = TRUE;
	if (pptr->pstate == PRRECV)	/* if receiver waits, start it	*/
		ready(pid, RESCHYES);
	else if (pptr->pstate == PRTRECV) {
		unsleep(pid);
		ready(pid, RESCHYES);
	}
	restore(ps);
	endTime = ctr1000;
	record_syscall(getpid(), "send", endTime-startTime);
	return(OK);
}
