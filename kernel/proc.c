#include <malloc.h>
#include <n7OS/proc.h>

// Tableau des processus
// Le processus de pid 100 sera stocké dans proc_table[100]
proc_t proc_table[NB_PROC];

proc_t *current_proc = proc_table;

// Ajouter un nouveau processus
// Renvoie le pid du processus créé, ou -1 s'il n'a pas pu être créé
int addProcess(char* name, fnptr function) {
    pid_t pid;
    // Trouver un emplacement libre dans la table des processus
    for (pid = 0; pid < NB_PROC && proc_table[pid].state != VIDE; pid++);
    
    if (pid == NB_PROC - 1) // Plus de place
        return -1;
    
    // Attribuer un numéro d'identifiant et son nom
    proc_table[pid].pid = pid;
    strcpy(proc_table[pid].name, name);
    
    // Stocker la fonction en sommet de pile
    proc_table[pid].stack[STACK_SIZE-1] = (uint32_t) function;

    // Initialiser les registres
    for (int i = 0; i < 5; i++)
        proc_table[pid].regs[i] = 0; // tout à 0
    proc_table[pid].regs[1] = (uint32_t) &proc_table[pid].stack[STACK_SIZE-1]; // sauf %esp sommet de la pile

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
    printf("Schedule1 : proc actuel = %i %s\n", current_proc->pid, current_proc->name);
    int nextProc;
    // Trouver le prochains processus prêt
    for (nextProc = 0; nextProc < NB_PROC && proc_table[nextProc].state != PRET; nextProc++);

    if (nextProc < NB_PROC && proc_table[nextProc].state == PRET) {
        printf("current_proc = %i\n", current_proc->pid);
        proc_t previous_proc = *current_proc;
        printf("previous_proc = %i\n", previous_proc.pid);
        previous_proc.state = PRET; // on passe le processus actuel de ELU à PRET

        *current_proc = proc_table[nextProc]; // on affecte le nouveau processus élu
        current_proc->state = ELU;

        printf("Schedule2 : proc actuel = %i %s, next proc = %i %s\n", previous_proc.pid, previous_proc.name, current_proc->pid, current_proc->name);
        ctx_sw(previous_proc.regs, current_proc->regs); // TODO: break ici et ça marche pas
    }
}

extern void processus1();
extern void idle();

void initProcess() {
    if (addProcess("idle", (fnptr) idle) != -1) {
      printf("Processus idle ajoute.\n");
    }
    if (addProcess("proc1", (fnptr) processus1) != -1) {
      printf("Processus 1 ajoute.\n");
    }

    current_proc->state = ELU;
    idle();
}
