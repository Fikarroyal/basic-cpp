#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <map>
#include <algorithm>
#include <ctime>
#include <sstream>

struct Transaksi {
    std::string tipe;
    std::string kategori;
    double jumlah;
    std::string deskripsi;
    std::string tanggal;
};

double saldo = 0.0;
std::vector<Transaksi> daftarTransaksi;

const int TOTAL_WIDTH = 80;

void printSeparator(char c = '=') {
    std::cout << std::string(TOTAL_WIDTH, c) << std::endl;
}

void printHeader(const std::string& title) {
    printSeparator();
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2 - title.length() / 2) << " "
              << title << std::endl;
    printSeparator();
}

std::string getInputString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, input);
    return input;
}

double getInputDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan angka.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

std::string getCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[11];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
    return buf;
}

void catatTransaksi() {
    printHeader("CATAT TRANSAKSI BARU");
    Transaksi t;

    std::cout << "Pilih Tipe Transaksi:\n";
    std::cout << "1. Pemasukan\n";
    std::cout << "2. Pengeluaran\n";
    int pilihanTipe;
    while (true) {
        std::cout << "Pilihan (1/2): ";
        std::cin >> pilihanTipe;
        if (std::cin.fail() || (pilihanTipe != 1 && pilihanTipe != 2)) {
            std::cout << "Pilihan tidak valid. Masukkan 1 atau 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            t.tipe = (pilihanTipe == 1) ? "Pemasukan" : "Pengeluaran";
            break;
        }
    }

    t.kategori = getInputString("Kategori (contoh: Gaji, Makanan, Transportasi, Hiburan, Belanja): ");
    t.jumlah = getInputDouble("Jumlah Transaksi: Rp ");
    t.deskripsi = getInputString("Deskripsi Singkat: ");
    t.tanggal = getCurrentDate();

    if (t.tipe == "Pemasukan") {
        saldo += t.jumlah;
    } else {
        saldo -= t.jumlah;
    }

    daftarTransaksi.push_back(t);
    std::cout << "\nTransaksi berhasil dicatat!\n";
    printSeparator('-');
}

void lihatSaldo() {
    printHeader("SALDO SAAT INI");
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Saldo Anda:"
              << "Rp " << std::right << std::setw(TOTAL_WIDTH / 2 - std::string("Rp ").length()) << saldo << std::endl;
    printSeparator();
}

void lihatDaftarTransaksi() {
    printHeader("DAFTAR TRANSAKSI");

    if (daftarTransaksi.empty()) {
        std::cout << std::left << std::setw(TOTAL_WIDTH) << "Belum ada transaksi yang dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(12) << "Tanggal"
                  << std::setw(12) << "Tipe"
                  << std::setw(15) << "Kategori"
                  << std::setw(15) << "Jumlah (Rp)"
                  << std::setw(26) << "Deskripsi" << std::endl;
        printSeparator('-');

        std::cout << std::fixed << std::setprecision(2);
        for (const auto& t : daftarTransaksi) {
            std::cout << std::left << std::setw(12) << t.tanggal
                      << std::setw(12) << t.tipe
                      << std::setw(15) << t.kategori
                      << std::right << std::setw(12) << t.jumlah << "    "
                      << std::left << std::setw(26) << t.deskripsi << std::endl;
        }
    }
    printSeparator();
}

void rekapTransaksi() {
    printHeader("REKAPITULASI TRANSAKSI");

    if (daftarTransaksi.empty()) {
        std::cout << std::left << std::setw(TOTAL_WIDTH) << "Belum ada transaksi untuk direkap." << std::endl;
        printSeparator();
        return;
    }

    std::map<std::string, double> pemasukanPerKategori;
    std::map<std::string, double> pengeluaranPerKategori;
    
    double totalPemasukan = 0.0;
    double totalPengeluaran = 0.0;

    for (const auto& t : daftarTransaksi) {
        if (t.tipe == "Pemasukan") {
            pemasukanPerKategori[t.kategori] += t.jumlah;
            totalPemasukan += t.jumlah;
        } else {
            pengeluaranPerKategori[t.kategori] += t.jumlah;
            totalPengeluaran += t.jumlah;
        }
    }

    std::cout << "\n--- Rekap Pemasukan per Kategori ---\n";
    printSeparator('-');
    if (pemasukanPerKategori.empty()) {
        std::cout << std::left << std::setw(TOTAL_WIDTH) << "Tidak ada pemasukan." << std::endl;
    } else {
        std::cout << std::left << std::setw(30) << "Kategori"
                  << std::right << std::setw(20) << "Jumlah (Rp)" << std::endl;
        printSeparator('-');
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& pair : pemasukanPerKategori) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::right << std::setw(20) << pair.second << std::endl;
        }
    }
    printSeparator('-');
    std::cout << std::left << std::setw(30) << "TOTAL PEMASUKAN"
              << "Rp " << std::right << std::setw(17) << totalPemasukan << std::endl;
    printSeparator();

    std::cout << "\n--- Rekap Pengeluaran per Kategori ---\n";
    printSeparator('-');
    if (pengeluaranPerKategori.empty()) {
        std::cout << std::left << std::setw(TOTAL_WIDTH) << "Tidak ada pengeluaran." << std::endl;
    } else {
        std::cout << std::left << std::setw(30) << "Kategori"
                  << std::right << std::setw(20) << "Jumlah (Rp)" << std::endl;
        printSeparator('-');
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& pair : pengeluaranPerKategori) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::right << std::setw(20) << pair.second << std::endl;
        }
    }
    printSeparator('-');
    std::cout << std::left << std::setw(30) << "TOTAL PENGELUARAN"
              << "Rp " << std::right << std::setw(17) << totalPengeluaran << std::endl;
    printSeparator();
}


int main() {
    int pilihan;
    
    printSeparator();
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2 - std::string("SISTEM PENGELOLAAN UANG").length() / 2) << " "
              << "SISTEM PENGELOLAAN UANG" << std::endl;
    printSeparator();

    do {
        std::cout << "\n--- MENU UTAMA ---\n";
        std::cout << "1. Catat Transaksi Baru\n";
        std::cout << "2. Lihat Saldo Saat Ini\n";
        std::cout << "3. Lihat Daftar Semua Transaksi\n";
        std::cout << "4. Rekapitulasi Transaksi per Kategori\n";
        std::cout << "5. Keluar\n";
        printSeparator('-');

        std::cout << "Masukkan pilihan Anda: ";
        std::cin >> pilihan;

        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan angka pilihan menu.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (pilihan) {
            case 1: catatTransaksi(); break;
            case 2: lihatSaldo(); break;
            case 3: lihatDaftarTransaksi(); break;
            case 4: rekapTransaksi(); break;
            case 5:
                printHeader("Terima kasih telah menggunakan sistem pengelolaan uang!");
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi.\n";
                break;
        }
        std::cout << "\n";
        if (pilihan != 5) {
            std::cout << "Tekan Enter untuk melanjutkan...";
            std::cin.get();
        }
        
    } while (pilihan != 5);

    return 0;
}
