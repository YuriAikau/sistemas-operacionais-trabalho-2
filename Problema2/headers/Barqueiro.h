#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include "utils.h"

// Thread responsável por realizar as ações do barqueiro até que não haja mais pessoas esperando
void* thread_barqueiro(void*);

// Função responsável por realizar a espera do barqueiro na margem até que o barco fique cheio (3 jovens estão no barco)
void barqueiro_espera_margem();

// Função responsável por realizar a espera do barqueiro até que os 3 jovens que ele transportava atravessem o matagal (sendo bem sucedido ou não)
void barqueiro_espera_travessia();