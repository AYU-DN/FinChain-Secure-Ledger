#include "budgetmanager.h"
#include "transaction.h"

#include <iostream>
#include <iomanip>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"

void garis(){
    std::cout <<"\n========================================\n";
}

void clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}

void pauseScreen(){
    std::cout <<"\nTekan Enter..."; 
    std::cin.ignore(1000, '\n');
    std::cin.get();
}

void tampilBar(double persen) {
    int bar = persen / 10;
    std::cout <<"[";
    for(int i=0; i<10; i++)  {
        if(i<bar) std::cout <<"█";
        else std::cout <<"░";
    }
    std::cout <<"] ";
}

void lihatStatusBudget(){
    double total = needsSpent + wantsSpent + savingsSpent;
    garis();
    std::cout <<"BUDGET STATUS\n";
    garis();
    if(total==0) {
        std::cout <<"\nBelum ada transaksi.\n";
        pauseScreen();
        return;
    }
    double pNeeds = (needsSpent / total) * 100;
    double pWants = (wantsSpent / total) * 100;
    double pSavings = (savingsSpent / total) * 100;
    std::cout <<"\nNeeds (50%) : ";
    tampilBar(pNeeds);
    std::cout <<std::fixed <<std::setprecision(1) <<pNeeds <<"%\n";
    std::cout <<"\nWants (30%) : ";
    tampilBar(pWants);
    std::cout <<pWants <<"%\n";
    std::cout <<"\nSavings (20%) : ";
    tampilBar(pSavings);
    std::cout << pSavings <<"%\n";
    pauseScreen();
}

void monitoringBudget(){
    double total = needsSpent + wantsSpent + savingsSpent;
    garis();
    std::cout <<"MONITORING\n";
    garis();
    if(total==0){
        std::cout <<"\nBelum ada data.\n";
        pauseScreen();
        return;
    }

    double score =  100;
    if(needsSpent > total*0.5) score-=30;
    if(wantsSpent > total*0.3) score-=40;
    if(savingsSpent < total*0.2) score-=30;
    if(score>=80) std::cout <<GREEN <<"\nKeuangan Sehat\n";
    else if( score>=50) std::cout <<YELLOW <<"\nKeuangan Kurang Sehat\n";
    else std::cout <<RED <<"\nKeuangan Tidak Sehat\n";
    std::cout <<RESET;
    pauseScreen();
}

void laporanBudget(){
    garis();
    std::cout <<"LAPORAN\n";
    garis();
    std::cout <<"\nTotal Income : Rp " <<totalIncome;
    std::cout <<"\nTotal Expense : Rp " <<totalExpense;
    std::cout <<"\nSaldo Bersih : Rp " <<(totalIncome - totalExpense);
    pauseScreen();
}

void menuBudget(){
    int pilih;
    do{
        clearScreen();
        garis();
        std::cout <<"BUDGET MANAGER\n";
        garis();
        std::cout <<"\n1. Lihat Status"
                  <<"\n2. Monitoring"
                  <<"\n3. Laporan"
                  <<"\n0. Kembali"
                  <<"\n\nPilih : ";
        std::cin >> pilih;
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout <<"\nInput harus angka.\n";
            pauseScreen();
            continue;
        }
        switch(pilih){
            case 1:
            lihatStatusBudget();
            break;
            case 2:
            monitoringBudget();
            break;
            case 3:
            laporanBudget();
            break;
            case 0:
            break;
            default:
            std::cout <<"\nPilihan tidak valid.\n";
            pauseScreen();
        }
    } while(pilih != 0 );
}