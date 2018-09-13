/* PA0 Task 3 Author: Anupam Godse Email: angodse@ncsu.edu
 * function printos prints the top of the stack content 
 * before and after calling the function. It also prints
 * address and contents of upto 4 stack locations below
 * the top of stack.
 */

#include<stdio.h>

unsigned long *esp, *ebp;

void printtos() {
	
	/*loading values of esp and ebp registers into esp 
 	 *and esp variables*/
	
	asm("movl %esp, esp");
	asm("movl %ebp, ebp");
	
	/*stack frame size taken from objdump*/
	unsigned long stackFrameSize = 0x4c;

	/*size of a single stack element*/	
	unsigned int sizeOfStackElement = sizeof(unsigned long);
	
	/*Stack Pointer(esp) content just after the printtos() call*/
	unsigned long *stackPointerAddressAfterFunctionCall = esp + 
		stackFrameSize/sizeOfStackElement;
		
	printf("Before[0x%08x]: 0x%08x\n"
		"After[0x%08x]: 0x%08x\n"
		"\telement[0x%08x]: 0x%08x\n"
		"\telement[0x%08x]: 0x%08x\n"
		"\telement[0x%08x]: 0x%08x\n"
		"\telement[0x%08x]: 0x%08x\n",
		stackPointerAddressAfterFunctionCall+1,
		*(stackPointerAddressAfterFunctionCall+1), 
		stackPointerAddressAfterFunctionCall,
		*stackPointerAddressAfterFunctionCall, 
		esp, *esp, esp+1, *(esp+1), esp+2, *(esp+2),
		esp+3, *(esp+3));
	
	return;
}

