#ifndef __TIME_H__
#define __TIME_H__

struct time {          
    int msec; // Millisecondes  [0-999] 
    int sec;  // Secondes       [0-59]
    int min;  // Minutes        [0-59]
    int hour; // Heures         [0-23]
};

typedef struct time Time;

// Démarrer le timer
void init_timer();

// Afficher le timer
void show_timer();

#endif
