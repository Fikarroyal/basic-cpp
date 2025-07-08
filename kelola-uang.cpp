#include <iostream>   // Untuk input/output dasar (cout, cin)
#include <vector>     // Untuk menyimpan daftar transaksi
#include <string>     // Untuk std::string
#include <iomanip>    // Untuk manipulasi output (setw, left, right)
#include <limits>     // Untuk std::numeric_limits (untuk clear buffer)
#include <map>        // Untuk rekapitulasi berdasarkan kategori
#include <algorithm>  // Untuk std::transform (kalau perlu untuk case-insensitivity)
#include <ctime>      // Untuk waktu dan tanggal
#include <sstream>    // Untuk stringstream

// Struktur untuk merepresentasikan satu transaksi
struct Transaksi {
    std::string tipe;       // "Pemasukan" atau "Pengeluaran"
    std::string kategori;   // Contoh: Gaji, Makanan, Transportasi, Hiburan
    double jumlah;
    std::string deskripsi;
    std::string tanggal;    // Format: DD-MM-YYYY
};

// Variabel global untuk saldo
double saldo = 0.0;
// Vector global untuk menyimpan semua transaksi
std::vector<Transaksi> daftarTransaksi;

// --- Fungsi Pembantu untuk Tampilan ---
const int TOTAL_WIDTH = 80; // Lebar total tampilan konsol

void printSeparator(char c = '=') {
    std::cout << std::string(TOTAL_WIDTH, c) << std::endl;
}

void printHeader(const std::string& title) {
    printSeparator();
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2 - title.length() / 2) << " " 
              << title << std::endl;
    printSeparator();
}

// Fungsi untuk mendapatkan input string dari pengguna
std::string getInputString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, input); 
    return input;
}

// Fungsi untuk mendapatkan input double dari pengguna
double getInputDouble(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan angka.\n";
            std::cin.clear(); // Hapus flag error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa input
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
            return value;
        }
    }
}

// Fungsi untuk mendapatkan tanggal saat ini
std::string getCurrentDate() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[11]; // DD-MM-YYYY + null terminator
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y", &tstruct);
    return buf;
}

// --- Fungsi Inti Program ---

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
    t.tanggal = getCurrentDate(); // Otomatis mengisi tanggal saat ini

    // Update saldo
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
    std::cout << std::fixed << std::setprecision(2); // Format 2 angka di belakang koma
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Saldo Anda:" 
              << "Rp " << std::right << std::setw(TOTAL_WIDTH / 2 - std::string("Rp ").length()) << saldo << std::endl;
    printSeparator();
}

void lihatDaftarTransaksi() {
    printHeader("DAFTAR TRANSAKSI");

    if (daftarTransaksi.empty()) {
        std::cout << std::left << std::setw(TOTAL_WIDTH) << "Belum ada transaksi yang dicatat." << std::endl;
    } else {
        // Header tabel
        std::cout << std::left << std::setw(12) << "Tanggal"
                  << std::setw(12) << "Tipe"
                  << std::setw(15) << "Kategori"
                  << std::setw(15) << "Jumlah (Rp)"
                  << std::setw(26) << "Deskripsi" << std::endl;
        printSeparator('-');

        // Isi tabel
        std::cout << std::fixed << std::setprecision(2);
        for (const auto& t : daftarTransaksi) {
            std::cout << std::left << std::setw(12) << t.tanggal
                      << std::setw(12) << t.tipe
                      << std::setw(15) << t.kategori
                      << std::right << std::setw(12) << t.jumlah << "   " // Spasi tambahan agar rata
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

    // Rekap Pemasukan
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

    // Rekap Pengeluaran
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


// Fungsi utama program
int main() {
    int pilihan;
    
    // Header utama aplikasi
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

        // Validasi input pilihan menu
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan angka pilihan menu.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Kembali ke awal loop
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline

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
        std::cout << "\n"; // Spasi setelah setiap aksi
        if (pilihan != 5) {
            std::cout << "Tekan Enter untuk melanjutkan...";
            std::cin.get(); // Menunggu Enter
        }
        
    } while (pilihan != 5);

    return 0;
}
