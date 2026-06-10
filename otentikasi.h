#ifndef OTENTIKASI_H
#define OTENTIKASI_H
 
struct User
{
    char username[30];
    char password[30];
};
 
extern User users[50];
extern int jumlahUser;
extern int userAktif; 
 
void clearScreen();
void pauseProgram();
void signUp();
bool signIn(); 
void Banner();
 
#endif