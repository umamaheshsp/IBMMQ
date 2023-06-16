#include <stdio.h>

int main( int argc, char *argv[] )
{
  fork();
  printf( "process id is %d\n", getpid() );
}
