#include <iostream>
#include <vector>
#include <string>
#include <fstream>     
#include <sstream>     
#include <map>       
#include <chrono>     
#include <iomanip>    
#include <algorithm>   
#include <random>     

struct Habit {
    std::string name;
    int currentStreak;      
    int longestStreak;      
    int totalCompletions;  
    std::string lastCompletionDate;
    bool isCompletedToday; 
};
std::map<std::string, Habit> habits;
std::string currentDate;          

std::vector<std::string> motivationMessages = {
    "Luar biasa! Konsistensi adalah kunci!",
    "Satu langkah kecil hari ini, perubahan besar di masa depan!",
    "Teruslah bergerak maju. Kamu bisa!",
    "Setiap kebiasaan baik adalah investasi untuk dirimu sendiri.",
    "Banggalah dengan progresmu, sekecil apa pun itu!",
    "Kamu sedang membangun fondasi kesuksesanmu.",
    "Energi positifmu menular! Teruslah berbuat baik.",
    "Ingat, bahkan perjalanan seribu mil dimulai dengan satu langkah.",
    "Hari ini adalah kesempatan baru untuk menjadi lebih baik.",
    "Jangan menyerah, kamu lebih kuat dari yang kamu kira!"
};
std::mt19937 rng(static_cast<unsigned int>(std::time(0)));
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d");
    return ss.str();
}
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
            std::cout << "Input tidak boleh kosong. Mohon masukkan nama yang valid." << std::endl;
        } else {
            return input;
        }
    }
}

void saveData() {
    std::ofstream outFile("habits.txt");
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Gagal membuka file untuk menyimpan data." << std::endl;
        return;
    }

    outFile << currentDate << "\n";

    for (const auto& pair : habits) {
        const Habit& h = pair.second;
        outFile << h.name << "|"
                << h.currentStreak << "|"
                << h.longestStreak << "|"
                << h.totalCompletions << "|"
                << h.lastCompletionDate << "|"
                << (h.isCompletedToday ? "1" : "0") << "\n";
    }
    outFile.close();
    std::cout << "[INFO] Data kebiasaan berhasil disimpan." << std::endl;
}

