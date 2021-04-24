.text
# cette directive sert a rendre l'etiquette publique
    .globl handler_IT
# debut du traitant
handler_IT:
# sauvegarde des registres importants
    pushl %eax
    pushl %edx
    pushl %ecx
# appel a la fonction C realisant le traitant
    call handler_en_C
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %eax
# fin du traitant
    iret

# IT 50
.text
    .globl handler_IT_50
handler_IT_50:
    pushl %eax
    pushl %edx
    pushl %ecx
    call gestionIT50
    popl %ecx
    popl %edx
    popl %eax
    iret

# IT 32 : timer
.text
    .globl handler_IT_32
handler_IT_32:
    pushl %eax
    pushl %edx
    pushl %ecx
    call gestionIT32
    popl %ecx
    popl %edx
    popl %eax
    iret
