#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <string.h>


int main( int argc, char *argv[] )
{
   int fd,stat,len;
   char buf[128];
   char rcvbuf[128];
   struct sockaddr_un localaddr, remoteaddr;

   fd = socket( PF_UNIX, SOCK_DGRAM, 0 );
   if( fd < 0 )
   {
     perror( "socket" );
     exit(1);
   }

   memset( &localaddr, '\0', sizeof(localaddr) );
   localaddr.sun_family = PF_UNIX;

   strcpy( localaddr.sun_path, "/tmp/up.socket");

   stat = bind( fd, (struct sockaddr *) &localaddr, sizeof(localaddr) );

   chmod( "/tmp/up.socket", 0777 );

   memset( &remoteaddr, '\0', sizeof(remoteaddr) );
   remoteaddr.sun_family = PF_UNIX;
   strcpy(remoteaddr.sun_path,"/tmp/mv.socket");

   strcpy(buf, "test message - test message - test message");

   recvfrom( fd, (void *) rcvbuf, strlen(buf), 0, (struct sockaddr *) &remoteaddr, &len );

   printf( " msg [%s]\n", rcvbuf );

   sendto( fd, (void *) buf, strlen(buf), 0, (struct sockaddr *) &remoteaddr, sizeof(remoteaddr) );

   close(fd);

   return(0);
}
