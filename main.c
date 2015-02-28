#include <stdio.h>
#include <time.h>

#define N 0x10000
static int data[N]; 

///--------------------------------------------------
/// Recursive search
static int* bsearch0( int *array, int x, int n ) {
    if (n == 1) {
        if (x < *array) return array;
        else return NULL; 
    } 
    
    /// Num of elements in first half
    int half = n / 2;   

    /// Check last elem in first half
    if (array[half-1] > x) {
        return bsearch0(array,x,half);
    } else {
        return bsearch0(array+half,x,n-half);
    }
}

///--------------------------------------------------
/// Recursive search param check
int bsearch( int *array, int x, int n ) {
    int err = 0;

    err = n > 0 ? 0 : -3;
    if (err) return err;
    
    err = array != NULL ? 0 : -2;
    if (err) return err;

    int *res_pt = bsearch0(array, x, n);
    if (res_pt == NULL) {
      return -1;
    }
    else {
      return res_pt - array;
    }
}

///--------------------------------------------------
static void bsearch_test(int x) {
    int res = bsearch(data, x, N); 
    int val = res > 0 ? data[res] : 0;
    printf("x: %3d i: %3d val[i]: %3d \n", x, res, val);
}

///--------------------------------------------------
int main () {
    
    int i;
    data[0]=-100;
    for(i=1; i<N; i++) {
        data[i] =data[i-1] + 5;
    }

    struct timespec start,end;
    clock_gettime(CLOCK_REALTIME,&start);

    bsearch_test(-1);
    bsearch_test(1);
    bsearch_test(10);
    bsearch_test(100);
    bsearch_test(1000);
    bsearch_test(10000);
    bsearch_test(0xfffffff);

    clock_gettime(CLOCK_REALTIME,&end);

    printf("%s: sec: %ld, nanosecs: %lu\n",  __func__,
           end.tv_sec - start.tv_sec, (unsigned long)end.tv_nsec - start.tv_nsec);

    return 0;
}
