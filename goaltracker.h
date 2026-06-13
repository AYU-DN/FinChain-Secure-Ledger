#ifndef GOALTRACKER_H
#define GOALTRACKER_H

#include <string>

struct Goal {
    std::string nama;
    double target;
    double progress;
};

extern Goal daftarGoal[20];
extern int jumlahGoal;

void tambahGoal();
void updateProgressGoal();
void lihatGoal();
void hapusGoal();

void menuGoalTracker();

#endif