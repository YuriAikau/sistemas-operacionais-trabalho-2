#include "Jovem.h"

extern sem_t    semaforo_pode_entrar_no_barco,
                semaforo_qnt_pessoas_barco,
                semaforo_qnt_pessoas_fila,
                semaforo_barco_cheio,
                semaforo_jovem_pode_atravessar,
                semaforo_travessia,
                semaforo_barco_pode_sair;

extern int      qnt_pessoas_barco,
                qnt_pessoas_fila,
                foi_pego;

static void jovem_tenta_pegar_barco(int id);
static void jovem_espera_barco_sair();
static void jovem_espera_barco_atravessar(int id);
static void jovem_tenta_atravessar_matagal(int id, bool eh_escolhido);

// Funcao que inicia a execucao da thread do do jovem
void* thread_jovem(void* arg) {
    atributo_jovem_t ja = *(atributo_jovem_t*) arg;
    int id = ja.id;
    bool eh_escolhido = ja.eh_escolhido;

    jovem_tenta_pegar_barco(id);
    jovem_espera_barco_sair(id);
    jovem_espera_barco_atravessar(id);
    jovem_tenta_atravessar_matagal(id, eh_escolhido);

    return arg;
}

// Funcao responsavel pela entrada do jovem no barco
// Inicialmente, o jovem tenta entrar diretamente no barco, caso ele falhe, ele entra na fila e fica esperando ate ser possivel entrar no barco novamente
// Caso a quantidade de jovens no barco chegue em 3, o ultimo jovem sinaliza o barqueiro com o semaforo_barco_cheio
static void jovem_tenta_pegar_barco(int id) {
    printf("Jovem %02d vai tentar pegar o barco...\n", id);

    if( sem_trywait(&semaforo_pode_entrar_no_barco) == 0 ) {
        sem_wait(&semaforo_qnt_pessoas_barco);
        qnt_pessoas_barco++;

        printf("Jovem %02d entrou no barco e espera que este saia.\n", id); 

        if(qnt_pessoas_barco == 3)
            sem_post(&semaforo_barco_cheio);
        else
            sem_post(&semaforo_pode_entrar_no_barco);

        sem_post(&semaforo_qnt_pessoas_barco);
    }
    else {
        sem_wait(&semaforo_qnt_pessoas_fila);
        printf("Jovem %02d entrou na fila, tem %d jove%s na sua frente.\n", id, qnt_pessoas_fila, qnt_pessoas_fila == 1? "m" : "ns");
        qnt_pessoas_fila++;
        sem_post(&semaforo_qnt_pessoas_fila);

        sem_wait(&semaforo_pode_entrar_no_barco);
        sem_wait(&semaforo_qnt_pessoas_barco);
        qnt_pessoas_barco++;

        sem_wait(&semaforo_qnt_pessoas_fila);
        qnt_pessoas_fila--;
        sem_post(&semaforo_qnt_pessoas_fila);
        printf("Jovem %02d saiu da fila e entrou no barco.\n", id);

        if(qnt_pessoas_barco == 3)
            sem_post(&semaforo_barco_cheio);
        else
            sem_post(&semaforo_pode_entrar_no_barco);

        sem_post(&semaforo_qnt_pessoas_barco);
    }
}

// Jovem espera o semaforo que sinaliza que o barco ira sair
static void jovem_espera_barco_sair(int id) {
    sem_wait(&semaforo_barco_pode_sair);
    printf("Jovem %02d sai com o barco.\n", id);
}

// Jovem espera o semaforo que sinaliza que o barco atravessou
static void jovem_espera_barco_atravessar(int id) {
    sem_wait(&semaforo_jovem_pode_atravessar);
}

// Jovem tenta atravessar o matagal
// Diminiu a quantidade de pessoas no barco
// Caso o jovem seja o escolhido, ele eh pego pelo seguranca
// Caso algum jovem ja tenha sido pego, os outros falham em entrar tambem
// Quando o barco fica vazio, o ultimo jovem a sair sinaliza o barqueiro com o semaforo_travessia
static void jovem_tenta_atravessar_matagal(int id, bool eh_escolhido) {
    sem_wait(&semaforo_qnt_pessoas_barco);
    qnt_pessoas_barco--;
    
    printf("Jovem %02d atravessando matagal...\n", id);
    sleep(TEMPO_TRAVESSIA_MATAGAL);

    if(eh_escolhido) {
        printf("Jovem %02d preso na cerca... Shii! Os seguranças me pegaram!\n", id);
        foi_pego = 1;
    }
    else if(foi_pego)
        printf("Opa! Os seguranças pegaram um ... não vou poder entrar no show... o jeito vai ser o telão mesmo!\n");
    else
        printf("Jovem %02d entrou no show...\n", id);
    
    if(qnt_pessoas_barco == 0)
        sem_post(&semaforo_travessia);
    else
        sem_post(&semaforo_jovem_pode_atravessar);

    sem_post(&semaforo_qnt_pessoas_barco);
}