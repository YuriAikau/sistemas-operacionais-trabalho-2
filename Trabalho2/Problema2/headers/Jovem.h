#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "utils.h"

// Struct com as informacoes do jovem: seu id e se ele eh escolhido (para ser pego pelos guardas) ou nao
typedef struct atributo_jovem {
    int id;
    bool eh_escolhido;
} atributo_jovem_t;

// Funcao responsavel pela entrada do jovem no barco
// Inicialmente, o jovem tenta entrar diretamente no barco, caso ele falhe, ele entra na fila e fica esperando ate ser possivel entrar no barco novamente
// Caso a quantidade de jovens no barco chegue em 3, o ultimo jovem sinaliza o barqueiro com o semaforo_barco_cheio
void* thread_jovem(void* id_jovem);