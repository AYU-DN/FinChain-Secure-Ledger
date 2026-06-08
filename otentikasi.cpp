#include "otentikasi.h"
#include <iostream>
#include <cstring>

User users[50];
int jumlahUser = 0;

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void pauseProgram() {
    std::cout << "Tekan Enter untuk melanjutkan..." << std::endl;
    std::cin.ignore(1000);
    std::cin.get();
}

void signUp()
{
    if (jumlahUser >= 2)
    {
        std::cout << "User penuh! Tidak bisa registrasi lagi.\n";
        return;
    }
    std::cout << "======== SIGN UP ========" << std::endl; 
    std::cout << "Username : ";
    std::cin >> users[jumlahUser].username;
    std::cout << "Password : ";
    std::cin >> users[jumlahUser].password;
    jumlahUser++;
    std::cout << std::endl << "Registrasi berhasil!" << std::endl;
}

bool signIn()
{
    if (jumlahUser == 0)
    {
        std::cout << "\nBelum ada akun. Silakan Sign Up terlebih dahulu!\n";
        return false;
    }
    char username[30];
    char password[30];
    std::cout << "======== SIGN IN ========" << std::endl; 
    std::cout << "Username : ";
    std::cin >> username;
    std::cout << "Password : ";
    std::cin >> password;

    for (int i = 0; i < jumlahUser; i++)
    {
        if (username == users[i].username &&
            password == users[i].password)
        {
            return true;
        }
    }
    return false;
}