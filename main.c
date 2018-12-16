#include <string.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "key.h"

int main(){
  int shm_id = shmget(KEY, sizeof(int), 0);
  int sem_id = semget(KEY, 1, 0);

  if(shm_id == -1 || sem_id == -1){
    printf("Error: %s\n", strerror(errno));
    return 0;
  }
  int * line_size;

  line_size = shmat(shm_id, 0, 0);

  printf("Size of last line added: %d\n", *line_size);

  struct sembuf op;
  op.sem_op = -1;
  op.sem_num = 0;
  op.sem_flg = SEM_UNDO;

  // decrement the semaphore
  semop(sem_id, &op, 1);

  // read the previous line
  int fd = open("story.txt", O_CREAT | O_RDWR, 0666);

  char last_line[*line_size];

  lseek(fd, 0, SEEK_END);
  lseek(fd, -1 * *line_size, SEEK_CUR);

  read(fd, last_line, *line_size + 1);
  last_line[strlen(last_line) - 1] = 0;

  printf("Last line:\n %s\n", last_line);
  printf("Please enter the next line (max 1023 chars): \n ");

  char next_line[1024];

  fgets(next_line, 1024, stdin);

  *line_size = strlen(next_line);

  lseek(fd, 0, SEEK_END);
  write(fd, next_line, strlen(next_line));

  close(fd);

  // increment the semaphore
  op.sem_op = 1;
  semop(sem_id, &op, 1);

  return 0;
}
