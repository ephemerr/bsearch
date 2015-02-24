#pragma once

typedef unsigned long long ui64;
typedef unsigned int ui32;

// Секундомер.
typedef struct STStopwatch *TStopwatch;

// Сброс.
void StopwatchResetStartValues(TStopwatch);

// Обязательная инициализация. Выделяется память, необходимо требует последующего вызова StopwatchDelete.
TStopwatch Stopwatch_New(const char* label);

// Освобождение ресурсов.
void StopwatchDelete(TStopwatch);

// Время в секундах.
long int Stopwatch_GetSecsShift(TStopwatch);

// Время в клоках.
long int Stopwatch_GetNanoSecskShift(TStopwatch);

// Приращение значения Time Stamp Counter.
ui64 StopwatchGetTscShift(TStopwatch);

// Сброс значения Time Stamp Counter.
void StopwatchResetTsc(TStopwatch);

// Добавление промежутка времени в "копилку" для подсчета среднего.
void StopwatchAverage_Count(TStopwatch);

// Сброс "копилки" для подсчета среднего.
void StopwatchAverage_ResetCounter(TStopwatch);

// Вычесление среднего времени на сохраненных промежутках.
ui64 StopwatchAverage_GetTsc(TStopwatch);

// Вывод данных секундомера.
void StopwatchPrint(TStopwatch);
