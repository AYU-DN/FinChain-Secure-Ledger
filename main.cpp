#include <iostream>
#include "otentikasi.cpp"
#include "otentikasi.h"

int main()
{
    int pilihan;
    bool sudahSignUp = false;
    int gagalLogin = 0;
    do
    {
        clearScreen();
        std::cout << "\n=========================\n";
        std::cout << " FINCHAIN SECURE LEDGER\n";
        std::cout << "=========================\n";
        std::cout << "1. Sign Up\n";
        std::cout << "2. Sign In\n";
        std::cout << "3. Exit\n";
        std::cout << "\nPilih : ";
        std::cin >> pilihan;
        switch(pilihan)
        {
            case 1:
                signUp();
                sudahSignUp = true;
                gagalLogin = 0;
                break;
            case 2:
                if(!sudahSignUp)
                {
                    std::cout << "\nAnda belum Sign Up! Silakan daftar dulu.\n";
                    pauseProgram();
                    break;
                }

                if(signIn())
                {
                    std::cout << "\nLogin berhasil!\n";
                    std::cout << "Selamat Datang!\n";
                    pauseProgram();
                }
                else
                {
                    gagalLogin++;
                    std::cout << "\nUsername atau Password salah!\n";
                    std::cout << "Percobaan: " << gagalLogin << "/3\n";

                    if(gagalLogin >= 3)
                    {
                        std::cout << "\nAnda gagal login 3 kali.\n";
                        std::cout << "AKSES DIBLOKIR. PROGRAM KELUAR.\n";
                        return 0; 
                    }
                    pauseProgram();
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