#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main( int argc, char *argv[] )
{
   int fd;
   char buf[128];
   struct flock lockinfo;

   fd = open( argv[1], O_RDWR );
   if( fd < 0 )
   {
     perror( argv[1] );
     return 1;
   }

   memset( buf, '1', sizeof(buf) );
   lockinfo.l_type = F_WRLCK;
   lockinfo.l_whence = SEEK_SET;
   lockinfo.l_start = 1;
   lockinfo.l_len = 100;

   fcntl( fd, F_SETLKW, & lockinfo );
   perror( "fcntl" );

   write( fd, buf, 128 );

   sleep(10);

   lockinfo.l_type = F_UNLCK;
   lockinfo.l_whence = SEEK_SET;
   lockinfo.l_start = 1;
   lockinfo.l_len = 100;

   fcntl( fd, F_SETLK, & lockinfo );

   close(fd);

   return(0);
}
