#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <stdio.h>

int prX;
void halt();

/*------------------------------------------------------------------------
 *  *  main  --  user main program
 *   *------------------------------------------------------------------------
 *    */
prch(c)
char c;
{
	int i;
	sleep(5);	
}
int main()
{
	kprintf("task 1 (zfunction)\n");
	kprintf("input: 0x%08x\toutput: 0x%08x\n\n", 0xaabbccdd, 
		zfunction(0xaabbccdd));
	
	kprintf("task 2 (printsegaddress)\n");
	printsegaddress();
	kprintf("\n");
	
	kprintf("task 3 (printtos)\n");
	printtos();
	kprintf("\n");
	
	kprintf("task 4 (printprocstks)\n");
	printprocstks(0);
	kprintf("\n");
	
	kprintf("task 5 (printsyscallsummary)\n");
	syscallsummary_start();        
	resume(prX = create(prch,2000,20,"proc X",1,'A'));
	sleep(10);
	syscallsummary_stop();
	printsyscallsummary();
	return 0;
}

