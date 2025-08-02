#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

struct PaketWisata {
    std::string nama;
    std::string destinasi;
    int durasiHari;
    double hargaPerOrang;
};
void tampilkanPaket(const std::vector<PaketWisata>& daftarPaket) {
    std::cout << "\n========================================" << std::endl;
    std::cout << "          DAFTAR PAKET WISATA           " << std::endl;
    std::cout << "========================================\n" << std::endl;
    for (size_t i = 0; i < daftarPaket.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << daftarPaket[i].nama << std::endl;
        std::cout << "     Destinasi  : " << daftarPaket[i].destinasi << std::endl;
        std::cout << "     Durasi     : " << daftarPaket[i].durasiHari << " Hari" << std::endl;
        std::cout << "     Harga/Org  : Rp " << std::fixed << std::setprecision(2) << daftarPaket[i].hargaPerOrang << std::endl;
        std::cout << "----------------------------------------" << std::endl;
    }
}
int main() {
    std::vector<PaketWisata> daftarPaket = {
        {"Pesona Bali", "Bali", 4, 2500000.00},
        {"Keindahan Lombok", "Lombok", 3, 2000000.00},
        {"Eksotis Raja Ampat", "Raja Ampat", 5, 7500000.00},
        {"Petualangan Jogja", "Yogyakarta", 3, 1800000.00},
        {"Surga Bawah Laut Bunaken", "Bunaken", 4, 3000000.00},
        {"Healing Gili Trawangan", "Gili Trawangan", 3, 2200000.00},
        {"Mystical Toraja", "Toraja", 5, 4500000.00},
        {"Amazing Komodo Island", "Pulau Komodo", 4, 6000000.00},
        {"Explore Bromo Tengger", "Gunung Bromo", 2, 1750000.00},
        {"Wisata Danau Toba", "Danau Toba", 3, 2300000.00},
        {"Liburan Keluarga Bandung", "Bandung", 3, 1900000.00},
        {"Cultural Trip Solo-Semarang", "Solo & Semarang", 4, 2800000.00},
        {"Dive & Snorkel Karimunjawa", "Karimunjawa", 3, 3200000.00},
        {"Tracking Rinjani Experience", "Gunung Rinjani", 6, 5000000.00},
        {"Aceh Sejarah & Budaya", "Aceh", 4, 3500000.00}
    };

    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI AGEN TRAVEL KAMI!               *" << std::endl;
    std::cout << "* Temukan petualangan impian Anda bersama kami!     *" << std::endl;
    std::cout << "*****************************************************" << std::endl;
    tampilkanPaket(daftarPaket);

    int pilihanPaket;
    int jumlahPeserta;
    char konfirmasi;
    
    do {
        std::cout << "\nMasukkan nomor paket yang ingin Anda pesan (1-" << daftarPaket.size() << "): ";
        std::cin >> pilihanPaket;
        if (std::cin.fail() || pilihanPaket < 1 || pilihanPaket > daftarPaket.size()) {
            std::cout << "Pilihan tidak valid. Mohon masukkan nomor yang benar." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    } while (pilihanPaket < 1 || pilihanPaket > daftarPaket.size());

    do {
        std::cout << "Masukkan jumlah peserta: ";
        std::cin >> jumlahPeserta;
        if (std::cin.fail() || jumlahPeserta <= 0) {
            std::cout << "Jumlah peserta harus lebih dari 0 dan berupa angka." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    } while (jumlahPeserta <= 0);

    PaketWisata paketTerpilih = daftarPaket[pilihanPaket - 1];
    double totalHarga = paketTerpilih.hargaPerOrang * jumlahPeserta;
    std::cout << "\n==========================================" << std::endl;
    std::cout << "       DETAIL PEMESANAN ANDA               " << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "  Paket Terpilih  : " << paketTerpilih.nama << std::endl;
    std::cout << "  Destinasi       : " << paketTerpilih.destinasi << std::endl;
    std::cout << "  Durasi          : " << paketTerpilih.durasiHari << " Hari" << std::endl;
    std::cout << "  Harga per Orang : Rp " << std::fixed << std::setprecision(2) << paketTerpilih.hargaPerOrang << std::endl;
    std::cout << "  Jumlah Peserta  : " << jumlahPeserta << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "  TOTAL HARGA     : Rp " << std::fixed << std::setprecision(2) << totalHarga << std::endl;
    std::cout << "==========================================" << std::endl;

    std::cout << "\nApakah Anda ingin melanjutkan pemesanan? (Y/N): ";
    std::cin >> konfirmasi;

    if (konfirmasi == 'Y' || konfirmasi == 'y') {
        std::cout << "\n🎉 **Selamat!** Pemesanan paket **'" << paketTerpilih.nama << "'** Anda telah berhasil!" << std::endl;
        std::cout << "Tim kami akan segera menghubungi Anda untuk detail pembayaran dan keberangkatan." << std::endl;
        std::cout << "Terima kasih telah memilih layanan kami! Kami tunggu petualangan Anda! 🙏" << std::endl;
    } else {
        std::cout << "\nBaik, pemesanan dibatalkan. Semoga Anda menemukan paket yang sesuai di lain waktu!" << std::endl;
        std::cout << "Sampai jumpa! 👋" << std::endl;
    }

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