void loadData() {
    std::ifstream inFile("habits.txt");
    if (!inFile.is_open()) {
        std::cout << "[INFO] File data tidak ditemukan. Membuat data baru." << std::endl;
        currentDate = getCurrentDate();
        return;
    }

    std::string line;
    if (std::getline(inFile, line)) {
        currentDate = line;
    } else {
        inFile.close();
        currentDate = getCurrentDate();
        return;
    }

    std::string today = getCurrentDate();

    habits.clear();
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;

        while (std::getline(ss, segment, '|')) {
            segments.push_back(segment);
        }

        if (segments.size() == 6) {
            Habit h;
            h.name = segments[0];
            h.currentStreak = std::stoi(segments[1]);
            h.longestStreak = std::stoi(segments[2]);
            h.totalCompletions = std::stoi(segments[3]);
            h.lastCompletionDate = segments[4];
            h.isCompletedToday = (segments[5] == "1");
            if (h.lastCompletionDate != today) {
                if (h.lastCompletionDate != "N/A" && 
                    h.lastCompletionDate == currentDate && 
                    !h.isCompletedToday
                ) {
                    h.currentStreak = 0;
                } else if (h.lastCompletionDate != "N/A" && h.lastCompletionDate < today) {
                }
                h.isCompletedToday = false;
            }
            habits[h.name] = h;
        }
    }
    inFile.close();
    std::cout << "[INFO] Data kebiasaan berhasil dimuat." << std::endl;

    currentDate = today;
}
void displayHabitTree(int totalCompletions) {
    std::cout << "\n           Visualisasi Pohon Progres Anda            " << std::endl;
    std::cout << "=====================================================" << std::endl;
    if (totalCompletions <= 0) {
        std::cout << "                     🌳                     " << std::endl;
        std::cout << "                 (Benih Kebiasaan)           " << std::endl;
        std::cout << "  Mulailah menanam kebiasaan baik Anda hari ini!" << std::endl;
    } else if (totalCompletions <= 5) {
        std::cout << "                    _ _                    " << std::endl;
        std::cout << "                   |_|_|                   " << std::endl;
        std::cout << "                  /  |  \\                  " << std::endl;
        std::cout << "                 /___|___\\                 " << std::endl;
        std::cout << "                    | |                    " << std::endl;
        std::cout << "                    | |                    " << std::endl;
        std::cout << "                 Pohon Kecil 🌱             " << std::endl;
    } else if (totalCompletions <= 15) {
        std::cout << "                    .-'|'-.                  " << std::endl;
        std::cout << "                   / _ | _ \\                 " << std::endl;
        std::cout << "                  /  '-.-'  \\                " << std::endl;
        std::cout << "                 /___________\\               " << std::endl;
        std::cout << "                    |  |  |                  " << std::endl;
        std::cout << "                    |  |  |                  " << std::endl;
        std::cout << "                 Pohon Sedang 🌳             " << std::endl;
    } else if (totalCompletions <= 30) {
        std::cout << "                      /\\                       " << std::endl;
        std::cout << "                     /  \\                      " << std::endl;
        std::cout << "                    /____\\                     " << std::endl;
        std::cout << "                   / _||_ \\                    " << std::endl;
        std::cout << "                  /  ____  \\                   " << std::endl;
        std::cout << "                 /__________\\                  " << std::endl;
        std::cout << "                     ||||                      " << std::endl;
        std::cout << "                     ||||                      " << std::endl;
        std::cout << "                  Pohon Rimbun 🌲             " << std::endl;
    } else { // 31+
        std::cout << "                      ,;:,                     " << std::endl;
        std::cout << "                  ,;::::;'                     " << std::endl;
        std::cout << "               ,;::::;,                           " << std::endl;
        std::cout << "             ,;::;;'                             " << std::endl;
        std::cout << "            ,::::,'                              " << std::endl;
        std::cout << "         ,;::::;,                                " << std::endl;
        std::cout << "       ,;::::;'                                  " << std::endl;
        std::cout << "      ;::::::;                                   " << std::endl;
        std::cout << "      ;::::::;                                   " << std::endl;
        std::cout << "     ,;::::;,                                   " << std::endl;
        std::cout << "    ,::::,'                                     " << std::endl;
        std::cout << "   ,;::::;,                                     " << std::endl;
        std::cout << "  ,;::::;'                                      " << std::endl;
        std::cout << "  ;::::::;                                      " << std::endl;
        std::cout << "  ;::::::;                                      " << std::endl;
        std::cout << "   \\;;;;/                                       " << std::endl;
        std::cout << "    \\\\//                                        " << std::endl;
        std::cout << "     ||                                         " << std::endl;
        std::cout << "     ||                                         " << std::endl;
        std::cout << "   Pohon Legendaris! 🌳✨                           " << std::endl;
    }
    std::cout << "=====================================================\n" << std::endl;
}
void viewHabits() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   DAFTAR KEBIASAAN ANDA                  " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (habits.empty()) {
        std::cout << "Anda belum memiliki kebiasaan yang dilacak. Tambahkan satu!" << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "No."
                  << std::left << std::setw(30) << "Nama Kebiasaan"
                  << std::left << std::setw(10) << "Streak"
                  << std::left << std::setw(10) << "Selesai Hari Ini" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        int i = 1;
        for (const auto& pair : habits) {
            std::cout << std::left << std::setw(5) << i++ << ".";
            std::cout << std::left << std::setw(30) << pair.second.name;
            std::cout << std::left << std::setw(10) << pair.second.currentStreak;
            std::cout << std::left << std::setw(10) << (pair.second.isCompletedToday ? "✅" : "❌") << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}
