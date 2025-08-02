#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

struct Buku {
    std::string judul;
    std::string penulis;
    std::string isbn;
    bool isTersedia;
};

void tampilkanDaftarBuku(const std::vector<Buku>& daftar) {
    std::cout << "\n========================================================" << std::endl;
    std::cout << "               DAFTAR BUKU PERPUSTAKAAN              " << std::endl;
    std::cout << "========================================================\n" << std::endl;
    std::cout << std::left << std::setw(4) << "No."
              << std::left << std::setw(25) << "Judul Buku"
              << std::left << std::setw(20) << "Penulis"
              << std::left << std::setw(15) << "Status" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    for (size_t i = 0; i < daftar.size(); ++i) {
        std::cout << std::left << std::setw(4) << i + 1 << ".";
        std::cout << std::left << std::setw(25) << daftar[i].judul;
        std::cout << std::left << std::setw(20) << daftar[i].penulis;
        std::cout << std::left << std::setw(15) << (daftar[i].isTersedia ? "Tersedia" : "Dipinjam") << std::endl;
    }
    std::cout << "========================================================\n" << std::endl;
}

int main() {
    std::vector<Buku> daftarBuku = {
        {"Filosofi Teras", "Henry Manampiring", "978-602-03-3453-3", true},
        {"Atomic Habits", "James Clear", "978-602-06-3312-3", true},
        {"Rich Dad Poor Dad", "Robert T. Kiyosaki", "978-979-433-475-4", true},
        {"Bumi Manusia", "Pramoedya Ananta Toer", "978-979-910-128-2", true},
        {"Laskar Pelangi", "Andrea Hirata", "978-979-122-777-6", true},
        {"Pulang", "Tere Liye", "978-602-03-2474-9", true},
        {"Laut Bercerita", "Leila S. Chudori", "978-602-06-1216-6", true},
        {"Sapiens: A Brief History", "Yuval Noah Harari", "978-602-03-3112-9", true},
        {"The Alchemist", "Paulo Coelho", "978-979-22-2608-2", true},
        {"Sebuah Seni untuk Bersikap Bodo Amat", "Mark Manson", "978-602-06-1906-8", true},
        {"Negeri 5 Menara", "A. Fuadi", "978-979-114-126-5", true},
        {"Dilan 1990", "Pidi Baiq", "978-602-861-125-1", true},
        {"Cantik Itu Luka", "Eka Kurniawan", "978-602-03-3084-9", true},
        {"Anak Semua Bangsa", "Pramoedya Ananta Toer", "978-979-910-141-1", true},
        {"Merayakan Kehilangan", "Brian Khrisna", "978-602-03-7521-5", true}
    };

    int pilihan;
    char lanjutAplikasi;

    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI PERPUSTAKAAN DIGITAL KAMI!    *" << std::endl;
    std::cout << "* Baca, Belajar, Berkembang Bersama Kami!       *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    do {
        std::cout << "\n--- MENU UTAMA PERPUSTAKAAN ---" << std::endl;
        std::cout << "1. Lihat Daftar Buku" << std::endl;
        std::cout << "2. Pinjam Buku" << std::endl;
        std::cout << "3. Kembalikan Buku" << std::endl;
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
                tampilkanDaftarBuku(daftarBuku);
                break;
            }
            case 2: {
                tampilkanDaftarBuku(daftarBuku);
                int pilihanBukuPinjam;

                do {
                    std::cout << "\nMasukkan nomor buku yang ingin dipinjam: ";
                    std::cin >> pilihanBukuPinjam;
                    if (std::cin.fail() || pilihanBukuPinjam < 1 || pilihanBukuPinjam > daftarBuku.size()) {
                        std::cout << "Nomor buku tidak valid. Mohon masukkan nomor yang benar." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    } else if (!daftarBuku[pilihanBukuPinjam - 1].isTersedia) {
                        std::cout << "Maaf, buku '" << daftarBuku[pilihanBukuPinjam - 1].judul << "' sedang tidak tersedia (sedang dipinjam)." << std::endl;
                        pilihanBukuPinjam = 0;
                    }
                } while (pilihanBukuPinjam < 1 || pilihanBukuPinjam > daftarBuku.size() || !daftarBuku[pilihanBukuPinjam - 1].isTersedia);

                Buku& bukuDipinjam = daftarBuku[pilihanBukuPinjam - 1];

                std::cout << "\n==========================================" << std::endl;
                std::cout << "          KONFIRMASI PEMINJAMAN BUKU        " << std::endl;
                std::cout << "==========================================" << std::endl;
                std::cout << "  Judul Buku    : " << bukuDipinjam.judul << std::endl;
                std::cout << "  Penulis       : " << bukuDipinjam.penulis << std::endl;
                std::cout << "  ISBN          : " << bukuDipinjam.isbn << std::endl;
                std::cout << "------------------------------------------" << std::endl;
                std::cout << "  Status        : Tersedia (akan dipinjam)" << std::endl;
                std::cout << "==========================================" << std::endl;

                char konfirmasiPinjam;
                std::cout << "\nKonfirmasi peminjaman buku ini? (Y/N): ";
                std::cin >> konfirmasiPinjam;
                if (konfirmasiPinjam == 'Y' || konfirmasiPinjam == 'y') {
                    bukuDipinjam.isTersedia = false;
                    std::cout << "\n📚 **Peminjaman Berhasil!** Buku '" << bukuDipinjam.judul << "' telah berhasil Anda pinjam." << std::endl;
                    std::cout << "Selamat membaca! Jangan lupa dikembalikan tepat waktu ya! 😊" << std::endl;
                } else {
                    std::cout << "\nPeminjaman dibatalkan. Sampai jumpa lagi! 👋" << std::endl;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 3: {
                bool adaBukuDipinjam = false;
                for (const auto& b : daftarBuku) {
                    if (!b.isTersedia) {
                        adaBukuDipinjam = true;
                        break;
                    }
                }

                if (!adaBukuDipinjam) {
                    std::cout << "\nTidak ada buku yang sedang dipinjam saat ini." << std::endl;
                    break;
                }

                std::cout << "\n==========================================" << std::endl;
                std::cout << "          DAFTAR BUKU SEDANG DIPINJAM      " << std::endl;
                std::cout << "==========================================" << std::endl;
                int counter = 1;
                for (size_t i = 0; i < daftarBuku.size(); ++i) {
                    if (!daftarBuku[i].isTersedia) {
                        std::cout << "  " << counter++ << ". " << daftarBuku[i].judul
                                  << " (Penulis: " << daftarBuku[i].penulis << ")" << std::endl;
                    }
                }
                std::cout << "------------------------------------------" << std::endl;

                int pilihanBukuKembali;
                bool inputValid = false;
                do {
                    std::cout << "\nMasukkan nomor buku yang ingin dikembalikan (berdasarkan daftar di atas): ";
                    std::cin >> pilihanBukuKembali;
                    if (std::cin.fail() || pilihanBukuKembali <= 0 || pilihanBukuKembali > daftarBuku.size()) {
                        std::cout << "Nomor buku tidak valid. Mohon masukkan nomor yang benar." << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        continue;
                    }

                    int originalIndex = -1;
                    int currentCount = 0;
                    for (size_t i = 0; i < daftarBuku.size(); ++i) {
                        if (!daftarBuku[i].isTersedia) {
                            currentCount++;
                            if (currentCount == pilihanBukuKembali) {
                                originalIndex = i;
                                break;
                            }
                        }
                    }

                    if (originalIndex != -1) {
                        Buku& bukuDikembalikan = daftarBuku[originalIndex];
                        if (!bukuDikembalikan.isTersedia) {
                            bukuDikembalikan.isTersedia = true;
                            std::cout << "\n👍 Buku '" << bukuDikembalikan.judul << "' telah berhasil dikembalikan." << std::endl;
                            std::cout << "Terima kasih telah membaca! 📖" << std::endl;
                            inputValid = true;
                        } else {
                            std::cout << "Kesalahan: Buku ini tidak dalam status dipinjam." << std::endl;
                        }
                    } else {
                        std::cout << "Nomor buku tidak ditemukan dalam daftar buku yang sedang dipinjam." << std::endl;
                    }
                } while (!inputValid);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
            case 4: {
                std::cout << "\nTerima kasih telah menggunakan layanan Perpustakaan Digital kami. Sampai jumpa! 👋" << std::endl;
                break;
            }
        }

        if (pilihan != 4) {
            std::cout << "\nKembali ke Menu Utama Perpustakaan? (Y/N): ";
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
