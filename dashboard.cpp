#include "dashboard.h"
#include <iostream>
#include <iomanip>

void garis(){
    std::cout << "=========================================\n";
}
void bannerDashboard(){
    garis();
    std::cout << "|        PERSONAL DASHBOARD        |\n";
    garis();
}

struct finansialOverview {
    int asset;
    int income;
    int exspense;
    int balance;
};

struct walletSummary {
    int saldoCash;
    int saldoWallet;
    int saldoBanking;
};

void PersonalDashboard(){
    char pilihan;

    do{
    bannerDashboard();
    std::cout << "*** Makes Your Finances Stable through the Financial Management System ***\n";
    garis();
    std::cout << "A. Financial Overview\n";
    std::cout << "B. Wallet Summary\n";
    std::cout << "C. Budget Overview\n";
    std::cout << "D. Goal Overview\n";
    std::cout << "E. Recent Activity\n";
    std::cout << "F. Progress Bar\n";
    std::cout << " Masukan Pilih Anda (A-F) : ";
    std::cin >> pilihan;
        switch (pilihan){
            case 'A':
                bannerDashboard();
                std::cout << "|    Financial Overview    |\n";
                garis();
                std::cout << "  Total Asset    : "; 
                std::cout << "  Total Income   : "; 
                std::cout << "  Total Expense  : "; 
                std::cout << "  Net Balance    : ";
                break;
            case 'B':
                bannerDashboard();
                std::cout << "|    Wallet Summary    |\n";
                break;
            case 'C':
                bannerDashboard();
                std::cout << "|    Budget Overview    |\n";
                break;
            case 'D':
                bannerDashboard();
                std::cout << "|    Goal Overview    |\n";
                break;
            case 'E':
                bannerDashboard();
                std::cout << "|    Recent Activity    |\n";
                break;
            case 'F':
                bannerDashboard();
                std::cout << "|    Progress Bar    |\n";
                break; 
            default:
                std::cout << "\n[ERROR] Input tidak valid! Masukkan huruf A sampai F!\n";
                std::system("pause");
                std::system("cls");
        }
    } while (pilihan != 'A' && pilihan != 'B' && pilihan != 'C' && pilihan != 'D' && pilihan != 'E' && pilihan != 'F');
}


