#include "transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>

#define RESET    "\033[0m"
#define BOLD     "\033[1m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define CYAN     "\033[36m"
#define MAGENTA  "\033[35m"

std::vector<Transaction> transactions;

double totalIncome  = 0;
double totalExpense = 0;
double needsSpent   = 0;
double wantsSpent   = 0;
double savingsSpent = 0;

static std::string getCurrentTime() {
    time_t now = time(0);
    struct tm* ti = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", ti);
    return std::string(buf);
}

static std::string generateTXID() {
    const std::string pool = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::string id = "TX-";
    
    static bool seeded = false;
    if (!seeded) {
        srand((unsigned int)time(0));
        seeded = true;
    }
    
    for (int i = 0; i < 16; i++) {
        id += pool[rand() % (int)pool.size()];
        if (i == 3 || i == 7 || i == 11) id += "-";
    }
    return id;
}

static std::string generateHash(const std::string& data) {
    unsigned long h = 5381;
    for (unsigned char c : data) h = ((h << 5) + h) + c;
    std::stringstream ss;
    ss << std::hex << std::uppercase << h;
    std::string s = ss.str();
    while ((int)s.size() < 16) s = "0" + s;
    return s.substr(0, 16);
}

static std::string formatRupiah(double v) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(0) << v;
    std::string s = ss.str();
    bool neg = (!s.empty() && s[0] == '-');
    if (neg) s = s.substr(1);
    std::string out;
    int n = (int)s.size();
    for (int i = 0; i < n; i++) {
        if (i > 0 && (n - i) % 3 == 0) out += ".";
        out += s[i];
    }
    return "Rp " + std::string(neg ? "-" : "") + out;
}

static void printHeader(const std::string& title) {
    std::cout << CYAN << BOLD;
    std::cout << "\n  ╔══════════════════════════════════════════════════╗\n";
    std::cout << "  ║ " << std::left << std::setw(50) << title << "║\n";
    std::cout << "  ╚══════════════════════════════════════════════════╝" << RESET << "\n";
}

static void printDivider() {
    std::cout << CYAN << "  ──────────────────────────────────────────────────" << RESET << "\n";
}

