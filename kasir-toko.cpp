#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Untuk manipulasi output (setw, setprecision, fixed)
#include <limits>  // Untuk numeric_limits
#include <ctime>   // Untuk mendapatkan waktu saat ini

// Struktur untuk merepresentasikan sebuah produk
struct Produk {
    std::string nama;
    double harga;
    int stok;
};

// Struktur untuk item yang dibeli dalam transaksi
struct ItemPembelian {
    Produk produk; // Menyimpan detail produk
    int jumlah;    // Jumlah yang dibeli
};

// Fungsi untuk mendapatkan waktu saat ini
std::string getCurrentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Format: DD-MM-YYYY HH:MM:SS
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M:%S", &tstruct);
    return buf;
}

// Fungsi untuk menampilkan daftar produk yang tersedia
void tampilkanDaftarProduk(const std::vector<Produk>& daftarProduk) {
    std::cout << "\n===================================" << std::endl;
    std::cout << "         DAFTAR PRODUK TOKO          " << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << std::left << std::setw(5) << "No."
              << std::left << std::setw(25) << "Nama Produk"
              << std::right << std::setw(10) << "Harga"
              << std::right << std::setw(8) << "Stok" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    for (size_t i = 0; i < daftarProduk.size(); ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << "."
                  << std::left << std::setw(25) << daftarProduk[i].nama
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << daftarProduk[i].harga
                  << std::right << std::setw(8) << daftarProduk[i].stok << std::endl;
    }
    std::cout << "===================================" << std::endl;
}

// Fungsi untuk proses pembelian produk
void prosesPembelian(std::vector<Produk>& daftarProduk, std::vector<ItemPembelian>& keranjang) {
    int pilihanProduk;
    int jumlahBeli;
    bool lanjutkanBeli = true;

    while (lanjutkanBeli) {
        tampilkanDaftarProduk(daftarProduk);
        std::cout << "\nMasukkan nomor produk yang ingin anda beli (0 untuk selesai): ";
        std::cin >> pilihanProduk;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Mohon masukkan angka." << std::endl;
            continue;
        }

        if (pilihanProduk == 0) {
            lanjutkanBeli = false;
            break;
        }

        if (pilihanProduk < 1 || pilihanProduk > daftarProduk.size()) {
            std::cout << "Nomor produk tidak valid. Silakan coba lagi." << std::endl;
            continue;
        }

        // Ambil produk yang dipilih
        Produk& produkTerpilih = daftarProduk[pilihanProduk - 1];

        std::cout << "Masukkan jumlah '" << produkTerpilih.nama << "' yang ingin dibeli: ";
        std::cin >> jumlahBeli;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Mohon masukkan angka." << std::endl;
            continue;
        }

        if (jumlahBeli <= 0) {
            std::cout << "Jumlah pembelian harus lebih dari 0." << std::endl;
        } else if (jumlahBeli > produkTerpilih.stok) {
            std::cout << "Maaf, stok '" << produkTerpilih.nama << "' tidak cukup. Sisa stok: " << produkTerpilih.stok << std::endl;
        } else {
            // Tambahkan ke keranjang belanja
            bool ditemukan = false;
            for (auto& item : keranjang) {
                if (item.produk.nama == produkTerpilih.nama) {
                    item.jumlah += jumlahBeli;
                    ditemukan = true;
                    break;
                }
            }
            if (!ditemukan) {
                keranjang.push_back({produkTerpilih, jumlahBeli});
            }
            produkTerpilih.stok -= jumlahBeli; // Kurangi stok
            std::cout << "Produk berhasil ditambahkan ke keranjang." << std::endl;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Bersihkan buffer
        std::cout << "\nLanjutkan belanja? (y/n): ";
        char choice;
        std::cin >> choice;
        if (std::tolower(choice) == 'n') {
            lanjutkanBeli = false;
        }
    }
}

