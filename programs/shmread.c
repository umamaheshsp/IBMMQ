#include<stdio.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>


int main( int argc, char *argv[] )
{

  int pid;
  int fd;
  int stat,len;
  int shmkey, shmid;
  char *buf;

  shmkey = ftok( "/tmp/myshmipc", 'm' );
  printf( "shmkey = [%d][0x%x]\n", shmkey, shmkey );

  shmid = shmget( shmkey, 0, SHM_RDONLY );
  printf( "shmid = [%d][0x%x]\n", shmid, shmid );

  buf = (char *) shmat( shmid, NULL, 0 );

  printf( "message read [%s]\n", buf );
  return 0;
}
