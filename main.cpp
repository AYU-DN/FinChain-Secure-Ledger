/*
 * ╔══════════════════════════════════════════════════════════════════════╗
 * ║         FINCHAIN SECURE LEDGER  —  ADVANCED TERMINAL UI              ║
 * ║         Sistem Manajemen Keuangan Pribadi Berbasis Terminal          ║
 * ║         Blockchain-Inspired Hash Chain · 50/30/20 Budget Rule        ║
 * ╚══════════════════════════════════════════════════════════════════════╝
 *
 *  STRUKTUR FILE  (1 file, tidak modular, hanya diberi penanda bagian):
 *  ─────────────────────────────────────────────────────────────────────
 *  [A]  KONSTANTA WARNA & GAYA ANSI
 *  [B]  UTILITAS  — bantuan (bersihLayar, jeda, warna, progressBar, dll.)
 *  [C]  STRUKTUR DATA — Pengguna, Dompet, Transaksi, Goal, LogAudit
 *  [D]  STATE GLOBAL (variabel sesi)
 *  [E]  FUNGSI BANTU KALKULASI
 *  [F]  FUNGSI BANTU UI  — header, divider, banner, kartu, tabel
 *  [G]  MODUL AUDIT LOG
 *  [H]  MODUL PERSONAL DASHBOARD
 *  [I]  MODUL WALLET MANAGEMENT
 *  [J]  MODUL TRANSACTION MANAGEMENT
 *  [K]  MODUL BUDGET MANAGER
 *  [L]  MODUL GOAL TRACKER
 *  [M]  MODUL AUDIT LOG (Menu)
 *  [N]  FINANCIAL MANAGEMENT SYSTEM (Menu Utama Setelah Login)
 *  [O]  AUTHENTICATION SYSTEM
 *  [P]  MAIN PROGRAM
 */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <limits>
#include <cstdlib>
#include <cstring>

using namespace std;

// ═══════════════════════════════════════════════════════════════════════
// [A]  KONSTANTA WARNA & GAYA ANSI
// ═══════════════════════════════════════════════════════════════════════

// Reset & gaya dasar
#define RESET           "\033[0m"
#define TEBAL           "\033[1m"
#define REDUP           "\033[2m"
#define MIRING          "\033[3m"
#define GARIS_BAWAH     "\033[4m"
#define KEDIP           "\033[5m"

// Warna teks
#define HITAM           "\033[30m"
#define MERAH           "\033[31m"
#define HIJAU           "\033[32m"
#define KUNING          "\033[33m"
#define BIRU            "\033[34m"
#define MAGENTA         "\033[35m"
#define CYAN            "\033[36m"
#define PUTIH           "\033[37m"

// Warna terang
#define MERAH_TERANG    "\033[91m"
#define HIJAU_TERANG    "\033[92m"
#define KUNING_TERANG   "\033[93m"
#define BIRU_TERANG     "\033[94m"
#define MAGENTA_TERANG  "\033[95m"
#define CYAN_TERANG     "\033[96m"
#define PUTIH_TERANG    "\033[97m"

// Background warna
#define BG_HITAM        "\033[40m"
#define BG_MERAH        "\033[41m"
#define BG_HIJAU        "\033[42m"
#define BG_KUNING       "\033[43m"
#define BG_BIRU         "\033[44m"
#define BG_MAGENTA      "\033[45m"
#define BG_CYAN         "\033[46m"
#define BG_PUTIH        "\033[47m"

// Karakter box-drawing
#define GARIS_H         "─"
#define GARIS_V         "│"
#define POJOK_KI_AT     "╔"
#define POJOK_KA_AT     "╗"
#define POJOK_KI_BW     "╚"
#define POJOK_KA_BW     "╝"
#define SILANG_AT       "╦"
#define SILANG_BW       "╩"
#define SILANG_KI       "╠"
#define SILANG_KA       "╣"
#define SILANG_TENGAH   "╬"
#define GARIS_DH        "═"
#define GARIS_DV        "║"

// Simbol ikon
#define IKON_CEK        "\u2713"
#define IKON_SILANG     "\u2717"
#define IKON_BINTANG    "\u2605"
#define IKON_PANAH      "\u25B6"
#define IKON_UANG       "\u20AC"
#define IKON_GEMBOK     "\uD83D\uDD12"
#define IKON_GRAFIK     "\u25B2"
#define IKON_BLOK       "\u2588"
#define IKON_BLOK_SETENGAH "\u2593"
#define IKON_BLOK_TIPIS "\u2591"
#define IKON_PANAH_BAWAH "\u25BC"
#define IKON_LINGKARAN  "\u25CF"
#define IKON_KOTAK      "\u25A0"

// ═══════════════════════════════════════════════════════════════════════
// [B]  UTILITAS  —  Bantuan (bersihLayar, jeda, warna, progressBar, dll.)
// ═══════════════════════════════════════════════════════════════════════

void bersihLayar() {
    cout << "\033[2J\033[H";
    cout.flush();
}

void jedaLayar() {
    cout << "\n" << CYAN << "  " << IKON_PANAH << " Tekan Enter untuk melanjutkan..." << RESET;
    cin.get();
}

// Mendapatkan waktu saat ini dalam format dd/mm/yyyy HH:MM:SS
string dapatkanWaktuSekarang() {
    time_t sekarang = time(0);
    struct tm* infoWaktu = localtime(&sekarang);
    char penyangga[80];
    strftime(penyangga, sizeof(penyangga), "%d/%m/%Y %H:%M:%S", infoWaktu);
    return string(penyangga);
}

// Mendapatkan tanggal saja
string dapatkanTanggalSekarang() {
    time_t sekarang = time(0);
    struct tm* infoWaktu = localtime(&sekarang);
    char penyangga[20];
    strftime(penyangga, sizeof(penyangga), "%d/%m/%Y", infoWaktu);
    return string(penyangga);
}

// Generate TXID unik: TX-XXXX-XXXX-XXXX-XXXX (permutasi karakter kapital, kecil, angka)
string hasilkanIDTransaksi() {
    const string karakterValid =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string txid = "TX-";
    srand((unsigned int)time(0) + rand() + (unsigned int)(size_t)(&txid));
    for (int i = 0; i < 8; i++) {
        txid += karakterValid[rand() % karakterValid.size()];
        if (i == 3) txid += "-";
    }
    return txid;
}

// Generate hash berbasis djb2 (blockchain-inspired)
string hasilkanHash(const string& dataInput) {
    unsigned long nilaiHash = 5381;
    for (unsigned char karakter : dataInput) {
        nilaiHash = ((nilaiHash << 5) + nilaiHash) + karakter;
    }
    stringstream alirString;
    alirString << hex << uppercase << nilaiHash;
    string hasilHash = alirString.str();
    while ((int)hasilHash.size() < 16) hasilHash = "0" + hasilHash;
    return hasilHash.substr(0, 16);
}

// Format angka ke Rupiah: Rp 1.000.000
string formatRupiah(double jumlah) {
    stringstream alirString;
    alirString << fixed << setprecision(0) << jumlah;
    string teks = alirString.str();
    bool negatif = false;
    if (!teks.empty() && teks[0] == '-') { negatif = true; teks = teks.substr(1); }
    int panjang = teks.size();
    string hasil = "";
    for (int i = 0; i < panjang; i++) {
        if (i > 0 && (panjang - i) % 3 == 0) hasil += ".";
        hasil += teks[i];
    }
    return string("Rp ") + (negatif ? "-" : "") + hasil;
}

// Cetak progress bar ANSI berwarna, panjang = jumlahBlok (default 10)
void cetakProgressBar(double persentase, int jumlahBlok, const string& warna) {
    if (persentase < 0) persentase = 0;
    if (persentase > 100) persentase = 100;
    int terisi = (int)(persentase / 100.0 * jumlahBlok);
    if (terisi > jumlahBlok) terisi = jumlahBlok;

    cout << warna;
    for (int i = 0; i < jumlahBlok; i++) {
        if (i < terisi) cout << IKON_BLOK;
        else cout << IKON_BLOK_TIPIS;
    }
    cout << RESET;
    cout << " " << fixed << setprecision(1) << persentase << "%";
}

// Cetak progress bar gradient (hijau → kuning → merah otomatis)
void cetakProgressBarOtomatis(double persentase, int jumlahBlok) {
    string warna;
    if (persentase >= 80)      warna = HIJAU_TERANG;
    else if (persentase >= 50) warna = KUNING_TERANG;
    else if (persentase >= 25) warna = KUNING;
    else                       warna = MERAH_TERANG;
    cetakProgressBar(persentase, jumlahBlok, warna);
}

// Cetak progress bar budget (merah kalau lewat batas)
void cetakProgressBarBudget(double persentase, int jumlahBlok) {
    string warna;
    if (persentase <= 60)       warna = HIJAU_TERANG;
    else if (persentase <= 85)  warna = KUNING_TERANG;
    else if (persentase <= 100) warna = KUNING;
    else                        warna = MERAH_TERANG;
    // Batasi tampilan bar di 100%
    double persentaseTampil = min(persentase, 100.0);
    cetakProgressBar(persentaseTampil, jumlahBlok, warna);
}

// Ulangi karakter n kali
string ulangKarakter(const string& karakter, int jumlah) {
    string hasil = "";
    for (int i = 0; i < jumlah; i++) hasil += karakter;
    return hasil;
}

// Hitung lebar tampilan (jumlah codepoint UTF-8), bukan jumlah byte.
// Diperlukan karena judulHalaman sering berisi ikon/simbol multi-byte (·, ▶, dll.)
// sehingga string::size() akan menghitung lebih besar dari lebar yang sebenarnya
// tampil di layar dan membuat border/box jadi tidak center.
int lebarTampilUTF8(const string& teks) {
    int lebar = 0;

    for (size_t i = 0; i < teks.size();) {
        unsigned char c = (unsigned char)teks[i];

        // ASCII
        if (c < 0x80) {
            lebar++;
            i++;
        }

        // UTF-8 2 byte
        else if ((c & 0xE0) == 0xC0) {
            lebar += 1;
            i += 2;
        }

        // UTF-8 3 byte
        else if ((c & 0xF0) == 0xE0) {
            lebar += 1;
            i += 3;
        }

        // UTF-8 4 byte (emoji)
        else if ((c & 0xF8) == 0xF0) {
            lebar += 2;
            i += 4;
        }

        else {
            lebar++;
            i++;
        }
    }

    return lebar;
}

// Potong teks bila terlalu panjang
string potongTeks(const string& teks, int panjangMaks) {
    if ((int)teks.size() <= panjangMaks) return teks;
    return teks.substr(0, panjangMaks - 2) + "..";
}

// Baca input integer dengan validasi
int bacaInteger(const string& prompt) {
    int nilai;
    while (true) {
        cout << prompt;
        string baris;
        getline(cin, baris);
        if (baris.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return -999;
        }
        istringstream ss(baris);
        if (ss >> nilai) return nilai;
        cout << MERAH << "  Input tidak valid, masukkan angka.\n" << RESET;
        jedaLayar();
        return -999;
    }
}

// Baca input double dengan validasi
double bacaDouble(const string& prompt) {
    double nilai;
    while (true) {
        cout << prompt;
        string baris;
        getline(cin, baris);
        if (baris.empty()) {
            cout << MERAH << "  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return -999;
        }
        istringstream ss(baris);
        if (ss >> nilai) return nilai;
        cout << MERAH << "  Input tidak valid, masukkan angka.\n" << RESET;
        jedaLayar();
        return -999;
    }
}

// ═══════════════════════════════════════════════════════════════════════
// [C]  STRUKTUR DATA
// ═══════════════════════════════════════════════════════════════════════

// ── Struktur Data: Pengguna ──────────────────────────────────────────
struct Pengguna {
    string namaAkun;
    string kataKunci;
    string waktuDaftar;
};

// ── Struktur Data: Dompet / Wallet ──────────────────────────────────
struct Dompet {
    string identifikasiDompet;    // contoh: W1, W2
    string namaDompet;
    string jenisDompet;           // "Cash", "E-Wallet", "M-Banking"
    string penyediaDompet;        // GoPay, BCA, dll.
    double saldoDompet;
    string waktuDibuat;
};

// ── Struktur Data: Transaksi ─────────────────────────────────────────
struct Transaksi {
    string idTransaksi;           // TXID unik
    string jenisTransaksi;        // "Income" / "Expense"
    string kategoriTransaksi;     // "Needs" / "Wants" / "Savings"
    string deskripsiTransaksi;
    double jumlahTransaksi;
    string idDompetTransaksi;
    string namaDompetTransaksi;
    string waktuTransaksi;
    string hashSebelumnya;
    string hashSaatIni;
};

struct RiwayatSetoranTarget {
    string idDompet;
    string namaDompet;
    double nominal;
    string waktu;
};

