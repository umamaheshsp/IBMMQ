#include<stdio.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main( int argc, char *argv[] )
{

  int pid;
  int fd;
  int stat,len;

  mknod("/tmp/mytestpipe", S_IFIFO|0666,0);

  fd = open("/tmp/mytestpipe", O_WRONLY);
  if( fd == -1 )
    perror("pipe");

   while(1)
   {
    len = write(fd, "TESTMESSAGE", 11);
    perror( "write" );
    printf( " len writeen = %d\n", len);
   }
  return 0;
}
