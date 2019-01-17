#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "mycom.h"
#include "mynet.h"

#include <pthread.h>

int np, mp, nl, ier, lp;
char pname[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;
double tick, t1, t2, t3;

typedef struct tag_data_t {
  int n, nt, mt, np, mp;
  double a, b, s, *sum;
} data_t;

const int nt = 2;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_t* threads;
data_t* data;

double a = 0;
double b = 1;
int ni = 1000000000;
double sum = 0;

double f1(double x);
double f1(double x) { return 4.0/(1.0+x*x); }

void* myjobt(void* d);
void* myjobt(void* d)
{
  int n1; double a1, b1, h1;
  data_t* dd = (data_t *)d;

  n1 = dd->n / (dd->nt * dd->np);

  h1 = (dd->b - dd->a) / (dd->nt * dd->np);

  a1 = dd->a + h1 * (dd->nt * dd->mp + dd->mt);

  if (dd->mt == dd->nt - 1
      && dd->mp == dd->np
      )
      b1 = dd->b;
  else
      b1 = a1 + h1;

  dd->s = integrate(f1,a1,b1,n1);

  printf("Thr: mp=%d mt=%d a1=%le b1=%le n1=%d s1=%le\n",dd->mp,dd->mt,a1,b1,n1,dd->s);

  pthread_mutex_lock(&mut); // lock

  *dd->sum += dd->s;

  pthread_mutex_unlock(&mut); // unlock

  return 0;
}

void ThreadInit();
void ThreadInit()
{
  int i;

  if (!(threads = (pthread_t*) malloc(nt*sizeof(pthread_t))))
    myerr("Not enough memory",1);

  if (!(data = (data_t*) malloc(nt*sizeof(data_t))))
    myerr("Not enough memory",1);

  for (i=0; i<nt; i++){
    (data+i)->a=a;
    (data+i)->b=b;
    (data+i)->n=ni;
    (data+i)->nt=nt;
    (data+i)->mt=i;
    (data+i)->np=np;
    (data+i)->mp=mp;
    (data+i)->sum = &sum;
    if (pthread_create(threads+i,0,myjobt,(void*)(data+i)))
      myerr("Can not create thread",2);
  }

  return;
}

void ThreadDone();
void ThreadDone()
{
  int i;

  for (i=0; i<nt; i++)
    if (pthread_join(threads[i],0))
      myerr("Can not wait thread",3);

  free(data);

  free(threads);

  return;
}


int main(int argc, char* argv[])
{
  MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);

  if (np<2) {
    t1 = MPI_Wtime();
    sum = integrate(f1,a,b,ni);
    t2 = MPI_Wtime();
    t3 = t2;
  }
  else {
    int i; double p;

    t1 = MPI_Wtime();

    ThreadInit();
    ThreadDone();

    t2 = MPI_Wtime();

    for(i = 1; pow(2,i-1) < np; i++) {
		if (mp%((int)pow(2,i)) == 0) {
			if (mp+pow(2,i-1) < np) {
		    	MPI_Recv(&p, 1, MPI_DOUBLE, mp+pow(2,i-1), MY_TAG, MPI_COMM_WORLD, &status);
		    	sum = sum + p;
			}
		} else {
		    MPI_Send(&sum, 1, MPI_DOUBLE, mp-pow(2,i-1), MY_TAG, MPI_COMM_WORLD);
			break;
		}
	}
    MPI_Barrier(MPI_COMM_WORLD);
    t3 = MPI_Wtime();
  }

  t1 = t2 - t1;
  t2 = t3 - t2;
  t3 = t1 + t2;

  fprintf(stderr,"Prc: mp=%d t1=%lf t2=%lf t3=%lf int=%22.15le\n",mp,t1,t2,t3,sum);

  MPI_Finalize();
  return 0;
}
