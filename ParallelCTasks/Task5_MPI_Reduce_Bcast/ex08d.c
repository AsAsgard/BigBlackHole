#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "mycom.h"
#include "mynet.h"

double f1(double x);
double f1(double x) {
  double e=exp(x);
  return 0.5*(e+1.0/e);
}

double f2(double x);
double f2(double x) {
  double e=exp(x);
  return 0.5*(e-1.0/e);
}

double f3(double x);
double f3(double x) {
  double e=exp(x);
  return 0.5*(e+2.0/e);
}

double f(double x, double y, double z);
double f(double x, double y, double z) {
  return f1(x)*f2(y)*f3(z);
}

int np, mp, nl, ier, lp;
int np1, np2, np3, mp1, mp2, mp3, iflag;
char pname[MPI_MAX_PROCESSOR_NAME];
char sname[13] = "ex08d.out_p00";
MPI_Status Sta[100];
MPI_Request Req[100];
union_t buf;
double tick, t1, t2, t3;

struct data_t_impl {
    double hx, hy, hz, s, *sum;
    int ix1, ix2, jy1, jy2, kz1, kz2;
    int mp, mt;
};

typedef struct data_t_impl data_t;


const int nt = 8;
pthread_t * threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
data_t * data;
double s, hx, hy, hz;
int ix1, ix2, jy1, jy2, kz1, kz2;

FILE *Fi = NULL;
FILE *Fo = NULL;
int nx, ny, nz, fl=1;
double x_begin, x_end, y_begin, y_end, z_begin, z_end;


void * Calculation(void * args) {

    int kz =0, jy = 0, ix = 0;
    double x = 0.0, y = 0.0, z =0.0, hxyz = 0.0;

    data_t * _data = (data_t *) args;
    fprintf(stderr,"Thr: mp=%d mt=%d i1=%d i2=%d j1=%d j2=%d k1=%d k2=%d\n"
            ,_data->mp,_data->mt
            ,_data->ix1,_data->ix2
            ,_data->jy1,_data->jy2
            ,_data->kz1,_data->kz2);

    _data->s = 0;
    hxyz = _data->hx * _data->hy * _data->hz;

    for (kz=_data->kz1; kz<=_data->kz2; kz++) {
      z = z_begin + (kz*1.0+.5)*_data->hz;
      for (jy=_data->jy1; jy<=_data->jy2; jy++) {
        y = y_begin + (jy*1.0+.5)*_data->hy;
        for (ix=_data->ix1; ix<=_data->ix2; ix++) {
          x = x_begin + (ix*1.0+.5)*_data->hx;
          _data->s = _data->s + hxyz * f(x,y,z);
        }
      }
    }

    pthread_mutex_lock(&mutex);

    *_data->sum += _data->s;

    pthread_mutex_unlock(&mutex);

    return 0;
}

void CreateThreads() {
    int ix1_l, ix2_l, jy1_l, jy2_l, kz1_l, kz2_l;

    if (!(threads = (pthread_t*) malloc(nt*sizeof (pthread_t))))
        myerr("Bad day for threads!",1);

    if (!(data = (data_t *) malloc(nt*sizeof (data_t))))
        myerr("Bad day for data of threads!",2);

    int u = 0;
    for (int k = 0; k < 2; ++k) {

        if (k == 1) { kz1_l = (kz1+kz2) / 2 + 1; kz2_l = kz2;}
        else { kz1_l = kz1; kz2_l = (kz1+kz2) / 2;}

        for (int j = 0; j < 2; ++j) {

            if (j == 1) { jy1_l = (jy1+jy2) / 2 + 1; jy2_l = jy2;}
            else { jy1_l = jy1; jy2_l = (jy1+jy2) / 2;}

            for (int i = 0; i < 2; ++i) {

                if (i == 1) { ix1_l = (ix1+ix2) / 2 + 1; ix2_l = ix2;}
                else { ix1_l = ix1; ix2_l = (ix1+ix2) / 2;}

                (data+u)->mp = mp;
                (data+u)->mt = u;
                (data+u)->sum = &s;
                (data+u)->hx = hx;
                (data+u)->hy = hy;
                (data+u)->hz = hz;
                (data+u)->ix1 = ix1_l;
                (data+u)->ix2 = ix2_l;
                (data+u)->jy1 = jy1_l;
                (data+u)->jy2 = jy2_l;
                (data+u)->kz1 = kz1_l;
                (data+u)->kz2 = kz2_l;
                if (pthread_create(threads+u, 0, Calculation, (void *) (data+u)))
                    myerr("Bad day for creating threads!",3);
                ++u;
           }
        }
    }

}


