#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mynet.h"
#include "myrand.h"

void merge(int * arr1, size_t N1, int * arr2, size_t N2, int * res_arr)
{
    int * arr1_ptr = arr1;
    int * arr2_ptr = arr2;
    int * res_arr_ptr = res_arr;
    while (arr1_ptr != arr1+N1) {
        if (arr2_ptr == arr2+N2) {
            *res_arr_ptr = *arr1_ptr;
            ++arr1_ptr;
            ++res_arr_ptr;
            continue;
        }
        if (*arr1_ptr < *arr2_ptr) {
            *res_arr_ptr = *arr1_ptr;
            ++arr1_ptr;
            ++res_arr_ptr;
        } else {
            *res_arr_ptr = *arr2_ptr;
            ++arr2_ptr;
            ++res_arr_ptr;
        }
    }

    while (arr2_ptr != arr2+N2) {
        *res_arr_ptr = *arr2_ptr;
        ++arr2_ptr;
        ++res_arr_ptr;
    }
}

void copy(int * source, int * dest, size_t N)
{
    for (int i = 0; i < N; ++i) dest[i] = source[i];
}

void MergeSort(int * arr, size_t N) {
    if (N == 1) return;
    int * tmp_arr = (int *) malloc(N*sizeof (int));
    for (int i = 0; i < N; ++i) tmp_arr[i] = arr[i];
    int * border = arr + N / 2;
    MergeSort(tmp_arr, N / 2);
    MergeSort(border, N - N/2);
    merge(tmp_arr, N/2, border, N - N/2, arr);
    free(tmp_arr);
}

int np, mp, nl;
double tick;
char pname[MPI_MAX_PROCESSOR_NAME];
MPI_Status status;

int *arr;
int arr_size, *curr_arr, *tmp_arr;

int BIG_VALUE = 1000;

void myobj(int mp_l)
{
    double h = BIG_VALUE / np;
    int el_count = floor(h);
    if (mp_l < np-2) {
        arr_size = el_count;
        curr_arr = arr + mp_l * el_count;
    } else if (mp_l == np-2) {
        arr_size = floor((BIG_VALUE - el_count * (np - 2)) * 1.0 / 2.0);
        curr_arr = arr + mp_l * el_count;
    } else {
        int delta = floor((BIG_VALUE - el_count * (np - 2)) * 1.0 / 2.0);
        arr_size = BIG_VALUE - el_count * (np - 2) - delta;
        curr_arr = arr + (mp_l - 1) * el_count + delta;
    }
}

int main(int argc, char **argv)
{
    int recv_size;

    MyNetInit(&argc,&argv,&np,&mp,&nl,pname,&tick);

    fprintf(stderr,"Netsize: %d, process: %d, system: %s, tick=%12le\n",np,mp,pname,tick);
    sleep(1);

    arr = (int *) malloc(BIG_VALUE * sizeof (int));
    tmp_arr = (int *) malloc(BIG_VALUE * sizeof (int));

    if (mp == 0) {
        for (int i = 0; i < BIG_VALUE; ++i)
            arr[i] = myrand();
    }

    if (np==1) {
        MergeSort(arr, BIG_VALUE);
    } else {
        MPI_Bcast(arr, BIG_VALUE, MPI_INT, 0, MPI_COMM_WORLD);

        myobj(mp);

        MergeSort(curr_arr, arr_size);

        for(int i = 1; pow(2,i-1) < np; i++) {
            if (mp%((int)pow(2,i)) == 0) {
                if (mp+pow(2,i-1) < np) {
                    copy(curr_arr, tmp_arr, arr_size);
                    MPI_Recv(&recv_size, 1, MPI_INT, mp+pow(2,i-1), MY_TAG, MPI_COMM_WORLD, &status);
                    MPI_Recv(tmp_arr + arr_size, recv_size, MPI_INT, mp+pow(2,i-1), MY_TAG, MPI_COMM_WORLD, &status);
                    merge(tmp_arr, arr_size, tmp_arr + arr_size, recv_size, arr);
                    curr_arr = arr;
                    arr_size += recv_size;
                }
            } else {
                MPI_Send(&arr_size, 1, MPI_INT, mp-pow(2,i-1), MY_TAG, MPI_COMM_WORLD);
                MPI_Send(curr_arr, arr_size, MPI_INT, mp-pow(2,i-1), MY_TAG, MPI_COMM_WORLD);
                break;
            }
        }
        free(tmp_arr);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    if (mp == 0)
        for (int i = 0; i < BIG_VALUE; ++i)
            printf("%d ", arr[i]);

    free(arr);

    MPI_Finalize();

    return 0;
}
