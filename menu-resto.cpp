#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Untuk setprecision, fixed
#include <limits>  // Untuk numeric_limits

// --- Struktur Data Item Menu ---
struct MenuItem {
    std::string nama;
    double harga;
};

// --- Fungsi untuk Menampilkan Menu ---
void tampilkanMenu(const std::vector<MenuItem>& menu) {
    std::cout << "\n==========================================" << std::endl;
    std::cout << "               DAFTAR MENU               " << std::endl;
    std::cout << "==========================================\n" << std::endl;
    for (size_t i = 0; i < menu.size(); ++i) {
        std::cout << "  " << std::left << std::setw(2) << i + 1 << ". "
                  << std::left << std::setw(25) << menu[i].nama
                  << "Rp " << std::fixed << std::setprecision(2) << menu[i].harga << std::endl;
    }
    std::cout << "------------------------------------------" << std::endl;
}

// --- Fungsi Utama Program ---
int main() {
    // Inisialisasi daftar menu dengan 15 item
    std::vector<MenuItem> daftarMenu = {
        {"Nasi Goreng Spesial", 25000.00},
        {"Mie Ayam Bakso", 20000.00},
        {"Sate Ayam (10 tusuk)", 35000.00},
        {"Sop Buntut", 45000.00},
        {"Ayam Bakar Madu", 30000.00},
        {"Es Teh Manis", 5000.00},
        {"Es Jeruk", 7000.00},
        {"Gado-Gado", 22000.00},
        {"Ketoprak", 20000.00},
        {"Soto Ayam Lamongan", 28000.00},
        {"Capcay Kuah", 32000.00},
        {"Fuyunghai", 38000.00},
        {"Kopi Susu", 15000.00},
        {"Aneka Jus Buah", 18000.00},
        {"Kerupuk (per porsi)", 3000.00}
    };

    // --- Sambutan Awal ---
    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI RESTO KAMI!                     *" << std::endl;
    std::cout << "* Siap melayani selera makan Anda!                  *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    // Tampilkan menu
    tampilkanMenu(daftarMenu);

    std::vector<std::pair<int, int>> pesanan; // Menyimpan pair: {indeks_menu, jumlah_pesanan}
    int pilihanMenu;
    int jumlahPesanan;
    double totalPembayaran = 0.0;
    char tambahLagi;

    // --- Proses Pemesanan ---
    do {
        std::cout << "\nMasukkan nomor menu yang ingin Anda pesan (1-" << daftarMenu.size() << ", 0 untuk selesai): ";
        std::cin >> pilihanMenu;

        // Validasi input nomor menu
        if (std::cin.fail() || pilihanMenu < 0 || pilihanMenu > daftarMenu.size()) {
            std::cout << "Pilihan tidak valid. Mohon masukkan nomor menu yang benar." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Lanjut ke iterasi berikutnya
        }

        if (pilihanMenu == 0) {
            break; // Keluar dari loop jika selesai
        }

        // --- Input Jumlah Pesanan ---
        std::cout << "Berapa porsi/jumlah yang Anda inginkan untuk " << daftarMenu[pilihanMenu - 1].nama << "? : ";
        std::cin >> jumlahPesanan;

        // Validasi input jumlah
        if (std::cin.fail() || jumlahPesanan <= 0) {
            std::cout << "Jumlah pesanan tidak valid. Mohon masukkan angka lebih dari 0." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue; // Lanjut ke iterasi berikutnya
        }

        pesanan.push_back({pilihanMenu - 1, jumlahPesanan}); // Simpan indeks menu dan jumlahnya
        std::cout << "✅ " << jumlahPesanan << " " << daftarMenu[pilihanMenu - 1].nama << " ditambahkan ke pesanan Anda." << std::endl;

        std::cout << "Ada lagi yang ingin dipesan? (Y/N): ";
        std::cin >> tambahLagi;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Bersihkan buffer

    } while (tambahLagi == 'Y' || tambahLagi == 'y');

    // --- Tampilkan Detail Pesanan & Hitung Total ---
    std::cout << "\n==========================================" << std::endl;
    std::cout << "          RINGKASAN PESANAN ANDA          " << std::endl;
    std::cout << "==========================================" << std::endl;

    if (pesanan.empty()) {
        std::cout << "Anda belum memesan apapun. Sampai jumpa lagi! 👋" << std::endl;
    } else {
        for (const auto& item : pesanan) {
            int indeksMenu = item.first;
            int jumlah = item.second;
            double subtotal = daftarMenu[indeksMenu].harga * jumlah;
            totalPembayaran += subtotal;

            std::cout << std::left << std::setw(3) << jumlah << "x "
                      << std::left << std::setw(25) << daftarMenu[indeksMenu].nama
                      << "Rp " << std::fixed << std::setprecision(2) << subtotal << std::endl;
        }
        std::cout << "------------------------------------------" << std::endl;
        std::cout << std::left << std::setw(29) << "TOTAL PEMBAYARAN"
                  << "Rp " << std::fixed << std::setprecision(2) << totalPembayaran << std::endl;
        std::cout << "==========================================" << std::endl;

        char konfirmasiPembayaran;
        std::cout << "\nLanjutkan ke pembayaran? (Y/N): ";
        std::cin >> konfirmasiPembayaran;

        if (konfirmasiPembayaran == 'Y' || konfirmasiPembayaran == 'y') {
            std::cout << "\n💵 Pembayaran sebesar Rp " << std::fixed << std::setprecision(2) << totalPembayaran << " telah berhasil!" << std::endl;
            std::cout << "Pesanan Anda sedang disiapkan. Selamat menikmati!️" << std::endl;
        } else {
            std::cout << "\nBaik, pesanan dibatalkan. Kami harap Anda kembali lagi! 👋" << std::endl;
        }
    }

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Membersihkan buffer input
    std::cin.get();    // Menunggu user menekan enter

    return 0;
}
