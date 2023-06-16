#include<stdio.h>
#include<signal.h>

void SIGINT_handler( int signal )
{
  printf( "Received signal [%s]\n", signal==2?"SIGINT":(signal==3?"SIGQUIT":"OTHER") );
}
int main( int argc, char *argv[] )
{
  int count=10;
  int var;
  sigset_t myset, oldset;

  if( SIG_ERR == signal( SIGINT, SIGINT_handler ))
  {
     perror( "signal SIGINT" );
  }
  if( SIG_ERR == signal( SIGQUIT, SIGINT_handler ))
  {
     perror( "signal SIGQUIT" );
  }
  if( SIG_ERR == signal( SIGTTIN, SIGINT_handler ))
  {
     perror( "signal SIGTTIN" );
  }

  sigemptyset( &myset );
  sigaddset( &myset, SIGINT );
  sigaddset( &myset, SIGQUIT );
  sigprocmask( SIG_BLOCK, &myset, &oldset );

  while(count--)
  {
     printf( "%d\n", count );
     sleep(1);
  }
  sigprocmask( SIG_UNBLOCK, &myset, &oldset );

  sleep(10);
}
