#include <malloc.h>
#include <n7OS/proc.h>

// Tableau des processus
// Le processus de pid 100 sera stocké dans proc_table[100]
proc_t proc_table[NB_PROC];

proc_t* proc_new = &proc_table[0];

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
    proc_table[pid].regs[1] = (uint32_t) &proc_table[pid].stack[STACK_SIZE-1]; // %esp sommet de la pile

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
    int nextPID;
    // Trouver le prochains processus prêt
    for (nextPID = 0; nextPID < NB_PROC && proc_table[nextPID].state != PRET; nextPID++);
    if (nextPID == NB_PROC) {
        printf("Pas de processus pret.\n");
        return; // Pas de processus prêt trouvé, on garde le processus actuel
    }
    
    // On passe le processus actuel à prêt
    proc_new->state = PRET;

    // On sauvegarde le processus actuel pour accéder à ses registres après
    proc_t* proc_old = malloc(sizeof(proc_t));
    *proc_old = *proc_new;

    // On assigne le nouveau processus
    proc_new = &proc_table[nextPID];
    proc_new->state = ELU;

    for (size_t i = 0; i < 500000000; i++); // juste pour ralentir l'affichage, à enlever

    // Changement de contexte
    printf("\nSchedule: %s (esp=%x) -> %s (esp=%x)\n", proc_old->name, proc_old->regs[1], proc_new->name, proc_new->regs[1]);
    ctx_sw(proc_old->regs, proc_new->regs);
}

extern void processus1();
extern void idle();

// Initialiser les processus
void initProcess() {
    // Ajouter les processus
    addProcess("idle", (fnptr) idle);
    addProcess("proc1", (fnptr) processus1);

    // Lancer le premier processus
    proc_new->state = ELU;
    idle();
}
