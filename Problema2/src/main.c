#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "utils.h"
#include "Barqueiro.h"
#include "Jovem.h"

pthread_t   barqueiro_thread,
            jovem_thread[N_JOVENS];


sem_t       semaforo_pode_entrar_no_barco, //controla o numero de pessoas que podem entrar no barco
            semaforo_qnt_pessoas_barco, //permite modificar o conteudo da variavel que armazena a quantidade de pessoas no barco
            semaforo_qnt_pessoas_fila, //permite modificar o conteudo da variavel que armazena a quantidade de pessoas na fila
            semaforo_barco_cheio,  //sinaliza ao barqueiro que o barco esta cheio
            semaforo_jovem_pode_atravessar, //controla o numero de jovens que podem atravessar o matagal (apenas 1 por vez)
            semaforo_travessia, //controla o barqueiro para que espere todos os jovens atravessaram 
            semaforo_barco_pode_sair;  //controla a saida do barco, para que apenas saia quando estiver cheio

int         qnt_pessoas_barco,
            qnt_pessoas_fila,
            foi_pego;

atributo_jovem_t    atributo_jovem[N_JOVENS];

void inicializar_semaforos(); //inicializa todos os semaforos usados nesse trabalho
void rock_in_rio();
void destruir_semaforos(); //destroi todos os semaforos usados nesse trabalho

int main(int argc, char** argv, char** envp) {
    inicializar_semaforos();
    rock_in_rio();
    destruir_semaforos();

    return 0;
}

void inicializar_semaforos(){
    sem_init(&semaforo_pode_entrar_no_barco, 0, 0);
    sem_init(&semaforo_qnt_pessoas_barco, 0, 1);
    sem_init(&semaforo_qnt_pessoas_fila, 0, 1);
    sem_init(&semaforo_barco_pode_sair, 0, 0);
    sem_init(&semaforo_barco_cheio, 0, 0);
    sem_init(&semaforo_travessia, 0, 0);
    sem_init(&semaforo_jovem_pode_atravessar, 0, 0);
}

void rock_in_rio(){
    int return_status = 0;
    //cria thread do barqueiro
    if( (return_status = pthread_create(&barqueiro_thread, NULL, thread_barqueiro, NULL)) != 0 ) {
        printf("Erro ao criar thread do barqueiro\n");
        exit(return_status);
    }

    //marca jovem que ira ser pego ao atravessar matagal
    srand(time(NULL));
    int sus = rand() % N_JOVENS;
    atributo_jovem[sus].eh_escolhido = true;
    atributo_jovem[sus].id = sus + 1;

    for(int i = 0; i < N_JOVENS; i++) {
        if(i != sus) atributo_jovem[i] = (atributo_jovem_t) { i + 1, false }; //jovens recebem seus identificadores
            //cria thread dos jovens
            if( (return_status = pthread_create(&jovem_thread[i], NULL, thread_jovem, (void*) &atributo_jovem[i])) != 0 ) {
            printf("Erro ao criar a thread do jovem %d\n", i);
            exit(return_status);
        }
    }
    
    return_status = pthread_join(barqueiro_thread, NULL);
    if(return_status != 0) { //espera thread do banqueiro terminar
        printf("Erro ao dar join na thread do barqueiro\n");
        exit(return_status);
    }

    //espera thread dos jovens terminar
    for(int i = 0, *j = NULL; i < N_JOVENS; i++) {
        if( ( return_status = pthread_join(jovem_thread[i], (void**) &j) ) != 0 ) {
            printf("Erro ao dar join na thread do jovem %d\n", *j);
            exit(return_status);
        }
    }
}

void destruir_semaforos() {
    sem_destroy(&semaforo_pode_entrar_no_barco);
    sem_destroy(&semaforo_qnt_pessoas_barco);
    sem_destroy(&semaforo_qnt_pessoas_fila);
    sem_destroy(&semaforo_barco_pode_sair);
    sem_destroy(&semaforo_barco_cheio);
    sem_destroy(&semaforo_travessia);
    sem_destroy(&semaforo_jovem_pode_atravessar);
}