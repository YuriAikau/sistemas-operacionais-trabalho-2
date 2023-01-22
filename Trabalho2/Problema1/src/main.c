#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/signal.h>

#include "../headers/utils.h"

#include "../headers/Torcedores.h"

void inicializador();
void finalizador();

// Arrays estaticos para as threads e os id's dos flamenguistas e vascainos
pthread_t flamenguistas[N_MAX_TIME], vascainos[N_MAX_TIME];
int flamenguistas_id[N_MAX_TIME] = { 0 }, vascaino_id[N_MAX_TIME] = { 0 };

// Mutex para o banheiro, e paras as variaveis que controlam a quantidade de vascainos e flamenguistas querendo entrar no banheiro
pthread_mutex_t mutex_banheiro, flamenguista_quer_entrar_mutex, vascaino_quer_entrar_mutex;

// Variaveis de condicao que sinalizam se o banheiro esta vazio, se tem vagas para os flamenguistas ou para os vascarinos
pthread_cond_t banheiro_vazio, tem_vaga_flamenguista, tem_vaga_vascaino;

// Variaveis que indicam a quantidade de torcedores no banheiro
int flamenguistas_no_banheiro = 0;
int vascainos_no_banheiro = 0;

// Variaveis que indicam a quantidade de torcedores querendo entrar no banheiro
int flamenguistas_querendo_entrar = 0;
int vascainos_querendo_entrar = 0;

int main(int argc, char** argv, char** envp) {
    inicializador();

    // Cria as trhreads para os vascainos e flamenguistas
    for (int i = 0; i < N_MAX_TIME; i++) {
        flamenguistas_id[i] = i;
        vascaino_id[i] = i;
        pthread_create(&flamenguistas[i], NULL, thread_flamenguista, (void*) &flamenguistas_id[i]);
        pthread_create(&vascainos[i], NULL, thread_vascaino, (void*) &vascaino_id[i]);
    }

    // Quando o banheiro fica vazio, a thread principal decide quais torcedores entrarao no banheiro
    while(1) {
        // Verificamos primeiro se vascaínos querem entrar já que eles possuem prioridade sobre os flamenguistas
        if(vascainos_querendo_entrar)
            pthread_cond_broadcast(&tem_vaga_vascaino);
        else
            pthread_cond_broadcast(&tem_vaga_flamenguista);

        // Espera o banheiro ficar novamente vazio (o único caso onde os times no banheiro podem variar)
        pthread_mutex_lock(&mutex_banheiro);
        pthread_cond_wait(&banheiro_vazio, &mutex_banheiro);
        pthread_mutex_unlock(&mutex_banheiro);
    }

    // Espera pelas threads dos flamenguistas e vascainos
    // Nunca chega executada pois as threads nunca finalizam
    for(int i = 0; i < N_MAX_TIME; i++) {
        pthread_join(flamenguistas[i], NULL);
        pthread_join(vascainos[i], NULL);
    }        

    finalizador();

    return 0;
}

// Inicializa os mutexes e as variaveis de condicao usadas no programa
void inicializador(){
    pthread_mutex_init(&mutex_banheiro, NULL);
    pthread_mutex_init(&flamenguista_quer_entrar_mutex, NULL);
    pthread_mutex_init(&vascaino_quer_entrar_mutex, NULL);

    pthread_cond_init(&banheiro_vazio, NULL);
    pthread_cond_init(&tem_vaga_flamenguista, NULL);
    pthread_cond_init(&tem_vaga_vascaino, NULL);
}

// Libera a memoria utilizada pelos mutexes e pelas variaveis de condicao
// Essa funcao nunca chega a ser chamada pelo programa pois ele roda em loop infinito
void finalizador(){
    pthread_cond_destroy(&banheiro_vazio);
    pthread_cond_destroy(&tem_vaga_flamenguista);
    pthread_cond_destroy(&tem_vaga_vascaino);

    pthread_mutex_destroy(&mutex_banheiro);
    pthread_mutex_destroy(&flamenguista_quer_entrar_mutex);
    pthread_mutex_destroy(&vascaino_quer_entrar_mutex);
}