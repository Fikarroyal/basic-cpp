#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

struct Barang {
    std::string idBarang;
    std::string nama;
    int stok;
    double hargaSatuan;
};

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getValidIntegerInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Mohon masukkan angka." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

double getValidDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Mohon masukkan angka desimal." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

void tampilkanSemuaBarang(const std::vector<Barang>& inventaris) {
    std::cout << "\n=====================================================================" << std::endl;
    std::cout << "                                 DAFTAR INVENTARIS BARANG                                 " << std::endl;
    std::cout << "=====================================================================\n" << std::endl;
    if (inventaris.empty()) {
        std::cout << "Inventaris kosong. Silakan tambahkan barang terlebih dahulu." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "No."
                  << std::left << std::setw(15) << "ID Barang"
                  << std::left << std::setw(30) << "Nama Barang"
                  << std::left << std::setw(10) << "Stok"
                  << std::left << std::setw(15) << "Harga Satuan" << std::endl;
        std::cout << "---------------------------------------------------------------------" << std::endl;
        for (size_t i = 0; i < inventaris.size(); ++i) {
            std::cout << std::left << std::setw(5) << i + 1 << ".";
            std::cout << std::left << std::setw(15) << inventaris[i].idBarang;
            std::cout << std::left << std::setw(30) << inventaris[i].nama;
            std::cout << std::left << std::setw(10) << inventaris[i].stok;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << inventaris[i].hargaSatuan << std::endl;
        }
    }
    std::cout << "=====================================================================\n" << std::endl;
}

