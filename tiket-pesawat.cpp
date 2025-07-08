#include <iostream>
#include <vector>
#include <string>
#include <iomanip> // Untuk setprecision, fixed
#include <limits>  // Untuk numeric_limits

// --- Struktur Data Penerbangan ---
struct Penerbangan {
    std::string kodePenerbangan;
    std::string asal;
    std::string tujuan;
    std::string tanggal;
    std::string jamBerangkat;
    double hargaTiket;
    int kursiTersedia;
};

// --- Fungsi untuk Menampilkan Jadwal Penerbangan ---
void tampilkanJadwalPenerbangan(const std::vector<Penerbangan>& daftar) {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "             JADWAL PENERBANGAN TERSEDIA                " << std::endl;
    std::cout << "========================================================\n" << std::endl;
    for (size_t i = 0; i < daftar.size(); ++i) {
        std::cout << "  " << i + 1 << ". Kode Penerbangan : " << daftar[i].kodePenerbangan << std::endl;
        std::cout << "     Rute             : " << daftar[i].asal << " -> " << daftar[i].tujuan << std::endl;
        std::cout << "     Tanggal          : " << daftar[i].tanggal << std::endl;
        std::cout << "     Jam Berangkat    : " << daftar[i].jamBerangkat << std::endl;
        std::cout << "     Harga Tiket      : Rp " << std::fixed << std::setprecision(2) << daftar[i].hargaTiket << std::endl;
        std::cout << "     Kursi Tersedia   : " << daftar[i].kursiTersedia << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
    }
}

// --- Fungsi untuk Mencetak Tiket (Simulasi) ---
void cetakTiket(const Penerbangan& infoPenerbangan, int jumlahTiket, double totalHarga, const std::string& namaPenumpang) {
    std::cout << "\n\n********************************************************" << std::endl;
    std::cout << "* ✈️ E-TIKET PESAWAT ANDA ✈️             *" << std::endl;
    std::cout << "********************************************************" << std::endl;
    std::cout << std::left << std::setw(20) << " Nama Penumpang" << ": " << namaPenumpang << std::endl;
    std::cout << std::left << std::setw(20) << " Kode Penerbangan" << ": " << infoPenerbangan.kodePenerbangan << std::endl;
    std::cout << std::left << std::setw(20) << " Rute" << ": " << infoPenerbangan.asal << " -> " << infoPenerbangan.tujuan << std::endl;
    std::cout << std::left << std::setw(20) << " Tanggal" << ": " << infoPenerbangan.tanggal << std::endl;
    std::cout << std::left << std::setw(20) << " Jam Berangkat" << ": " << infoPenerbangan.jamBerangkat << std::endl;
    std::cout << std::left << std::setw(20) << " Jumlah Tiket" << ": " << jumlahTiket << std::endl;
    std::cout << std::left << std::setw(20) << " Harga per Tiket" << ": Rp " << std::fixed << std::setprecision(2) << infoPenerbangan.hargaTiket << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << std::left << std::setw(20) << " TOTAL PEMBAYARAN" << ": Rp " << std::fixed << std::setprecision(2) << totalHarga << std::endl;
    std::cout << "********************************************************" << std::endl;
    std::cout << " Mohon tiba 2 jam sebelum keberangkatan. Terima kasih!" << std::endl;
    std::cout << "********************************************************\n" << std::endl;
}

