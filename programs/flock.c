#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main( int argc, char *argv[] )
{
   int fd;
   char buf[128];

   fd = open( argv[1], O_RDONLY );
   if( fd < 0 )
   {
     perror( argv[1] );
     return 1;
   }
   flock( fd, LOCK_EX|LOCK_NB );
   perror( "");
   read( fd, buf, 128 );
   write( 1, buf, 128 );
   sleep(10);
   flock( fd, LOCK_UN );

   close(fd);

   return(0);
}
