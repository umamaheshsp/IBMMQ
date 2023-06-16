#include<stdio.h>
#include<pthread.h>

int fact( int n)
{
  if ( n  <= 1 )
    return 1;
  else 
    return n * fact(n-1);
}

void* myfun( void* ptr)
{
   int n = *((int *) ptr);
   int result;
   result = fact(n);
   printf( " val = %d result = %d\n", n, result );
   return (void *) result;
}

int main( int argc, char *argv[] )
{
  pthread_t tid[10];
  int fval = 10;
  int *result;
  pthread_create( tid, NULL, myfun, (void *) &fval );

  pthread_join( tid[0], (void *)&result);

  printf("printf main = %d\n", result);

  return 0;
   
}