// Fungsi untuk menampilkan nota pembayaran
void tampilkanNota(const std::vector<ItemPembelian>& keranjang) {
    if (keranjang.empty()) {
        std::cout << "\nKeranjang belanja kosong. Tidak ada nota yang bisa dicetak." << std::endl;
        return;
    }

    double totalHarga = 0.0;
    double ppn = 0.0; // PPN 11% (contoh)
    double totalBayar = 0.0;

    std::cout << "\n\n";
    std::cout << "==================================================" << std::endl;
    std::cout << "               TOKO SEMBAKO SERBAGUNA            " << std::endl;
    std::cout << "         Jl. Super No. 123, Kota Yogyakarta          " << std::endl;
    std::cout << "              Telp: 0822-1267-9800                  " << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "Tanggal & Waktu : " << getCurrentDateTime() << std::endl;
    std::cout << "Nomor Transaksi : TRN-" << std::setw(6) << std::setfill('0') << (rand() % 900000 + 100000) << std::endl; // Nomor transaksi random
    std::cout << std::setfill(' ') << std::endl; // Reset fill character

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(4) << "No."
              << std::left << std::setw(20) << "Nama Produk"
              << std::right << std::setw(7) << "Qty"
              << std::right << std::setw(10) << "Harga"
              << std::right << std::setw(10) << "Subtotal" << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;

    for (size_t i = 0; i < keranjang.size(); ++i) {
        double subtotal = keranjang[i].produk.harga * keranjang[i].jumlah;
        totalHarga += subtotal;
        std::cout << std::left << std::setw(4) << i + 1 << "."
                  << std::left << std::setw(20) << keranjang[i].produk.nama
                  << std::right << std::setw(7) << keranjang[i].jumlah
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << keranjang[i].produk.harga
                  << std::right << std::setw(10) << std::fixed << std::setprecision(2) << subtotal << std::endl;
    }

    ppn = totalHarga * 0.11; // Contoh PPN 11%
    totalBayar = totalHarga + ppn;

    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(37) << "Total Belanja"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << totalHarga << std::endl;
    std::cout << std::left << std::setw(37) << "PPN (11%)"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << ppn << std::endl;
    std::cout << "--------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(37) << "TOTAL YANG HARUS DIBAYAR"
              << std::right << std::setw(15) << std::fixed << std::setprecision(2) << totalBayar << std::endl;
    std::cout << "==================================================" << std::endl;
    std::cout << "             TERIMA KASIH ATAS KUNJUNGAN ANDA     " << std::endl;
    std::cout << "==================================================" << std::endl;
}


int main() {
    // Inisialisasi daftar produk (contoh)
    std::vector<Produk> daftarProduk = {
        {"Sabun Mandi", 5500.00, 50},
        {"Shampo", 12000.00, 30},
        {"Pasta Gigi", 8500.00, 40},
        {"Sikat Gigi", 6000.00, 60},
        {"Mie Instan", 3000.00, 100},
        {"Beras 5kg", 60000.00, 20},
        {"Gula Pasir 1kg", 14000.00, 25},
        {"Minyak Goreng 1L", 18000.00, 35}
    };

    std::vector<ItemPembelian> keranjangBelanja;
    int pilihanMenu;

    do {
        std::cout << "\n===================================" << std::endl;
        std::cout << "         SISTEM KASIR TOKO           " << std::endl;
        std::cout << "===================================" << std::endl;
        std::cout << "1. Mulai Transaksi Baru" << std::endl;
        std::cout << "2. Lihat Daftar Produk" << std::endl;
        std::cout << "3. Keluar" << std::endl;
        std::cout << "-----------------------------------" << std::endl;
        std::cout << "Pilih opsi: ";
        std::cin >> pilihanMenu;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input tidak valid. Mohon masukkan angka." << std::endl;
            continue;
        }

        switch (pilihanMenu) {
            case 1:
                keranjangBelanja.clear(); // Bersihkan keranjang untuk transaksi baru
                prosesPembelian(daftarProduk, keranjangBelanja);
                tampilkanNota(keranjangBelanja);
                break;
            case 2:
                tampilkanDaftarProduk(daftarProduk);
                break;
            case 3:
                std::cout << "Terima kasih telah menggunakan Sistem Kasir." << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl;
                break;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Membersihkan buffer setelah setiap input pilihan

    } while (pilihanMenu != 3);

    return 0;
}
