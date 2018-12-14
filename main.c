#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include "header.h"
#include <errno.h>

#define KEY 1337


union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
   		                     (Linux-specific) */
} arg;




int main(){

	init();
	int semaphore = semget(KEY,1,IPC_CREAT | 0644);
	struct sembuf operations[1];

	   if(semaphore < 0)
	   {
	      printf("Program sema cannot find semaphore, exiting.\n");
	      exit(0);
	}

	/* Which semaphore in the semaphore array : */
    operations[0].sem_num = 0;
    /* Which operation? Add 1 to semaphore value : */
    operations[0].sem_op = -1;
    /* Set the flag so we will wait : */   
	operations[0].sem_flg = 0;
	//retval = semop(id, operations, 0);
	if(semctl(semaphore,operations,GETVAL)){
		//semop(semaphore, operations, 0);
		int oh = semctl(semaphore,0,GETVAL);
		printf("%s\n",strerror(errno));
		printf("semctl:%d\n",oh);
		
		arg.val = 0;
		semctl(semaphore,0,SETVAL);
		printf("%s\n",strerror(errno));
		semop(semaphore, 0, -1);
		printf("%s\n",strerror(errno));
		oh = semctl(semaphore,0,GETVAL);
		printf("%s\n",strerror(errno));		
		printf("semctl:%d\n",oh);
		printf("write to file:\n");
				char* str;
				str = malloc(256);
				scanf("%s",str);
				int file = open("test.txt",O_APPEND | O_RDWR);
				printf("str:%s\n",str);
				printf("strlen:%d\n",strlen(str));
				printf("test:%d\n",file);
				
				if(write(file, str, strlen(str))== -1) {
	  				  printf("Oh dear, something went wrong with write()! %s\n", strerror(errno));
				}
				close(file);
	}
	/*
	char * args;
	args = malloc(256);
	printf("please give an argument: (type --help for help)\n");
	scanf("%s",args);
	while(1){
		//for debug only
		//for our actual program, use argc/argv instead

		if(!strcmp("--help",args)){
			printf("create: -C\nremove: -r\nview: -v\n");
			free(args);
		}else if(!strcmp("-C",args)){

			//create shared memory
			int shared_mem = shmget(KEY,1024,IPC_CREAT | 0644);
			char *str = (char*)shmat(shared_mem,0,0);
			str = malloc(256);
			printf("write something:\n");
			scanf("%s",str);
			free(str);
			shmdt(shared_mem);
			
			
			
			//create semaphore
			printf("oohh\n");
			int semaphore = semget(KEY,2,IPC_CREAT | 0644);
			
		
			//create file
			int file = open("test.txt", O_CREAT | O_APPEND | 0644);
			printf("write to file:\n");
			scanf("%s",str);
			write(file, str, sizeof(*str));
			
			close(file);


			break;
		}else if(!strcmp("-v",args)){
			char *str;
			int seg_id;
			//printf("id please:\n");
			//scanf("%d",seg_id);
			int shared_mem = shmget(KEY,1024,0);
			str = (char*)shmat(shared_mem,0,0);
			//printf("wut?\n");
			printf("%s\n",str);
			shmdt(shared_mem);
			break;
		}else{
			printf("retry\n");
			free(args);
		}
		scanf("%s",args);
	}
	*/
	return 0;
}
