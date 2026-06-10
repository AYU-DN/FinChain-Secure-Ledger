#include "otentikasi.h"
#include <iostream>
#include <cstring>
#include <iomanip>

#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"
#define BG_GREEN    "\033[42m"
#define BG_YELLOW   "\033[43m"
#define BG_RED      "\033[41m"
#define BG_BLUE     "\033[44m"
#define BG_CYAN     "\033[46m"
 
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
    clearScreen();
    std::cout << CYAN << BOLD;
    std::cout << "\n";
    std::cout << "  ███████╗██╗███╗   ██╗ ██████╗██╗  ██╗ █████╗ ██╗███╗   ██╗\n";
    std::cout << "  ██╔════╝██║████╗  ██║██╔════╝██║  ██║██╔══██╗██║████╗  ██║\n";
    std::cout << "  █████╗  ██║██╔██╗ ██║██║     ███████║███████║██║██╔██╗ ██║\n";
    std::cout << "  ██╔══╝  ██║██║╚██╗██║██║     ██╔══██║██╔══██║██║██║╚██╗██║\n";
    std::cout << "  ██║     ██║██║ ╚████║╚██████╗██║  ██║██║  ██║██║██║ ╚████║\n";
    std::cout << "  ╚═╝     ╚═╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\n";
    std::cout << RESET;
    std::cout << YELLOW << BOLD;
    std::cout << "        Secure Ledger — Sistem Manajemen Keuangan Pribadi\n";
    std::cout << "        Powered by Blockchain-inspired Hash Chain\n" << RESET;
    std::cout << CYAN << "  ──────────────────────────────────────────────────────────\n" << RESET;
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
    if (strlen(users[jumlahUser].password) < 4) {
        std::cout << "Password minimal 4 karakter!" << std::endl;
        return;
    }

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