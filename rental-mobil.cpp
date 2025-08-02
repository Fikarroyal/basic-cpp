#include <iostream>
#include <vector>
#include <string>
#include <iomanip> 
#include <limits>  

struct Mobil {
    std::string merk;
    std::string model;
    std::string platNomor;
    double hargaSewaPerHari;
    bool isTersedia;
};

void tampilkanDaftarMobil(const std::vector<Mobil>& daftar) {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "                 DAFTAR MOBIL RENTAL                    " << std::endl;
    std::cout << "========================================================\n" << std::endl;
    std::cout << std::left << std::setw(4) << "No."
              << std::left << std::setw(15) << "Merk & Model"
              << std::left << std::setw(15) << "Plat Nomor"
              << std::left << std::setw(15) << "Harga/Hari"
              << std::left << std::setw(10) << "Status" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    for (size_t i = 0; i < daftar.size(); ++i) {
        std::cout << std::left << std::setw(4) << i + 1 << ".";
        std::cout << std::left << std::setw(15) << (daftar[i].merk + " " + daftar[i].model);
        std::cout << std::left << std::setw(15) << daftar[i].platNomor;
        std::cout << std::left << std::setw(15) << std::fixed << std::setprecision(2) << daftar[i].hargaSewaPerHari;
        std::cout << std::left << std::setw(10) << (daftar[i].isTersedia ? "Tersedia" : "Disewa") << std::endl;
    }
    std::cout << "========================================================\n" << std::endl;
}
int main() {
    std::vector<Mobil> daftarMobil = {
        {"Toyota", "Avanza", "B 1234 CD", 300000.00, true},
        {"Daihatsu", "Xenia", "B 5678 EF", 300000.00, true},
        {"Honda", "Mobilio", "B 9012 GH", 350000.00, true},
        {"Suzuki", "Ertiga", "B 3456 IJ", 320000.00, true},
        {"Mitsubishi", "Xpander", "B 7890 KL", 380000.00, true},
        {"Nissan", "Grand Livina", "B 1122 MN", 310000.00, true},
        {"Wuling", "Confero", "B 3344 OP", 280000.00, true},
        {"Hyundai", "Creta", "B 5566 QR", 450000.00, true},
        {"Toyota", "Rush", "B 7788 ST", 400000.00, true},
        {"Daihatsu", "Terios", "B 9900 UV", 400000.00, true},
        {"Honda", "HR-V", "B 1011 WX", 500000.00, true},
        {"Mazda", "CX-5", "B 1213 YZ", 600000.00, true},
        {"Mercedes-Benz", "C-Class", "B 1415 AB", 1500000.00, true},
        {"BMW", "3 Series", "B 1617 CD", 1600000.00, true},
        {"Toyota", "Innova Reborn", "B 1819 EF", 550000.00, true}
    };

    int pilihan;
    char lanjutAplikasi;

    std::cout << "*****************************************************" << std::endl;
    std::cout << "*   SELAMAT DATANG DI LAYANAN RENTAL MOBIL KAMI!  *" << std::endl;
    std::cout << "*       Nikmati perjalanan Anda dengan kami!      *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    do {
        std::cout << "\n----- MENU UTAMA -----" << std::endl;
        std::cout << "1. Lihat Daftar Mobil" << std::endl;
        std::cout << "2. Sewa Mobil" << std::endl;
        std::cout << "3. Kembalikan Mobil" << std::endl;
        std::cout << "4. Keluar" << std::endl;
        std::cout << "Pilih opsi (1-4): ";
        std::cin >> pilihan;

        if (std::cin.fail() || pilihan < 1 || pilihan > 4) {
            std::cout << "Pilihan tidak valid. Mohon masukkan angka 1-4." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            lanjutAplikasi = 'Y';
            continue;
        }

        switch (pilihan) {
            case 1: {
                tampilkanDaftarMobil(daftarMobil);
                break;
            }
            case 2: {
                tampilkanDaftarMobil(daftarMobil);
                int pilihanMobilSewa;
                int durasiSewaHari;

                do {
                    std::cout << "\nMasukkan nomor mobil yang ingin disewa: ";
                    std::cin >> pilihanMobilSewa;
                    if (std::cin.fail() || pilihanMobilSewa < 1 || pilihanMobilSewa > daftarMobil.size()) {
                        std::cout << "Nomor mobil tidak valid. Mohon masukkan nomor yang benar." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else if (!daftarMobil[pilihanMobilSewa - 1].isTersedia) {
                        std::cout << "Maaf, mobil ini sedang tidak tersedia (sedang disewa). Pilih yang lain." << std::endl;
                        pilihanMobilSewa = 0;
                    }
                } while (pilihanMobilSewa < 1 || pilihanMobilSewa > daftarMobil.size() || !daftarMobil[pilihanMobilSewa - 1].isTersedia);

                do {
                    std::cout << "Masukkan durasi sewa (hari): ";
                    std::cin >> durasiSewaHari;
                    if (std::cin.fail() || durasiSewaHari <= 0) {
                        std::cout << "Durasi sewa tidak valid. Mohon masukkan angka lebih dari 0." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                } while (durasiSewaHari <= 0);

                Mobil& mobilDisewa = daftarMobil[pilihanMobilSewa - 1];
                double totalBiayaSewa = mobilDisewa.hargaSewaPerHari * durasiSewaHari;

                std::cout << "\n==========================================" << std::endl;
                std::cout << "         KONFIRMASI SEWA MOBIL            " << std::endl;
                std::cout << "==========================================" << std::endl;
                std::cout << "  Mobil Dipilih   : " << mobilDisewa.merk << " " << mobilDisewa.model << " (" << mobilDisewa.platNomor << ")" << std::endl;
                std::cout << "  Harga per Hari  : Rp " << std::fixed << std::setprecision(2) << mobilDisewa.hargaSewaPerHari << std::endl;
                std::cout << "  Durasi Sewa     : " << durasiSewaHari << " Hari" << std::endl;
                std::cout << "------------------------------------------" << std::endl;
                std::cout << "  TOTAL BIAYA     : Rp " << std::fixed << std::setprecision(2) << totalBiayaSewa << std::endl;
                std::cout << "==========================================" << std::endl;

                char konfirmasiSewa;
                std::cout << "\nKonfirmasi sewa mobil ini? (Y/N): ";
                std::cin >> konfirmasiSewa;
                if (konfirmasiSewa == 'Y' || konfirmasiSewa == 'y') {
                    mobilDisewa.isTersedia = false;
                    std::cout << "\n🎉 **Sewa Berhasil!** Mobil " << mobilDisewa.merk << " " << mobilDisewa.model << " (" << mobilDisewa.platNomor << ") telah berhasil Anda sewa." << std::endl;
                    std::cout << "Selamat menikmati perjalanan Anda! 🚗💨" << std::endl;
                } else {
                    std::cout << "\nPenyewaan dibatalkan. Sampai jumpa lagi! 👋" << std::endl;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 3: {
                bool adaMobilDisewa = false;
                for (const auto& m : daftarMobil) {
                    if (!m.isTersedia) {
                        adaMobilDisewa = true;
                        break;
                    }
                }

                if (!adaMobilDisewa) {
                    std::cout << "\nTidak ada mobil yang sedang disewa saat ini." << std::endl;
                    break;
                }

                std::cout << "\n==========================================" << std::endl;
                std::cout << "         DAFTAR MOBIL DIJALANKAN          " << std::endl;
                std::cout << "==========================================" << std::endl;
                int counter = 1;
                for (size_t i = 0; i < daftarMobil.size(); ++i) {
                    if (!daftarMobil[i].isTersedia) {
                        std::cout << "  " << counter++ << ". " << daftarMobil[i].merk << " " << daftarMobil[i].model
                                  << " (Plat: " << daftarMobil[i].platNomor << ")" << std::endl;
                    }
                }
                std::cout << "------------------------------------------" << std::endl;

                int pilihanMobilKembali;
                bool inputValid = false;
                do {
                    std::cout << "\nMasukkan nomor mobil yang ingin dikembalikan (berdasarkan daftar di atas): ";
                    std::cin >> pilihanMobilKembali;
                    if (std::cin.fail() || pilihanMobilKembali <= 0 || pilihanMobilKembali > daftarMobil.size()) {
                        std::cout << "Nomor mobil tidak valid. Mohon masukkan nomor yang benar." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }

                    int originalIndex = -1;
                    int currentCount = 0;
                    for (size_t i = 0; i < daftarMobil.size(); ++i) {
                        if (!daftarMobil[i].isTersedia) {
                            currentCount++;
                            if (currentCount == pilihanMobilKembali) {
                                originalIndex = i;
                                break;
                            }
                        }
                    }

                    if (originalIndex != -1) {
                         Mobil& mobilDikembalikan = daftarMobil[originalIndex];
                         if (!mobilDikembalikan.isTersedia) {
                            mobilDikembalikan.isTersedia = true;
                            std::cout << "\n👍 Mobil " << mobilDikembalikan.merk << " " << mobilDikembalikan.model << " (" << mobilDikembalikan.platNomor << ") telah berhasil dikembalikan." << std::endl;
                            std::cout << "Terima kasih telah menggunakan layanan kami! 😊" << std::endl;
                            inputValid = true;
                         } else {
                            std::cout << "Kesalahan: Mobil ini tidak dalam status disewa." << std::endl;
                         }
                    } else {
                        std::cout << "Nomor mobil tidak ditemukan dalam daftar mobil yang sedang disewa." << std::endl;
                    }
                } while (!inputValid);
                 std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 4: {
                std::cout << "\nTerima kasih telah menggunakan layanan Rental Mobil kami. Sampai jumpa! 👋" << std::endl;
                break;
            }
        }

        if (pilihan != 4) {
            std::cout << "\nKembali ke Menu Utama? (Y/N): ";
            std::cin >> lanjutAplikasi;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            lanjutAplikasi = 'N';
        }
    } while (lanjutAplikasi == 'Y' || lanjutAplikasi == 'y');

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
