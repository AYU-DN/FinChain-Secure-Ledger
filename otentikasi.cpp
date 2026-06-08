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
