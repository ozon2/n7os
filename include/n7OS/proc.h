#ifndef __PROC_H__
#define __PROC_H__

#include <inttypes.h>

#define NB_PROC 1000    // Nombre de processus du système
#define STACK_SIZE 1024 // Taille de la stack

typedef uint16_t pid_t;

typedef enum STATES {
    VIDE,   // Le processus peut être supprimé
    ELU,    // Processus en cours de fonctionnement
    PRET,   // En attente de l'unité centrale
    BLOQUE, // En attente de ressource
} state_t;

typedef struct {
  uint32_t stack[STACK_SIZE];
  char name[20];
  state_t state;
  pid_t pid;
  uint32_t regs[5];
} proc_t;

// Type pointeur sur une fonction
typedef void* (*fnptr)();

// TODO: définir les appels systèmes suivants :
// pid_t fork(const char *name, fnptr function);
// int exit();
// pid_t getpid();
// int sleep(int seconds);

int addProcess(char* name, fnptr function);

void removeProcess(pid_t pid);

void schedule();

void initProcess();

#endif
