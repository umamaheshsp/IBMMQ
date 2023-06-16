//Server program
#include<stdio.h>
#include<string.h>
//#include<iostream.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<netinet/in.h>

#define MSGSIZE 26
#define RSEMKEY 10
#define WSEMKEY 20
#define SHMKEY 30

char msgbuf[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
typedef struct message
{
   int nread;
   char msgbuf[500];
}message;


void main(void)
{
   int rsemid, wsemid;
   int shmid,i;
   struct sembuf rbuf, wbuf;
   message  *p_mem;
   int myval;

   rsemid = semget(RSEMKEY,1,IPC_CREAT | 0600); 
   wsemid = semget(WSEMKEY,1,IPC_CREAT | 0600); 
   shmid  = shmget(SHMKEY,1024,IPC_CREAT | 0600); 

   semctl(rsemid,0,SETVAL,0);
   semctl(wsemid,0,SETVAL,0);

   myval = semctl(rsemid,0,GETVAL,0);
   printf("myval1[%d]\n", myval);
   myval = semctl(wsemid,0,GETVAL,0);
   printf("myval2[%d]\n", myval);

   p_mem = (message *) shmat(shmid,0,0);

   wbuf.sem_num = 0; 
   wbuf.sem_op  = -1; 
   wbuf.sem_flg = 0; 

   rbuf.sem_num = 0; 
   rbuf.sem_op  = 1; 
   rbuf.sem_flg = 0; 
   
   printf("Server is waiting for Message...\n");
   semop(rsemid,&wbuf,1);
   printf("Message Received by Server from Client  : [%s]\n",p_mem->msgbuf);
   for(i=0;i<26;i++)
      p_mem->msgbuf[i] +=  32 ; 

   myval = semctl(rsemid,0,GETVAL,0);
   printf("myval4[%d]\n", myval);

   semop(wsemid,&rbuf, 1);
   semop(wsemid,&rbuf, 1);
   printf("Result Sent by Server to Client         : [%s]\n",p_mem->msgbuf);
   myval = semctl(wsemid,0,GETVAL,0);
   printf("myval5[%d]\n", myval);
   
}
