#include "wallet.h"
#include <iostream>
#include <iomanip>

std::vector<Wallet> wallets;
std::string buatIdWallet(){
    return "WL-" + std::to_string(wallets.size()+1);
}

void tambahWallet(){
    Wallet w;
    std::cout << "\nNama Wallet : ";
    std::getline(std::cin >> std::ws, w.nama );
    std::cout << "\nTipe" << "\n1 Cash" << "\n2 E-Wallet" << "\n3 M-Banking" << "\nPilihan: ";
    int pilih;
    std::cin >> pilih;

    if(pilih==1)  w.tipe="Cash";
    else if(pilih==2) w.tipe="E-Wallet";
    else w.tipe="M-Banking";

    std::cout << "\nSaldo Awal: ";
    std::cin >> w.saldo;
    w.id= buatIdWallet();
    wallets.push_back(w);
    std::cout << "\nWallet dibuat\n";
}

void lihatWallet(){
    if( wallets.empty()){
        std::cout << "\nBelum ada wallet\n";
        return;
    }
    for(int i=0; i<wallets.size(); i++){
        std::cout << "\n[" << i+1 << "] " << wallets[i].nama 
                  << "\nID : " << wallets[i].id 
                  << "\nJenis : " << wallets[i].tipe
                  << "\nSaldo : Rp " << std::fixed << std::setprecision(0)  << wallets[i].saldo << "\n";
    }
}

void transferWallet(){
    lihatWallet();
    int asal;
    int tujuan;
    double nominal;
    std::cout << "\nAsal: ";
    std::cin >> asal;
    std::cout << "Tujuan: ";
    std::cin >> tujuan;
    std::cout << "Nominal: ";
    std::cin >> nominal;
    asal--;
    tujuan--;
    if(wallets[asal].saldo < nominal){
        std::cout << "\nSaldo tidak cukup\n";
        return;
    }
    wallets[asal].saldo -= nominal;
    wallets[tujuan].saldo += nominal;
    std::cout << "\nTransfer berhasil\n";
}

void hapusWallet(){
    lihatWallet();
    int pilih;
    std::cin >> pilih;
    pilih--;
    wallets.erase(wallets.begin() + pilih);
    std::cout << "\nWallet dihapus\n";
}

void editWallet(){
    lihatWallet();
    int pilih;
    std::cin >> pilih;
    pilih--;
    std::cout << "\nNama baru: ";
    std::getline(std::cin >> std::ws, wallets[pilih].nama);
}

void menuWallet(){
    int pilih;
    do{
        std::cout << "\n===== WALLET =====" << "\n1 Tambah"
                  << "\n2 Edit"
                  << "\n3 Hapus"
                  << "\n4 Lihat"
                  << "\n5 Transfer"
                  << "\n0 Kembali"
                  << "\nPilihan: ";
        std::cin >> pilih;
        if(!(std::cin >> pilih)){
            std::cin.clear();
            std::cin.ignore(1000,'\n');
            std::cout << "\nInput harus angka!\n";
            continue;
        }
        switch(pilih){
            case 1:
            tambahWallet();
            break;
            case 2:
            editWallet();
            break;
            case 3:
            hapusWallet();
            break;
            case 4:
            lihatWallet();
            break;
            case 5:
            transferWallet();
            break;
            default:
            std::cout << "\n[ERROR] Pilihan tidak valid!" << "\nMasukkan angka 0–5\n";
        }

    }
    while(pilih =0);
}