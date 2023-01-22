#include "Barqueiro.h"

extern sem_t    semaforo_pode_entrar_no_barco,
                semaforo_qnt_pessoas_barco,
                semaforo_barco_cheio,
                semaforo_travessia,
                semaforo_jovem_pode_atravessar,
                semaforo_barco_pode_sair;

extern int      qnt_pessoas_fila,
                foi_pego;

// Thread responsável por realizar as ações do barqueiro até que não haja mais pessoas esperando
void* thread_barqueiro(void* arg) {
    do {
        printf("Barqueiro está esperando os jovens entrarem no barco...\n");
        barqueiro_espera_margem();

        printf("Barqueiro recolhe a ajuda de custo e sai com o barco...\n");
        sleep(TEMPO_TRAVESSIA_IDA); // 3 segundos

        printf("Barqueiro chega na outra margem e libera 3 passageiros...\n");
        printf("Barqueiro espera todos atravessarem o matagal...\n");
        barqueiro_espera_travessia();

        printf("Tudo certo! Barqueiro retorna para fazer outra viagem...\n");
        sleep(TEMPO_TRAVESSIA_VOLTA); // 2 segundos

    } while(qnt_pessoas_fila > 0); // Se não houver mais pessoas na fila, finalizar

    return NULL;
}

// Função responsável por realizar a espera do barqueiro na margem até que o barco fique cheio (3 jovens estão no barco)
void barqueiro_espera_margem(){
    // Barqueiro sinaliza que se pode entrar no barco
    sem_post(&semaforo_pode_entrar_no_barco);

    // Barqueiro espera o barco ficar cheio
    sem_wait(&semaforo_barco_cheio);

    // Barqueiro sinaliza que pode sair
    sem_post(&semaforo_barco_pode_sair);
    sem_post(&semaforo_barco_pode_sair);
    sem_post(&semaforo_barco_pode_sair);
}

// Função responsável por realizar a espera do barqueiro até que os 3 jovens que ele transportava atravessem o matagal (sendo bem sucedido ou não)
void barqueiro_espera_travessia(){
    // Barqueiro sinaliza que os jovens podem começar a atravessar o matagal
    sem_post(&semaforo_jovem_pode_atravessar);

    // Barqueiro espera que todos os jovens atravessem
    sem_wait(&semaforo_travessia);
    // Caso algum dos jovens que atravessava o matagal foi pego, ele finaliza e cria o named pipe com a mensagem
    if(foi_pego){
        printf("Opa! Problemas! Os seguranças pegaram um! Acabou meu esquema...\n");
        sem_post(&semaforo_jovem_pode_atravessar);
        
        mkfifo("easter_egg_da_band", 0777);
        char* mensagem = "O caminho mais curto nem sempre é o melhor ...\n -Jadir Lucas\n";
        int fd = open("easter_egg_da_band", O_WRONLY);
        if(write( fd, mensagem, sizeof(char)*(strlen(mensagem)+1)) == -1) perror("Write deu ruim");
        close(fd);
        pthread_exit(NULL);
    }
}