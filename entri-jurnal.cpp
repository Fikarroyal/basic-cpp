#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>

struct JournalEntry {
    int id;
    std::string title;
    std::string content;
    std::string date;
};

std::vector<JournalEntry> journalEntries;
int nextEntryId = 1;
const std::string DATA_FILE = "knowledge_journal.txt";

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getValidIntegerInput(const std::string& prompt, int minVal = 0, int maxVal = std::numeric_limits<int>::max()) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cout << "Input tidak valid. Mohon masukkan angka bulat antara " << minVal << " dan " << maxVal << "." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

std::string getNonEmptyStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty() || input.find_first_not_of(' ') == std::string::npos) {
            std::cout << "Input tidak boleh kosong. Mohon masukkan teks yang valid." << std::endl;
        } else {
            return input;
        }
    }
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d");
    return ss.str();
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

void saveData() {
    std::ofstream outFile(DATA_FILE);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Gagal membuka file untuk menyimpan data." << std::endl;
        return;
    }

    outFile << nextEntryId << "\n";
    for (const auto& entry : journalEntries) {
        outFile << entry.id << "|||"
                << entry.title << "|||"
                << entry.content << "|||"
                << entry.date << "\n";
    }
    outFile.close();
    std::cout << "[INFO] Data jurnal berhasil disimpan." << std::endl;
}

void loadData() {
    std::ifstream inFile(DATA_FILE);
    if (!inFile.is_open()) {
        std::cout << "[INFO] File data jurnal tidak ditemukan. Membuat data baru." << std::endl;
        nextEntryId = 1;
        return;
    }

    std::string line;
    if (std::getline(inFile, line)) {
        try {
            nextEntryId = std::stoi(line);
        } catch (const std::exception& e) {
            std::cerr << "Peringatan: Gagal membaca nextEntryId. Mengatur ulang ke 1. Error: " << e.what() << std::endl;
            nextEntryId = 1;
        }
    } else {
        inFile.close();
        nextEntryId = 1;
        return;
    }

    journalEntries.clear();
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;

        size_t pos = 0;
        std::string token;
        while ((pos = line.find("|||")) != std::string::npos) {
            token = line.substr(0, pos);
            segments.push_back(token);
            line.erase(0, pos + 3);
        }
        segments.push_back(line);

        if (segments.size() == 4) {
            JournalEntry entry;
            try {
                entry.id = std::stoi(segments[0]);
                entry.title = segments[1];
                entry.content = segments[2];
                entry.date = segments[3];
                journalEntries.push_back(entry);
                if (entry.id >= nextEntryId) {
                    nextEntryId = entry.id + 1;
                }
            } catch (const std::exception& e) {
                std::cerr << "Peringatan: Baris data korup dilewati. Error: " << e.what() << " - Data: " << line << std::endl;
            }
        } else {
            std::cerr << "Peringatan: Format baris data tidak valid, dilewati. Data: " << line << std::endl;
        }
    }
    inFile.close();
    std::cout << "[INFO] Data jurnal berhasil dimuat. Total entri: " << journalEntries.size() << std::endl;
}

void addEntry() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   TAMBAH ENTRI BARU                     " << std::endl;
    std::cout << "==========================================================" << std::endl;

    JournalEntry newEntry;
    newEntry.id = nextEntryId++;
    newEntry.date = getCurrentDate();

    std::cout << "Tanggal: " << newEntry.date << std::endl;
    newEntry.title = getNonEmptyStringInput("Masukkan judul entri: ");
    std::cout << "Masukkan isi entri (ketik 'END' pada baris baru untuk selesai):\n";
    std::string line;
    newEntry.content = "";
    while (std::getline(std::cin, line) && line != "END") {
        newEntry.content += line + "\n";
    }
    if (!newEntry.content.empty() && newEntry.content.back() == '\n') {
        newEntry.content.pop_back();
    }

    journalEntries.push_back(newEntry);
    std::cout << "\n[✅] Entri '" << newEntry.title << "' berhasil ditambahkan dengan ID: " << newEntry.id << std::endl;
    saveData();
}

