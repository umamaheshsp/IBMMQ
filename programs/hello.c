#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int main( int argc, char *argv[] )
{
  int fd;
  fd = open("junk", O_RDONLY);
  perror("junk");
}