int main() {
    std::vector<Barang> inventaris;
    int pilihan;
    char lanjutAplikasi;

    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI SISTEM MANAJEMEN INVENTARIS!    *" << std::endl;
    std::cout << "* Kelola barang Anda dengan mudah dan efisien.    *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    do {
        std::cout << "\n---- MENU UTAMA INVENTARIS ----" << std::endl;
        std::cout << "1. Tambah Barang Baru" << std::endl;
        std::cout << "2. Lihat Semua Barang" << std::endl;
        std::cout << "3. Cari Barang" << std::endl;
        std::cout << "4. Update Stok Barang" << std::endl;
        std::cout << "5. Hapus Barang" << std::endl;
        std::cout << "6. Hitung Total Nilai Inventaris" << std::endl;
        std::cout << "7. Keluar" << std::endl;
        std::cout << "Pilih opsi (1-7): ";
        std::cin >> pilihan;
        clearInputBuffer();

        if (std::cin.fail() || pilihan < 1 || pilihan > 7) {
            std::cout << "Pilihan tidak valid. Mohon masukkan angka 1-7." << std::endl;
            std::cin.clear();
            lanjutAplikasi = 'Y';
            continue;
        }

        switch (pilihan) {
            case 1: {
                Barang newItem;
                std::cout << "\n----- Tambah Barang Baru ------" << std::endl;
                std::cout << "Masukkan ID Barang (unik): ";
                std::getline(std::cin, newItem.idBarang);

                bool idExists = false;
                for (const auto& barang : inventaris) {
                    if (barang.idBarang == newItem.idBarang) {
                        idExists = true;
                        break;
                    }
                }
                if (idExists) {
                    std::cout << "ID Barang ini sudah ada. Mohon gunakan ID yang lain." << std::endl;
                    break;
                }

                std::cout << "Masukkan Nama Barang: ";
                std::getline(std::cin, newItem.nama);

                newItem.stok = getValidIntegerInput("Masukkan Jumlah Stok: ");
                if (newItem.stok < 0) {
                    std::cout << "Stok tidak bisa negatif. Pembatalan penambahan barang." << std::endl;
                    break;
                }

                newItem.hargaSatuan = getValidDoubleInput("Masukkan Harga Satuan: Rp ");
                if (newItem.hargaSatuan < 0) {
                    std::cout << "Harga tidak bisa negatif. Pembatalan penambahan barang." << std::endl;
                    break;
                }

                inventaris.push_back(newItem);
                std::cout << "✅ Barang '" << newItem.nama << "' berhasil ditambahkan ke inventaris!" << std::endl;
                break;
            }
            case 2: {
                tampilkanSemuaBarang(inventaris);
                break;
            }
            case 3: {
                std::cout << "\n--- Cari Barang ---" << std::endl;
                std::cout << "Cari berdasarkan (1: ID Barang, 2: Nama Barang): ";
                int cariOpsi = getValidIntegerInput("");

                if (cariOpsi == 1) {
                    std::string idCari;
                    std::cout << "Masukkan ID Barang yang dicari: ";
                    std::getline(std::cin, idCari);

                    auto it = std::find_if(inventaris.begin(), inventaris.end(),
                                             [&](const Barang& b){ return b.idBarang == idCari; });

                    if (it != inventaris.end()) {
                        std::cout << "\nBarang ditemukan:" << std::endl;
                        std::cout << "  ID Barang     : " << it->idBarang << std::endl;
                        std::cout << "  Nama Barang   : " << it->nama << std::endl;
                        std::cout << "  Stok          : " << it->stok << std::endl;
                        std::cout << "  Harga Satuan  : Rp " << std::fixed << std::setprecision(2) << it->hargaSatuan << std::endl;
                    } else {
                        std::cout << "Barang dengan ID '" << idCari << "' tidak ditemukan." << std::endl;
                    }
                } else if (cariOpsi == 2) {
                    std::string namaCari;
                    std::cout << "Masukkan Nama Barang yang dicari: ";
                    std::getline(std::cin, namaCari);

                    bool found = false;
                    std::cout << "\n--- Hasil Pencarian ---" << std::endl;
                    for (const auto& barang : inventaris) {
                        if (barang.nama.find(namaCari) != std::string::npos) {
                            std::cout << "  ID Barang     : " << barang.idBarang << std::endl;
                            std::cout << "  Nama Barang   : " << barang.nama << std::endl;
                            std::cout << "  Stok          : " << barang.stok << std::endl;
                            std::cout << "  Harga Satuan  : Rp " << std::fixed << std::setprecision(2) << barang.hargaSatuan << std::endl;
                            std::cout << "------------------------------------------" << std::endl;
                            found = true;
                        }
                    }
                    if (!found) {
                        std::cout << "Barang dengan nama mengandung '" << namaCari << "' tidak ditemukan." << std::endl;
                    }
                } else {
                    std::cout << "Pilihan pencarian tidak valid." << std::endl;
                }
                break;
            }
            case 4: {
                if (inventaris.empty()) {
                    std::cout << "\nInventaris kosong. Tidak ada barang untuk diupdate." << std::endl;
                    break;
                }
                tampilkanSemuaBarang(inventaris);
                int indexUpdate = getValidIntegerInput("Masukkan Nomor Barang yang ingin diupdate stoknya: ");
                
                if (indexUpdate < 1 || indexUpdate > inventaris.size()) {
                    std::cout << "Nomor barang tidak valid." << std::endl;
                    break;
                }

                int stokBaru = getValidIntegerInput("Masukkan Stok Baru untuk '" + inventaris[indexUpdate - 1].nama + "': ");
                if (stokBaru < 0) {
                    std::cout << "Stok tidak bisa negatif. Pembatalan update." << std::endl;
                    break;
                }

                inventaris[indexUpdate - 1].stok = stokBaru;
                std::cout << "👍 Stok barang '" << inventaris[indexUpdate - 1].nama << "' berhasil diupdate menjadi " << stokBaru << "." << std::endl;
                break;
            }
            case 5: {
                if (inventaris.empty()) {
                    std::cout << "\nInventaris kosong. Tidak ada barang untuk dihapus." << std::endl;
                    break;
                }
                tampilkanSemuaBarang(inventaris);
                int indexHapus = getValidIntegerInput("Masukkan Nomor Barang yang ingin dihapus: ");

                if (indexHapus < 1 || indexHapus > inventaris.size()) {
                    std::cout << "Nomor barang tidak valid." << std::endl;
                    break;
                }

                std::string namaBarangDihapus = inventaris[indexHapus - 1].nama;
                inventaris.erase(inventaris.begin() + (indexHapus - 1));
                std::cout << "🗑️ Barang '" << namaBarangDihapus << "' berhasil dihapus dari inventaris." << std::endl;
                break;
            }
            case 6: {
                if (inventaris.empty()) {
                    std::cout << "\nInventaris kosong. Total nilai inventaris: Rp 0.00" << std::endl;
                    break;
                }
                double totalNilai = 0.0;
                for (const auto& barang : inventaris) {
                    totalNilai += (barang.stok * barang.hargaSatuan);
                }
                std::cout << "\n==========================================" << std::endl;
                std::cout << "         TOTAL NILAI INVENTARIS          " << std::endl;
                std::cout << "==========================================" << std::endl;
                std::cout << "  Jumlah Barang Unik: " << inventaris.size() << std::endl;
                std::cout << "  Total Nilai       : Rp " << std::fixed << std::setprecision(2) << totalNilai << std::endl;
                std::cout << "==========================================" << std::endl;
                break;
            }
            case 7: {
                std::cout << "\nTerima kasih telah menggunakan Sistem Manajemen Inventaris. Sampai jumpa! 👋" << std::endl;
                break;
            }
        }

        if (pilihan != 7) {
            std::cout << "\nKembali ke Menu Utama Inventaris? (Y/N): ";
            std::cin >> lanjutAplikasi;
            clearInputBuffer();
        } else {
            lanjutAplikasi = 'N';
        }
    } while (lanjutAplikasi == 'Y' || lanjutAplikasi == 'y');

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
