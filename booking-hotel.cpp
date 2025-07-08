#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Untuk setprecision, fixed
#include <limits>  // Untuk numeric_limits

// --- Struktur Data Kamar ---
struct KamarHotel {
    std::string tipe;
    double hargaPerMalam;
    int kapasitasMaksimal;
    int jumlahTersedia; // Jumlah kamar yang masih kosong
};

// --- Fungsi untuk Menampilkan Daftar Kamar ---
void tampilkanDaftarKamar(const std::vector<KamarHotel>& daftar) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "          DAFTAR KAMAR HOTEL           " << std::endl;
    std::cout << "========================================\n" << std::endl;
    for (size_t i = 0; i < daftar.size(); ++i) {
        std::cout << "  " << i + 1 << ". Tipe Kamar    : " << daftar[i].tipe << std::endl;
        std::cout << "     Harga/Malam : Rp " << std::fixed << std::setprecision(2) << daftar[i].hargaPerMalam << std::endl;
        std::cout << "     Kapasitas   : " << daftar[i].kapasitasMaksimal << " Orang" << std::endl;
        std::cout << "     Tersedia    : " << daftar[i].jumlahTersedia << " Kamar" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}

// --- Fungsi Utama Program ---
int main() {
    // Inisialisasi daftar kamar hotel
    std::vector<KamarHotel> daftarKamar = {
        {"Standard Room", 500000.00, 2, 10},
        {"Deluxe Room", 850000.00, 3, 7},
        {"Suite Room", 1500000.00, 4, 3},
        {"Family Suite", 2000000.00, 6, 2},
        {"Presidential Suite", 5000000.00, 4, 1},
        {"Twin Bed Room", 600000.00, 2, 8},
        {"Single Room", 400000.00, 1, 5},
        {"Connecting Room", 1200000.00, 4, 4},
        {"Studio Apartment", 1800000.00, 3, 2},
        {"Penthouse", 7000000.00, 6, 1},
        {"Executive Room", 1000000.00, 2, 6},
        {"Junior Suite", 1300000.00, 3, 3},
        {"Grand Deluxe", 950000.00, 3, 5},
        {"Bungalow", 2500000.00, 5, 2},
        {"Resort Villa", 10000000.00, 8, 1}
    };

    // --- Sambutan Awal ---
    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI SISTEM PEMESANAN HOTEL KAMI!    *" << std::endl;
    std::cout << "* Dapatkan pengalaman menginap terbaik bersama kami!*" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    // Tampilkan daftar kamar yang tersedia
    tampilkanDaftarKamar(daftarKamar);

    int pilihanKamar;
    int jumlahMalam;
    int jumlahKamarDipesan;
    char konfirmasi;

    // --- Input Pilihan Kamar ---
    do {
        std::cout << "\nMasukkan nomor tipe kamar yang ingin Anda pesan (1-" << daftarKamar.size() << "): ";
        std::cin >> pilihanKamar;

        // Validasi input non-angka atau di luar rentang
        if (std::cin.fail() || pilihanKamar < 1 || pilihanKamar > daftarKamar.size()) {
            std::cout << "Pilihan tidak valid. Mohon masukkan nomor yang benar." << std::endl;
            std::cin.clear(); // Bersihkan flag error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang input yang salah
        } else if (daftarKamar[pilihanKamar - 1].jumlahTersedia == 0) {
            std::cout << "Maaf, kamar tipe '" << daftarKamar[pilihanKamar - 1].tipe << "' sedang tidak tersedia." << std::endl;
            // Set pilihanKamar ke nilai tidak valid agar loop berulang
            pilihanKamar = 0;
        }
    } while (pilihanKamar < 1 || pilihanKamar > daftarKamar.size() || daftarKamar[pilihanKamar - 1].jumlahTersedia == 0);


    // Dapatkan detail kamar yang dipilih
    KamarHotel& kamarTerpilih = daftarKamar[pilihanKamar - 1]; // Gunakan referensi untuk update jumlahTersedia

    // --- Input Jumlah Kamar yang Dipesan ---
    do {
        std::cout << "Masukkan jumlah kamar yang ingin dipesan (Tersedia: " << kamarTerpilih.jumlahTersedia << "): ";
        std::cin >> jumlahKamarDipesan;

        // Validasi input non-angka atau di luar batas ketersediaan
        if (std::cin.fail() || jumlahKamarDipesan <= 0 || jumlahKamarDipesan > kamarTerpilih.jumlahTersedia) {
            std::cout << "Jumlah kamar tidak valid. Mohon masukkan angka yang benar dan sesuai ketersediaan." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (jumlahKamarDipesan <= 0 || jumlahKamarDipesan > kamarTerpilih.jumlahTersedia);

    // --- Input Jumlah Malam Menginap ---
    do {
        std::cout << "Masukkan berapa malam Anda akan menginap: ";
        std::cin >> jumlahMalam;

        // Validasi input non-angka atau kurang dari 1
        if (std::cin.fail() || jumlahMalam <= 0) {
            std::cout << "Jumlah malam tidak valid. Mohon masukkan angka yang benar dan minimal 1 malam." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (jumlahMalam <= 0);

    double totalBiaya = kamarTerpilih.hargaPerMalam * jumlahKamarDipesan * jumlahMalam;

    // --- Konfirmasi Pemesanan ---
    std::cout << "\n==========================================" << std::endl;
    std::cout << "       DETAIL PEMESANAN KAMAR               " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "  Tipe Kamar      : " << kamarTerpilih.tipe << std::endl;
    std::cout << "  Harga/Malam     : Rp " << std::fixed << std::setprecision(2) << kamarTerpilih.hargaPerMalam << std::endl;
    std::cout << "  Jumlah Kamar    : " << jumlahKamarDipesan << std::endl;
    std::cout << "  Jumlah Malam    : " << jumlahMalam << " Malam" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "  TOTAL BIAYA     : Rp " << std::fixed << std::setprecision(2) << totalBiaya << std::endl;
    std::cout << "==========================================" << std::endl;

    std::cout << "\nApakah Anda ingin mengkonfirmasi pemesanan ini? (Y/N): ";
    std::cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y') {
        // Update jumlah kamar tersedia setelah pemesanan
        kamarTerpilih.jumlahTersedia -= jumlahKamarDipesan;

        std::cout << "\n🏨 **Pemesanan Berhasil!** Kamar tipe **'" << kamarTerpilih.tipe << "'** Anda telah berhasil dipesan." << std::endl;
        std::cout << "Kami akan mengirimkan konfirmasi detail ke email Anda. Selamat menikmati penginapan Anda!" << std::endl;
        std::cout << "Terima kasih telah memilih hotel kami! ✨" << std::endl;
    } else {
        std::cout << "\nBaik, pemesanan dibatalkan. Sampai jumpa lagi! 👋" << std::endl;
    }

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Membersihkan buffer input
    std::cin.get();    // Menunggu user menekan enter

    return 0;
}
