
/*PA0-Task 2 Author: Anupam Godse email: angodse@ncsu.edu
 * Print the addresses and their contents indicating the end of the text, 
 * data, and BSS segments of the current process. You can refer to the 
 * manual page for "etext". Also print the 4-byte contents 
 * (in hexadecimal) preceding and after those addresses. This function 
 * can be written in C.
 */
#include<stdio.h>
extern unsigned long  etext, edata, end;

void printsegaddress() {
     
    printf("Current: etext[0x%08x]=0x%08x, edata[0x%08x]=0x%08x, ebss[0x%08x]"
        "=0x%08x\n", &etext, *(&etext), &edata, *(&edata), &end, *(&end));

    printf("Preceding: etext[0x%08x]=0x%08x, edata[0x%08x]=0x%08x, "
        "ebss[0x%08x]=0x%08x\n", &etext - 1, *(&etext - 1),
        &edata - 1, *(&edata - 1), &end - 1, *(&end - 1));


    printf("After: etext[0x%08x]=0x%08x, edata[0x%08x]=0x%08x, "
        "ebss[0x%08x]=0x%08x\n", &etext + 1, *(&etext + 1), &edata + 1, 
	*(&edata + 1), &end + 1, *(&end + 1));
    
    return;
}

