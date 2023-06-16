#include<stdio.h>

int main( int argc, char *argv[] )
{

  int pid;
  int fd[2];
  int stat,len;

  pipe(fd);
  pid = fork();
  if( pid == 0 )
  {
    char buf[25];
    printf("Child\n");
    close(fd[1]);
   while(1)
   {
    len = read(fd[0],buf,24);
    buf[len] = '\0';
    printf("Child msg [%s]\n", buf);
   }
  }
  else
  {
   close(fd[0]);
   printf("Parent\n");
   while(1)
   {
    write(fd[1], "TESTMESSAGE", 11);
   }
  }
  return 0;
}
