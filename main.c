#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>

#define KEY 1337


int main(){

	char * args;
	args = malloc(256);
	printf("please give an argument: (type --help for help)\n");
	scanf("%s",args);
	while(1){
		if(!strcmp("--help",args)){
			printf("create: -C\nremove: -r\nview: -v\n");
			free(args);
		}else if(!strcmp("-C",args)){
			int shared_mem = shmget(KEY,1024,IPC_CREAT | 0644);
			char *str = (char*)shmat(shared_mem,0,0);
			printf("write something:\n");
			scanf("%s",str);
			shmdt(shared_mem);
			break;
		}else if(!strcmp("-v",args)){
			char *str;
			int seg_id;
			//printf("id please:\n");
			//scanf("%d",seg_id);
			str = (char*)shmat(KEY,(void*) 0x5000000,0);
			//printf("wut?\n");
			printf("%s\n",str);
			break;
		}else{
			printf("retry\n");
			free(args);
		}
		scanf("%s",args);
	}

	return 0;
}
