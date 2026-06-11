#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <vector>

struct Transaction {
    std::string txid;
    std::string type;
    std::string category;
    std::string description;
    double amount;
    std::string walletId;
    std::string walletName;
    std::string datetime;
    std::string prevHash;
    std::string currentHash;
};

struct Wallet {
    std::string id;
    std::string name;
    std::string type;
    double balance;
};

extern std::vector<Transaction> transactions;
extern std::vector<Wallet> wallets;

extern double totalIncome;
extern double totalExpense;
extern double needsSpent;
extern double wantsSpent;
extern double savingsSpent;

Wallet* cariWalletById(const std::string& id);
void tambahTransaksi(const std::string& type);
void riwayatTransaksi();
void cariTransaksi();
void hapusTransaksi();
void menuTransactionManagement();

void addAuditLog(const std::string& action, const std::string& details);

#endif