void JoinThreads() {
    for (int i = 0; i < nt; ++i)
        if(pthread_join(*(threads+i),0))
            myerr("Bad day for join threads",4);

    free(data);

    free(threads);
}

int main(int argc, char *argv[])
{
  int i, ix, jy, kz;
  double p, x, y, z;

  MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);


  sprintf(sname+11,"%02d",mp);
  ier = fopen_m(&Fo,sname,"wt");
  if (ier!=0) mpierr("Protocol file not opened",1);

  if (mp==0) {
    ier = fopen_m(&Fi,"ex08d.in","rt");
    if (ier!=0) mpierr("Data file not opened",2);
    i = fscanf(Fi,"xa=%le\n",&x_begin);
    i = fscanf(Fi,"xb=%le\n",&x_end);
    i = fscanf(Fi,"ya=%le\n",&y_begin);
    i = fscanf(Fi,"yb=%le\n",&y_end);
    i = fscanf(Fi,"za=%le\n",&z_begin);
    i = fscanf(Fi,"zb=%le\n",&z_end);
    i = fscanf(Fi,"nx=%d\n",&nx);
    i = fscanf(Fi,"ny=%d\n",&ny);
    i = fscanf(Fi,"nz=%d\n",&nz);
    i = fscanf(Fi,"fl=%d\n",&fl);
    fclose_m(&Fi);
    fprintf(stderr,"read is OK\n");
  }

  MPI_Bcast(&fl,1,MPI_INT,0,MPI_COMM_WORLD);

  if (np>1) {
    if (fl<1){ // variant 1:
      if (mp==0) {
        buf.ddata[0] = x_begin; buf.ddata[1] = x_end;
        buf.ddata[2] = y_begin; buf.ddata[3] = y_end;
        buf.ddata[4] = z_begin; buf.ddata[5] = z_end;
        buf.idata[8] = nx; buf.idata[9] = ny; buf.idata[10] = nz;
        for (i=1; i<np; i++) {
          MPI_Isend(buf.ddata,7,MPI_DOUBLE,i,MY_TAG,MPI_COMM_WORLD,Req+i);
        }
        MPI_Waitall(np-1,Req+1,Sta+1);
      }
      else {
        MPI_Recv(buf.ddata,7,MPI_DOUBLE,0,MY_TAG,MPI_COMM_WORLD,Sta+0);
        x_begin = buf.ddata[0]; x_end = buf.ddata[1];
        y_begin = buf.ddata[2]; y_end = buf.ddata[3];
        z_begin = buf.ddata[4]; z_end = buf.ddata[5];
        nx = buf.idata[8]; ny = buf.idata[9]; nz = buf.idata[10];
      }
    }
    else { // variant 2:
      if (mp==0) {
          buf.ddata[0] = x_begin; buf.ddata[1] = x_end;
          buf.ddata[2] = y_begin; buf.ddata[3] = y_end;
          buf.ddata[4] = z_begin; buf.ddata[5] = z_end;
          buf.idata[8] = nx; buf.idata[9] = ny; buf.idata[10] = nz;
      }
      MPI_Bcast(buf.ddata,7,MPI_DOUBLE,0,MPI_COMM_WORLD);
      if (mp>0) {
        x_begin = buf.ddata[0]; x_end = buf.ddata[1];
        y_begin = buf.ddata[2]; y_end = buf.ddata[3];
        z_begin = buf.ddata[4]; z_end = buf.ddata[5];
        nx = buf.idata[8]; ny = buf.idata[9]; nz = buf.idata[10];
      }
    }
  }

  MPI_Barrier(MPI_COMM_WORLD);

