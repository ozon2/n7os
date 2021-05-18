#include <malloc.h>
#include <n7OS/proc.h>

// Tableau des processus
// Le processus de pid 100 sera stocké dans proc_table[100]
proc_t proc_table[NB_PROC];

proc_t *proc_elu = proc_table;

// Ajouter un nouveau processus
// Renvoie le pid du processus créé, ou -1 s'il n'a pas pu être créé
int addProcess(fnptr function) {
    pid_t pid;
    // Trouver un emplacement libre dans la table des processus
    for (pid = 0; pid < NB_PROC && proc_table[pid].state != VIDE; pid++);
    
    if (pid == NB_PROC - 1) // Plus de place
        return -1;
    
    // Attribuer un numéro d'identifiant
    proc_table[pid].pid = pid;
    
    // Stocker la fonction en sommet de pile
    proc_table[pid].stack[0] = function;

    // Le processus est prêt à s'exécuter
    proc_table[pid].state = PRET;

    return pid;
}

// Supprimer un processus
void removeProcess(pid_t pid) {
    proc_table[pid].state = VIDE;
    memset(proc_table[pid].stack, 0, STACK_SIZE);
}

extern void ctx_sw(void *ctx_old, void *ctx_new);

// Ordonnanceur
void schedule() {
    proc_table[0].state = ELU;
}
