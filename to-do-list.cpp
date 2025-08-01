#include <iostream>
#include <vector>
#include <string>
#include <limits>

struct Tugas {
    std::string deskripsi;
    bool selesai;
};

void tampilkanMenu() {
    std::cout << "\n===============================" << std::endl;
    std::cout << "      APLIKASI TO-DO LIST      " << std::endl;
    std::cout << "===============================" << std::endl;
    std::cout << "1. Tampilkan Daftar Tugas" << std::endl;
    std::cout << "2. Tambah Tugas Baru" << std::endl;
    std::cout << "3. Tandai Tugas Selesai" << std::endl;
    std::cout << "4. Hapus Tugas" << std::endl;
    std::cout << "5. Keluar" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Pilih opsi: ";
}

void tampilkanDaftarTugas(const std::vector<Tugas>& daftarTugas) {
    std::cout << "\n--- DAFTAR TUGAS ANDA ---" << std::endl;
    if (daftarTugas.empty()) {
        std::cout << "Belum ada tugas dalam daftar." << std::endl;
    } else {
        for (size_t i = 0; i < daftarTugas.size(); ++i) {
            std::cout << i + 1 << ". "
                      << (daftarTugas[i].selesai ? "[Selesai] " : "[Belum Selesai] ")
                      << daftarTugas[i].deskripsi << std::endl;
        }
    }
    std::cout << "-------------------------" << std::endl;
}

void tambahTugas(std::vector<Tugas>& daftarTugas) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string deskripsiTugas;
    std::cout << "Masukkan deskripsi tugas baru: ";
    std::getline(std::cin, deskripsiTugas);
    daftarTugas.push_back({deskripsiTugas, false});
    std::cout << "Tugas berhasil ditambahkan!" << std::endl;
}

void tandaiTugasSelesai(std::vector<Tugas>& daftarTugas) {
    tampilkanDaftarTugas(daftarTugas);
    if (daftarTugas.empty()) {
        return;
    }
    int nomorTugas;
    std::cout << "Masukkan nomor tugas yang ingin ditandai selesai: ";
    std::cin >> nomorTugas;

    if (std::cin.fail() || nomorTugas < 1 || nomorTugas > daftarTugas.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Nomor tugas tidak valid. Mohon masukkan nomor yang benar." << std::endl;
    } else {
        daftarTugas[nomorTugas - 1].selesai = true;
        std::cout << "Tugas '" << daftarTugas[nomorTugas - 1].deskripsi << "' berhasil ditandai selesai!" << std::endl;
    }
}

void hapusTugas(std::vector<Tugas>& daftarTugas) {
    tampilkanDaftarTugas(daftarTugas);
    if (daftarTugas.empty()) {
        return;
    }
    int nomorTugas;
    std::cout << "Masukkan nomor tugas yang ingin dihapus: ";
    std::cin >> nomorTugas;

    if (std::cin.fail() || nomorTugas < 1 || nomorTugas > daftarTugas.size()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Nomor tugas tidak valid. Mohon masukkan nomor yang benar." << std::endl;
    } else {
        std::string deskripsiYangDihapus = daftarTugas[nomorTugas - 1].deskripsi;
        daftarTugas.erase(daftarTugas.begin() + (nomorTugas - 1));
        std::cout << "Tugas '" << deskripsiYangDihapus << "' berhasil dihapus!" << std::endl;
    }
}

int main() {
    std::vector<Tugas> daftarTugas;
    int pilihan;

    do {
        tampilkanMenu();
        std::cin >> pilihan;

        switch (pilihan) {
            case 1:
                tampilkanDaftarTugas(daftarTugas);
                break;
            case 2:
                tambahTugas(daftarTugas);
                break;
            case 3:
                tandaiTugasSelesai(daftarTugas);
                break;
            case 4:
                hapusTugas(daftarTugas);
                break;
            case 5:
                std::cout << "Terima kasih telah menggunakan aplikasi To-Do List!" << std::endl;
                break;
            default:
                std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl;
                break;
        }
        if (std::cin.peek() == '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (pilihan != 5);

    return 0;
}
