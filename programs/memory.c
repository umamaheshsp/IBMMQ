#include <stdio.h>
#include <unistd.h>
#include <procinfo.h>  
#include <stdlib.h>


unsigned long getprocsize()
{
     struct procentry64 psInfo;
     int pid;

    pid = getpid();

        if( getprocs64(&psInfo, sizeof(psInfo), NULL, NULL, &pid, 1) )
          return -1;
         else
          return psInfo.pi_dvm;
          //return psInfo.pi_size;
}

int main(int argc, char *argv[])
{
  char *buf = NULL;
  int pid;
  int i;

  for int(i=1; i<5; i++)
  {
    printf("Before allocation [%lu] \n", getprocsize() );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep");

    buf = malloc(8096 * i);
    printf("After allocation [%lu] \n", getprocsize() );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep");

    free(buf);

    printf("After free [%lu] \n", getprocsize() );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep");

    sleep(15);

    printf("After sleeip 15 seconds [%lu] \n", getprocsize() );
    system( "ps -elf|egrep \"UID|memtest\"|grep -v grep");
  }

  return 0;
}