// ── Struktur Data: Target Keuangan / Goal ────────────────────────────
struct TargetKeuangan {
    string identifikasiTarget;    // G1, G2, ...
    string namaTarget;
    double nominalTarget;
    double nominalTerkumpul;
    string waktuDibuat;
    vector<RiwayatSetoranTarget>
    riwayatSetoran;
};

// ── Struktur Data: Entri Log Audit ──────────────────────────────────
struct EntriLogAudit {
    string waktuAktivitas;
    string namaAkunPengguna;
    string jenisAktivitas;
    string detailAktivitas;
};

// ═══════════════════════════════════════════════════════════════════════
// [D]  STATE GLOBAL (Variabel Sesi)
// ═══════════════════════════════════════════════════════════════════════

vector<Pengguna>        daftarSemuaPengguna;

// Data sesi aktif (direset saat logout + reset)
string                  namaAkunAktif   = "";
vector<Dompet>          daftarDompet;
vector<Transaksi>       daftarTransaksi;
vector<TargetKeuangan>  daftarTarget;
vector<EntriLogAudit>   daftarLogAudit;

// Statistik keuangan sesi
double totalPendapatan      = 0.0;
double totalPengeluaran     = 0.0;
double pengeluaranKebutuhan = 0.0;  // Needs
double pengeluaranKeinginan = 0.0;  // Wants
double pengeluaranTabungan  = 0.0;  // Savings

// Penghitung ID otomatis
int penghitungDompet = 1;
int penghitungTarget = 1;

// ═══════════════════════════════════════════════════════════════════════
// [E]  FUNGSI BANTU KALKULASI
// ═══════════════════════════════════════════════════════════════════════

double hitungTotalAset() {
    double total = 0.0;
    for (const auto& dompet : daftarDompet) total += dompet.saldoDompet;
    return total;
}

double hitungSaldoBersih() {
    return totalPendapatan - totalPengeluaran;
}

double hitungBatasKebutuhan()  { return totalPendapatan * 0.50; }
double hitungBatasKeinginan()  { return totalPendapatan * 0.30; }
double hitungBatasTabungan()   { return totalPendapatan * 0.20; }

double hitungPersentaseKesehatan() {
    if (totalPendapatan <= 0) return 0.0;
    double saldoBersih = hitungSaldoBersih();
    double persentase  = (saldoBersih / totalPendapatan) * 100.0;
    return max(0.0, min(100.0, persentase));
}

double hitungPersentaseBudget() {
    double jumlahOke = 0.0;
    if (pengeluaranKebutuhan <= hitungBatasKebutuhan()) jumlahOke++;
    if (pengeluaranKeinginan <= hitungBatasKeinginan()) jumlahOke++;
    if (pengeluaranTabungan  <= hitungBatasTabungan())  jumlahOke++;
    return (jumlahOke / 3.0) * 100.0;
}

double hitungRataRataProgressTarget() {
    if (daftarTarget.empty()) return 0.0;
    double jumlahPersentase = 0.0;
    for (const auto& target : daftarTarget) {
        if (target.nominalTarget > 0)
            jumlahPersentase += min(100.0, target.nominalTerkumpul / target.nominalTarget * 100.0);
    }
    return jumlahPersentase / daftarTarget.size();
}

// ═══════════════════════════════════════════════════════════════════════
// [F]  FUNGSI BANTU UI  — header, divider, banner, kartu, tabel
// ═══════════════════════════════════════════════════════════════════════

// Cetak baris pembagi tipis
void cetakPembagiTipis() {
    cout << CYAN << "  " << ulangKarakter(GARIS_H, 68) << RESET << "\n";
}

// Cetak baris pembagi tebal
void cetakPembagiTebal() {
    cout << CYAN_TERANG << "  " << ulangKarakter(GARIS_DH, 68) << RESET << "\n";
}

// Cetak header halaman dengan box
void cetakHeader(const string& judulHalaman) {
    int lebarKotak = 70;
    int panjangJudul = lebarTampilUTF8(judulHalaman);
    int sisaRuang = max(0, lebarKotak - panjangJudul);
    int kiriKosong = sisaRuang / 2;
    int kananKosong = sisaRuang - kiriKosong;

    cout << "\n" << CYAN_TERANG << TEBAL;
    cout << "  ╔" << ulangKarakter(GARIS_DH, lebarKotak) << "╗\n";
    cout << "  ║" << string(kiriKosong, ' ')
         << PUTIH_TERANG << judulHalaman << CYAN_TERANG
         << string(kananKosong, ' ') << "║\n";
    cout << "  ╚" << ulangKarakter(GARIS_DH, lebarKotak) << "╝" << RESET << "\n";
}

// Cetak header sub-menu (lebih kecil)
void cetakHeaderSub(const string& judulSub) {
    int lebarTarget = 70;
    int sisaRuang = max(0, lebarTarget - 4 - lebarTampilUTF8(judulSub));
    cout << "\n" << CYAN << TEBAL;
    cout << "  ┌─ " << judulSub << " " 
         << ulangKarakter("─", sisaRuang) << "┐\n";
    cout << RESET;
}

// Cetak baris kosong berpola
void cetakSpasi(int jumlahBaris = 1) {
    for (int i = 0; i < jumlahBaris; i++) cout << "\n";
}

// Cetak badge status berwarna
void cetakBadge(const string& teks, const string& warnaBG, const string& warnaTeks = "\033[97m") {
    cout << warnaBG << warnaTeks << TEBAL << " " << teks << " " << RESET;
}

// Cetak kartu info (kotak kecil berisi label dan nilai)
void cetakKartuInfo(const string& label, const string& nilai, const string& warnaLabel, const string& warnaNilai, int lebarKartu = 34) {
    int panjangIsi = lebarKartu - 2; 
    string teksLabel = potongTeks("  " + label, panjangIsi);
    string teksNilai = potongTeks("  " + nilai, panjangIsi);
    cout << "  ┌" << ulangKarakter("─", panjangIsi) << "┐\n";
    cout << "  │" << warnaLabel << TEBAL << left << teksLabel 
         << string(panjangIsi - lebarTampilUTF8(teksLabel), ' ') << RESET << "│\n";
    
    cout << "  │" << warnaNilai << TEBAL << left << teksNilai 
         << string(panjangIsi - lebarTampilUTF8(teksNilai), ' ') << RESET << "│\n";
    
    cout << "  └" << ulangKarakter("─", panjangIsi) << "┘\n";
}

// Cetak baris menu dengan nomor berwarna
void cetakItemMenu(const string& nomor, const string& label, const string& warnaNomor, const string& ikon = "") {
    cout << "  " << warnaNomor << TEBAL << "[" << nomor << "]" << RESET
         << "  " << (ikon.empty() ? "" : ikon + " ")
         << label << "\n";
}

// Cetak animasi loading singkat (3 titik)
void cetakLoading(const string& pesan) {
    cout << CYAN_TERANG << "\n  " << pesan;
    cout.flush();
    for (int i = 0; i < 3; i++) {
        // Simulasi jeda singkat
        for (volatile long j = 0; j < 5000000L; j++);
        cout << ".";
        cout.flush();
    }
    cout << " " << HIJAU_TERANG << IKON_CEK << RESET << "\n";
}

// Cetak footer konsisten di bawah halaman
void cetakFooter(const string& pesan = "") {
    cetakPembagiTipis();
    if (!pesan.empty()) {
        cout << "  " << REDUP << pesan << RESET << "\n";
        cetakPembagiTipis();
    }
}

// Cetak banner FINCHAIN pixel art (merah)
void cetakBannerFinchain() {
    std::cout << CYAN << TEBAL;
    std::cout << "\n";
    std::cout << "  ███████╗██╗███╗   ██╗ ██████╗██╗  ██╗ █████╗ ██╗███╗   ██╗\n";
    std::cout << "  ██╔════╝██║████╗  ██║██╔════╝██║  ██║██╔══██╗██║████╗  ██║\n";
    std::cout << "  █████╗  ██║██╔██╗ ██║██║     ███████║███████║██║██╔██╗ ██║\n";
    std::cout << "  ██╔══╝  ██║██║╚██╗██║██║     ██╔══██║██╔══██║██║██║╚██╗██║\n";
    std::cout << "  ██║     ██║██║ ╚████║╚██████╗██║  ██║██║  ██║██║██║ ╚████║\n";
    std::cout << "  ╚═╝     ╚═╝╚═╝  ╚═══╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝\n";
    std::cout << RESET;

    // Sub-judul
    cout << "\n";
    cout << "  " << KUNING_TERANG << TEBAL
         << "  ═══  Secure Ledger · Sistem Manajemen Keuangan Pribadi  ═══"
         << RESET << "\n";
    cout << "  " << CYAN << REDUP
         << "        Powered by Blockchain-Inspired Hash Chain Algorithm"
         << RESET << "\n";
    cetakPembagiTebal();
}

// Cetak status bar atas (waktu + user)
void cetakStatusBar(const string& namaUser = "") {
    string waktuSaatIni = dapatkanWaktuSekarang();
    cout << "  " << BG_BIRU << PUTIH_TERANG << TEBAL
         << " FinChain v2.0 "
         << RESET << BG_BIRU << PUTIH
         << " | " << waktuSaatIni
         << (namaUser.empty() ? "" : " | User: " + namaUser)
         << string(max(0, 30 - (int)namaUser.size()), ' ')
         << RESET << "\n";
}

// ═══════════════════════════════════════════════════════════════════════
// [G]  MODUL AUDIT LOG — fungsi tambah entri log
// ═══════════════════════════════════════════════════════════════════════

void tambahEntriLogAudit(const string& jenisAktivitas, const string& detailAktivitas) {
    EntriLogAudit entri;
    entri.waktuAktivitas     = dapatkanWaktuSekarang();
    entri.namaAkunPengguna   = namaAkunAktif;
    entri.jenisAktivitas     = jenisAktivitas;
    entri.detailAktivitas    = detailAktivitas;
    daftarLogAudit.push_back(entri);
}

// ═══════════════════════════════════════════════════════════════════════
// [H]  MODUL PERSONAL DASHBOARD
// ═══════════════════════════════════════════════════════════════════════

