#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>

#include "key.h"

union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
	 		                     (Linux-specific) */
};

int main(int argc, char * argv[]){
  // make sure there are enough arguments
  if(argc < 2){
    printf("Not enough arguments\n");
    return 0;
  }
  if(argc > 2){
    printf("Too many arguments\n");
    return 0;
  }

  // figure out the argument and do the right something
  char *cmd = argv[1];
  if(!strcmp("-c", cmd)){
    printf("Creating all neccesarry resources\n");

    int shm_id = shmget(KEY, sizeof(int), IPC_CREAT | IPC_EXCL | 0666);
    int sem_id = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0666);
    int fd = open("story.txt", O_CREAT | O_TRUNC | O_EXCL, 0666);

    if(shm_id != -1)
      printf(" Shared memory created!\n");
    else
      printf(" Shared memory already exists!\n");

    if(sem_id != -1)
      printf(" Semaphore created!\n");
    else
      printf(" Semaphore already exists!\n");

    if(fd != -1)
      printf(" File created!\n");
    else
      printf(" File already exists!\n");

    // set semaphore to 1
    if(sem_id != -1){
      union semun set_val;
      set_val.val = 1;
      semctl(sem_id, 0, SETVAL, set_val);
    }

    close(fd);
  }
  if(!strcmp("-r", cmd)){

    int shm_id = shmget(KEY, sizeof(int), 0);
    int sem_id = semget(KEY, 1, 0);

    // down the semaphore (so other processes won't access in the middle of reading)
    struct sembuf op;
    op.sem_op = -1;
    op.sem_num = 0;
    op.sem_flg = SEM_UNDO;

    semop(sem_id, &op, 1);

    // print contents of story

    printf("Printing out contents of the entire story: \n\n");

    int fd = open("story.txt", O_CREAT | O_RDONLY, 0666);

    char line[1024];

    while(read(fd, line, 1024)){
      printf("%s", line);
    }

    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);
    remove("story.txt");

    printf("\nEverything successfully removed!\n\n");

  }
  if(!strcmp("-v", cmd)){ // uses semaphores just because

    int sem_id = semget(KEY, 1, 0);
    if(sem_id == -1)
      printf("Something went wrong, maybe you should initialize everything first...\n");

    struct sembuf op;
    op.sem_op = -1;
    op.sem_num = 0;
    op.sem_flg = SEM_UNDO;

    printf("Printing out contents of the entire story: \n\n");

    // decrement the semaphore
    semop(sem_id, &op, 1);

    int fd = open("story.txt", O_CREAT | O_RDONLY, 0666);

    char line[1024];

    while(read(fd, line, 1024)){
      printf("%s", line);
    }

    // increment the semaphore
    op.sem_op = 1;
    semop(sem_id, &op, 1);

    close(fd);
  }

  return 0;
}
