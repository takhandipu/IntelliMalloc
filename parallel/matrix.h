#ifndef MATRIX_H
#define MATRIX_H
#define SIZE 6
#define CORE_COUNT 4
#if SIZE < CORE_COUNT
#define THREAD_SIZE SIZE
#else
#define THREAD_SIZE CORE_COUNT
#endif
#define PIN_MODE 0
#endif
