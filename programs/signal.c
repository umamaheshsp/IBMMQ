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
  while(count--)
  {
     var = getchar();    
     putchar(var);
  }
}
