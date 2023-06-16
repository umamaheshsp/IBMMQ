#include <stdio.h>
#include <sys/wait.h>

int main( int argc, char *argv[] )
{
  int ret;
  int waitstat;
  ret = fork();
  if( 0 == ret )
  {
    printf("child\n");
    exit( 300 );
  }
  else
  {
    wait( &waitstat );
    printf("Parent status %d - %d\n", waitstat, (waitstat & 0xFF) );
    printf( "stat %d\n", WEXITSTATUS(waitstat));
  }
}
