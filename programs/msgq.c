#include<stdio.h>
#include<fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <sys/msg.h>

  struct msgbuf 
  {
    long mtype;       /* message type, must be > 0 */
    char msg[100];
  };

int main( int argc, char *argv[] )
{

  int pid;
  int fd;
  int stat,len;
  int msgreadqid, msgwriteqid, msgreadkey, msgwritekey;
  struct msgbuf msg1, msg2;

  msgwritekey = ftok( "/tmp/mqmbv", 'm' );
  printf( "msgwritekey = [%d][0x%x]\n", msgwritekey, msgwritekey );

  msgreadkey = ftok( "/tmp/mqup", 'u' );
  printf( "msgreadkey = [%d][0x%x]\n", msgreadkey, msgreadkey );

  msgreadqid = msgget( msgreadkey, 0 );
  printf( "msgreadqid = [%d][0x%x]\n", msgreadqid, msgreadqid );

  msgwriteqid = msgget( msgwritekey, 0 );
  printf( "msgwriteqid = [%d][0x%x]\n", msgwriteqid, msgwriteqid );

  msg1.mtype=getpid();
  strcpy(msg1.msg,"mqseries mq 6.0 7.0 test message");

  if( msgsnd( msgwriteqid, (void *) &msg1, sizeof(msg1), 0) == -1 )
   perror("msgsnd");


  len = msgrcv( msgreadqid, (void *) &msg2, sizeof(msg2), getpid(), 0);
  if(len > 1)
  {
    printf("Msg received [%s]\n", msg2.msg);
  }
 
  return 0;
}