void viewAllEntries() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                     SEMUA ENTRI JURNAL                     " << std::endl;
    std::cout << "==========================================================" << std::endl;

    if (journalEntries.empty()) {
        std::cout << "Jurnal Anda masih kosong. Tambahkan entri baru!" << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(40) << "Judul" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& entry : journalEntries) {
            std::cout << std::left << std::setw(5) << entry.id
                      << std::left << std::setw(15) << entry.date
                      << std::left << std::setw(40) << entry.title << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void viewEntryDetails() {
    viewAllEntries();
    if (journalEntries.empty()) return;

    int idToView = getValidIntegerInput("Masukkan ID entri yang ingin dilihat detailnya: ");

    bool found = false;
    for (const auto& entry : journalEntries) {
        if (entry.id == idToView) {
            std::cout << "\n==========================================================" << std::endl;
            std::cout << "                 DETAIL ENTRI ID: " << entry.id << std::endl;
            std::cout << "==========================================================" << std::endl;
            std::cout << "Judul   : " << entry.title << std::endl;
            std::cout << "Tanggal : " << entry.date << std::endl;
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << entry.content << std::endl;
            std::cout << "==========================================================\n" << std::endl;
            found = true;
            break;
        }
    }
    if (!found) {
        std::cout << "[!] Entri dengan ID " << idToView << " tidak ditemukan." << std::endl;
    }
}

void editEntry() {
    viewAllEntries();
    if (journalEntries.empty()) return;

    int idToEdit = getValidIntegerInput("Masukkan ID entri yang ingin diedit: ");

    bool found = false;
    for (auto& entry : journalEntries) {
        if (entry.id == idToEdit) {
            std::cout << "\n==========================================================" << std::endl;
            std::cout << "                     EDIT ENTRI ID: " << entry.id << std::endl;
            std::cout << "==========================================================" << std::endl;
            std::cout << "Judul saat ini: " << entry.title << std::endl;
            std::string newTitle = getNonEmptyStringInput("Masukkan judul baru (biarkan kosong untuk tidak mengubah): ");
            if (!newTitle.empty() && newTitle.find_first_not_of(' ') != std::string::npos) {
                entry.title = newTitle;
            }

            std::cout << "Isi saat ini:\n" << entry.content << std::endl;
            std::cout << "Masukkan isi baru (ketik 'END' pada baris baru untuk selesai, biarkan kosong untuk tidak mengubah):\n";
            std::string line;
            std::string newContent = "";
            bool inputGiven = false;
            while (std::getline(std::cin, line)) {
                if (line == "END") break;
                newContent += line + "\n";
                inputGiven = true;
            }
            if (inputGiven) {
                if (!newContent.empty() && newContent.back() == '\n') {
                    newContent.pop_back();
                }
                entry.content = newContent;
            }
            
            std::cout << "\n[✅] Entri ID " << entry.id << " berhasil diperbarui." << std::endl;
            found = true;
            saveData();
            break;
        }
    }
    if (!found) {
        std::cout << "[!] Entri dengan ID " << idToEdit << " tidak ditemukan." << std::endl;
    }
}

void deleteEntry() {
    viewAllEntries();
    if (journalEntries.empty()) return;

    int idToDelete = getValidIntegerInput("Masukkan ID entri yang ingin dihapus: ");

    auto it = std::remove_if(journalEntries.begin(), journalEntries.end(),
                             [idToDelete](const JournalEntry& entry) {
                                 return entry.id == idToDelete;
                             });
    
    if (it != journalEntries.end()) {
        journalEntries.erase(it, journalEntries.end());
        std::cout << "[✅] Entri ID " << idToDelete << " berhasil dihapus." << std::endl;
        saveData();
    } else {
        std::cout << "[!] Entri dengan ID " << idToDelete << " tidak ditemukan." << std::endl;
    }
}

void searchByTitle() {
    if (journalEntries.empty()) {
        std::cout << "Jurnal kosong. Tidak ada yang bisa dicari." << std::endl;
        return;
    }
    std::string keyword = getNonEmptyStringInput("Masukkan kata kunci judul: ");
    std::string lowerKeyword = toLower(keyword);

    std::vector<JournalEntry> results;
    for (const auto& entry : journalEntries) {
        if (toLower(entry.title).find(lowerKeyword) != std::string::npos) {
            results.push_back(entry);
        }
    }

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   HASIL PENCARIAN JUDUL                  " << std::endl;
    std::cout << "                 Kata Kunci: '" << keyword << "'" << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (results.empty()) {
        std::cout << "Tidak ada entri yang cocok dengan kata kunci '" << keyword << "'." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(40) << "Judul" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& entry : results) {
            std::cout << std::left << std::setw(5) << entry.id
                      << std::left << std::setw(15) << entry.date
                      << std::left << std::setw(40) << entry.title << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void searchByContent() {
    if (journalEntries.empty()) {
        std::cout << "Jurnal kosong. Tidak ada yang bisa dicari." << std::endl;
        return;
    }
    std::string keyword = getNonEmptyStringInput("Masukkan kata kunci isi: ");
    std::string lowerKeyword = toLower(keyword);

    std::vector<JournalEntry> results;
    for (const auto& entry : journalEntries) {
        if (toLower(entry.content).find(lowerKeyword) != std::string::npos) {
            results.push_back(entry);
        }
    }

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   HASIL PENCARIAN ISI                    " << std::endl;
    std::cout << "                 Kata Kunci: '" << keyword << "'" << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (results.empty()) {
        std::cout << "Tidak ada entri yang cocok dengan kata kunci '" << keyword << "'." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(40) << "Judul" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& entry : results) {
            std::cout << std::left << std::setw(5) << entry.id
                      << std::left << std::setw(15) << entry.date
                      << std::left << std::setw(40) << entry.title << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void filterByDate() {
    if (journalEntries.empty()) {
        std::cout << "Jurnal kosong. Tidak ada yang bisa difilter." << std::endl;
        return;
    }
    std::string dateToFilter = getNonEmptyStringInput("Masukkan tanggal (YYYY-MM-DD) untuk filter: ");

    std::vector<JournalEntry> results;
    for (const auto& entry : journalEntries) {
        if (entry.date == dateToFilter) {
            results.push_back(entry);
        }
    }

    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   HASIL FILTER TANGGAL                     " << std::endl;
    std::cout << "                   Tanggal: " << dateToFilter << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (results.empty()) {
        std::cout << "Tidak ada entri yang dibuat pada tanggal '" << dateToFilter << "'." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(40) << "Judul" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& entry : results) {
            std::cout << std::left << std::setw(5) << entry.id
                      << std::left << std::setw(15) << entry.date
                      << std::left << std::setw(40) << entry.title << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showTotalEntries() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                     STATISTIK JURNAL                     " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << "Total entri dalam jurnal Anda: " << journalEntries.size() << std::endl;
    std::cout << "==========================================================\n" << std::endl;
}

void showDailySummary() {
    std::string date = getNonEmptyStringInput("Masukkan tanggal (YYYY-MM-DD) untuk ringkasan harian: ");
    
    int count = 0;
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "            RINGKASAN ENTRI TANGGAL: " << date << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    for (const auto& entry : journalEntries) {
        if (entry.date == date) {
            std::cout << "ID: " << entry.id << " - Judul: " << entry.title << std::endl;
            count++;
        }
    }
    
    if (count == 0) {
        std::cout << "Tidak ada entri yang dibuat pada tanggal " << date << "." << std::endl;
    } else {
        std::cout << "Total " << count << " entri ditemukan pada tanggal " << date << "." << std::endl;
    }
    std::cout << "==========================================================\n" << std::endl;
}


int main() {
    loadData();

    int choice;
    do {
        std::cout << "\n==========================================================" << std::endl;
        std::cout << "               JURNAL BELAJAR & PENGETAHUAN               " << std::endl;
        std::cout << "               Hari Ini: " << getCurrentDate() << std::endl;
        std::cout << "==========================================================" << std::endl;
        std::cout << "1.  Tambah Entri Baru" << std::endl;
        std::cout << "2.  Lihat Semua Entri" << std::endl;
        std::cout << "3.  Lihat Detail Entri (berdasarkan ID)" << std::endl;
        std::cout << "4.  Edit Entri (berdasarkan ID)" << std::endl;
        std::cout << "5.  Hapus Entri (berdasarkan ID)" << std::endl;
        std::cout << "6.  Cari Entri Berdasarkan Judul" << std::endl;
        std::cout << "7.  Cari Entri Berdasarkan Isi" << std::endl;
        std::cout << "8.  Filter Entri Berdasarkan Tanggal" << std::endl;
        std::cout << "9.  Lihat Statistik Jurnal" << std::endl;
        std::cout << "10. Lihat Ringkasan Entri Harian (berdasarkan tanggal)" << std::endl;
        std::cout << "11. Keluar" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;

        choice = getValidIntegerInput("Pilihan Anda: ", 1, 11);

        switch (choice) {
            case 1: addEntry(); break;
            case 2: viewAllEntries(); break;
            case 3: viewEntryDetails(); break;
            case 4: editEntry(); break;
            case 5: deleteEntry(); break;
            case 6: searchByTitle(); break;
            case 7: searchByContent(); break;
            case 8: filterByDate(); break;
            case 9: showTotalEntries(); break;
            case 10: showDailySummary(); break;
            case 11:
                saveData();
                std::cout << "\nTerima kasih telah menggunakan Jurnal Belajar & Pengetahuan! Sampai jumpa lagi." << std::endl;
                break;
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 11);

    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