// Next code:

#define S(arg1,arg2,arg3)  pow((double)np * pow((double)n##arg1,2) / ((double)n##arg2 * (double)n##arg3) ,1.0/3.0)

  if (np==1) { np1 = 1; np2 = 1; np3 = 1;}
  else {
    s = S(x,y,z);
    np1 = floor(s); if (s>0.5+((double)np1)) np1++;
    s = S(y,x,z);
    np2 = floor(s); if (s>0.5+((double)np2)) np2++;
    np3 = np / (np1*np2);
    if (np1*np2*np3!=np) {
      if (nx>ny && nx>nz) {
          np1 = np;
          np2 = 1;
          np3 = 1;
      } else if (ny>nx && ny>nz) {
          np1 = 1;
          np2 = np;
          np3 = 1;
      } else {
          np1 = 1;
          np2 = 1;
          np3 = np;
      }
    }
  }

  mp3 = mp / (np1*np2);
  mp2 = (mp / np1) % np2;
  mp1 = mp % np1;

  if (mp==0) fprintf(stderr,"Grid=%dx%dx%d\n",np1,np2,np3);

  fprintf(Fo,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  fprintf(Fo,"Grid=%dx%dx%d coord=(%d,%d,%d)\n",np1,np2,np3,mp1,mp2,mp3);
  fprintf(Fo,
          "xa=%le xb=%le ya=%le yb=%le za=%le zb=%le nx=%d ny=%d nz=%d fl=%d\n"
          ,x_begin,x_end
          ,y_begin,y_end
          ,z_begin,z_end
          ,nx,ny,nz
          ,fl);

  t1 = MPI_Wtime();

  hx=(x_end-x_begin)/nx; hy=(y_end-y_begin)/ny; hz=(z_end-z_begin)/nz;


  if (np1==1) {
    ix1 = 0; ix2 = nx-1;
  }
  else {
    ix1 = mp1 * (nx/np1); if (mp1<np1-1) ix2 = ix1+(nx/np1)-1; else ix2 = nx-1;
  }

  if (np2==1) {
    jy1 = 0; jy2 = ny-1;
  }
  else {
    jy1 = mp2 * (ny/np2); if (mp2<np2-1) jy2 = jy1+(ny/np2)-1; else jy2 = ny-1;
  }

  if (np3==1) {
    kz1 = 0; kz2 = nz-1;
  }
  else {
    kz1 = mp3 * (nz/np3); if (mp3<np3-1) kz2 = kz1+(nz/np3)-1; else kz2 = nz-1;
  }

  fprintf(stderr,"Prc: mp=%d i1=%d i2=%d j1=%d j2=%d k1=%d k2=%d\n"
          ,mp
          ,ix1,ix2
          ,jy1,jy2
          ,kz1,kz2);

  s = 0;

  CreateThreads();
  JoinThreads();


  t2 = MPI_Wtime();

  t1 = t2 - t1;

  if (np==1)
    t2 = 0;
  else {
    p = s;
    MPI_Reduce(&p, &s, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    t2 = MPI_Wtime()-t2;
  }

  t3 = t1 + t2;

  if (mp==0) fprintf(stderr,"t1=%le t2=%le t3=%le int=%le\n",t1,t2,t3,s);

  fprintf(Fo,"i1=%d i2=%d j1=%d j2=%d k1=%d k2=%d\n",ix1,ix2,jy1,jy2,kz1,kz2);
  fprintf(Fo,"t1=%le t2=%le t3=%le int=%le\n",t1,t2,t3,s);

  ier = fclose_m(&Fo);

  MPI_Finalize();
  return 0;
}