void tampilkanPersonalDashboard() {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  " + string(IKON_LINGKARAN) + "  PERSONAL DASHBOARD  ·  " + namaAkunAktif + "  ");

    double totalAset  = hitungTotalAset();
    double saldoBersih = hitungSaldoBersih();

    // ── Bagian: Financial Overview ───────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " FINANCIAL OVERVIEW" << RESET << "\n";
    cetakPembagiTipis();

    // Empat kartu side-by-side (simulasi dengan cout baris demi baris)
    cout << "  ╔════════════════════════════════════════════════════════════╗\n";
    cout << "  ║ "
     << KUNING << TEBAL
     << "TOTAL PENDAPATAN"
     << RESET;
    cout << string(
        max(0, 42 - lebarTampilUTF8(formatRupiah(totalPendapatan))),
        ' '
    );
    cout << HIJAU << TEBAL
        << formatRupiah(totalPendapatan)
        << RESET
        << " ║\n";

    cout << "  ╚════════════════════════════════════════════════════════════╝\n";
    cout << "\n";
    cout << "  ╔════════════════════════════════════════════════════════════╗\n";

    cout << "  ║ "
        << MERAH_TERANG << TEBAL
        << "TOTAL PENGELUARAN"
        << RESET;

    cout << string(
        max(0, 41 - lebarTampilUTF8(formatRupiah(totalPengeluaran))),
        ' '
    );

    cout << MERAH << TEBAL
        << formatRupiah(totalPengeluaran)
        << RESET
        << " ║\n";

    cout << "  ╚════════════════════════════════════════════════════════════╝\n";

    // ── Bagian: Wallet Summary ───────────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " WALLET SUMMARY" << RESET << "\n";
    cetakPembagiTipis();

    double saldoCash = 0, saldoEWallet = 0, saldoMBanking = 0;
    for (const auto& dompet : daftarDompet) {
        if (dompet.jenisDompet == "Cash")       saldoCash    += dompet.saldoDompet;
        else if (dompet.jenisDompet == "E-Wallet")  saldoEWallet += dompet.saldoDompet;
        else if (dompet.jenisDompet == "M-Banking") saldoMBanking += dompet.saldoDompet;
    }

    cout << "  " << HIJAU << IKON_KOTAK << " Cash       : " << RESET << TEBAL << formatRupiah(saldoCash) << RESET << "\n";
    cout << "  " << KUNING << IKON_KOTAK << " E-Wallet   : " << RESET << TEBAL << formatRupiah(saldoEWallet) << RESET << "\n";
    cout << "  " << CYAN << IKON_KOTAK << " M-Banking  : " << RESET << TEBAL << formatRupiah(saldoMBanking) << RESET << "\n";

    // ── Bagian: Budget Overview ──────────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " BUDGET OVERVIEW  (Aturan 50/30/20)" << RESET << "\n";
    cetakPembagiTipis();

    auto cetakBarisAnggaran = [](const string& labelKategori, double pengeluaranKategori, double batasKategori) {
        double persentaseKategori = (batasKategori > 0) ? min(120.0, pengeluaranKategori / batasKategori * 100.0) : 0.0;
        cout << "  " << TEBAL << left << setw(18) << labelKategori << RESET;
        cetakProgressBarBudget(persentaseKategori, 10);
        cout << "  " << formatRupiah(pengeluaranKategori)
             << REDUP << " / " << formatRupiah(batasKategori) << RESET << "\n";
    };
    cetakBarisAnggaran("Needs  (50%)", pengeluaranKebutuhan, hitungBatasKebutuhan());
    cetakBarisAnggaran("Wants  (30%)", pengeluaranKeinginan, hitungBatasKeinginan());
    cetakBarisAnggaran("Savings(20%)", pengeluaranTabungan,  hitungBatasTabungan());

    // ── Bagian: Goal Overview ────────────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " GOAL OVERVIEW" << RESET << "\n";
    cetakPembagiTipis();

    if (daftarTarget.empty()) {
        cout << "  " << REDUP << MIRING << "Belum ada target. Tambahkan di Goal Tracker!" << RESET << "\n";
    } else {
        for (const auto& target : daftarTarget) {
            double persentaseTarget = (target.nominalTarget > 0)
                ? min(100.0, target.nominalTerkumpul / target.nominalTarget * 100.0) : 0.0;
            cout << "  " << TEBAL << potongTeks(target.namaTarget, 22) << RESET << " ";
            cetakProgressBarOtomatis(persentaseTarget, 10);
            cout << "  " << formatRupiah(target.nominalTerkumpul) << REDUP << "/" << RESET
                 << formatRupiah(target.nominalTarget);
            if (persentaseTarget >= 100.0)
                cout << "  " << BG_HIJAU << TEBAL << " TERCAPAI! " << RESET;
            cout << "\n";
        }
    }

    // ── Bagian: Recent Activity ──────────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " RECENT ACTIVITY  (5 Transaksi Terakhir)" << RESET << "\n";
    cetakPembagiTipis();

    if (daftarTransaksi.empty()) {
        cout << "  " << REDUP << MIRING << "Belum ada transaksi." << RESET << "\n";
    } else {
        int mulaiDari = max(0, (int)daftarTransaksi.size() - 5);
        cout << "  " << TEBAL << CYAN
             << left << setw(18) << "Waktu"
             << setw(9)  << "Jenis"
             << setw(17) << "Jumlah"
             << "Deskripsi" << RESET << "\n";
        cetakPembagiTipis();
        for (int i = (int)daftarTransaksi.size() - 1; i >= mulaiDari; i--) {
            const auto& t = daftarTransaksi[i];
            string warnaJenis = (t.jenisTransaksi == "Income") ? HIJAU_TERANG : MERAH_TERANG;
            string ikonJenis  = (t.jenisTransaksi == "Income") ? IKON_GRAFIK  : IKON_PANAH_BAWAH;
            cout << "  " << REDUP << t.waktuTransaksi.substr(0,16) << RESET
                 << "  " << warnaJenis << TEBAL << ikonJenis << " " << setw(7) << t.jenisTransaksi << RESET
                 << "  " << setw(15) << formatRupiah(t.jumlahTransaksi)
                 << "  " << potongTeks(t.deskripsiTransaksi, 25) << "\n";
        }
    }

    // ── Bagian: Visualisasi Kesehatan Keuangan ───────────────────────
    // ── Bagian: Visualisasi Kesehatan Keuangan ───────────────────────
cetakSpasi();
cout << "  " << KUNING_TERANG << TEBAL
     << IKON_BINTANG
     << " VISUALISASI KESEHATAN KEUANGAN"
     << RESET << "\n";

cetakPembagiTipis();

// Ambil data normal
double persentaseKesehatan = hitungPersentaseKesehatan();
double persentaseBudget    = hitungPersentaseBudget();
double persentaseGoal      = hitungRataRataProgressTarget();

// Cek apakah benar-benar belum ada data
bool belumAdaData =
    totalPendapatan == 0 &&
    totalPengeluaran == 0 &&
    daftarTarget.empty();

if (belumAdaData) {
    persentaseKesehatan = 0;
    persentaseBudget = 0;
    persentaseGoal = 0;
}

cout << "  " << TEBAL
     << left << setw(20)
     << "Financial Health"
     << RESET;

cetakProgressBarOtomatis(persentaseKesehatan, 10);
cout << "\n";

cout << "  "
     << TEBAL
     << left
     << setw(20)
     << "Budget Status"
     << RESET;

cetakProgressBarOtomatis(persentaseBudget, 10);
cout << "\n";

cout << "  "
     << TEBAL
     << left
     << setw(20)
     << "Goal Progress"
     << RESET;

cetakProgressBarOtomatis(persentaseGoal, 10);
cout << "\n";

cetakSpasi();

cout << "  Skor Finansial : ";

if (belumAdaData) {

    cetakBadge(" BELUM ADA DATA ", BG_BIRU);
    cout << " Tambahkan transaksi untuk mulai analisis.";

}
else {

    int skorTotal = 0;
    int jumlahkategori = 0; 

    jumlahkategori++;
    if (persentaseKesehatan >= 50)
        skorTotal++;
    
    jumlahkategori++;
    if (persentaseBudget >= 66)
        skorTotal++;

    if (!daftarTarget.empty()) {
        jumlahkategori++;
        if (persentaseGoal >= 30)
            skorTotal++;
    }

    double skorAkhir = (double)skorTotal / jumlahkategori;

    if (skorAkhir == 1.0) {

        cetakBadge(" SANGAT SEHAT ", BG_HIJAU);
        cout << " Keuangan Anda dalam kondisi prima!";

    }
    else if (skorAkhir == 0.67) {

        cetakBadge(" CUKUP SEHAT ", BG_KUNING, "\033[30m");
        cout << " Ada ruang untuk ditingkatkan.";

    }
    else if (skorAkhir == 0.34) {

        cetakBadge(" PERLU PERHATIAN ", BG_MERAH);
        cout << " Evaluasi pengeluaran Anda.";

    }
    else {

        cetakBadge(" KRITIS ", BG_MERAH);
        cout << " Segera ambil tindakan!";
    }
}

cout << "\n";

cetakFooter(
    "Dashboard diperbarui secara real-time dari data sesi."
);

jedaLayar();
}

// ═══════════════════════════════════════════════════════════════════════
// [I]  MODUL WALLET MANAGEMENT
// ═══════════════════════════════════════════════════════════════════════

// Tampilkan tabel daftar dompet
void tampilkanDaftarDompet(bool jedaSetelah = true) {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  WALLET MANAGEMENT  ·  Daftar Dompet  ");

    if (daftarDompet.empty()) {
        cetakSpasi();
        cout << "  " << KUNING << IKON_PANAH << " Belum ada dompet. Tambahkan dompet baru!" << RESET << "\n";
        if (jedaSetelah) jedaLayar();
        return;
    }

    cetakSpasi();
    cout << "  " << TEBAL << CYAN
         << left << setw(5) << "No"
         << setw(5)  << "ID"
         << setw(22) << "Nama Dompet"
         << setw(12) << "Jenis"
         << setw(14) << "Penyedia"
         << setw(18) << "Saldo"
         << "Dibuat" << RESET << "\n";
    cetakPembagiTipis();

    int nomor = 1;
    for (const auto& dompet : daftarDompet) {
        string warnaJenis;
        if (dompet.jenisDompet == "Cash")           warnaJenis = HIJAU_TERANG;
        else if (dompet.jenisDompet == "E-Wallet")  warnaJenis = KUNING_TERANG;
        else                                         warnaJenis = CYAN_TERANG;

        cout << "  " << left
             << setw(5)  << nomor++
             << setw(5)  << dompet.identifikasiDompet
             << setw(22) << potongTeks(dompet.namaDompet, 21)
             << warnaJenis << setw(12) << dompet.jenisDompet << RESET
             << setw(14) << potongTeks(dompet.penyediaDompet, 13)
             << HIJAU << setw(18) << formatRupiah(dompet.saldoDompet) << RESET
             << REDUP << dompet.waktuDibuat.substr(0, 10) << RESET << "\n";
    }

    cetakPembagiTipis();
    cout << "  " << TEBAL << "Total Aset: " << HIJAU_TERANG
         << formatRupiah(hitungTotalAset()) << RESET
         << "  (" << daftarDompet.size() << " dompet aktif)\n";

    if (jedaSetelah) jedaLayar();
}

void tambahDompet() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  WALLET MANAGEMENT  ·  Tambah Dompet Baru  ");
        cetakSpasi();

        string namaDompetBaru, penyediaDompetBaru;
        int pilihanJenis;

        cout << "  " << KUNING << TEBAL << "Nama Dompet   : " << RESET;
        getline(cin, namaDompetBaru);
        if (namaDompetBaru.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        cetakSpasi();
        cout << "  " << KUNING << TEBAL << "Jenis Dompet  :" << RESET << "\n";
        cetakItemMenu("1", "Cash (Uang Tunai)",                        HIJAU,  IKON_KOTAK);
        cetakItemMenu("2", "E-Wallet (GoPay, DANA, OVO, dll.)",        KUNING, IKON_KOTAK);
        cetakItemMenu("3", "M-Banking (BCA, BRI, BNI, Mandiri, dll.)", CYAN,   IKON_KOTAK);
        pilihanJenis = bacaInteger("\n  Pilihan: ");
        if (pilihanJenis == -999) return;

        string jenisDompetBaru;
        if (pilihanJenis == 1) {
            jenisDompetBaru    = "Cash";
            penyediaDompetBaru = "Cash";
        } else if (pilihanJenis == 2) {
            jenisDompetBaru = "E-Wallet";
            cout << "  " << KUNING << "Nama Penyedia (GoPay/DANA/OVO/dll.): " << RESET;
            getline(cin, penyediaDompetBaru);
            if (penyediaDompetBaru.empty()) {
                cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
                jedaLayar();
                return;
            }
        } else if (pilihanJenis == 3) {
            jenisDompetBaru = "M-Banking";
            cout << "  " << KUNING << "Nama Penyedia (BCA/BRI/BNI/Mandiri/dll.): " << RESET;
            getline(cin, penyediaDompetBaru);
            if (penyediaDompetBaru.empty()) {
                cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
                jedaLayar();
                return;
            }
        } else {
            cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
            jedaLayar();
            continue;
        }

        double saldoAwalDompet = bacaDouble("  Saldo Awal (Rp): ");
        if (saldoAwalDompet == -999) return;
        if (saldoAwalDompet < 0) {
            cout << MERAH << "\n  Saldo tidak boleh negatif.\n" << RESET;
            jedaLayar();
            continue;
        }

        Dompet dompetBaru;
        dompetBaru.identifikasiDompet = "W" + to_string(penghitungDompet++);
        dompetBaru.namaDompet         = namaDompetBaru;
        dompetBaru.jenisDompet        = jenisDompetBaru;
        dompetBaru.penyediaDompet     = penyediaDompetBaru;
        dompetBaru.saldoDompet        = saldoAwalDompet;
        dompetBaru.waktuDibuat        = dapatkanWaktuSekarang();

        if (saldoAwalDompet > 0) totalPendapatan += saldoAwalDompet;
        daftarDompet.push_back(dompetBaru);

        tambahEntriLogAudit("Tambah Dompet",
            "Dompet '" + namaDompetBaru + "' (" + jenisDompetBaru + "/" + penyediaDompetBaru + ")"
            + " saldo awal " + formatRupiah(saldoAwalDompet));

        cetakLoading("\n  Menyimpan data dompet");
        cout << "\n  " << HIJAU_TERANG << TEBAL << IKON_CEK << " Dompet berhasil ditambahkan!" << RESET << "\n";
        cout << "  ID Dompet : " << KUNING << dompetBaru.identifikasiDompet << RESET << "\n";
        jedaLayar();
        return;
    }
}

void editDompet() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  WALLET MANAGEMENT  ·  Edit Dompet  ");

        if (daftarDompet.empty()) {
            cout << MERAH << "\n  Belum ada dompet.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanDaftarDompet(false);
        int indexDompet = bacaInteger("\n  Nomor dompet yang ingin diedit: ") - 1;
        if (indexDompet == -1000) return;
        if (indexDompet < 0 || indexDompet >= (int)daftarDompet.size()) {
            cout << MERAH << "  Nomor tidak valid.\n" << RESET;
            jedaLayar();
            continue;
        }

        Dompet& dompetDipilih = daftarDompet[indexDompet];
        string namaBaruDompet, penyediaBaruDompet;

        cout << "\n  Nama baru [" << KUNING << dompetDipilih.namaDompet << RESET << "]: ";
        getline(cin, namaBaruDompet);
        if (!namaBaruDompet.empty()) dompetDipilih.namaDompet = namaBaruDompet;

        if (dompetDipilih.jenisDompet != "Cash") {
            cout << "  Penyedia baru [" << KUNING << dompetDipilih.penyediaDompet << RESET << "]: ";
            getline(cin, penyediaBaruDompet);
            if (!penyediaBaruDompet.empty()) dompetDipilih.penyediaDompet = penyediaBaruDompet;
        }

        tambahEntriLogAudit("Edit Dompet", "Dompet ID " + dompetDipilih.identifikasiDompet + " diperbarui");
        cout << HIJAU_TERANG << "\n  " << IKON_CEK << " Dompet berhasil diperbarui!\n" << RESET;
        jedaLayar();
        return;
    }
}

