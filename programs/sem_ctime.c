/*
* Copyright (C) 1999,2001,2010 by Manfred Spraul.
* 
* Redistribution of this file is permitted under the terms of the GNU 
* General Public License (GPL)
* $Header: /home/manfred/cvs-tree/manfred/ipcsem/undotest.c,v 1.2 2003/06/28 15:19:43 manfred Exp $
*/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

union semun {
int val;
struct semid_ds *buf;
unsigned short int *array;
struct seminfo* __buf;
};
int getval(char *str, int id)
{
union semun arg;
int res;
res = semctl(id,0,GETVAL,arg);
if(res==-1) {
printf("GETVAL failed for %s.\n", str);
exit(4);
}
printf(" %s: GETVAL now %d.\n",str, res);
return res;
}
void setval(int id, int val)
{
union semun arg;
int res;
arg.val = val;
res = semctl(id,0,SETVAL,arg);
if(res==-1) {
printf("SETVAL failed, errno %d.\n", errno);
exit(4);
}
printf(" SETVAL succeeded.\n");
}
long int print_ctime(char *str, int id)
{
union semun arg;
struct semid_ds info;
time_t cur = time(NULL);
int res;
arg.buf = &info;
res = semctl(id,0,IPC_STAT,arg);

if(res==-1) {
printf("semctl(IPC_STAT) failed (%d).\n",errno);
exit(5);
}
printf("%s: ctime now %ld (%ld).\n", str, 
(long int) info.sem_ctime, (long int) cur - info.sem_ctime);
return info.sem_ctime;
}
#define CHILD_COUNT 5
#define EXIT_SUCCESS_MAGIC 0x42

