#include<stdio.h>
#include<pthread.h>

struct arg
{
  int count;
  int ch;
};

void* myfun( void* ptr)
{
   struct arg *arg1;
   arg1 = (struct arg *) ptr;
   printf("%d\n", arg1->count);
   while( (arg1->count)-- )
   {
      write(2,&(arg1->ch),1);
   }
   return;
}

int main( int argc, char *argv[] )
{
  pthread_t tid[10];
  struct arg arg1, arg2;
  int count = 50000;

  arg1.count=20000;
  arg1.ch='0';

  arg2.count=30000;
  arg2.ch='x';

  pthread_create( tid, NULL, myfun, (void *) &arg1 );
  pthread_create( tid+1, NULL, myfun, (void *) &arg2 );

  while(count--) 
  {
      write(2, "1", 1);
  }

  printf("tids : %u - %u\n", tid[0], tid[1]);

  return 0;
   
}