void addHabit() {
    std::string name = getNonEmptyStringInput("Masukkan nama kebiasaan baru (contoh: Minum 8 gelas air): ");
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    bool exists = false;
    for (const auto& pair : habits) {
        std::string existingLowerName = pair.first;
        std::transform(existingLowerName.begin(), existingLowerName.end(), existingLowerName.begin(), ::tolower);
        if (existingLowerName == lowerName) {
            exists = true;
            break;
        }
    }

    if (exists) {
        std::cout << "Kebiasaan dengan nama '" << name << "' sudah ada. Mohon masukkan nama lain." << std::endl;
    } else {
        Habit newHabit;
        newHabit.name = name;
        newHabit.currentStreak = 0;
        newHabit.longestStreak = 0;
        newHabit.totalCompletions = 0;
        newHabit.lastCompletionDate = "N/A";
        newHabit.isCompletedToday = false;
        habits[name] = newHabit;
        std::cout << "Kebiasaan '" << name << "' berhasil ditambahkan!" << std::endl;
        saveData();
    }
}
void deleteHabit() {
    viewHabits();
    if (habits.empty()) return;

    std::string nameToDelete = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin dihapus: ");
    bool found = false;
    for (auto it = habits.begin(); it != habits.end(); ++it) {
        std::string currentHabitName = it->first;
        std::string lowerCurrentHabitName = currentHabitName;
        std::transform(lowerCurrentHabitName.begin(), lowerCurrentHabitName.end(), lowerCurrentHabitName.begin(), ::tolower);
        
        std::string lowerNameToDelete = nameToDelete;
        std::transform(lowerNameToDelete.begin(), lowerNameToDelete.end(), lowerNameToDelete.begin(), ::tolower);

        if (lowerCurrentHabitName == lowerNameToDelete) {
            habits.erase(it);
            std::cout << "Kebiasaan '" << currentHabitName << "' berhasil dihapus." << std::endl;
            found = true;
            saveData();
            break;
        }
    }
    if (!found) {
        std::cout << "Kebiasaan '" << nameToDelete << "' tidak ditemukan." << std::endl;
    }
}
void renameHabit() {
    viewHabits();
    if (habits.empty()) return;

    std::string oldName = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin diubah: ");
    std::string newName = getNonEmptyStringInput("Masukkan nama kebiasaan yang baru: ");
    Habit* targetHabit = nullptr;
    std::string actualOldName;
    for (auto& pair : habits) {
        std::string lowerCurrentHabitName = pair.first;
        std::transform(lowerCurrentHabitName.begin(), lowerCurrentHabitName.end(), lowerCurrentHabitName.begin(), ::tolower);
        
        std::string lowerOldName = oldName;
        std::transform(lowerOldName.begin(), lowerOldName.end(), lowerOldName.begin(), ::tolower);

        if (lowerCurrentHabitName == lowerOldName) {
            targetHabit = &pair.second;
            actualOldName = pair.first;
            break;
        }
    }

    if (!targetHabit) {
        std::cout << "Kebiasaan '" << oldName << "' tidak ditemukan." << std::endl;
        return;
    }
    std::string lowerNewName = newName;
    std::transform(lowerNewName.begin(), lowerNewName.end(), lowerNewName.begin(), ::tolower);
    bool newNameExists = false;
    for (const auto& pair : habits) {
        if (pair.first == actualOldName) continue;
        std::string existingLowerName = pair.first;
        std::transform(existingLowerName.begin(), existingLowerName.end(), existingLowerName.begin(), ::tolower);
        if (existingLowerName == lowerNewName) {
            newNameExists = true;
            break;
        }
    }

    if (newNameExists) {
        std::cout << "Nama kebiasaan '" << newName << "' sudah digunakan oleh kebiasaan lain." << std::endl;
        return;
    }
    targetHabit->name = newName;
    habits.erase(actualOldName);
    habits[newName] = *targetHabit;
    
    std::cout << "Nama kebiasaan '" << oldName << "' berhasil diubah menjadi '" << newName << "'." << std::endl;
    saveData();
}
void markHabitCompleted() {
    viewHabits();
    if (habits.empty()) return;

    std::string nameToMark = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin ditandai selesai hari ini: ");
    Habit* targetHabit = nullptr;
    std::string actualName;
    for (auto& pair : habits) {
        std::string lowerCurrentHabitName = pair.first;
        std::transform(lowerCurrentHabitName.begin(), lowerCurrentHabitName.end(), lowerCurrentHabitName.begin(), ::tolower);
        
        std::string lowerNameToMark = nameToMark;
        std::transform(lowerNameToMark.begin(), lowerNameToMark.end(), lowerNameToMark.begin(), ::tolower);

        if (lowerCurrentHabitName == lowerNameToMark) {
            targetHabit = &pair.second;
            actualName = pair.first;
            break;
        }
    }

    if (!targetHabit) {
        std::cout << "Kebiasaan '" << nameToMark << "' tidak ditemukan." << std::endl;
        return;
    }

    if (targetHabit->isCompletedToday) {
        std::cout << "Kebiasaan '" << targetHabit->name << "' sudah ditandai selesai untuk hari ini." << std::endl;
        return;
    }

    targetHabit->isCompletedToday = true;
    targetHabit->totalCompletions++;
    if (targetHabit->lastCompletionDate == "N/A" || targetHabit->lastCompletionDate != currentDate) {
        std::tm tmLast = {};
        if (targetHabit->lastCompletionDate != "N/A") {
            std::istringstream ls(targetHabit->lastCompletionDate);
            ls >> std::get_time(&tmLast, "%Y-%m-%d");
        }
        
        std::tm tmCurrent = {};
        std::istringstream cs(currentDate);
        cs >> std::get_time(&tmCurrent, "%Y-%m-%d");
        std::time_t timeLast = std::mktime(&tmLast);
        std::time_t timeCurrent = std::mktime(&tmCurrent);

        double secondsDiff = std::difftime(timeCurrent, timeLast);
        int daysDiff = static_cast<int>(secondsDiff / (60 * 60 * 24));

        if (targetHabit->lastCompletionDate != "N/A" && daysDiff == 1) {
            targetHabit->currentStreak++;
        } else if (targetHabit->lastCompletionDate != "N/A" && daysDiff > 1) {
            targetHabit->currentStreak = 1;
        } else {
            targetHabit->currentStreak = 1;
        }
    } else {
    }
    
    targetHabit->lastCompletionDate = currentDate;
    if (targetHabit->currentStreak > targetHabit->longestStreak) {
        targetHabit->longestStreak = targetHabit->currentStreak;
    }
    std::cout << "\n✅ Kebiasaan '" << targetHabit->name << "' berhasil ditandai selesai hari ini!" << std::endl;
    std::cout << "Streak saat ini: " << targetHabit->currentStreak << " hari." << std::endl;
    std::uniform_int_distribution<> distMessage(0, motivationMessages.size() - 1);
    std::cout << "\n\"" << motivationMessages[distMessage(rng)] << "\"" << std::endl;

    saveData();
}
void markHabitFailed() {
    viewHabits();
    if (habits.empty()) return;

    std::string nameToMark = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin ditandai gagal/lewat hari ini: ");

    Habit* targetHabit = nullptr;
    for (auto& pair : habits) {
        std::string lowerCurrentHabitName = pair.first;
        std::transform(lowerCurrentHabitName.begin(), lowerCurrentHabitName.end(), lowerCurrentHabitName.begin(), ::tolower);
        
        std::string lowerNameToMark = nameToMark;
        std::transform(lowerNameToMark.begin(), lowerNameToMark.end(), lowerNameToMark.begin(), ::tolower);

        if (lowerCurrentHabitName == lowerNameToMark) {
            targetHabit = &pair.second;
            break;
        }
    }

    if (!targetHabit) {
        std::cout << "Kebiasaan '" << nameToMark << "' tidak ditemukan." << std::endl;
        return;
    }

    if (!targetHabit->isCompletedToday) {
        std::cout << "Kebiasaan '" << targetHabit->name << "' memang belum selesai hari ini. Streak akan direset." << std::endl;
    }

    targetHabit->isCompletedToday = false;
    targetHabit->currentStreak = 0;

    std::cout << "❌ Kebiasaan '" << targetHabit->name << "' ditandai gagal/lewat hari ini. Streak direset." << std::endl;
    saveData();
}

