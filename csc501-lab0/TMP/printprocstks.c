#include <stdio.h>
#include <kernel.h>
#include <proc.h>

unsigned long *esp;
extern struct pentry proctab[];

void printprocstks(int priority) {
	struct pentry *proc;
	int pid;

	for(pid = 0; pid < NPROC; pid++) {
		proc = &proctab[pid];
		int pprio = proc->pprio;
		if(pprio > priority) {
			printf("Process [%s]\n", proc->pname);
			printf("\tpid: %d\n", pid);
			printf("\tpriority: %d\n", pprio);
			printf("\tbase: 0x%08x\n", proc->pbase);
			printf("\tlimit:0x%08x\n", proc->plimit);
			printf("\tlen: %d\n", proc->pstklen);
			asm("movl %esp, esp");
			printf("\tpointer: 0x%08x\n", esp);
		}
	}
	return;
}