void hapusDompet() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  WALLET MANAGEMENT  ·  Hapus Dompet  ");

        if (daftarDompet.empty()) {
            cout << MERAH << "\n  Belum ada dompet.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanDaftarDompet(false);
        int indexDompet = bacaInteger("\n  Nomor dompet yang ingin dihapus: ") - 1;
        if (indexDompet == -1000) return;
        if (indexDompet < 0 || indexDompet >= (int)daftarDompet.size()) {
            cout << MERAH << "  Nomor tidak valid.\n" << RESET;
            jedaLayar();
            continue;
        }

        cout << MERAH << "\n  Yakin hapus dompet '" << daftarDompet[indexDompet].namaDompet << "'? (ya/tidak): " << RESET;
        string konfirmasi;
        getline(cin, konfirmasi);
        if (konfirmasi.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        if (konfirmasi == "ya" || konfirmasi == "YA") {
            tambahEntriLogAudit("Hapus Dompet", "Dompet '" + daftarDompet[indexDompet].namaDompet + "' dihapus");
            daftarDompet.erase(daftarDompet.begin() + indexDompet);
            cout << HIJAU_TERANG << "\n  " << IKON_CEK << " Dompet berhasil dihapus!\n" << RESET;
        } else {
            cout << KUNING << "\n  Penghapusan dibatalkan.\n" << RESET;
        }
        jedaLayar();
        return;
    }
}

void transferAntarDompet() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  WALLET MANAGEMENT  ·  Transfer Antar Dompet  ");

        if ((int)daftarDompet.size() < 2) {
            cout << MERAH << "\n  Minimal diperlukan 2 dompet untuk transfer.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanDaftarDompet(false);
        int indexAsal = bacaInteger("\n  Dari dompet nomor  : ") - 1;
        if (indexAsal == -1000) return;

        int indexTujuan = bacaInteger("  Ke dompet nomor    : ") - 1;
        if (indexTujuan == -1000) return;

        if (indexAsal < 0 || indexAsal >= (int)daftarDompet.size() ||
            indexTujuan < 0 || indexTujuan >= (int)daftarDompet.size() ||
            indexAsal == indexTujuan) {
            cout << MERAH << "\n  Nomor dompet tidak valid atau sama.\n" << RESET;
            jedaLayar();
            continue;
        }

        double jumlahTransfer = bacaDouble("  Jumlah transfer (Rp): ");
        if (jumlahTransfer == -999) return;
        if (jumlahTransfer <= 0 || jumlahTransfer > daftarDompet[indexAsal].saldoDompet) {
            cout << MERAH << "\n  Jumlah tidak valid atau saldo tidak mencukupi.\n"
                 << "  Saldo tersedia: " << formatRupiah(daftarDompet[indexAsal].saldoDompet) << RESET << "\n";
            jedaLayar();
            continue;
        }

        daftarDompet[indexAsal].saldoDompet   -= jumlahTransfer;
        daftarDompet[indexTujuan].saldoDompet += jumlahTransfer;

        string detailTransfer = "Transfer " + formatRupiah(jumlahTransfer)
            + " dari '" + daftarDompet[indexAsal].namaDompet
            + "' ke '" + daftarDompet[indexTujuan].namaDompet + "'";
        tambahEntriLogAudit("Transfer Dompet", detailTransfer);

        cetakLoading("\n  Memproses transfer");
        cout << "\n  " << HIJAU_TERANG << TEBAL << IKON_CEK << " Transfer berhasil!" << RESET << "\n";
        cetakPembagiTipis();
        cout << "  " << MERAH   << daftarDompet[indexAsal].namaDompet << RESET
             << " => " << HIJAU << formatRupiah(daftarDompet[indexAsal].saldoDompet) << RESET << "\n";
        cout << "  " << HIJAU   << daftarDompet[indexTujuan].namaDompet << RESET
             << " => " << HIJAU << formatRupiah(daftarDompet[indexTujuan].saldoDompet) << RESET << "\n";
        jedaLayar();
        return;
    }
}

