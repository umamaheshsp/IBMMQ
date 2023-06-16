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
   int detachstate =0;
   pthread_attr_t tattr;
   arg1 = (struct arg *) ptr;
   printf("%d\n", arg1->count);


   pthread_attr_getdetachstate(&tattr,
                  &detachstate);

   if( detachstate == PTHREAD_CREATE_DETACHED )
   {
     printf("Detached thread\n");
   }
   if( detachstate == PTHREAD_CREATE_JOINABLE )
   {
     printf("Joinable thread\n");
   }

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
  int count = 10;
  pthread_attr_t tattr;

  arg1.count=20;
  arg1.ch='0';

  arg2.count=30;
  arg2.ch='x';

  pthread_attr_init( &tattr );
  pthread_attr_setdetachstate(&tattr,PTHREAD_CREATE_DETACHED);
  pthread_create( tid, &tattr, myfun, (void *) &arg1 );
  pthread_create( tid+1, NULL, myfun, (void *) &arg2 );
  pthread_detach( tid[1]);

  printf("tids : %u - %u\n", tid[0], tid[1]);

  return 0;
   
}
