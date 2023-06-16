/* Server program */
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
#include<netinet/in.h>

#define MSGSIZE 26
#define RSEMKEY 10
#define WSEMKEY 20
#define SHMKEY 30

union semun {
		int              val;    /* Value for SETVAL */
		struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
		unsigned short  *array;  /* Array for GETALL, SETALL */
		struct seminfo  *__buf;  /* Buffer for IPC_INFO
		(Linux specific) */
};

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
  struct semid_ds semid_ds;
  union semun semctl_cmd;
  time_t tvar;
  int rc;

  time(&tvar);
   rsemid = semget(RSEMKEY,1,IPC_CREAT | 0600); 
   wsemid = semget(WSEMKEY,1,IPC_CREAT | 0600); 
   shmid  = shmget(SHMKEY,1024,IPC_CREAT | 0600); 

  semctl_cmd.buf=&semid_ds;
  rc=semctl(rsemid, 0, IPC_STAT, semctl_cmd);
  perror("");
  printf("t[%d] rc[%d] st[%d]\n", tvar, rc, semid_ds.sem_ctime);

/*   semctl(rsemid,0,SETVAL,0);
   semctl(wsemid,0,SETVAL,0);
*/
   p_mem = (message *) shmat(shmid,0,0);

   wbuf.sem_num = 0; 
   wbuf.sem_op  = -1; 
   wbuf.sem_flg = 0; 

   rbuf.sem_num = 0; 
   rbuf.sem_op  = 1; 
   rbuf.sem_flg = 0; 
   
   printf("Server is waiting for Message...\n");
   semop(rsemid,&wbuf,1);
  rc=semctl(rsemid, 0, IPC_STAT, semctl_cmd);
  printf("t[%d] rc[%d] st[%d]\n", tvar, rc, semid_ds.sem_ctime);
   printf("Message Received by Server from Client  : [%s]\n",p_mem->msgbuf);
   for(i=0;i<26;i++)
      p_mem->msgbuf[i] +=  32 ; 

   semop(wsemid,&rbuf, 1);
   printf("Result Sent by Server to Client         : [%s]\n",p_mem->msgbuf);
   

}