// --- Fungsi Utama Program ---
int main() {
    // Inisialisasi daftar penerbangan (contoh 15 penerbangan)
    std::vector<Penerbangan> daftarPenerbangan = {
        {"GA101", "Jakarta", "Denpasar", "15/07/2025", "07:00 WIB", 1200000.00, 50},
        {"SJ205", "Surabaya", "Jakarta", "16/07/2025", "09:30 WIB", 950000.00, 75},
        {"ID610", "Medan", "Yogyakarta", "17/07/2025", "14:00 WIB", 1500000.00, 30},
        {"QZ320", "Bandung", "Surabaya", "15/07/2025", "11:00 WIB", 800000.00, 60},
        {"JT789", "Makassar", "Bali", "18/07/2025", "10:00 WITA", 1800000.00, 45},
        {"GA210", "Denpasar", "Surabaya", "16/07/2025", "16:00 WITA", 1100000.00, 40},
        {"SJ115", "Jakarta", "Padang", "17/07/2025", "08:45 WIB", 1300000.00, 55},
        {"ID777", "Yogyakarta", "Lombok", "18/07/2025", "13:30 WIB", 1050000.00, 65},
        {"QZ500", "Batam", "Jakarta", "19/07/2025", "10:15 WIB", 900000.00, 70},
        {"JT900", "Balikpapan", "Jakarta", "20/07/2025", "07:00 WITA", 2100000.00, 25},
        {"GA400", "Palembang", "Jakarta", "15/07/2025", "15:00 WIB", 1000000.00, 35},
        {"SJ300", "Jakarta", "Pontianak", "16/07/2025", "12:00 WIB", 1400000.00, 48},
        {"ID800", "Manado", "Jakarta", "17/07/2025", "06:30 WITA", 2500000.00, 20},
        {"QZ600", "Solo", "Bali", "18/07/2025", "09:00 WIB", 1350000.00, 52},
        {"JT123", "Ambon", "Makassar", "19/07/2025", "11:45 WIT", 1700000.00, 38}
    };

    // --- Sambutan Awal ---
    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI LAYANAN PEMESANAN TIKET PESAWAT!*" << std::endl;
    std::cout << "* Terbang nyaman ke destinasi impian Anda!          *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    // Tampilkan jadwal penerbangan
    tampilkanJadwalPenerbangan(daftarPenerbangan);

    int pilihanPenerbangan;
    int jumlahTiket;
    std::string namaPenumpang;
    char konfirmasi;

    // --- Input Pilihan Penerbangan ---
    do {
        std::cout << "\nMasukkan nomor penerbangan yang ingin Anda pesan (1-" << daftarPenerbangan.size() << "): ";
        std::cin >> pilihanPenerbangan;

        // Validasi input
        if (std::cin.fail() || pilihanPenerbangan < 1 || pilihanPenerbangan > daftarPenerbangan.size()) {
            std::cout << "Pilihan tidak valid. Mohon masukkan nomor yang benar." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (daftarPenerbangan[pilihanPenerbangan - 1].kursiTersedia == 0) {
            std::cout << "Maaf, penerbangan ini sudah penuh. Silakan pilih penerbangan lain." << std::endl;
            pilihanPenerbangan = 0; // Set ke nilai tidak valid agar loop berulang
        }
    } while (pilihanPenerbangan < 1 || pilihanPenerbangan > daftarPenerbangan.size() || daftarPenerbangan[pilihanPenerbangan - 1].kursiTersedia == 0);

    // Dapatkan detail penerbangan yang dipilih
    Penerbangan& penerbanganTerpilih = daftarPenerbangan[pilihanPenerbangan - 1]; // Gunakan referensi untuk update kursi

    // --- Input Jumlah Tiket ---
    do {
        std::cout << "Masukkan jumlah tiket yang ingin dipesan (Tersedia: " << penerbanganTerpilih.kursiTersedia << "): ";
        std::cin >> jumlahTiket;

        if (std::cin.fail() || jumlahTiket <= 0 || jumlahTiket > penerbanganTerpilih.kursiTersedia) {
            std::cout << "Jumlah tiket tidak valid. Mohon masukkan angka yang benar dan sesuai ketersediaan." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } while (jumlahTiket <= 0 || jumlahTiket > penerbanganTerpilih.kursiTersedia);

    // Membersihkan buffer sebelum input string
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // --- Input Nama Penumpang ---
    std::cout << "Masukkan Nama Lengkap Penumpang: ";
    std::getline(std::cin, namaPenumpang);

    double totalHarga = penerbanganTerpilih.hargaTiket * jumlahTiket;

    // --- Konfirmasi Pemesanan ---
    std::cout << "\n==========================================" << std::endl;
    std::cout << "       DETAIL PEMESANAN TIKET               " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "  Nama Penumpang    : " << namaPenumpang << std::endl;
    std::cout << "  Penerbangan       : " << penerbanganTerpilih.kodePenerbangan << std::endl;
    std::cout << "  Rute              : " << penerbanganTerpilih.asal << " -> " << penerbanganTerpilih.tujuan << std::endl;
    std::cout << "  Tanggal           : " << penerbanganTerpilih.tanggal << std::endl;
    std::cout << "  Jam Berangkat     : " << penerbanganTerpilih.jamBerangkat << std::endl;
    std::cout << "  Harga per Tiket   : Rp " << std::fixed << std::setprecision(2) << penerbanganTerpilih.hargaTiket << std::endl;
    std::cout << "  Jumlah Tiket      : " << jumlahTiket << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "  TOTAL PEMBAYARAN  : Rp " << std::fixed << std::setprecision(2) << totalHarga << std::endl;
    std::cout << "==========================================" << std::endl;

    std::cout << "\nApakah Anda ingin mengkonfirmasi dan mencetak tiket? (Y/N): ";
    std::cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y') {
        // Kurangi kursi yang tersedia
        penerbanganTerpilih.kursiTersedia -= jumlahTiket;

        std::cout << "\n✅ Pemesanan tiket Anda berhasil dikonfirmasi!" << std::endl;
        cetakTiket(penerbanganTerpilih, jumlahTiket, totalHarga, namaPenumpang);
        std::cout << "Jangan lupa simpan E-Tiket Anda. Selamat jalan! 🛫" << std::endl;
    } else {
        std::cout << "\nBaik, pemesanan dibatalkan. Sampai jumpa di lain kesempatan! 👋" << std::endl;
    }

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
