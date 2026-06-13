#include "goaltracker.h"
#include <iostream>
#include <iomanip>

Goal daftarGoal[20];
int jumlahGoal = 0;

void pauseGoal() {
    std::cin.ignore();
    std::cin.get();
}

void tambahGoal() {
    if (jumlahGoal >= 20) {
        std::cout << "\nMaksimal goal tercapai\n";
        return;
    }
    Goal& g = daftarGoal[jumlahGoal];
    std::cout << "\nNama Goal : ";
    std::getline(std::cin >> std::ws, g.nama);
    std::cout << "Target (Rp): ";
    std::cin >> g.target;
    g.progress = 0;
    jumlahGoal++;
    std::cout << "\nGoal berhasil dibuat\n";
}

void updateProgressGoal() {
    if (jumlahGoal == 0) {
        std::cout << "\nBelum ada goal\n";
        return;
    }
    lihatGoal();
    int pilih;
    std::cout << "\nPilih Goal : ";
    std::cin >> pilih;
    pilih--;
    if (pilih < 0 || pilih >= jumlahGoal)
        return;
    double tambah;
    std::cout << "Tambah progress : ";
    std::cin >> tambah;
    daftarGoal[pilih].progress += tambah;
    if (daftarGoal[pilih].progress > daftarGoal[pilih].target) {
        daftarGoal[pilih].progress = daftarGoal[pilih].target;
    }
    std::cout << "\nProgress diperbarui\n";
}

void lihatGoal() {
    if (jumlahGoal == 0) {
        std::cout << "\nBelum ada target\n";
        return;
    }
    std::cout << "\n===== GOAL LIST =====\n";
    for (int i=0;i<jumlahGoal;i++) {
        double persen = (daftarGoal[i].progress / daftarGoal[i].target) *100;
        std::cout << "\n[" << i+1 << "] " << daftarGoal[i].nama << "\nTarget   : Rp " << std::fixed << std::setprecision(0) << daftarGoal[i].target 
                  << "\nProgress : Rp " << std::fixed << std::setprecision(0) << daftarGoal[i].progress
                  << "\nStatus   : " << std::fixed << std::setprecision(1) << persen << "%\n";
    }
}

void hapusGoal() {
    lihatGoal();
    int pilih;
    std::cout << "\nHapus nomor: ";
    std::cin >> pilih;
    pilih--;
    if (pilih <0 || pilih>=jumlahGoal) return;
    for(int i=pilih; i<jumlahGoal-1; i++) {
        daftarGoal[i] = daftarGoal[i+1];
    }
    jumlahGoal--;
    std::cout << "\nGoal dihapus\n";
}

void menuGoalTracker() {
    int pilih;
    do {
        std::cout << "\n===== GOAL TRACKER =====\n";
        std::cout << "\n1 Tambah Goal";
        std::cout << "\n2 Update Progress";
        std::cout << "\n3 Lihat Goal";
        std::cout << "\n4 Hapus Goal";
        std::cout << "\n0 Kembali";
        std::cout << "\nPilihan: ";
        std::cin >> pilih;
        switch(pilih){
            case 1:
                tambahGoal();
                break;
            case 2:
                updateProgressGoal();
                break;
            case 3:
                lihatGoal();
                break;
            case 4:
                hapusGoal();
                break;
        }
        pauseGoal();
    } while(pilih!=0);
}