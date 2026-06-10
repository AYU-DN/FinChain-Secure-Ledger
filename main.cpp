#include <iostream>
#include "otentikasi.cpp"
#include "otentikasi.h"
#include "dashboard.cpp"
#include "dashboard.h"
 
int main()
{
    system("chcp 65001");
    char inputMenu[100];
    int pilihan;
    int gagalLogin = 0;
    do
    {
        clearScreen();
        Banner();
        std::cout << "  ==========================================================\n";
        std::cout << "  |                 FINCHAIN SECURE LEDGER                 |\n";
        std::cout << "  ==========================================================\n";
        std::cout << "  |                                                        |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |               |      1. Sign Up        |               |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |                                                        |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |               |      2. Sign In        |               |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |                                                        |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |               |       3. Exit          |               |\n";
        std::cout << "  |               ==========================               |\n";
        std::cout << "  |                                                        |\n";
        std::cout << "  ==========================================================\n";
        std::cout << "  Masukan Pilih Anda : ";
        std::cin >> inputMenu;
        if (inputMenu[1] != '\0')
        {
            cleanBuffer();
            std::cout << "\nPilihan tidak valid! Masukkan hanya satu angka tanpa simbol tambahan.\n";
            pauseProgram();
            continue;
        }
 
        if (inputMenu[0] != '1' && inputMenu[0] != '2' && inputMenu[0] != '3')
        {
            cleanBuffer();
            std::cout << "\nPilihan tidak valid! Masukkan angka 1-3.\n";
            pauseProgram();
            continue;
        }
        
        cleanBuffer();
        pilihan = inputMenu[0] - '0';
 
        switch(pilihan)
        {
            case 1:
                clearScreen();
                signUp();
                pauseProgram();
                break;
                
            case 2:
                {
                    if (jumlahUser == 0) {
                        std::cout << "\nAnda belum Sign Up! Silakan daftar dulu.\n";
                        pauseProgram();
                        break;
                    }

                    clearScreen();
                    if(signIn()) 
                    {
                        std::cout << "\nLogin berhasil!\n";
                        std::cout << "Selamat Datang!\n";
                        gagalLogin = 0;
                        pauseProgram();
                        PersonalDashboard();
                        pauseProgram();
                    }
                    else
                    {
                        gagalLogin++;
                        std::cout << "\nUsername atau Password salah!\n";
                        std::cout << "Percobaan: " << gagalLogin << "/3\n";
                        pauseProgram();

                        if (gagalLogin >= 3)
                        {
                            clearScreen();
                            std::cout << RED << BOLD;
                            std::cout << "\n  ╔══════════════════════════════════════════════════╗\n";
                            std::cout << "  ║  ✗ AKUN DIBLOKIR! Terlalu banyak percobaan.      ║\n";
                            std::cout << "  ║  Program akan dihentikan.                        ║\n";
                            std::cout << "  ╚══════════════════════════════════════════════════╝\n" << RESET; 
                            std::cout << "\nProgram selesai.\n";
                            pauseProgram();
                            return 0;
                        }
                    }
                }
                break;
                
            case 3:
                std::cout << "\nProgram selesai.\n";
                pauseProgram();
                break;
 
            default:
                std::cout << "\nPilihan tidak valid!\n";
                pauseProgram(); 
        }
    } while(pilihan != 3);
    return 0;
}
