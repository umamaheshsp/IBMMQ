#include <stdio.h>

int main( int argc, char *argv[] )
{
  int ret = fork();
  if( 0 == ret )
     execl("/bin/ls", "ls", "/bin", 0);
  else
      printf( "In parent pid is %d\n", getpid() );
}
