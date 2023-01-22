#include "../headers/MonitorBanheiro.h"

extern pthread_mutex_t mutex_banheiro, flamenguista_quer_entrar_mutex, vascaino_quer_entrar_mutex;
extern pthread_cond_t banheiro_vazio, tem_vaga_flamenguista, tem_vaga_vascaino;

extern int flamenguistas_no_banheiro;
extern int vascainos_no_banheiro;

extern int flamenguistas_querendo_entrar;
extern int vascainos_querendo_entrar;

// Função responsável por tratar o comportamento de um flamenguista quando ele quiser entrar no banheiro
void flamenguistaQuerEntrar() {
    pthread_mutex_lock(&flamenguista_quer_entrar_mutex);
    flamenguistas_querendo_entrar++;
    pthread_mutex_unlock(&flamenguista_quer_entrar_mutex);

    pthread_mutex_lock(&mutex_banheiro);
      
    do {
        pthread_cond_wait(&tem_vaga_flamenguista, &mutex_banheiro);
    } while(flamenguistas_no_banheiro >= N_MAX_BANHEIRO);

    flamenguistas_no_banheiro++;
    printf("Flamenguistas no banheiro: %d\t Vascainos no banheiro: %d\n", flamenguistas_no_banheiro, vascainos_no_banheiro);

    pthread_mutex_lock(&flamenguista_quer_entrar_mutex);
    flamenguistas_querendo_entrar--;
    pthread_mutex_unlock(&flamenguista_quer_entrar_mutex);

    pthread_mutex_unlock(&mutex_banheiro);
}

// Função responsável por tratar o comportamento de um flamenguista quando ele sair do banheiro
void flamenguistaSai() {
    pthread_mutex_lock(&mutex_banheiro);

    flamenguistas_no_banheiro--;

    if (!flamenguistas_no_banheiro) pthread_cond_broadcast(&banheiro_vazio);
    else pthread_cond_broadcast(&tem_vaga_flamenguista);

    pthread_mutex_unlock(&mutex_banheiro);
}

// Função responsável por tratar o comportamento de um vascaíno quando ele quiser entrar no banheiro
void vascainoQuerEntrar() {
    pthread_mutex_lock(&vascaino_quer_entrar_mutex);
    vascainos_querendo_entrar++;
    pthread_mutex_unlock(&vascaino_quer_entrar_mutex);

    pthread_mutex_lock(&mutex_banheiro);
      
    do {
        pthread_cond_wait(&tem_vaga_vascaino, &mutex_banheiro);
    } while(vascainos_no_banheiro >= N_MAX_BANHEIRO);

    vascainos_no_banheiro++;
    printf("Flamenguistas no banheiro: %d\t Vascainos no banheiro: %d\n", flamenguistas_no_banheiro, vascainos_no_banheiro);

    pthread_mutex_lock(&vascaino_quer_entrar_mutex);
    vascainos_querendo_entrar--;
    pthread_mutex_unlock(&vascaino_quer_entrar_mutex);

    pthread_mutex_unlock(&mutex_banheiro);
}

// Função responsável por tratar o comportamento de um vascaíno quando ele sair do banheiro
void vascainoSai() {
    pthread_mutex_lock(&mutex_banheiro);

    vascainos_no_banheiro--;

    if (!vascainos_no_banheiro) pthread_cond_broadcast(&banheiro_vazio);
    else pthread_cond_broadcast(&tem_vaga_vascaino);

    pthread_mutex_unlock(&mutex_banheiro);
}