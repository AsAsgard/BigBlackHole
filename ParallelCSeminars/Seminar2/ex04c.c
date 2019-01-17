#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "mycom.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define MSG_ID   7777
#define MSG_PERM 0600
#define LBUF     10

typedef struct tag_msg_t
{ int n; double buf[LBUF]; } msg_t;

static int num_processes, curr_process;
static int msgid;
msg_t msg;

static int num_threads;
static pthread_t* threads;
static double sums[LBUF];

static double a = 0;
static double b = 1;
static int ni = 1000000000;
static double sum = 0;

double f1(double x);
double f1(double x) { return 4.0/(1.0+x*x); }

void* myjobt(void* curr_thread_void_ptr);
void* myjobt(void* curr_thread_void_ptr)
{
  int n1; double a1, b1, h1;
  int curr_thread = (int)(((long long int) curr_thread_void_ptr)%1024);

  n1 = ni / (num_threads * num_processes);
  h1 = (b - a) / (num_threads * num_processes);
  a1 = a + h1 * (curr_process * num_threads + curr_thread);
  if (curr_thread==num_threads-1
      && curr_process==num_processes-1
      )
      b1 = b;
  else
      b1 = a1 + h1;

  sums[curr_thread] = integrate(f1,a1,b1,n1);

  printf("curr_pr=%d curr_th=%d a1=%le b1=%le n1=%d s1=%le\n",curr_process,curr_thread,a1,b1,n1,sums[curr_thread]);

  return 0;
}

void NetInit(int num_processes, int* curr_process);
void NetInit(int num_processes, int* curr_process)
{
  int i; pid_t spid = 0;

  if (num_processes>1){
    i = 1;
    while (i<num_processes){
      if (spid>0 || i==1){ *curr_process=i; spid = fork();}
      if (spid==0) return;
      i++;
    }
  }

  *curr_process = 0;
  return;
}

int main(int argc, char *argv[])
{
  int i; double t;

  if (argc<3){
    printf("Usage: %s <process number> <threads in single process>\n",argv[0]);
    return 1;
  }

  sscanf(argv[1],"%d",&num_processes); curr_process = 0;
  sscanf(argv[2],"%d",&num_threads);

  t = mytime(0);

  if (num_processes<2 && num_threads<2) {
    sum = integrate(f1,a,b,ni);

  } else {
      if (num_processes>=2) {
          NetInit(num_processes,&curr_process);

          if (curr_process == 0)
            msgid = msgget(MSG_ID, MSG_PERM | IPC_CREAT);
          else
            while((msgid = msgget(MSG_ID, MSG_PERM))<0);
      }

    // threads part
      if (!(threads = (pthread_t*) malloc(num_threads*sizeof(pthread_t))))
        myerr("server: not enough memory",1);

      for (i=0; i<num_threads; i++) {
        if (pthread_create(threads+i,0,myjobt,(void*)((long long int)i)))
          myerr("server: cannot create thread",2);
      }

      for (i=0; i<num_threads; i++)
        if (pthread_join(threads[i],0))
          myerr("server: cannot wait thread",3);
        else
          sum = sum + sums[i];

      free(threads);
  // end threads part

      if (num_processes>=2) {
          if (curr_process>0) {
            msg.n = 1; msg.buf[0] = sum;
            msgsnd(msgid, &msg, sizeof(msg_t), 0);
          }
          else{
            for(i=1;i<num_processes;i++){
              msgrcv(msgid,&msg,sizeof(msg_t),0,0);
              sum = sum + msg.buf[0];
            }
            msgctl(msgid,IPC_RMID,(struct msqid_ds *)0);
          }
      }
  }

  t = mytime(1);

  if (curr_process == 0)
    printf("time=%lf sum=%22.15le\n",t,sum);

  return 0;
}
