#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
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

FILE *Fi = NULL;
FILE *Fo = NULL;
int nx, ny, nz, fl=1;
double x_begin, x_end, y_begin, y_end, z_begin, z_end;

int main(int argc, char *argv[])
{
  int i, ix, ix1, ix2, jy, jy1, jy2, kz, kz1, kz2;
  double s, p, hx, hy, hz, hxyz, x, y, z;

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

  hx=(x_end-x_begin)/nx; hy=(y_end-y_begin)/ny; hz=(z_end-z_begin)/nz; hxyz=hx*hy*hz;

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

  fprintf(stderr,"mp=%d i1=%d i2=%d j1=%d j2=%d k1=%d k2=%d\n",mp,ix1,ix2,jy1,jy2,kz1,kz2);

  s = 0;

  for (kz=kz1; kz<=kz2; kz++) {
    z = z_begin + (kz*1.0+.5)*hz;
    for (jy=jy1; jy<=jy2; jy++) {
      y = y_begin + (jy*1.0+.5)*hy;
      for (ix=ix1; ix<=ix2; ix++) {
        x = x_begin + (ix*1.0+.5)*hx;
        s = s + hxyz * f(x,y,z);
      }
    }
  }


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
