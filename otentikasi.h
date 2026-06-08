#ifndef OTENTIFIKASI_H
#define OTENTIFIKASI_H

struct User {
    std::string username;
    std::string password;
};

extern User users[50];
extern int jumlahUser;

void clearScreen();
void pauseProgram();
void signUp();
bool signIn();

#endif