void menuWalletManagement() {
    int pilihanMenu;
    do {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  WALLET MANAGEMENT  ");
        cetakSpasi();

        // Info ringkas aset
        cout << "  " << TEBAL << "Total Aset Saat Ini: "
             << HIJAU_TERANG << formatRupiah(hitungTotalAset()) << RESET
             << "  (" << daftarDompet.size() << " dompet)\n";
        cetakSpasi();

        cetakItemMenu("1", "Tambah Dompet Baru",    HIJAU_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("2", "Edit Dompet",           KUNING_TERANG, IKON_LINGKARAN);
        cetakItemMenu("3", "Hapus Dompet",          MERAH_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("4", "Lihat Daftar Dompet",   CYAN_TERANG,   IKON_LINGKARAN);
        cetakItemMenu("5", "Transfer Antar Dompet", BIRU_TERANG,   IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali ke Menu Utama", MERAH,         IKON_PANAH);
        cetakPembagiTipis();

        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        switch (pilihanMenu) {
            case 1: tambahDompet();          break;
            case 2: editDompet();            break;
            case 3: hapusDompet();           break;
            case 4: tampilkanDaftarDompet(); break;
            case 5: transferAntarDompet();   break;
            case 0: break;
            default:
                cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
                jedaLayar();
        }
    } while (pilihanMenu != 0);
}

// ═══════════════════════════════════════════════════════════════════════
// [J]  MODUL TRANSACTION MANAGEMENT
// ═══════════════════════════════════════════════════════════════════════

// Cari dompet berdasarkan ID
Dompet* cariDompetBerdasarkanID(const string& idDompet) {
    for (auto& dompet : daftarDompet)
        if (dompet.identifikasiDompet == idDompet) return &dompet;
    return nullptr;
}

void tambahTransaksi(const string& jenisTransaksiBaru) {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  TRANSACTION MANAGEMENT  ·  Tambah " + jenisTransaksiBaru + "  ");

        if (daftarDompet.empty()) {
            cout << MERAH << "\n  Belum ada dompet. Tambahkan dompet terlebih dahulu!\n" << RESET;
            jedaLayar(); return;
        }

        cetakSpasi();
        cout << "  " << TEBAL << CYAN << "Daftar Dompet Tersedia:" << RESET << "\n";
        cetakPembagiTipis();
        for (int i = 0; i < (int)daftarDompet.size(); i++) {
            string warnaJenis = (daftarDompet[i].jenisDompet == "Cash") ? HIJAU
                              : (daftarDompet[i].jenisDompet == "E-Wallet") ? KUNING : CYAN;
            cout << "  [" << KUNING_TERANG << TEBAL << (i + 1) << RESET << "]  "
                 << left << setw(20) << potongTeks(daftarDompet[i].namaDompet, 19)
                 << warnaJenis << setw(12) << daftarDompet[i].jenisDompet << RESET
                 << HIJAU << formatRupiah(daftarDompet[i].saldoDompet) << RESET << "\n";
        }

        int indexDompetDipilih = bacaInteger("\n  Pilih nomor dompet: ") - 1;
        if (indexDompetDipilih == -1000) return;
        if (indexDompetDipilih < 0 || indexDompetDipilih >= (int)daftarDompet.size()) {
            cout << MERAH << "  Dompet tidak valid.\n" << RESET;
            jedaLayar();
            continue;
        }
        Dompet& dompetDipilih = daftarDompet[indexDompetDipilih];

        string deskripsiTransaksiBaru;
        cout << "\n  " << KUNING << "Deskripsi        : " << RESET;
        getline(cin, deskripsiTransaksiBaru);
        if (deskripsiTransaksiBaru.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        double jumlahTransaksiBaru = bacaDouble("  Jumlah (Rp)      : ");
        if (jumlahTransaksiBaru == -999) return;
        if (jumlahTransaksiBaru <= 0) {
            cout << MERAH << "  Jumlah harus lebih dari 0.\n" << RESET;
            jedaLayar();
            continue;
        }

        cetakSpasi();
        cout << "  " << KUNING << "Kategori Budget  :" << RESET << "\n";
        cetakItemMenu("1", "Needs    (Kebutuhan pokok)",   HIJAU,  IKON_LINGKARAN);
        cetakItemMenu("2", "Wants    (Keinginan)",          KUNING, IKON_LINGKARAN);
        cetakItemMenu("3", "Savings  (Tabungan/Investasi)", CYAN,   IKON_LINGKARAN);
        int pilihanKategori = bacaInteger("  Pilihan: ");
        if (pilihanKategori == -999) return;

        string kategoriTransaksiBaru;
        if (pilihanKategori == 1)      kategoriTransaksiBaru = "Needs";
        else if (pilihanKategori == 2) kategoriTransaksiBaru = "Wants";
        else if (pilihanKategori == 3) kategoriTransaksiBaru = "Savings";
        else {
            cout << MERAH << "  Kategori tidak valid.\n" << RESET;
            jedaLayar();
            continue;
        }

        if (jenisTransaksiBaru == "Expense" && dompetDipilih.saldoDompet < jumlahTransaksiBaru) {
            cout << MERAH << "\n  Saldo tidak mencukupi!\n"
                 << "  Saldo tersedia: " << formatRupiah(dompetDipilih.saldoDompet) << "\n" << RESET;
            jedaLayar();
            continue;
        }

        Transaksi transaksiObjek;
        transaksiObjek.idTransaksi         = hasilkanIDTransaksi();
        transaksiObjek.jenisTransaksi      = jenisTransaksiBaru;
        transaksiObjek.kategoriTransaksi   = kategoriTransaksiBaru;
        transaksiObjek.deskripsiTransaksi  = deskripsiTransaksiBaru;
        transaksiObjek.jumlahTransaksi     = jumlahTransaksiBaru;
        transaksiObjek.idDompetTransaksi   = dompetDipilih.identifikasiDompet;
        transaksiObjek.namaDompetTransaksi = dompetDipilih.namaDompet;
        transaksiObjek.waktuTransaksi      = dapatkanWaktuSekarang();

        string hashSebelumnya = daftarTransaksi.empty()
            ? "0000000000000000"
            : daftarTransaksi.back().hashSaatIni;
        transaksiObjek.hashSebelumnya = hashSebelumnya;
        string dataUntukHash = transaksiObjek.idTransaksi
            + transaksiObjek.jenisTransaksi
            + to_string(jumlahTransaksiBaru)
            + transaksiObjek.waktuTransaksi
            + hashSebelumnya;
        transaksiObjek.hashSaatIni = hasilkanHash(dataUntukHash);

        if (jenisTransaksiBaru == "Income") {
            dompetDipilih.saldoDompet += jumlahTransaksiBaru;
            totalPendapatan           += jumlahTransaksiBaru;
        } else {
            dompetDipilih.saldoDompet -= jumlahTransaksiBaru;
            totalPengeluaran          += jumlahTransaksiBaru;
            if (kategoriTransaksiBaru == "Needs")        pengeluaranKebutuhan += jumlahTransaksiBaru;
            else if (kategoriTransaksiBaru == "Wants")   pengeluaranKeinginan += jumlahTransaksiBaru;
            else if (kategoriTransaksiBaru == "Savings") pengeluaranTabungan  += jumlahTransaksiBaru;
        }

        daftarTransaksi.push_back(transaksiObjek);
        tambahEntriLogAudit("Tambah Transaksi",
            transaksiObjek.idTransaksi + " | " + jenisTransaksiBaru
            + " | " + formatRupiah(jumlahTransaksiBaru) + " | " + deskripsiTransaksiBaru);

        cetakLoading("\n  Memproses transaksi & validasi hash");
        cetakSpasi();
        cout << "  " << BG_HIJAU << TEBAL << PUTIH_TERANG << "  TRANSAKSI BERHASIL DICATAT  " << RESET << "\n";
        cetakPembagiTipis();
        cout << "  TXID          : " << KUNING_TERANG << TEBAL << transaksiObjek.idTransaksi << RESET << "\n";
        cout << "  Jenis         : " << (jenisTransaksiBaru == "Income" ? HIJAU_TERANG : MERAH_TERANG)
             << jenisTransaksiBaru << RESET << "\n";
        cout << "  Kategori      : " << transaksiObjek.kategoriTransaksi << "\n";
        cout << "  Jumlah        : " << TEBAL << formatRupiah(jumlahTransaksiBaru) << RESET << "\n";
        cout << "  Dompet        : " << dompetDipilih.namaDompet << "\n";
        cout << "  Saldo Dompet  : " << HIJAU << formatRupiah(dompetDipilih.saldoDompet) << RESET << "\n";
        cout << "  Waktu         : " << transaksiObjek.waktuTransaksi << "\n";
        cetakPembagiTipis();
        cout << "  Hash Sebelumnya: " << REDUP << transaksiObjek.hashSebelumnya << RESET << "\n";
        cout << "  Hash Saat Ini  : " << CYAN   << transaksiObjek.hashSaatIni  << RESET << "\n";
        cetakPembagiTipis();
        jedaLayar();
        return;
    }
}

void tampilkanRiwayatTransaksi(bool jedaSetelah = true) {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  TRANSACTION MANAGEMENT  ·  Riwayat Transaksi  ");

    if (daftarTransaksi.empty()) {
        cetakSpasi();
        cout << "  " << REDUP << MIRING << "Belum ada transaksi." << RESET << "\n";
        if (jedaSetelah) jedaLayar();
        return;
    }

    cetakSpasi();
    cout << "  " << TEBAL << CYAN
         << left
         << setw(22) << "TXID"
         << setw(10) << "Jenis"
         << setw(10) << "Kategori"
         << setw(16) << "Jumlah"
         << setw(14) << "Dompet"
         << "Waktu" << RESET << "\n";
    cetakPembagiTipis();

    for (const auto& transaksi : daftarTransaksi) {
        string warnaJenis = (transaksi.jenisTransaksi == "Income") ? HIJAU_TERANG : MERAH_TERANG;
        cout << "  " << KUNING << left << setw(22) << potongTeks(transaksi.idTransaksi, 21)
             << warnaJenis << setw(10) << transaksi.jenisTransaksi << RESET
             << CYAN << setw(10) << transaksi.kategoriTransaksi << RESET
             << setw(16) << formatRupiah(transaksi.jumlahTransaksi)
             << setw(14) << potongTeks(transaksi.namaDompetTransaksi, 13)
             << REDUP << transaksi.waktuTransaksi.substr(0, 16) << RESET << "\n";
    }

    cetakPembagiTipis();
    cout << "  " << TEBAL << "Total: " << daftarTransaksi.size() << " transaksi"
         << "  |  " << HIJAU << "Pendapatan: " << formatRupiah(totalPendapatan) << RESET
         << "  " << MERAH << "Pengeluaran: " << formatRupiah(totalPengeluaran) << RESET << "\n";
    if (jedaSetelah) jedaLayar();
}

void cariTransaksi() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  TRANSACTION MANAGEMENT  ·  Cari Transaksi  ");
        cetakSpasi();

        cout << "  " << KUNING << "Kata kunci (TXID / deskripsi / kategori): " << RESET;
        string kataKunciCari;
        getline(cin, kataKunciCari);
        if (kataKunciCari.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        bool ditemukan = false;
        cetakPembagiTipis();
        for (const auto& transaksi : daftarTransaksi) {
            if (transaksi.idTransaksi.find(kataKunciCari) != string::npos ||
                transaksi.deskripsiTransaksi.find(kataKunciCari) != string::npos ||
                transaksi.kategoriTransaksi.find(kataKunciCari) != string::npos ||
                transaksi.jenisTransaksi.find(kataKunciCari) != string::npos) {
                ditemukan = true;
                string warnaJenis = (transaksi.jenisTransaksi == "Income") ? HIJAU_TERANG : MERAH_TERANG;
                cout << "\n  " << KUNING << TEBAL << "TXID      : " << RESET << transaksi.idTransaksi << "\n";
                cout << "  Jenis     : " << warnaJenis << transaksi.jenisTransaksi << RESET << "\n";
                cout << "  Deskripsi : " << transaksi.deskripsiTransaksi << "\n";
                cout << "  Kategori  : " << transaksi.kategoriTransaksi << "\n";
                cout << "  Jumlah    : " << TEBAL << formatRupiah(transaksi.jumlahTransaksi) << RESET << "\n";
                cout << "  Dompet    : " << transaksi.namaDompetTransaksi << "\n";
                cout << "  Waktu     : " << transaksi.waktuTransaksi << "\n";
                cout << "  Hash      : " << CYAN << transaksi.hashSaatIni << RESET << "\n";
                cetakPembagiTipis();
            }
        }
        if (!ditemukan) {
            cout << "\n  " << MERAH << IKON_SILANG << " Tidak ditemukan transaksi dengan kata kunci: '"
                 << kataKunciCari << "'\n" << RESET;
        }
        jedaLayar();
        return;
    }
}

void hapusTransaksi() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  TRANSACTION MANAGEMENT  ·  Hapus Transaksi  ");

        if (daftarTransaksi.empty()) {
            cout << MERAH << "\n  Belum ada transaksi.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanRiwayatTransaksi(false);

        string txidYangDihapus;
        cout << "\n  " << KUNING << "Masukkan TXID yang akan dihapus: " << RESET;
        getline(cin, txidYangDihapus);
        if (txidYangDihapus.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        for (int i = 0; i < (int)daftarTransaksi.size(); i++) {
            if (daftarTransaksi[i].idTransaksi == txidYangDihapus) {
                Transaksi& t = daftarTransaksi[i];

                Dompet* dompetTransaksi = cariDompetBerdasarkanID(t.idDompetTransaksi);
                if (dompetTransaksi) {
                    if (t.jenisTransaksi == "Income") {
                        dompetTransaksi->saldoDompet -= t.jumlahTransaksi;
                        totalPendapatan              -= t.jumlahTransaksi;
                    } else {
                        dompetTransaksi->saldoDompet += t.jumlahTransaksi;
                        totalPengeluaran             -= t.jumlahTransaksi;
                    }
                }
                if (t.jenisTransaksi == "Expense") {
                    if (t.kategoriTransaksi == "Needs")        pengeluaranKebutuhan -= t.jumlahTransaksi;
                    else if (t.kategoriTransaksi == "Wants")   pengeluaranKeinginan -= t.jumlahTransaksi;
                    else if (t.kategoriTransaksi == "Savings") pengeluaranTabungan  -= t.jumlahTransaksi;
                }

                tambahEntriLogAudit("Hapus Transaksi", txidYangDihapus + " dihapus & saldo di-rollback");
                daftarTransaksi.erase(daftarTransaksi.begin() + i);
                cout << HIJAU_TERANG << "\n  " << IKON_CEK << " Transaksi berhasil dihapus dan saldo dirollback!\n" << RESET;
                jedaLayar(); return;
            }
        }

        cout << MERAH << "\n  " << IKON_SILANG << " TXID tidak ditemukan.\n" << RESET;
        jedaLayar();
        return;
    }
}

void menuTransactionManagement() {
    int pilihanMenu;
    do {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  TRANSACTION MANAGEMENT  ");
        cetakSpasi();

        cout << "  " << TEBAL << "Pendapatan: " << HIJAU_TERANG << formatRupiah(totalPendapatan) << RESET
             << "   Pengeluaran: " << MERAH_TERANG << formatRupiah(totalPengeluaran) << RESET
             << "   Saldo Bersih: "
             << (hitungSaldoBersih() >= 0 ? HIJAU_TERANG : MERAH_TERANG)
             << formatRupiah(hitungSaldoBersih()) << RESET << "\n";
        cetakSpasi();

        cetakItemMenu("1", "Tambah Income  (Pemasukan)",   HIJAU_TERANG,   IKON_GRAFIK);
        cetakItemMenu("2", "Tambah Expense (Pengeluaran)", MERAH_TERANG,   IKON_PANAH_BAWAH);
        cetakItemMenu("3", "Riwayat Transaksi",            CYAN_TERANG,    IKON_LINGKARAN);
        cetakItemMenu("4", "Cari Transaksi",               KUNING_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("5", "Hapus Transaksi",              MAGENTA_TERANG, IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali ke Menu Utama",        MERAH,          IKON_PANAH);
        cetakPembagiTipis();

        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        switch (pilihanMenu) {
            case 1: tambahTransaksi("Income");   break;
            case 2: tambahTransaksi("Expense");  break;
            case 3: tampilkanRiwayatTransaksi(); break;
            case 4: cariTransaksi();             break;
            case 5: hapusTransaksi();            break;
            case 0: break;
            default:
                cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
                jedaLayar();
        }
    } while (pilihanMenu != 0);
}

// ═══════════════════════════════════════════════════════════════════════
// [K]  MODUL BUDGET MANAGER
// ═══════════════════════════════════════════════════════════════════════

void tampilkanStatusBudget() {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  BUDGET MANAGER  ·  Status & Monitoring  ");
    cetakSpasi();

    double batasKebutuhan = hitungBatasKebutuhan();
    double batasKeinginan = hitungBatasKeinginan();
    double batasTabungan  = hitungBatasTabungan();

    cout << "  " << TEBAL << "Total Pendapatan : " << HIJAU_TERANG
         << formatRupiah(totalPendapatan) << RESET << "\n";
    cout << "  " << TEBAL << "Aturan Budget    : " << CYAN << "50% Needs  |  30% Wants  |  20% Savings" << RESET << "\n";
    cetakPembagiTipis();

    auto cetakBlokBudget = [](const string& labelKategori, const string& persentaseKategori,
                               double pengeluaranKategori, double batasKategori, const string& warnaLabel) {
        double persentaseTerpakai = (batasKategori > 0)
            ? min(120.0, pengeluaranKategori / batasKategori * 100.0) : 0.0;
        double sisaAnggaran = batasKategori - pengeluaranKategori;

        cout << "\n  " << warnaLabel << TEBAL << "▶ " << labelKategori
             << " (" << persentaseKategori << "  Rp " << batasKategori << ")" << RESET << "\n";
        cout << "  " << TEBAL << left << setw(20) << "  Terpakai:" << RESET
             << "  ";
        cetakProgressBarBudget(persentaseTerpakai, 10);
        cout << "\n";
        cout << "  " << "  Pengeluaran : " << formatRupiah(pengeluaranKategori)
             << "  /  Batas: " << formatRupiah(batasKategori) << "\n";
        cout << "  " << "  Sisa        : "
             << (sisaAnggaran >= 0 ? HIJAU_TERANG : MERAH_TERANG)
             << formatRupiah(sisaAnggaran) << RESET << "\n";
    };

    cetakBlokBudget("NEEDS",   "50% =", pengeluaranKebutuhan, batasKebutuhan, HIJAU_TERANG);
    cetakBlokBudget("WANTS",   "30% =", pengeluaranKeinginan, batasKeinginan, KUNING_TERANG);
    cetakBlokBudget("SAVINGS", "20% =", pengeluaranTabungan,  batasTabungan,  CYAN_TERANG);

    // ── Insight Keuangan ──────────────────────────────────────────────
    cetakSpasi();
    cout << "  " << KUNING_TERANG << TEBAL << IKON_BINTANG << " INSIGHT KEUANGAN" << RESET << "\n";
    cetakPembagiTipis();

    int skorKesehatan = 0;
    if (pengeluaranKebutuhan <= batasKebutuhan) skorKesehatan++;
    if (pengeluaranKeinginan <= batasKeinginan) skorKesehatan++;
    if (pengeluaranTabungan  <= batasTabungan)  skorKesehatan++;

    if (skorKesehatan == 3) {
        cetakBadge(" SANGAT SEHAT ", BG_HIJAU);
        cout << " Semua kategori budget dalam batas. Pertahankan!\n";
    } else if (skorKesehatan == 2) {
        cetakBadge(" KURANG SEHAT ", BG_KUNING, "\033[30m");
        cout << " 1 kategori melampaui batas. Perlu perhatian.\n";
        if (pengeluaranKebutuhan > batasKebutuhan)
            cout << "  " << MERAH << IKON_PANAH << " Pengeluaran Needs melebihi 50% pendapatan!\n" << RESET;
        if (pengeluaranKeinginan > batasKeinginan)
            cout << "  " << MERAH << IKON_PANAH << " Pengeluaran Wants melebihi 30% pendapatan!\n" << RESET;
        if (pengeluaranTabungan > batasTabungan)
            cout << "  " << MERAH << IKON_PANAH << " Pengeluaran Savings melebihi 20% pendapatan!\n" << RESET;
    } else if (skorKesehatan == 1) {
        cetakBadge(" TIDAK SEHAT  ", BG_MERAH);
        cout << " 2 kategori melampaui batas. Segera evaluasi!\n";
    } else {
        cetakBadge(" KRITIS       ", BG_MERAH);
        cout << " Semua kategori melampaui batas. Keuangan dalam bahaya!\n";
        cout << "  " << MERAH << "Saran: Kurangi pengeluaran dan tingkatkan pendapatan segera.\n" << RESET;
    }

    cetakSpasi();
    jedaLayar();
}

void tampilkanMonitoringBudget() {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  BUDGET MANAGER  ·  Monitoring per Transaksi  ");
    cetakSpasi();

    bool adaTransaksiPengeluaran = false;
    cout << "  " << TEBAL << CYAN
         << left << setw(22) << "TXID"
         << setw(10) << "Kategori"
         << setw(16) << "Jumlah"
         << "Waktu" << RESET << "\n";
    cetakPembagiTipis();

    for (const auto& transaksi : daftarTransaksi) {
        if (transaksi.jenisTransaksi == "Expense") {
            adaTransaksiPengeluaran = true;
            string warnaKat = (transaksi.kategoriTransaksi == "Needs") ? HIJAU
                            : (transaksi.kategoriTransaksi == "Wants") ? KUNING : CYAN;
            cout << "  " << KUNING << left << setw(22) << transaksi.idTransaksi
                 << warnaKat << setw(10) << transaksi.kategoriTransaksi << RESET
                 << setw(16) << formatRupiah(transaksi.jumlahTransaksi)
                 << REDUP << transaksi.waktuTransaksi.substr(0, 16) << RESET << "\n";
        }
    }

    if (!adaTransaksiPengeluaran) {
        cout << "\n  " << REDUP << MIRING << "Belum ada transaksi pengeluaran.\n" << RESET;
    }
    jedaLayar();
}

void tampilkanLaporanBudget() {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  BUDGET MANAGER  ·  Laporan Budget  ");
    cetakSpasi();

    double batasKebutuhan = hitungBatasKebutuhan();
    double batasKeinginan = hitungBatasKeinginan();
    double batasTabungan  = hitungBatasTabungan();
    double saldoBersih    = hitungSaldoBersih();

    cout << "  " << TEBAL << "Total Pendapatan : " << HIJAU << formatRupiah(totalPendapatan) << RESET << "\n";
    cout << "  " << TEBAL << "Total Pengeluaran: " << MERAH  << formatRupiah(totalPengeluaran) << RESET << "\n";
    cout << "  " << TEBAL << "Saldo Bersih     : "
         << (saldoBersih >= 0 ? HIJAU_TERANG : MERAH_TERANG) << TEBAL
         << formatRupiah(saldoBersih) << RESET << "\n";
    cetakSpasi();

    // Tabel laporan
    cout << "  " << TEBAL << CYAN
         << left
         << setw(11) << "Kategori"
         << setw(18) << "Batas Anggaran"
         << setw(18) << "Terpakai"
         << setw(18) << "Sisa"
         << "Status" << RESET << "\n";
    cetakPembagiTipis();

    auto cetakBaris = [](const string& kategori, double batas, double terpakai) {
        double sisa = batas - terpakai;
        string statusTeks, warnaStatus;
        if (terpakai <= batas * 0.6)       { statusTeks = "AMAN";    warnaStatus = HIJAU_TERANG; }
        else if (terpakai <= batas * 0.85) { statusTeks = "WASPADA"; warnaStatus = KUNING_TERANG; }
        else if (terpakai <= batas)        { statusTeks = "MENDEKATI"; warnaStatus = KUNING; }
        else                               { statusTeks = "OVER!";   warnaStatus = MERAH_TERANG; }

        cout << "  " << left
             << setw(11) << kategori
             << setw(18) << formatRupiah(batas)
             << setw(18) << formatRupiah(terpakai)
             << setw(18) << formatRupiah(sisa)
             << warnaStatus << TEBAL << statusTeks << RESET << "\n";
    };

    cetakBaris("Needs",   batasKebutuhan, pengeluaranKebutuhan);
    cetakBaris("Wants",   batasKeinginan, pengeluaranKeinginan);
    cetakBaris("Savings", batasTabungan,  pengeluaranTabungan);
    cetakPembagiTipis();
    cetakBaris("TOTAL",   totalPendapatan, totalPengeluaran);

    cetakSpasi();
    jedaLayar();
}

void menuBudgetManager() {
    int pilihanMenu;
    do {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  BUDGET MANAGER  ");

        // Preview singkat di atas menu
        cetakSpasi();
        double batasKebutuhan = hitungBatasKebutuhan();
        double batasKeinginan = hitungBatasKeinginan();
        double batasTabungan  = hitungBatasTabungan();

        auto cetakPreview = [](const string& label, double terpakai, double batas) {
            double pct = (batas > 0) ? min(120.0, terpakai / batas * 100.0) : 0.0;
            cout << "  " << left << setw(10) << label;
            cetakProgressBarBudget(pct, 8);
            cout << "  " << formatRupiah(terpakai) << " / " << formatRupiah(batas) << "\n";
        };
        cetakPreview("Needs",   pengeluaranKebutuhan, batasKebutuhan);
        cetakPreview("Wants",   pengeluaranKeinginan, batasKeinginan);
        cetakPreview("Savings", pengeluaranTabungan,  batasTabungan);
        cetakSpasi();

        cetakItemMenu("1", "Lihat Status Budget",           HIJAU_TERANG,   IKON_LINGKARAN);
        cetakItemMenu("2", "Monitoring Budget (per transaksi)", KUNING_TERANG, IKON_LINGKARAN);
        cetakItemMenu("3", "Laporan Budget Lengkap",        CYAN_TERANG,    IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali ke Menu Utama",         MERAH,          IKON_PANAH);
        cetakPembagiTipis();

        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        switch (pilihanMenu) {
            case 1: tampilkanStatusBudget();    break;
            case 2: tampilkanMonitoringBudget(); break;
            case 3: tampilkanLaporanBudget();   break;
            case 0: break;
            default:
                cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
                jedaLayar();
        }
    } while (pilihanMenu != 0);
}

// ═══════════════════════════════════════════════════════════════════════
// [L]  MODUL GOAL TRACKER
// ═══════════════════════════════════════════════════════════════════════

void tampilkanDaftarTarget(bool jedaSetelah = true) {
    bersihLayar();
    cetakStatusBar(namaAkunAktif);
    cetakHeader("  GOAL TRACKER  ·  Daftar Target Keuangan  ");
    cetakSpasi();

    if (daftarTarget.empty()) {
        cout << "  " << KUNING << IKON_PANAH << " Belum ada target. Tambahkan wishlist keuangan Anda!\n" << RESET;
        cout << "  " << REDUP << MIRING << "Contoh: Laptop, Dana Darurat, Liburan, Kendaraan" << RESET << "\n";
        if (jedaSetelah) jedaLayar();
        return;
    }

    int nomor = 1;
    for (const auto& target : daftarTarget) {
        double persentaseTarget = (target.nominalTarget > 0)
            ? min(100.0, target.nominalTerkumpul / target.nominalTarget * 100.0) : 0.0;
        double sisaTarget = target.nominalTarget - target.nominalTerkumpul;

        cout << "  " << BG_BIRU << PUTIH_TERANG << TEBAL
             << " [" << nomor++ << "] " << potongTeks(target.namaTarget, 25)
             << string(max(0, 25 - (int)target.namaTarget.size()), ' ')
             << " " << target.identifikasiTarget << " "
             << RESET << "\n";
        cout << "  " << "  Progress : ";
        cetakProgressBarOtomatis(persentaseTarget, 10);
        cout << "\n";
        cout << "  " << "  Terkumpul: " << HIJAU_TERANG << formatRupiah(target.nominalTerkumpul) << RESET
             << "  dari  " << KUNING << formatRupiah(target.nominalTarget) << RESET << "\n";
        cout << "  " << "  Sisa     : "
             << (sisaTarget <= 0 ? HIJAU_TERANG : MERAH)
             << formatRupiah(max(0.0, sisaTarget)) << RESET;
        if (persentaseTarget >= 100.0) {
            cout << "  ";
            cetakBadge("  TERCAPAI!  ", BG_HIJAU);
        }
        cout << "\n";
        cout << "  " << REDUP << "  Dibuat   : " << target.waktuDibuat.substr(0, 10) << RESET << "\n";
        cetakPembagiTipis();
    }

    if (jedaSetelah) jedaLayar();
}

void tambahTarget() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  GOAL TRACKER  ·  Tambah Target Keuangan  ");
        cetakSpasi();

        cout << "  " << KUNING << "Nama Target (contoh: Laptop, Dana Darurat, Liburan): " << RESET;
        string namaTargetBaru;
        getline(cin, namaTargetBaru);
        if (namaTargetBaru.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            continue;
        }

        double nominalTargetBaru = bacaDouble("  Target Nominal (Rp): ");
        if (nominalTargetBaru == -999) return;
        if (nominalTargetBaru <= 0) {
            cout << MERAH << "\n  Nominal harus lebih dari 0.\n" << RESET;
            jedaLayar(); return;
        }

        TargetKeuangan targetBaru;
        targetBaru.identifikasiTarget = "G" + to_string(penghitungTarget++);
        targetBaru.namaTarget         = namaTargetBaru;
        targetBaru.nominalTarget      = nominalTargetBaru;
        targetBaru.nominalTerkumpul   = 0.0;
        targetBaru.waktuDibuat        = dapatkanWaktuSekarang();
        daftarTarget.push_back(targetBaru);

        tambahEntriLogAudit("Tambah Target", "Goal '" + namaTargetBaru + "' target " + formatRupiah(nominalTargetBaru));

        cetakLoading("\n  Menyimpan target");
        cout << "\n  " << HIJAU_TERANG << TEBAL << IKON_CEK << " Target berhasil ditambahkan!\n" << RESET;
        cout << "  ID Target : " << KUNING << targetBaru.identifikasiTarget << RESET << "\n";
        jedaLayar();
        return;
    }
}

void perbaruiProgressTarget() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  GOAL TRACKER  ·  Update Progress Target  ");

        if (daftarTarget.empty()) {
            cout << MERAH << "\n  Belum ada target.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanDaftarTarget(false);
        int indexTarget = bacaInteger("\n  Nomor target yang ingin diperbarui: ") - 1;
        if (indexTarget == -1000) return;
        if (indexTarget < 0 || indexTarget >= (int)daftarTarget.size()) {
            cout << MERAH << "  Nomor tidak valid.\n" << RESET;
            jedaLayar(); return;
        }

        TargetKeuangan& targetDipilih = daftarTarget[indexTarget];
        if (daftarDompet.empty()) {
            cout << MERAH << "\n  Tidak ada dompet tersedia.\n" << RESET;
            jedaLayar();
            return;
        }
        cout << "\n  Pilih sumber dana:\n\n";
        for (int i = 0; i < (int)daftarDompet.size(); i++) {
            cout << "  ["  << i + 1 << "] " << daftarDompet[i].namaDompet << " (" << formatRupiah(daftarDompet[i].saldoDompet) << ")\n";
        }

        int indexDompet = bacaInteger("\n  Ambil dana dari dompet: ") - 1;
        if (indexDompet == -1000) return;
        if (indexDompet < 0 || indexDompet >= (int)daftarDompet.size()) {
            cout << MERAH << "  Dompet tidak valid.\n" << RESET;
            jedaLayar();
            return;
        }

        Dompet& dompet = daftarDompet[indexDompet];
        double tambahanDana = bacaDouble("  Tambah dana (Rp): ");
        if (tambahanDana == -999) return;
        if (tambahanDana <= 0) {
            cout << MERAH << "  Jumlah harus lebih dari 0.\n" << RESET;
            jedaLayar();
            return;
        }

        if (tambahanDana > dompet.saldoDompet) {
            cout << MERAH << "  Saldo dompet tidak cukup.\n" << RESET;
            jedaLayar();
            return;
        }
        double sisaTarget = targetDipilih.nominalTarget - targetDipilih.nominalTerkumpul;
        if (sisaTarget <= 0) {
            cout << KUNING << "  Target sudah tercapai.\n" << RESET;
            jedaLayar();
            return;
        }
        if (tambahanDana > sisaTarget) {
            cout << KUNING << "\n  Nominal melebihi target.\n"
                           << "  Disesuaikan menjadi " << formatRupiah(sisaTarget) << RESET << "\n";
            tambahanDana =  sisaTarget;
        }
        dompet.saldoDompet -= tambahanDana;
        targetDipilih.nominalTerkumpul += tambahanDana;
        double persentaseBaru = targetDipilih.nominalTarget > 0 ? min(100.0,(targetDipilih.nominalTerkumpul/targetDipilih.nominalTarget) * 100):0;

        Transaksi tx;
        tx.idTransaksi = hasilkanIDTransaksi();
        tx.jenisTransaksi = "Expense";
        tx.kategoriTransaksi = "Savings";
        tx.jumlahTransaksi = tambahanDana;
        tx.namaDompetTransaksi = dompet.namaDompet;
        tx.idDompetTransaksi = dompet.identifikasiDompet;
        tx.deskripsiTransaksi = "Goal: " + targetDipilih.namaTarget;
        tx.waktuTransaksi = dapatkanWaktuSekarang();
        daftarTransaksi.push_back(tx); 

        tambahEntriLogAudit("Update Target", "Goal '" + targetDipilih.namaTarget + "' +Rp" + to_string((long long)tambahanDana));
        cout << "\n  " << HIJAU_TERANG << TEBAL << IKON_CEK << " Progress berhasil diperbarui!\n\n" << RESET;
        cout << "  " << TEBAL << targetDipilih.namaTarget << RESET << "\n";
        cout << "  ";
        cetakProgressBarOtomatis(persentaseBaru, 10);
        cout << "\n";
        cout << "  Terkumpul: " << HIJAU_TERANG << formatRupiah(targetDipilih.nominalTerkumpul) << RESET
             << "  dari  " << formatRupiah(targetDipilih.nominalTarget) << "\n";
        cout << KUNING  << "\n  Saldo tersisa: " << formatRupiah(dompet.saldoDompet) << RESET << "\n";

        if (persentaseBaru >= 100.0) {
            cetakSpasi();
            cetakBadge(" SELAMAT! TARGET TERCAPAI! ", BG_HIJAU);
            cout << "\n";
        }
        jedaLayar();
        return;
    }
}

void hapusTarget() {
    while (true) {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  GOAL TRACKER  ·  Hapus Target  ");

        if (daftarTarget.empty()) {
            cout << MERAH << "\n  Belum ada target.\n" << RESET;
            jedaLayar(); return;
        }

        tampilkanDaftarTarget(false);
        int indexTarget = bacaInteger("\n  Nomor target yang ingin dihapus: ") - 1;
        if (indexTarget == -1000) return;
        if (indexTarget < 0 || indexTarget >= (int)daftarTarget.size()) {
            cout << MERAH << "  Nomor tidak valid.\n" << RESET;
            jedaLayar(); return;
        }

        cout << MERAH << "\n  Yakin hapus target '" << daftarTarget[indexTarget].namaTarget << "'? (ya/tidak): " << RESET;
        string konfirmasiHapus;
        getline(cin, konfirmasiHapus);
        if (konfirmasiHapus.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            return;
        }

        if (konfirmasiHapus == "ya" || konfirmasiHapus == "YA") {
            TargetKeuangan& target = daftarTarget[indexTarget];
            double danaTarget = target.nominalTerkumpul;
            if (danaTarget > 0) {
                cout << "\n  Dana terkumpul: " << formatRupiah(danaTarget) << "\n" << "  Kembalikan ke dompet? (ya/tidak): ";
                string pilihan;
                getline(cin, pilihan);
                if (pilihan.empty()) {
                    cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
                    jedaLayar();
                    return;
                } else if (pilihan == "ya" || pilihan == "YA") {
                    cout << "\n  Pilih dompet:\n";
                    for (int i = 0; i < (int)daftarDompet.size(); i++) {
                    cout << "  ["  << i + 1 << "] " << daftarDompet[i].namaDompet << "\n";}
                    int indexDompet = bacaInteger("\n  Nomor dompet: ") - 1;
                    if (indexDompet >= 0 && indexDompet < (int)daftarDompet.size()) {
                        daftarDompet[indexDompet].saldoDompet += danaTarget;
                        cout << HIJAU_TERANG << "\n  Dana berhasil dikembalikan.\n" << RESET;
                    } else {
                        cout << MERAH << "\n  Nomor dompet tidak valid. Dana tidak dikembalikan.\n" << RESET;
                    }
                }
            }
            tambahEntriLogAudit("Hapus Target", "Goal '" + target.namaTarget + "' dihapus");
            daftarTarget.erase(daftarTarget.begin() + indexTarget);
            cout << HIJAU_TERANG << "\n  " << IKON_CEK << " Target berhasil dihapus!\n" << RESET;
        }
        else {
            cout << KUNING << "\n  Penghapusan dibatalkan.\n" << RESET;
        }
        jedaLayar();
        return;
    }
}

void menuGoalTracker() {
    int pilihanMenu;

    auto cetakHalamanGoalTracker = []() {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  GOAL TRACKER  ");
        cetakSpasi();

        // Preview ringkas goal
        if (!daftarTarget.empty()) {
            int tercapai = 0;
            for (const auto& t : daftarTarget)
                if (t.nominalTerkumpul >= t.nominalTarget) tercapai++;
            cout << "  " << TEBAL << daftarTarget.size() << " target aktif"
                 << "  |  " << HIJAU << tercapai << " tercapai" << RESET << "\n";
            cout << "  Progress rata-rata: ";
            cetakProgressBarOtomatis(hitungRataRataProgressTarget(), 10);
            cout << "\n";
            cetakSpasi();
        }

        cetakItemMenu("1", "Tambah Target Keuangan",    HIJAU_TERANG,   IKON_LINGKARAN);
        cetakItemMenu("2", "Update Progress Target",    KUNING_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("3", "Lihat Daftar Target",       CYAN_TERANG,    IKON_LINGKARAN);
        cetakItemMenu("4", "Hapus Target",              MERAH_TERANG,   IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali ke Menu Utama",     MERAH,          IKON_PANAH);
        cetakPembagiTipis();
    };

    do {
        cetakHalamanGoalTracker();

        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        switch (pilihanMenu) {
            case 1: tambahTarget();            break;
            case 2: perbaruiProgressTarget();  break;
            case 3: tampilkanDaftarTarget();   break;
            case 4: hapusTarget();             break;
            case 0: break;
            default:
                cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
                jedaLayar();
        }
    } while (pilihanMenu != 0);
}

// ═══════════════════════════════════════════════════════════════════════
// [M]  MODUL AUDIT LOG (Menu)
// ═══════════════════════════════════════════════════════════════════════

void cetakTabelLogAudit(const vector<EntriLogAudit>& daftarEntri) {
    if (daftarEntri.empty()) {
        cout << "\n  " << REDUP << MIRING << "Belum ada aktivitas tercatat.\n" << RESET;
        return;
    }
    cout << "\n  " << TEBAL << CYAN
         << left
         << setw(20) << "Waktu"
         << setw(16) << "User"
         << setw(24) << "Aktivitas"
         << "Detail" << RESET << "\n";
    string garis(76, '-');
    cout << "  " << CYAN << garis << RESET << "\n";
    for (const auto& entri : daftarEntri) {
        string warnaAktivitas;
        if (entri.jenisAktivitas == "Login" || entri.jenisAktivitas == "Register")
            warnaAktivitas = HIJAU;
        else if (entri.jenisAktivitas == "Logout" || entri.jenisAktivitas == "Block")
            warnaAktivitas = MERAH;
        else if (entri.jenisAktivitas.find("Hapus") != string::npos)
            warnaAktivitas = MERAH_TERANG;
        else if (entri.jenisAktivitas.find("Tambah") != string::npos)
            warnaAktivitas = HIJAU_TERANG;
        else
            warnaAktivitas = KUNING;

        cout << "  " << left
             << REDUP << setw(20) << potongTeks(entri.waktuAktivitas, 19) << RESET
             << CYAN  << setw(16) << potongTeks(entri.namaAkunPengguna, 15) << RESET
             << warnaAktivitas << setw(24) << potongTeks(entri.jenisAktivitas, 23) << RESET
             << potongTeks(entri.detailAktivitas, 30) << "\n";
    }
    cout << "  " << REDUP << "Total: " << daftarEntri.size() << " entri" << RESET << "\n";
}

void menuAuditLog() {
    int pilihanMenu;

    auto cetakHalamanAuditLog = []() {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);
        cetakHeader("  AUDIT LOG  ·  Riwayat Aktivitas  ");
        cetakSpasi();

        cout << "  " << TEBAL << "Total entri log: " << KUNING_TERANG << daftarLogAudit.size() << RESET << "\n";
        cetakSpasi();

        cetakItemMenu("1", "Lihat Semua Aktivitas",       CYAN_TERANG,    IKON_LINGKARAN);
        cetakItemMenu("2", "Cari Aktivitas",              KUNING_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("3", "Riwayat Login & Logout",      HIJAU_TERANG,   IKON_LINGKARAN);
        cetakItemMenu("4", "Riwayat Perubahan Data",      BIRU_TERANG,    IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali ke Menu Utama",       MERAH,          IKON_PANAH);
        cetakPembagiTipis();
    };

    do {
        cetakHalamanAuditLog();

        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        if (pilihanMenu == 1) {
            bersihLayar();
            cetakStatusBar(namaAkunAktif);
            cetakHeader("  AUDIT LOG  ·  Semua Aktivitas  ");
            cetakTabelLogAudit(daftarLogAudit);
            jedaLayar();
        }
        else if (pilihanMenu == 2) {
            bersihLayar();
            cetakStatusBar(namaAkunAktif);
            cetakHeader("  AUDIT LOG  ·  Cari Aktivitas  ");
            cetakSpasi();
            cout << "  " << KUNING << "Kata kunci (aktivitas / detail / user): " << RESET;
            string kataKunciLog;
            getline(cin, kataKunciLog);
            if (kataKunciLog.empty()) {
                cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
                jedaLayar();
                continue;
            }
            vector<EntriLogAudit> hasilFilter;
            for (const auto& entri : daftarLogAudit) {
                if (entri.jenisAktivitas.find(kataKunciLog) != string::npos ||
                    entri.detailAktivitas.find(kataKunciLog) != string::npos ||
                    entri.namaAkunPengguna.find(kataKunciLog) != string::npos)
                    hasilFilter.push_back(entri);
            }
            cetakTabelLogAudit(hasilFilter);
            if (hasilFilter.empty())
                cout << "\n  " << MERAH << IKON_SILANG << " Tidak ada hasil untuk: '" << kataKunciLog << "'\n" << RESET;
            jedaLayar();
        }
        else if (pilihanMenu == 3) {
            bersihLayar();
            cetakStatusBar(namaAkunAktif);
            cetakHeader("  AUDIT LOG  ·  Riwayat Login & Logout  ");
            vector<EntriLogAudit> hasilFilter;
            for (const auto& entri : daftarLogAudit)
                if (entri.jenisAktivitas == "Login" || entri.jenisAktivitas == "Logout"
                    || entri.jenisAktivitas == "Register" || entri.jenisAktivitas == "Block")
                    hasilFilter.push_back(entri);
            cetakTabelLogAudit(hasilFilter);
            jedaLayar();
        }
        else if (pilihanMenu == 4) {
            bersihLayar();
            cetakStatusBar(namaAkunAktif);
            cetakHeader("  AUDIT LOG  ·  Riwayat Perubahan Data  ");
            vector<EntriLogAudit> hasilFilter;
            for (const auto& entri : daftarLogAudit)
                if (entri.jenisAktivitas != "Login" && entri.jenisAktivitas != "Logout"
                    && entri.jenisAktivitas != "Register" && entri.jenisAktivitas != "Block")
                    hasilFilter.push_back(entri);
            cetakTabelLogAudit(hasilFilter);
            jedaLayar();
        }
        else if (pilihanMenu != 0) {
            cout << MERAH << "\n  Pilihan tidak valid.\n" << RESET;
            jedaLayar();
        }
    } while (pilihanMenu != 0);
}

// ═══════════════════════════════════════════════════════════════════════
// [N]  FINANCIAL MANAGEMENT SYSTEM  —  Menu Utama Setelah Login
// ═══════════════════════════════════════════════════════════════════════

void resetDataSesi() {
    daftarDompet.clear();
    daftarTransaksi.clear();
    daftarTarget.clear();
    // daftarLogAudit TIDAK direset — audit bersifat persisten lintas sesi
    totalPendapatan      = 0.0;
    totalPengeluaran     = 0.0;
    pengeluaranKebutuhan = 0.0;
    pengeluaranKeinginan = 0.0;
    pengeluaranTabungan  = 0.0;
    penghitungDompet     = 1;
    penghitungTarget     = 1;
}

// Mengembalikan true bila user memilih Kembali (logout ke menu autentikasi)
bool menuFinancialManagementSystem() {
    int pilihanMenu;

    auto cetakHalamanMenuUtama = []() {
        bersihLayar();
        cetakStatusBar(namaAkunAktif);

        // Header menu utama
        cetakHeader("  FINCHAIN SECURE LEDGER v2.0 - MENU UTAMA  ");
        cout << "  " << REDUP << "Financial Management System  |  User: " << RESET
             << KUNING_TERANG << potongTeks(namaAkunAktif, 30) << RESET << "\n";

        // Status cepat
        cetakSpasi();
        cout << "  " << REDUP << "Aset Total  : " << RESET << HIJAU_TERANG << TEBAL
             << formatRupiah(hitungTotalAset()) << RESET;
        cout << "   " << REDUP << "Saldo Bersih: " << RESET
             << (hitungSaldoBersih() >= 0 ? HIJAU_TERANG : MERAH_TERANG) << TEBAL
             << formatRupiah(hitungSaldoBersih()) << RESET;
        cout << "   " << REDUP << "Dompet: " << RESET << KUNING << daftarDompet.size() << RESET
             << "   " << REDUP << "Transaksi: " << RESET << KUNING << daftarTransaksi.size() << RESET << "\n";
        cetakSpasi();

        cetakPembagiTipis();
        cetakItemMenu("1", "Personal Dashboard",       HIJAU_TERANG,   IKON_LINGKARAN);
        cetakItemMenu("2", "Wallet Management",        KUNING_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("3", "Transaction Management",   CYAN_TERANG,    IKON_LINGKARAN);
        cetakItemMenu("4", "Budget Manager",           BIRU_TERANG,    IKON_LINGKARAN);
        cetakItemMenu("5", "Goal Tracker",             MAGENTA_TERANG, IKON_LINGKARAN);
        cetakItemMenu("6", "Audit Log",                PUTIH_TERANG,   IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("0", "Kembali (Logout)",         MERAH_TERANG,   IKON_PANAH);
        cetakPembagiTipis();
    };

    do {
        cetakHalamanMenuUtama();
        pilihanMenu = bacaInteger("  Pilihan: ");
        if (pilihanMenu == -999) continue;

        switch (pilihanMenu) {
            case 1: tampilkanPersonalDashboard();       break;
            case 2: menuWalletManagement();             break;
            case 3: menuTransactionManagement();        break;
            case 4: menuBudgetManager();                break;
            case 5: menuGoalTracker();                  break;
            case 6: menuAuditLog();                     break;
            case 0: {
                tambahEntriLogAudit("Logout", "User " + namaAkunAktif + " logout");
                cout << "\n  " << KUNING_TERANG << "  Sampai jumpa, " << TEBAL
                    << namaAkunAktif << RESET << KUNING_TERANG << "! Sesi diakhiri.\n" << RESET;

                cout << "\n  Reset data sesi ini? (ya/tidak): ";
                string pilihanReset;
                getline(cin, pilihanReset);

                if (pilihanReset.empty()) {
                    cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
                    jedaLayar();
                    break; // batal logout → balik ke menu utama
                }

                if (pilihanReset != "ya" && pilihanReset != "YA" &&
                    pilihanReset != "tidak" && pilihanReset != "TIDAK") {
                    cout << MERAH << "\n  Jawaban tidak valid, ketik 'ya' atau 'tidak'.\n" << RESET;
                    jedaLayar();
                    break; // batal logout → balik ke menu utama
                }

                if (pilihanReset == "ya" || pilihanReset == "YA") {
                    resetDataSesi();
                    cout << KUNING << "  Data sesi berhasil direset.\n" << RESET;
                }
                jedaLayar();
                return true; // kembali ke autentikasi
            }
            default:
                cout << MERAH << "\n  Pilihan tidak valid. Coba lagi.\n" << RESET;
                jedaLayar();
        }
    } while (true);
}

// ═══════════════════════════════════════════════════════════════════════
// [O]  AUTHENTICATION SYSTEM
// ═══════════════════════════════════════════════════════════════════════
bool prosesRegistrasi() {
    while (true) {
        bersihLayar();
        cetakStatusBar();
        cetakHeader("  AUTHENTICATION  ·  Sign Up — Daftar Akun Baru  ");
        cetakSpasi();

        string namaAkunBaru, kataKunciBaru, konfirmasiKataKunci;

        cout << "  " << KUNING << TEBAL << "Username     : " << RESET;
        getline(cin, namaAkunBaru);
        if (namaAkunBaru.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            continue;
        }

        if (namaAkunBaru.size() < 3) {
            cout << MERAH << "\n  " << IKON_SILANG << " Username minimal 3 karakter!\n" << RESET;
            jedaLayar(); return false;
        }
        // Cek duplikasi
        for (const auto& pengguna : daftarSemuaPengguna) {
            if (pengguna.namaAkun == namaAkunBaru) {
                cout << MERAH << "\n  " << IKON_SILANG << " Username '" << namaAkunBaru << "' sudah terdaftar!\n" << RESET;
                jedaLayar(); return false;
            }
        }

        cout << "  " << KUNING << TEBAL << "Password     : " << RESET;
        getline(cin, kataKunciBaru);
        if (kataKunciBaru.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            continue;
        }

        cout << "  " << KUNING << TEBAL << "Konfirmasi   : " << RESET;
        getline(cin, konfirmasiKataKunci);
        if (konfirmasiKataKunci.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            continue;
        }

        if ((int)kataKunciBaru.size() < 4) {
            cout << MERAH << "\n  " << IKON_SILANG << " Password minimal 4 karakter!\n" << RESET;
            jedaLayar(); return false;
        }
        if (kataKunciBaru != konfirmasiKataKunci) {
            cout << MERAH << "\n  " << IKON_SILANG << " Password tidak cocok!\n" << RESET;
            jedaLayar(); return false;
        }
        
        // Audit global untuk registrasi
        Pengguna penggunaBaru;
        penggunaBaru.namaAkun    = namaAkunBaru;
        penggunaBaru.kataKunci   = kataKunciBaru;
        penggunaBaru.waktuDaftar = dapatkanWaktuSekarang();
        daftarSemuaPengguna.push_back(penggunaBaru);

        EntriLogAudit entriRegistrasi;
        entriRegistrasi.waktuAktivitas   = dapatkanWaktuSekarang();
        entriRegistrasi.namaAkunPengguna = namaAkunBaru;
        entriRegistrasi.jenisAktivitas   = "Register";
        entriRegistrasi.detailAktivitas  = "Akun baru terdaftar pada " + dapatkanWaktuSekarang();
        daftarLogAudit.push_back(entriRegistrasi);

        cetakLoading("\n  Menyimpan akun");
        cout << "\n  " << BG_HIJAU << PUTIH_TERANG << TEBAL << " " << IKON_CEK << " Registrasi berhasil! Silakan Sign In. " << RESET << "\n";
        jedaLayar();
        return true;
    }
}

// Mengembalikan: 1 = berhasil login, 0 = username tidak ditemukan, -1 = diblokir (3x salah)
int prosesLogin() {
    while (true) {
        bersihLayar();
        cetakStatusBar();
        cetakHeader("  AUTHENTICATION  ·  Sign In — Masuk ke Akun  ");
        cetakSpasi();

        if (daftarSemuaPengguna.empty()) {
            cout << MERAH << "\n  " << IKON_SILANG << " Belum ada akun. Silakan Sign Up terlebih dahulu!\n" << RESET;
            jedaLayar(); return 0;
        }

        string namaAkunLogin;
        cout << "  " << KUNING << TEBAL << "Username  : " << RESET;
        getline(cin, namaAkunLogin);
        if (namaAkunLogin.empty()) {
            cout << MERAH << "\n  Input tidak boleh kosong. Harap masukkan data.\n" << RESET;
            jedaLayar();
            continue;
        }
        // Cari pengguna
        Pengguna* penggunaDipilih = nullptr;
        for (auto& pengguna : daftarSemuaPengguna) {
            if (pengguna.namaAkun == namaAkunLogin) { penggunaDipilih = &pengguna; break; }
        }
        if (!penggunaDipilih) {
            cout << MERAH << "\n  " << IKON_SILANG << " Username tidak ditemukan. Silakan Sign Up dulu.\n" << RESET;
            jedaLayar(); return 0;
        }

        const int BATAS_PERCOBAAN_LOGIN = 3;
        int jumlahPercobaan = 0;

        while (jumlahPercobaan < BATAS_PERCOBAAN_LOGIN) {
            cout << "  " << KUNING << TEBAL << "Password  : " << RESET;
            string kataKunciLogin;
            getline(cin, kataKunciLogin);

            if (kataKunciLogin.empty()) {
                jumlahPercobaan++;
                int sisaPercobaan = BATAS_PERCOBAAN_LOGIN - jumlahPercobaan;
                if (sisaPercobaan > 0) {
                    cout << MERAH << "\n  Input tidak boleh kosong. Sisa percobaan: "
                         << TEBAL << sisaPercobaan << RESET << "\n\n";
                }
                continue;
            }

            if (kataKunciLogin == penggunaDipilih->kataKunci) {
                namaAkunAktif = namaAkunLogin;
                tambahEntriLogAudit("Login", "Login berhasil dari sesi baru");

                cetakLoading("\n  Memverifikasi kredensial");
                cout << "\n  " << BG_HIJAU << PUTIH_TERANG << TEBAL
                     << " " << IKON_CEK << " Selamat datang, " << namaAkunLogin << "! " << RESET << "\n";
                cout << "  " << REDUP << "Terdaftar pada: " << penggunaDipilih->waktuDaftar << RESET << "\n";
                jedaLayar();
                return 1;
            } else {
                jumlahPercobaan++;
                int sisaPercobaan = BATAS_PERCOBAAN_LOGIN - jumlahPercobaan;
                if (sisaPercobaan > 0) {
                    cout << MERAH << "\n  " << IKON_SILANG << " Password salah! Sisa percobaan: "
                         << TEBAL << sisaPercobaan << RESET << "\n\n";
                }
            }
        }
        // Akun diblokir setelah 3x salah
        {
            string baris1 = IKON_SILANG + string(" AKUN DIBLOKIR! Terlalu banyak percobaan!.");
            string baris2 = "Program akan dihentikan secara paksa.";
            const int lebarIsi = 48;
            auto cetakBarisTengah = [&](const string& teks) {
                int lebarTeks = lebarTampilUTF8(teks);
                int kiri = max(0, (lebarIsi - lebarTeks) / 2);
                int kanan = max(0, lebarIsi - kiri - lebarTeks);
                cout << "║"
                    << string(kiri, ' ')
                    << teks
                    << string(kanan, ' ')
                    << "║\n";
            };
            cout << "\n"<< BG_MERAH << PUTIH_TERANG << TEBAL;
            cout << "╔" << ulangKarakter("═", lebarIsi) << "╗\n";
            cetakBarisTengah(baris1);
            cetakBarisTengah(baris2);
            cout << "╚" << ulangKarakter("═", lebarIsi) << "╝\n" << RESET;
        }

        tambahEntriLogAudit("Block", "Akun '" + namaAkunLogin + "' diblokir — 3x gagal login");
        jedaLayar();
        return -1; // diblokir
    }
}

// ═══════════════════════════════════════════════════════════════════════
// [P]  MAIN PROGRAM
// ═══════════════════════════════════════════════════════════════════════

int main() {
    // Aktifkan UTF-8 di Windows
    #ifdef _WIN32
    system("chcp 65001 > nul");
    #endif
    srand((unsigned int)time(0));

    bool keluar          = false;
    bool paksoKeluar     = false; // true jika diblokir

    while (!keluar && !paksoKeluar) {
        bersihLayar();
        cetakBannerFinchain();
        cetakSpasi();

        // Info singkat
        cout << "  " << REDUP << "Pengguna terdaftar: " << RESET
             << KUNING << daftarSemuaPengguna.size() << RESET
             << "   " << REDUP << "Tanggal: " << RESET
             << CYAN << dapatkanTanggalSekarang() << RESET << "\n";
        cetakSpasi();

        cetakItemMenu("1", "Sign Up   — Daftar Akun Baru",   HIJAU_TERANG,  IKON_LINGKARAN);
        cetakItemMenu("2", "Sign In   — Masuk ke Akun",      KUNING_TERANG, IKON_LINGKARAN);
        cetakPembagiTipis();
        cetakItemMenu("3", "Exit      — Keluar Program",      MERAH_TERANG,  IKON_PANAH);
        cetakPembagiTipis();
        cout << "  " << REDUP << MIRING
             << "Catatan: Sign In gagal 3x → akun diblokir & program dihentikan."
             << RESET << "\n";
        cetakPembagiTipis();

        int pilihanUtama = bacaInteger("  Pilihan: ");
        if (pilihanUtama == -999) continue;

        switch (pilihanUtama) {
            case 1:
                prosesRegistrasi();
                break;

            case 2: {
                int hasilLogin = prosesLogin();
                if (hasilLogin == -1) {
                    paksoKeluar = true; // diblokir, paksa keluar
                } else if (hasilLogin == 1) {
                    // Berhasil login → masuk ke Financial Management System
                    bool kembaliKeAuth = menuFinancialManagementSystem();
                    if (kembaliKeAuth) namaAkunAktif = ""; // reset sesi
                }
                // hasilLogin == 0 → kembali ke menu autentikasi (username tidak ditemukan)
                break;
            }

            case 3: {
                bersihLayar();
                cetakBannerFinchain();
                cout << "\n  " << KUNING_TERANG << TEBAL
                     << "  Terima kasih telah menggunakan FinChain Secure Ledger!\n" << RESET;
                cout << "  " << REDUP
                     << "  Semoga keuangan Anda selalu sehat dan terkelola.\n" << RESET;
                cetakSpasi();
                jedaLayar();
                keluar = true;
                break;
            }

            default:
                cout << MERAH << "\n  Pilihan tidak valid. Coba lagi.\n" << RESET;
                jedaLayar();
                // while loop otomatis bersihLayar + reprint di atas
        }
    }

    if (paksoKeluar) {
        bersihLayar();
        cout << "\n  " << BG_MERAH << PUTIH_TERANG << TEBAL
             << "  Program dihentikan: akun diblokir karena 3x salah password.  "
             << RESET << "\n\n";
        jedaLayar();
    }

    bersihLayar();
    cout << "\n  " << CYAN_TERANG << TEBAL
         << "  FinChain Secure Ledger — Program Selesai.\n" << RESET << "\n";
    return 0;
}