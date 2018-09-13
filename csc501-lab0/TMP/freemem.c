/* freemem.c - freemem */

#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <stdio.h>

/*------------------------------------------------------------------------
 *  freemem  --  free a memory block, returning it to memlist
 *------------------------------------------------------------------------
 */

/*changes for printsyscallsummary*/
extern unsigned long ctr1000;
extern void record_syscall(int, char *, unsigned long);

SYSCALL	freemem(struct mblock *block, unsigned size)
{	
	int pid = getpid();	
	unsigned long startTime, endTime;
	startTime = ctr1000;	
	
	STATWORD ps;    
	struct	mblock	*p, *q;
	unsigned top;

	if (size==0 || (unsigned)block>(unsigned)maxaddr
	    || ((unsigned)block)<((unsigned) &end)) {
		endTime = ctr1000;
		record_syscall(pid, "freemem", endTime - startTime);	
		return(SYSERR);
	}
	size = (unsigned)roundmb(size);
	disable(ps);
	for( p=memlist.mnext,q= &memlist;
	     p != (struct mblock *) NULL && p < block ;
	     q=p,p=p->mnext )
		;
	if (((top=q->mlen+(unsigned)q)>(unsigned)block && q!= &memlist) ||
	    (p!=NULL && (size+(unsigned)block) > (unsigned)p )) {
		restore(ps);
		endTime = ctr1000;
		record_syscall(pid, "freemem", endTime - startTime);	
		return(SYSERR);
	}
	if ( q!= &memlist && top == (unsigned)block )
			q->mlen += size;
	else {
		block->mlen = size;
		block->mnext = p;
		q->mnext = block;
		q = block;
	}
	if ( (unsigned)( q->mlen + (unsigned)q ) == (unsigned)p) {
		q->mlen += p->mlen;
		q->mnext = p->mnext;
	}
	restore(ps);
	
	endTime = ctr1000;

	record_syscall(pid, "freemem", endTime - startTime);	
	return(OK);
}
