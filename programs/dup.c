#include <sys/fcntl.h>

int main( int argc, char *argv[] )
{
  int fd;
    fd = open( "newfile.txt", O_WRONLY );
    close(1);
   dup(fd);
   printf("This appears on the screen\n");
}
