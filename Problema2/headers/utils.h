#pragma once

#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

#define N_JOVENS 15
#define N_MAX_BARCO 3
#define TEMPO_TRAVESSIA_IDA 3 //segundos
#define TEMPO_TRAVESSIA_VOLTA 2 //segundos
#define TEMPO_TRAVESSIA_MATAGAL 1 //segundos

typedef struct sigaction sigaction_t;