/* resched.c  -  resched */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <q.h>
#include <sched.h>
#include <stdio.h>
#include <math.h>

#define LAMBDA 0.1

unsigned long currSP;	/* REAL sp of current process */
extern int ctxsw(int, int, int, int);
extern double expdev(double);
int getexpsched(int , int, int);
void init_epoch();
int getlinuxsched(int, int);

//class of scheduler
int class;

int reset_epoch = 0; //1 then start new epoch


/*-----------------------------------------------------------------------
 * resched  --  reschedule processor to highest priority ready process
 *
 * Notes:	Upon entry, currpid gives current process id.
 *		Proctab[currpid].pstate gives correct NEXT state for
 *			current process if other than PRREADY.
 *------------------------------------------------------------------------
 */
int resched()
{
	
	register struct	pentry	*optr;	/* pointer to old process entry */
	register struct	pentry	*nptr;	/* pointer to new process entry */
	int random;

	
	if(class == EXPDISTSCHED) { //exponential distribution schedular
		optr = &proctab[currpid];	

		/* force context switch */
		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		//generate a random number 
		//the number returned by expdev is double 
		//typecasting it to int dosent have any drawbacks 
		//because we need process just greater than the randomely 
		//generated number
		random = (int) expdev(LAMBDA); 

		//get next process to schedule
		nptr = &proctab[ (currpid = getexpsched(rdyhead, rdytail, random)) ];
		//make it as currently running
		nptr->pstate = PRCURR;		/* mark it currently running	*/

	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif
	}
	else if(class == LINUXSCHED){ //Linux like schedular
		//if preempt is negative make it 0
		if(preempt < 0) {
			preempt = 0;
		}
	
		//take old process in ptr in optr
		optr = &proctab[currpid];
	
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}
	
		//Assign counter to remaining time of the process
		optr->quantum = preempt;

		//the process is in current epoch because it is scheduled
		//forced to handle first call of resched after scheduler type
		//is changed
		optr->in_epoch = 1;
	
		//if the process has used its time remove it from current epoch
		if(preempt == 0) {
			optr->in_epoch = 0;
		}
		
		
		//if reset_epoch is 1 then start new epoch
		if(reset_epoch == 1) {
			init_epoch();
		}
			

		//get next process to be scheduled
		nptr = &proctab[currpid = getlinuxsched(rdyhead, rdytail)];
		
		//mark it as currently running process
		nptr->pstate = PRCURR;

		//if the process is not nullprocess then reduce epoch by the 
		//time quantum given to process and assign preempt to 
		//time quantum left for a process
		if(currpid != 0) {
			reset_epoch = 0;
			preempt = nptr->quantum;
		}
		else { 
			//now as the only proces left in current epoch is null process		
			//we should reset the epoch so that next time resched is called 
			//the next epoch starts
			preempt = 0;
			reset_epoch = 1;
		}
	}
	else { //default schedular
		if ( ( (optr= &proctab[currpid])->pstate == PRCURR) &&
		   (lastkey(rdytail)<optr->pprio)) {
			return(OK);
		}
		
		/* force context switch */

		if (optr->pstate == PRCURR) {
			optr->pstate = PRREADY;
			insert(currpid,rdyhead,optr->pprio);
		}

		/* remove highest priority process at end of ready list */

		nptr = &proctab[ (currpid = getlast(rdytail)) ];
		nptr->pstate = PRCURR;		/* mark it currently running	*/
	
	#ifdef	RTCLOCK
		preempt = QUANTUM;		/* reset preemption counter	*/
	#endif

	}

	
	ctxsw((int)&optr->pesp, (int)optr->pirmask, (int)&nptr->pesp, (int)nptr->pirmask);
	
	/* The OLD process returns here when resumed. */
	return OK;
}


/* gets the item with key just than target*/
int getexpsched(int head, int tail, int target) {
	int next;
	
	//Starting to search target priority from head		
	next = q[head].qnext;

	//We want process with priority just greater than the random value
	//so keep on doing qnext until the next has priority greater than random
	//also check if next is not equal to tail
	while((next != tail) && (q[next].qkey <= target)) {
		next = q[next].qnext;
	}

	//Now we have process with priority just greater than the random number
	//this loop handles the case in which there are other processes in the
	//ready queue with same priority i.e. just greater than random
	//We now want the process closer to tail so that round robin in preserved
	while((next != tail) && (q[q[next].qnext].qkey == q[next].qkey)) {
		next = q[next].qnext;
	}
	
	//if we reach tail then schedule the qprev of tail
	if(next == tail) {
		next = q[next].qprev;
	}

	//remove from ready queue
	dequeue(next);

	return next;
	
}


void setschedclass (int sched_class) {
	//set scheduling class
	class = sched_class;
	return;	
}

int getschedclass() {
	//return scheduling class
	return class;
}
void init_epoch() {
	int i;
	//if process is not free then include it in epoch and assign it time quantum
	for(i = 0; i < NPROC; i++) {
		if(proctab[i].pstate != PRFREE) {
			if(proctab[i].in_epoch == 1) {
				proctab[i].quantum =  ((proctab[i].quantum / 2) + proctab[i].pprio);
			}
			else {
				proctab[i].quantum = proctab[i].pprio;
				proctab[i].in_epoch = 1;
			}
			proctab[i].pprio_epoch = proctab[i].pprio;
		}	
		else {
			proctab[i].in_epoch = 0;
			proctab[i].quantum = 0;
		}
	} 
	return;
}

int getlinuxsched(int head, int tail) {
	int goodness, max_goodness = -1, max_goodness_idx = 0;
	int next = q[head].qnext;
	while(next != tail) {
		if(proctab[next].in_epoch == 1) {
			goodness = proctab[next].quantum + proctab[next].pprio_epoch;
			if(goodness > max_goodness) {
				max_goodness = goodness;
				max_goodness_idx = next;
			}
		}
		next = q[next].qnext;
	}
	dequeue(max_goodness_idx);
	return max_goodness_idx;
}
