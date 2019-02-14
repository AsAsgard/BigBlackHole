#include <stdio.h>
#include <unistd.h>
#include <math.h>
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
MPI_Status status;
double tick, t1, t2, t3;

double x_begin=0.0, x_end=1.0, y_begin=0.0, y_end=1.0, z_begin = 0.0, z_end = 1.0;
int nx=1000, ny=1000, nz=1000;

int main(int argc, char *argv[])
{
  int ix, ix1, ix2, jy, jy1, jy2, kz, kz1, kz2;
  double s, p, hx, hy, hz, hxyz, x, y, z;

  MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

  fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
  sleep(1);

  if (argc<4) mpierr("Too small arguments",1);

  sscanf(argv[1],"%d",&np1);
  sscanf(argv[2],"%d",&np2);
  sscanf(argv[3],"%d",&np3);
  sscanf(argv[4],"%d",&iflag);

  if (np1*np2*np3!=np)
    mpierr("Bad processor grid",2);

  mp3 = mp / (np1*np2);
  mp2 = (mp / np1) % np2;
  mp1 = mp % np1;

  fprintf(stderr,"mp=%d grid=%dx%dx%d coord=(%d,%d,%d)\n",mp,np1,np2,np3,mp1,mp2,mp3);

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
    if (iflag==0)
      MPI_Reduce(&p, &s, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    else	
      MPI_Allreduce(&p, &s, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    t2 = MPI_Wtime()-t2;
  }

  t3 = t1 + t2;

  fprintf(stderr,"mp=%d t1=%le t2=%le t3=%le int=%le\n",mp,t1,t2,t3,s);

  MPI_Finalize();
  return 0;
}
