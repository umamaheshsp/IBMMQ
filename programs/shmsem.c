#include<stdio.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

union semun {
		int              val;    /* Value for SETVAL */
		struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;  /* Array for GETALL, SETALL */
		struct seminfo  *__buf;  /* Buffer for IPC_INFO
		(Linux specific) */
};


int main( int argc, char *argv[] )
{

  int pid;
  int fd;
  int stat,len;
  int shmkey, shmid, semkey, semid;
  char *buf;
  union semun sem_val;
  struct sembuf sem_buf;

  shmkey = ftok( "/tmp/myshmipc", 'm' );
  printf( "shmkey = [%d][0x%x]\n", shmkey, shmkey );

  shmid = shmget( shmkey, 2048, IPC_CREAT|0777 );
  printf( "shmid = [%d][0x%x]\n", shmid, shmid );

  buf = (char *) shmat( shmid, NULL, 0 );

  semkey = ftok( "/tmp/myshmipc", 's' );
  printf( "semkey = [%d][0x%x]\n", semkey, semkey );

  semid = semget( semkey, 1, IPC_CREAT|0777 );
  printf( "semid = [%d][0x%x]\n", semid, semid );

  sem_val.val = 2; 
  semctl(semid, 0, SETVAL, sem_val);


  sem_buf.sem_num = 0;
  sem_buf.sem_op = -1;
  sem_buf.sem_flg = SEM_UNDO;

  semop(semid, &sem_buf, 1);

  memset( buf, '\0',  2048 );
  strcpy( buf, "MYTESTMESSAGE" );

  while(1);

  return 0;
}
