#include "stopwatch.h"

//#include "aerr.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*
 * Value of time-stamp counter register, works for AMD64.
 */
/*
inline ui64 GetTimeStampCounter() {
    ui64 out=0;
    asm (
        "rdtsc; \
        sal $32, %%rdx; \
        add %%rax, %%rdx; \
        mov %%rdx, %0;"
        :"=r"(out)
        :
        :"%rdx","%rax"
    );
    return out;
}*/

ui64 StopwatchGetTimeStampCounter(TStopwatch stopwatch) {
    ui64 out=0;
    return out;
}

enum {
    AVERAGE_TEST_CYCLES = 512
};

struct STStopwatch {
    time_t start_secs;
    clock_t start_clocks;
    ui64 start_tsc;
    ui64 counts;
    ui64 cummulative_tsc;  //      
    ui64 dummy_tsc; // average time for an empty start/stop cycle
    char *label;
};

//public:

void StopwatchResetStartValues(TStopwatch stopwatch) {
    struct timespec realtime;
    clock_gettime(CLOCK_REALTIME,&realtime);
    stopwatch->start_secs = realtime.tv_sec;
    stopwatch->start_clocks = realtime.tv_nsec;
    StopwatchResetTsc(stopwatch);
}    

void StopwatchSetDummyTsc(TStopwatch stopwatch) {
    stopwatch->dummy_tsc = 0;
    ui32 i;
    for (i=0; i<AVERAGE_TEST_CYCLES; i++) {
        StopwatchResetTsc(stopwatch);
        StopwatchAverage_Count(stopwatch);
    }
    stopwatch->dummy_tsc = (ui64)StopwatchAverage_GetTsc(stopwatch);
}

TStopwatch Stopwatch_New (const char *label) {
    int err;
    TStopwatch stopwatch = calloc(1,sizeof(struct STStopwatch));
    err = stopwatch != NULL ? 0 : 1;
    if (err) goto FINISH_STOPWATCH_NEW;
    
    if (label != NULL) {
        const int len = strlen(label);
        err = len <= 256 && len > 0 ? 0 : 2;
        if (err) goto FINISH_STOPWATCH_NEW;
        
        stopwatch->label = calloc(1, len+1);
        err = stopwatch->label != NULL ? 0 : 3;
        if (err) goto FINISH_STOPWATCH_NEW;

        strcpy(stopwatch->label, label);        
    }
    
    StopwatchSetDummyTsc(stopwatch);
    StopwatchAverage_ResetCounter(stopwatch);
    StopwatchResetStartValues(stopwatch);

FINISH_STOPWATCH_NEW:
    if (err) {
        printf("%s(): error: %d\n",__func__,err);
        if  (stopwatch->label != NULL) {
            free(stopwatch->label);
        }
        if (stopwatch != NULL) {
            free(stopwatch);
        }
        return NULL;
    } else {        
        return stopwatch;
    }
}

void StopwatchDelete (TStopwatch stopwatch) {
    if (stopwatch != NULL) {
        if (stopwatch->label != NULL) free(stopwatch->label);
        free(stopwatch);
    }
}

long int Stopwatch_GetSecsShift(TStopwatch stopwatch) {
    struct timespec realtime;
    clock_gettime(CLOCK_REALTIME,&realtime);
    clock_t cur_time = realtime.tv_sec;
    //time_t cur_time = time(NULL);
    long int res = difftime(cur_time, stopwatch->start_secs);
//     printf("%s(): current time: %u res: %d\n",__func__,(unsigned int)cur_time,res);
    return res;
}

long int Stopwatch_GetNanoSecskShift(TStopwatch stopwatch) {
    struct timespec realtime;
    clock_gettime(CLOCK_REALTIME,&realtime);
    clock_t got = realtime.tv_nsec;
    long int res = got - stopwatch->start_clocks;
    if (res < 0) {
        res += 1e9;
    }         
//     printf("%s(): current clock: %ld res: \n -- %ld\n",__func__,got,res);
    return res;
}

ui64 StopwatchGetTscShift(TStopwatch stopwatch) {
    return StopwatchGetTimeStampCounter(stopwatch) - stopwatch->start_tsc - stopwatch->dummy_tsc;
}

void StopwatchResetTsc(TStopwatch stopwatch) {
    stopwatch->start_tsc = StopwatchGetTimeStampCounter(stopwatch);
}
void StopwatchAverage_Count(TStopwatch stopwatch) {
    stopwatch->counts++;
    const ui64 tsc_shift = StopwatchGetTscShift(stopwatch);
    stopwatch->cummulative_tsc += tsc_shift;
}
void StopwatchAverage_ResetCounter(TStopwatch stopwatch) {
    stopwatch->counts = 0;
    stopwatch->cummulative_tsc = 0;
}
/* Used with ResetTsc() and Count().*/
ui64 StopwatchAverage_GetTsc(TStopwatch stopwatch) {
    if (stopwatch->counts == 0) {
        StopwatchAverage_ResetCounter(stopwatch);
        return -1;
    } else {
        ui64 res = stopwatch->cummulative_tsc/stopwatch->counts;
        StopwatchAverage_ResetCounter(stopwatch);
        return res;
    }
}

void StopwatchPrint(TStopwatch stopwatch) {
//     const ui64 tsc_value = StopwatchGetTimeStampCounter(stopwatch);
//     const clock_t clocks_value = clock();
//     const time_t time_value = time(NULL);
    struct timespec realtime;
    clock_gettime(CLOCK_REALTIME,&realtime);
    const clock_t clocks_value = realtime.tv_nsec;
    const time_t time_value = realtime.tv_sec;
    const char * label = stopwatch->label != NULL ? stopwatch->label : "stopwatch";
    printf("%s: sec: %ld, nanosecs: %lu\n", label,
           time_value - stopwatch->start_secs, (unsigned long)clocks_value - stopwatch->start_clocks);
}

