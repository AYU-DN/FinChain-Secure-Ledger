#include "otentikasi.h"
#include <iostream>
#include <cstring>
#include <iomanip>
 
User users[50];
int jumlahUser = 0;
int userAktif = -1;
 
void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void cleanBuffer() {
    std::cin.clear();
    std::cin.ignore(1000, '\n');
}
 
void pauseProgram() {
    std::cout << "Tekan Enter untuk melanjutkan..." << std::endl;
    std::cin.get(); 
}

void Banner() {
    const std::string merah = "\033[41m";
    const std::string reset = "\033[0m"; 
    int pola[5][38] = {
        //  F         I           N           C         H          A        I         N
        {1,1,1,0, 1,1,1,0,  1,1,0,0,1,0,  1,1,1,1, 0,1,0,0,1,0, 1,1,1,0, 1,1,1,0, 1,1,0,0,1}, // Baris 1
        {1,0,0,0, 0,1,0,0,  1,1,0,0,1,0,  1,0,0,0, 0,1,0,0,1,0, 1,0,1,0, 0,1,0,0, 1,1,0,0,1}, // Baris 2
        {1,1,1,0, 0,1,0,0,  1,0,1,0,1,0,  1,0,0,0, 0,1,1,1,1,0, 1,1,1,0, 0,1,0,0, 1,0,1,0,1}, // Baris 3
        {1,0,0,0, 0,1,0,0,  1,0,0,1,1,0,  1,0,0,0, 0,1,0,0,1,0, 1,0,1,0, 0,1,0,0, 1,0,0,1,1}, // Baris 4
        {1,0,0,0, 1,1,1,0,  1,0,0,1,1,0,  1,1,1,1, 0,1,0,0,1,0, 1,0,1,0, 1,1,1,0, 1,0,0,1,1}  // Baris 5
    };
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 38; j++) {
            if (pola[i][j] == 1) {
                std::cout << merah << "  " << reset; 
            } else {
                std::cout << "  ";             
            }
        }
        std::cout << std::endl; 
    }
}
 
void signUp()
{
    std::cout << "======== SIGN UP ========" << std::endl; 
    
    if (jumlahUser >= 50) {
        std::cout << "Error: Kapasitas pengguna sistem telah penuh!" << std::endl;
        return;
    }

    std::cout << "Username: ";
    std::cin >> std::setw(30) >> users[jumlahUser].username;
    cleanBuffer();
 
    for (int i = 0; i < jumlahUser; i++)
    {
        if (strcmp(users[jumlahUser].username, users[i].username) == 0)
        {
            std::cout << "Username sudah terdaftar! Gunakan username lain." << std::endl;
            return;
        }
    }
 
    std::cout << "Password: ";
    std::cin >> std::setw(30) >> users[jumlahUser].password;
    cleanBuffer();

    jumlahUser++;
    std::cout << std::endl << "Registrasi berhasil!" << std::endl;
}
 
bool signIn()
{
    char username[30];
    char password[30];
    std::cout << "======== SIGN IN ========" << std::endl; 
    
    std::cout << "Username : ";
    std::cin >> std::setw(30) >> username;
    cleanBuffer();

    std::cout << "Password : ";
    std::cin >> std::setw(30) >> password;
    cleanBuffer();
 
    for(int i = 0; i < jumlahUser; i++)
    {
        if(strcmp(username, users[i].username) == 0 &&
           strcmp(password, users[i].password) == 0)
        {
            userAktif = i; 
            return true;
        }
    }
    return false; 
}
