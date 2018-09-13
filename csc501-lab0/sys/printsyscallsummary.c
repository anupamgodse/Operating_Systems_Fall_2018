#include<stdio.h>
#include<kernel.h>
#include<proc.h>
#define NB_SYSCALLS 27
#define NB_PROCESSES 50

int sysCallTraceEnable;
int count[NB_PROCESSES][NB_SYSCALLS];
unsigned long time[NB_PROCESSES][NB_SYSCALLS];

int get_index(char *syscall) {
	char *sys_calls[NB_SYSCALLS];
	sys_calls[0] = "freemem";
        sys_calls[1] = "chprio";
        sys_calls[2] = "getpid";
        sys_calls[3] = "getprio";
        sys_calls[4] = "gettime";
        sys_calls[5] = "kill";
        sys_calls[6] = "receive";
        sys_calls[7] = "recvclr";
        sys_calls[8] = "recvtim";
        sys_calls[9] = "resume";
        sys_calls[10] = "scount";
        sys_calls[11] = "sdelete";
        sys_calls[12] = "send";
        sys_calls[13] = "setdev";
        sys_calls[14] = "setnok";
        sys_calls[15] = "screate";
        sys_calls[16] = "signal";
        sys_calls[17] = "signaln";
        sys_calls[18] = "sleep";
        sys_calls[19] = "sleep10";
        sys_calls[20] = "sleep100";
        sys_calls[21] = "sleep1000";
        sys_calls[22] = "sreset";
        sys_calls[23] = "stacktrace";
        sys_calls[24] = "suspend";
        sys_calls[25] = "unsleep";
        sys_calls[26] = "wait";

	int i;
	for(i = 0; i<NB_SYSCALLS; i++) {
		if(strcmp(sys_calls[i], syscall) == 0) {
			return i;
		}
	}
	return -1;
}

void record_syscall(int pid, char *syscall, unsigned long t) {
	int index = get_index(syscall);
	
	if(index != -1) {
		if(sysCallTraceEnable==1) {
			count[pid][index] += 1;
			time[pid][index] += t;
		}
	}
	return;	
}


void syscallsummary_start() {
	sysCallTraceEnable = 1;
	int i, j;
	for(i=0; i<NB_PROCESSES; i++) {
		for(j=0; j<NB_SYSCALLS; j++) {
			count[i][j] = 0;
			time[i][j] = 0;
		}
	}
	return;
}

void syscallsummary_stop() {
	sysCallTraceEnable = 0;
}

void printsyscallsummary() {
	char *sys_calls[27];
	sys_calls[0] = "sys_freemem";
	sys_calls[1] = "sys_chprio";
	sys_calls[2] = "sys_getpid";
	sys_calls[3] = "sys_getprio";
	sys_calls[4] = "sys_gettime";
	sys_calls[5] = "sys_kill";
	sys_calls[6] = "sys_receive";
	sys_calls[7] = "sys_recvclr";
	sys_calls[8] = "sys_recvtim";
	sys_calls[9] = "sys_resume";
	sys_calls[10] = "sys_scount";
	sys_calls[11] = "sys_sdelete";
	sys_calls[12] = "sys_send";
	sys_calls[13] = "sys_setdev"; 
	sys_calls[14] =	"sys_setnok"; 
	sys_calls[15] = "sys_screate";
	sys_calls[16] =	"sys_signal"; 
	sys_calls[17] = "sys_signaln";
	sys_calls[18] = "sys_sleep";
	sys_calls[19] = "sys_sleep10";
	sys_calls[20] =	"sys_sleep100"; 
	sys_calls[21] = "sys_sleep1000";
	sys_calls[22] =	"sys_sreset";
	sys_calls[23] =	"sys_stacktrace";
	sys_calls[24] =	"sys_suspend";
	sys_calls[25] =	"sys_unsleep";
	sys_calls[26] =	"sys_wait";
	
	int i, j, k, flag;	
	for(i=0; i<NB_PROCESSES; i++) {
		flag = 0;
		for(k=0; k<NB_SYSCALLS; k++) {
			if(count[i][k] != 0) {
				flag = 1;
			}	
		}
		if(flag == 0) {
			continue;
		}
		printf("Process [pid:%d]\n", i);
		for(j=0; j<NB_SYSCALLS; j++) {
			if(count[i][j] != 0) {
				printf("\tSyscall: %s, count: %d, average execution " 
					"time: %d (ms)\n", sys_calls[j], count[i][j], 
					time[i][j]/count[i][j]);
			}
		}
	}
	return;
}
