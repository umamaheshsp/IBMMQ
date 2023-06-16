#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  char *buf = NULL;
  int pid, blksize;

  for(int i=1; i<=50; i++ )
  {
    printf( "Block size[%d]\n", (blksize=16384*i) );
    printf( "Before allocation\n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");

    buf = malloc(blksize);
    printf( "After allocation\n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");

    free(buf);

    printf( "After free \n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");
    printf( "------------------------------------------------------------------------------------------------\n" );
  }

    buf = malloc(1024*1024);
    if(buf == NULL)
      return -1;

    printf( "After big allocation \n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");


    free(buf);
    printf( "After big allocation and free \n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");

    sleep(15);

    printf( "After 15 seconds \n" );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep" );
    system( "ps -ef -o vsz,args|egrep \"VSZ|memtest\"|grep -v grep");

  return 0;
}