void viewDailySummary() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                RINGKASAN HARIAN (" << currentDate << ")                " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (habits.empty()) {
        std::cout << "Tidak ada kebiasaan untuk ditampilkan." << std::endl;
    } else {
        std::cout << std::left << std::setw(35) << "Nama Kebiasaan"
                  << std::left << std::setw(15) << "Status Hari Ini" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : habits) {
            std::cout << std::left << std::setw(35) << pair.second.name;
            std::cout << std::left << std::setw(15) << (pair.second.isCompletedToday ? "Selesai ✅" : "Belum ❌") << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void viewHabitStatistics() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                     STATISTIK KEBIASAAN                  " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (habits.empty()) {
        std::cout << "Tidak ada kebiasaan untuk ditampilkan statistiknya." << std::endl;
    } else {
        for (const auto& pair : habits) {
            const Habit& h = pair.second;
            std::cout << "----------------------------------------------------------" << std::endl;
            std::cout << "Kebiasaan       : " << h.name << std::endl;
            std::cout << "Streak Saat Ini : " << h.currentStreak << " hari" << std::endl;
            std::cout << "Streak Terpanjang: " << h.longestStreak << " hari" << std::endl;
            std::cout << "Total Selesai   : " << h.totalCompletions << " kali" << std::endl;
            std::cout << "Terakhir Selesai: " << h.lastCompletionDate << std::endl;
            displayHabitTree(h.totalCompletions);
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}
int main() {
    loadData();
    std::string today = getCurrentDate();
    if (currentDate != today) {
        std::cout << "[INFO] Hari telah berganti ke " << today << ". Memperbarui status kebiasaan..." << std::endl;
        for (auto& pair : habits) {
            Habit& h = pair.second;
            if (h.lastCompletionDate == currentDate && !h.isCompletedToday) {
                h.currentStreak = 0;
            } 

            h.isCompletedToday = false;
        }
        currentDate = today;
        saveData();
    }

    int choice;
    do {
        std::cout << "\n==========================================================" << std::endl;
        std::cout << "                 POHON KEBIASAAN BAIK                    " << std::endl;
        std::cout << "               Tanggal: " << currentDate << std::endl;
        std::cout << "==========================================================" << std::endl;
        std::cout << "1.  Lihat Daftar Kebiasaan" << std::endl;
        std::cout << "2.  Tambah Kebiasaan Baru" << std::endl;
        std::cout << "3.  Hapus Kebiasaan" << std::endl;
        std::cout << "4.  Ubah Nama Kebiasaan" << std::endl;
        std::cout << "5.  Tandai Kebiasaan Selesai Hari Ini" << std::endl;
        std::cout << "6.  Tandai Kebiasaan Gagal/Lewat Hari Ini" << std::endl;
        std::cout << "7.  Lihat Ringkasan Harian" << std::endl;
        std::cout << "8.  Lihat Statistik Kebiasaan (dan Pohon Progres)" << std::endl;
        std::cout << "9.  Keluar & Simpan" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;

        choice = getValidIntegerInput("Pilihan Anda: ", 1, 9);

        switch (choice) {
            case 1: viewHabits(); break;
            case 2: addHabit(); break;
            case 3: deleteHabit(); break;
            case 4: renameHabit(); break;
            case 5: markHabitCompleted(); break;
            case 6: markHabitFailed(); break;
            case 7: viewDailySummary(); break;
            case 8: viewHabitStatistics(); break;
            case 9: saveData(); break;
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 9);

    std::cout << "\nTerima kasih telah menggunakan Pohon Kebiasaan Baik! Sampai jumpa lagi." << std::endl;
    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
