#include "../headers/Torcedores.h"

// cada flamenguista terá um identificador de 1 a 10
void* thread_flamenguista(void* id) {
    while(true) {
        printf("Eu sou flamenguista - %d: ... Estou apertado(a)! Vou no banheiro!\n", *(int*)id);
        flamenguistaQuerEntrar();
        printf ("Eu sou flamenguista - %d: ... UFA! Entrei no banheiro!\n", *(int*)id);
        sleep(5);
        flamenguistaSai();
        printf ("Eu sou flamenguista - %d: ... Estou aliviado! Vou torcer!\n", *(int*)id);
        sleep(5);
    }

    return NULL;
}

// cada vascaino terá um identificador de 1 a 10
void* thread_vascaino(void* id) {
    while(true) {
        printf("Eu sou vascaino - %d: ... Estou apertado(a)! Vou no banheiro!\n", *(int*)id);
        vascainoQuerEntrar();
        printf ("Eu sou vascaino - %d: ... UFA! Entrei no banheiro!\n", *(int*)id);
        sleep(1);
        vascainoSai();
        printf ("Eu sou vascaino - %d: ... Estou aliviado! Vou torcer!\n", *(int*)id);
        sleep(30);
    }

    return NULL;
}