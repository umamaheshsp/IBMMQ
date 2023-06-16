/* Client program */
#include<stdio.h>
#include<string.h> 
/* #include<iostream.h> */
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
   int shmid;
   struct sembuf rbuf, wbuf;
   message  *p_mem;

   rsemid = semget(RSEMKEY,1,IPC_CREAT | 0600); 
   wsemid = semget(WSEMKEY,1,IPC_CREAT | 0600); 
   shmid  = shmget(SHMKEY,1024,IPC_CREAT | 0600); 

   semctl(rsemid,0,SETVAL,0);
   semctl(wsemid,0,SETVAL,0);

   p_mem = (message *) shmat(shmid,0,0);

   strcpy(p_mem->msgbuf,msgbuf);
   p_mem->nread = strlen(msgbuf);
   
   printf("Message Sent from Client to Server      : [%s]\n",p_mem->msgbuf);

   wbuf.sem_num = 0; 
   wbuf.sem_op  = -1; 
   wbuf.sem_flg = 0; 

   rbuf.sem_num = 0; 
   rbuf.sem_op  = 1; 
   rbuf.sem_flg = 0; 

   semop(rsemid,&rbuf,1);
   semop(wsemid,&wbuf,1);

   printf("Result Received by Client from Server   : [%s]\n",p_mem->msgbuf);

}

