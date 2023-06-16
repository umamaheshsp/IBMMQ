#include <stdio.h>

int main( int argc, char *argv[] )
{
  int ret;
  ret = fork();
  if( 0 == ret )
  {
    printf("child\n");
    sleep(15);
  }
  else
  {
    printf("Parent\n");
  }
}
