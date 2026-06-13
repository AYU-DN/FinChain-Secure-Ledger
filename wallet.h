#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>

struct Wallet {
    std::string id;
    std::string nama;
    std::string tipe;

    double saldo;
};
extern std::vector<Wallet> wallets;

void tambahWallet();
void editWallet();
void hapusWallet();
void lihatWallet();
void transferWallet();
void menuWallet();

#endif