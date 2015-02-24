#include <stdio.h>

#include "stopwatch.h"

#define N 1<<15

static int data[N]; 

///--------------------------------------------------
static void data_init() {
    int i;
    data[0]=-100;
    for(i=1; i<N; i++) {
        data[i] =data[i-1] + 5;
    }
}

///--------------------------------------------------
/// Simple and slow search
int bsearch_simple( int *array, int x, int n ) {
    int i=0;
    int err = 0;

    err = n > 0 ? 0 : -3;
    if (err) return err;
    
    err = array != NULL ? 0 : -2;
    if (err) return err;

    for (i=0; i<n; i++ ) {
        if (array[i] > x ) break;
    }
     
    err = i != n ? 0 : -1;
    if (err) return err;

    return i;
}

///--------------------------------------------------
/// Recursive search
static int bsearch0( int *array, int x, int n ) {
    if (n == 1) {
        if (x < array[0]) return 0;
        else return -1; 
    } 
    
    /// Num of elements in first half
    int half = n / 2;   
    
    int new_n; 
    int *new_array;

    /// Check last elem in first half
    if (array[half-1] > x) {
        new_n = half;
        new_array = array;
        int res = bsearch0(new_array,x,new_n);
        return res;
    } else {
        new_n = n - half;
        new_array = array + half;   
        int res = bsearch0(new_array,x,new_n);
        return res >= 0 ? half + res : res;
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

    return bsearch0(array, x, n);
}
static void bsearch_test(int x) {
    int res = bsearch(data, x, N); 
    int val = res > 0 ? data[res] : 0;
    printf("x: %3d i: %3d val[i]: %3d \n", x, res, val);
}


///--------------------------------------------------
int main () {

    data_init();

    TStopwatch swatch = Stopwatch_New(NULL);   

    bsearch_test(-1);
    bsearch_test(1);
    bsearch_test(10);
    bsearch_test(100);
    bsearch_test(1000);
    bsearch_test(10000);
    bsearch_test(100000);

    StopwatchPrint(swatch);

    return 0;
}