/* test1: check cpu consumption for complex sleeps */
void test1()
{
int childs[CHILD_COUNT];
unsigned long long total_sys, total_user;
int res;
int i;
int id;
printf(" ****************************************\n");
printf(" test1: do a non-trivial sleep\n");
/* create array */
res = semget(IPC_PRIVATE, 1, 0700 | IPC_CREAT);
printf(" got semaphore array %xh.\n",res);
if(res == -1) {
printf(" create failed.\n");
return;
}
id = res;
setval(id, 3);

/* create sub-process */
for (i=0;i<CHILD_COUNT;i++) {
res = fork();
if (res < 0) {
printf("Fork failed (errno=%d). Aborting.\n", errno);
res = semctl(id,1,IPC_RMID,NULL);
exit(1);
}
fflush(stdout);
if (!res) {
struct sembuf sop[15];
/* child: */
/* Do an "increase by 15, decrease by 30" in step size 3*/
for (i=0;i<sizeof(sop)/sizeof(sop[0]);i++) {
sop[i].sem_num=0;
if (i<sizeof(sop)/sizeof(sop[0])/3)
sop[i].sem_op=3;
else
sop[i].sem_op=-3;
sop[i].sem_flg=0;
}
errno = 0;

res = semop(id,sop,sizeof(sop)/sizeof(sop[0]));
if(res!=-1 || errno !=EIDRM) {
printf("child: semop returned %d, errno %d, expected %d/%d.\n", res, errno, -1, EIDRM);
exit(1);
}
fflush(stdout);
exit(EXIT_SUCCESS_MAGIC);
}
childs[i] = res;
}
sleep(5);
errno = 0;
res = semctl(id,1,IPC_RMID,NULL);
printf(" IPC_RMID returned %d, errno now %d (expected: 0, 0).\n", res, errno);

total_user = 0;
total_sys = 0;
for (i=0;i<CHILD_COUNT;i++) {
int retval;
struct rusage r;
retval = wait4(childs[i], &res, 0, &r);

if (retval != childs[i]) {
printf("wait4 returned unexpeted value %d (expected %d), errno now %d.\n", 
retval, childs[i], errno);
}
if (WIFEXITED(res) == 0 || WEXITSTATUS(res) != EXIT_SUCCESS_MAGIC) {
printf("unexpected child exit code %d (raw 0x%x. Expected: %d).\n",
WEXITSTATUS(res), res, EXIT_SUCCESS_MAGIC);
}
printf(" Child %d(%d): sys %ld.%06ld, user %ld.%06ld.\n",
i, childs[i],
(long)r.ru_utime.tv_sec, (long)r.ru_utime.tv_usec, 
(long)r.ru_stime.tv_sec, (long)r.ru_stime.tv_usec);
total_user += r.ru_utime.tv_sec*1000000;
total_user += r.ru_utime.tv_usec;
total_sys += r.ru_stime.tv_sec*1000000;
total_sys += r.ru_stime.tv_usec;
fflush(stdout);
}
printf(" Total time: sys %ld.%06ld, user %ld.%06ld.\n",
(long)total_sys/1000000, (long)total_sys,
(long)total_user/1000000, (long)total_user);
if (total_user < 100000 && total_sys < 100000) {
printf("+++ test1 success: No Livelock\n");
fflush(stdout);
} else {
printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
printf("test 1 bad: OS should hide in shame, Livelock detected.\n");
fflush(stdout);
}
}
/* test2: test sem_ctime updates */
void test2(int id)
{
unsigned long ctime_before, ctime_after;
printf(" ****************************************\n");
printf(" test2: does SETVAL update sem_ctime?\n");
sleep(3);
ctime_before = print_ctime(" before SETVAL", id);
setval(id, 2);
ctime_after = print_ctime(" before SETVAL", id);

if (ctime_after != ctime_before)
printf(" INFO: OS updates sem_ctime in semctl(SETVAL)\n");
else
printf(" INFO: OS performs no update to sem_ctime in semctl(SETVAL)\n");
}
/* test3: test wait for non-zero */
void test3(int id)
{
int res;
printf(" ****************************************\n");
printf(" test3: check that kernel wakes up child that waits for non-zero value.\n");
setval(id, 2);

/* create sub-process */
res = fork();
if (res < 0) {
printf("Fork failed (errno=%d). Aborting.\n", errno);
res = semctl(id,1,IPC_RMID,NULL);
exit(1);
}
fflush(stdout);
if (!res) {
struct sembuf sop[3];
/* child: */
/* Do an "increase by 1 if value is 1" operation */
sop[0].sem_num=0;
sop[0].sem_op=-1;
sop[0].sem_flg=0;
sop[1].sem_num=0;
sop[1].sem_op=0;
sop[1].sem_flg=0;

sop[2].sem_num=0;
sop[2].sem_op=2;
sop[2].sem_flg=0;


res = semop(id,sop,3);
if(res==-1) {
printf("child: semop failed.\n");
exit(1);
}
res = getval("child completed, before exit", id);
if (res != 2) {
printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
printf("Bad: got unexpected value.\n");
exit(99);
}
fflush(stdout);
exit(EXIT_SUCCESS_MAGIC);
} else {
struct sembuf sop[1];
int retval;
int childid;
childid = res;
sleep(1);

/* Do an "decrease by 1" operation */
sop[0].sem_num=0;
sop[0].sem_op=-1;
sop[0].sem_flg=0;
res = semop(id,sop,1);
if(res==-1) {
printf("parent: semop failed.\n");
exit(1);
}
sleep(1);

res = getval("parent: after sleep", id);
if (res != 2) {
printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
printf("Test 3 bad: got unexpected value (child not woken up?).\n");
return;
}
retval = waitpid(childid, &res, 1);
if (retval != childid) {
printf("waitpid returned unexpeted value %d (expected %d), errno now %d.\n", 
retval, childid, errno);
}
if (WIFEXITED(res) == 0 || WEXITSTATUS(res) != EXIT_SUCCESS_MAGIC) {
printf("unexpected child exit code %d (raw 0x%x. Expected: %d).\n",
WEXITSTATUS(res), res, EXIT_SUCCESS_MAGIC);
}
res = getval("parent: after waitpid", id);
if (res != 2) {
printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n");
printf("Bad: got unexpected value.\n");
return;
}
}
printf(" +++ test3 success: kernel wakes up child correctly.\n");
}
int main(int argc,char** argv)
{
int res;
int id;
/* test 1 (array created/destroyed in test1()) */
test1();
/* test 2 */
/* create array */
res = semget(IPC_PRIVATE, 1, 0700 | IPC_CREAT);
printf(" got semaphore array %xh.\n",res);
if(res == -1) {
printf(" create failed.\n");
return 1;
}
id = res;
test2(id);
test3(id);
printf(" ****************************************\n");
printf(" Done, cleanup.\n");
errno = 0;
res = semctl(id,1,IPC_RMID,NULL);
printf(" IPC_RMID returned %d, errno now %d (expected: 0, 0).\n", res, errno);

return 0;
}