static void pauseScreen() {
    std::cout << "\n" << CYAN << "  Tekan Enter untuk melanjutkan..." << RESET;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

static void clearScreen() {
    std::cout << "\033[2J\033[H";
}

Wallet* cariWalletById(const std::string& id) {
    for (auto& w : wallets) {
        if (w.id == id) return &w;
    }
    return nullptr;
}

void tambahTransaksi(const std::string& type) {
    clearScreen();
    printHeader(std::string("TRANSACTION MANAGEMENT  |  Tambah ") + type);

    if (wallets.empty()) {
        std::cout << RED << "\n  Belum ada wallet. Tambah wallet dulu!\n" << RESET;
        pauseScreen(); 
        return;
    }

    std::cout << "\n  Daftar Wallet:\n";
    for (int i = 0; i < (int)wallets.size(); i++) {
        std::cout << "  [" << (i+1) << "] " << wallets[i].name << " (" << wallets[i].type << ") - " << formatRupiah(wallets[i].balance) << "\n";
    }
    std::cout << "  Pilih wallet: ";
    int wIdx; 
    std::cin >> wIdx; 
    wIdx--;

    if (wIdx < 0 || wIdx >= (int)wallets.size()) {
        std::cout << RED << "  Wallet tidak valid.\n" << RESET; 
        pauseScreen(); 
        return;
    }
    Wallet& w = wallets[wIdx];

    std::string desc;
    double amount;
    int catChoice;

    std::cout << "\n  Deskripsi        : ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, desc);
    
    std::cout << "  Jumlah (Rp)      : "; 
    std::cin >> amount;
    if (amount <= 0) { 
        std::cout << RED << "  Jumlah tidak valid.\n" << RESET; 
        pauseScreen(); 
        return; 
    }

    std::cout << "\n  Kategori Budget  :\n";
    std::cout << "  [1] Needs (Kebutuhan)\n";
    std::cout << "  [2] Wants (Keinginan)\n";
    std::cout << "  [3] Savings (Tabungan)\n";
    std::cout << "  Pilihan: "; 
    std::cin >> catChoice;

    std::string category;
    if      (catChoice == 1) category = "Needs";
    else if (catChoice == 2) category = "Wants";
    else if (catChoice == 3) category = "Savings";
    else { 
        std::cout << RED << "  Kategori tidak valid.\n" << RESET; 
        pauseScreen(); 
        return; 
    }

    if (type == "Expense" && w.balance < amount) {
        std::cout << RED << "\n  Saldo tidak cukup! Saldo: " << formatRupiah(w.balance) << "\n" << RESET;
        pauseScreen(); 
        return;
    }

    Transaction t;
    t.txid        = generateTXID();
    t.type        = type;
    t.category    = category;
    t.description = desc;
    t.amount      = amount;
    t.walletId    = w.id;
    t.walletName  = w.name;
    t.datetime    = getCurrentTime();

    std::string prevHash = transactions.empty() ? "0000000000000000" : transactions.back().currentHash;
    t.prevHash    = prevHash;
    std::string hashData = t.txid + t.type + std::to_string(amount) + t.datetime + prevHash;
    t.currentHash = generateHash(hashData);

    if (type == "Income") {
        w.balance    += amount;
        totalIncome  += amount;
        if (category == "Needs")         needsSpent   -= amount; 
        else if (category == "Savings")  savingsSpent += amount;
    } else {
        w.balance    -= amount;
        totalExpense += amount;
        if      (category == "Needs")    needsSpent   += amount;
        else if (category == "Wants")    wantsSpent   += amount;
        else if (category == "Savings")  savingsSpent += amount;
    }

    transactions.push_back(t);
    
    addAuditLog("Tambah Transaksi", t.txid + " | " + type + " | " + formatRupiah(amount) + " | " + desc);

    std::cout << GREEN << "\n  ✓ Transaksi berhasil dicatat!\n" << RESET;
    std::cout << "  TXID          : " << BOLD << YELLOW << t.txid << RESET << "\n";
    std::cout << "  Current Hash  : " << t.currentHash << "\n";
    std::cout << "  Saldo Wallet  : " << formatRupiah(w.balance) << "\n";
    pauseScreen();
}

void riwayatTransaksi() {
    clearScreen();
    printHeader("TRANSACTION MANAGEMENT  |  Riwayat Transaksi");
    if (transactions.empty()) {
        std::cout << "\n  " << CYAN << "Belum ada transaksi.\n" << RESET;
        pauseScreen(); 
        return;
    }
    std::cout << "\n  " << BOLD << std::left
              << std::setw(22) << "TXID"
              << std::setw(10) << "Tipe"
              << std::setw(10) << "Kategori"
              << std::setw(16) << "Jumlah"
              << std::setw(20) << "Deskripsi"
              << "Waktu" << RESET << "\n";
    printDivider();
    
    for (auto& t : transactions) {
        std::string col = (t.type == "Income") ? GREEN : RED;
        std::cout << "  " << std::left
                  << std::setw(22) << t.txid.substr(0, 20)
                  << col << std::setw(10) << t.type << RESET
                  << std::setw(10) << t.category
                  << std::setw(16) << formatRupiah(t.amount)
                  << std::setw(20) << t.description.substr(0, 18)
                  << t.datetime.substr(0, 16) << "\n";
    }
    pauseScreen();
}

void cariTransaksi() {
    clearScreen();
    printHeader("TRANSACTION MANAGEMENT  |  Cari Transaksi");
    std::string keyword;
    std::cout << "\n  Masukkan kata kunci (TXID/deskripsi): ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, keyword);

    bool found = false;
    for (auto& t : transactions) {
        if (t.txid.find(keyword) != std::string::npos || t.description.find(keyword) != std::string::npos) {
            if (!found) {
                std::cout << "\n  " << BOLD << "Hasil Pencarian:" << RESET << "\n";
                printDivider();
                found = true;
            }
            std::string col = (t.type == "Income") ? GREEN : RED;
            std::cout << "  TXID     : " << BOLD << YELLOW << t.txid << RESET << "\n";
            std::cout << "  Tipe     : " << col << t.type << RESET << "\n";
            std::cout << "  Kategori : " << t.category << "\n";
            std::cout << "  Jumlah   : " << formatRupiah(t.amount) << "\n";
            std::cout << "  Deskripsi: " << t.description << "\n";
            std::cout << "  Waktu    : " << t.datetime << "\n";
            std::cout << "  Hash     : " << t.currentHash << "\n";
            printDivider();
        }
    }
    if (!found) {
        std::cout << RED << "\n  Tidak ditemukan.\n" << RESET;
    }
    pauseScreen();
}

void hapusTransaksi() {
    clearScreen();
    printHeader("TRANSACTION MANAGEMENT  |  Hapus Transaksi");
    if (transactions.empty()) { 
        std::cout << RED << "\n  Belum ada transaksi.\n" << RESET; 
        pauseScreen(); 
        return; 
    }

    riwayatTransaksi();
    std::string txid;
    std::cout << "\n  Masukkan TXID yang akan dihapus: ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::getline(std::cin, txid);

    for (int i = 0; i < (int)transactions.size(); i++) {
        if (transactions[i].txid == txid) {
            Transaction& t = transactions[i];
            Wallet* w = cariWalletById(t.walletId);
            if (w) {
                if (t.type == "Income") { 
                    w->balance  -= t.amount; 
                    totalIncome -= t.amount; 
                } else { 
                    w->balance   += t.amount; 
                    totalExpense -= t.amount; 
                }
            }
            if (t.type == "Expense") {
                if      (t.category == "Needs")    needsSpent   -= t.amount;
                else if (t.category == "Wants")    wantsSpent   -= t.amount;
                else if (t.category == "Savings")  savingsSpent -= t.amount;
            }

            addAuditLog("Hapus Transaksi", txid + " dihapus");
            transactions.erase(transactions.begin() + i);
            std::cout << GREEN << "\n  ✓ Transaksi berhasil dihapus!\n" << RESET;
            pauseScreen(); 
            return;
        }
    }
    std::cout << RED << "\n  TXID tidak ditemukan.\n" << RESET;
    pauseScreen();
}

void menuTransactionManagement() {
    int choice;
    do {
        clearScreen();
        printHeader("FINCHAIN SECURE LEDGER  |  TRANSACTION MANAGEMENT");
        std::cout << "\n  " << GREEN  << "[1]" << RESET << " Tambah Income\n";
        std::cout << "  " << RED    << "[2]" << RESET << " Tambah Expense\n";
        std::cout << "  " << CYAN   << "[3]" << RESET << " Riwayat Transaksi\n";
        std::cout << "  " << YELLOW << "[4]" << RESET << " Cari Transaksi\n";
        std::cout << "  " << MAGENTA<< "[5]" << RESET << " Hapus Transaksi\n";
        std::cout << "  [0] Kembali\n";
        printDivider();
        std::cout << "  Pilihan: "; 
        std::cin >> choice;

        switch (choice) {
            case 1: tambahTransaksi("Income");  break;
            case 2: tambahTransaksi("Expense"); break;
            case 3: riwayatTransaksi();         break;
            case 4: cariTransaksi();            break;
            case 5: hapusTransaksi();           break;
            case 0: break;
            default: 
                std::cout << RED << "\n  Pilihan tidak valid.\n" << RESET; 
                pauseScreen();
        }
    } while (choice != 0);
}