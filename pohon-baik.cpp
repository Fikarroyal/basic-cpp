#include <iostream>
#include <vector>
#include <string>
#include <fstream>      // Untuk file I/O (menyimpan/memuat data)
#include <sstream>      // Untuk stringstream (parsing data)
#include <map>          // Untuk menyimpan data kebiasaan
#include <chrono>       // Untuk mendapatkan tanggal/waktu saat ini
#include <iomanip>      // Untuk setw, put_time
#include <algorithm>    // Untuk std::remove_if
#include <random>       // Untuk pesan motivasi acak

// --- Struktur Data ---

struct Habit {
    std::string name;
    int currentStreak;       // Hari berturut-turut berhasil
    int longestStreak;       // Streak terpanjang sepanjang masa
    int totalCompletions;    // Total berhasil dilakukan
    std::string lastCompletionDate; // Tanggal terakhir selesai (YYYY-MM-DD)
    bool isCompletedToday;   // Status hari ini
};

// --- Global Variables ---
std::map<std::string, Habit> habits; // Map untuk menyimpan kebiasaan (nama -> objek Habit)
std::string currentDate;             // Tanggal saat ini (YYYY-MM-DD)

// --- Generator Angka Acak untuk Pesan Motivasi ---
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
std::mt19937 rng(static_cast<unsigned int>(std::time(0))); // Seed generator dengan waktu saat ini

// --- Fungsi Pembantu ---

// Mendapatkan tanggal saat ini dalam format YYYY-MM-DD
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d");
    return ss.str();
}

// Membersihkan buffer input (penting setelah std::cin >> int/double)
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Mendapatkan input integer yang valid
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

// Mendapatkan input string (nama kebiasaan) yang tidak kosong
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

// --- Fungsi Penyimpanan dan Pemuatan Data ---

void saveData() {
    std::ofstream outFile("habits.txt");
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Gagal membuka file untuk menyimpan data." << std::endl;
        return;
    }

    outFile << currentDate << "\n"; // Simpan tanggal terakhir program dijalankan

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
        currentDate = getCurrentDate(); // Inisialisasi tanggal saat ini
        return;
    }

    std::string line;
    // Baca tanggal terakhir program dijalankan
    if (std::getline(inFile, line)) {
        currentDate = line;
    } else {
        // Jika file kosong, inisialisasi tanggal dan keluar
        inFile.close();
        currentDate = getCurrentDate();
        return;
    }

    std::string today = getCurrentDate();

    habits.clear(); // Bersihkan data sebelumnya
    while (std::getline(inFile, line)) {
        std::stringstream ss(line);
        std::string segment;
        std::vector<std::string> segments;

        while (std::getline(ss, segment, '|')) {
            segments.push_back(segment);
        }

        if (segments.size() == 6) { // Pastikan jumlah segmen sesuai
            Habit h;
            h.name = segments[0];
            h.currentStreak = std::stoi(segments[1]);
            h.longestStreak = std::stoi(segments[2]);
            h.totalCompletions = std::stoi(segments[3]);
            h.lastCompletionDate = segments[4];
            h.isCompletedToday = (segments[5] == "1");

            // --- Penanganan Tanggal Otomatis (Fitur 6) ---
            if (h.lastCompletionDate != today) {
                // Jika terakhir selesai bukan hari ini:
                if (h.lastCompletionDate != "N/A" && // Bukan kebiasaan baru yang belum pernah selesai
                    h.lastCompletionDate == currentDate && // Dan terakhir selesai adalah tanggal saat program terakhir dijalankan
                    !h.isCompletedToday // Dan belum ditandai selesai untuk hari itu (saat program terakhir ditutup)
                ) {
                    // Ini berarti kebiasaan gagal dilanjutkan kemarin (saat program ditutup)
                    h.currentStreak = 0; // Reset streak
                } else if (h.lastCompletionDate != "N/A" && h.lastCompletionDate < today) {
                    // Jika tanggal terakhir selesai lebih awal dari hari ini, dan bukan hari kemarin
                    // Artinya ada jeda hari, jadi reset streak (kecuali jika lastCompletionDate adalah hari kemarin, tidak perlu reset)
                    // Pengecekan ini lebih kompleks, untuk kesederhanaan kita reset jika tanggalnya berbeda dari hari ini
                    // Kecuali jika lastCompletionDate adalah kemarin dan sudah ditandai selesai
                    // Namun, karena kita tidak melacak tanggal untuk setiap hari, kita sederhanakan:
                    // Jika lastCompletionDate bukan hari ini, anggap belum selesai hari ini
                    // Dan jika ada gap, reset streak.
                    // Untuk lebih akjutat, perlu perhitungan tanggal yang lebih kompleks (beda hari antara today dan lastCompletionDate)
                    // Untuk sementara, jika lastCompletionDate bukan hari ini, kita anggap belum selesai untuk hari ini
                    // dan streak akan dihitung ulang saat ditandai selesai.
                }
                h.isCompletedToday = false; // Set ulang status selesai untuk hari ini
            }
            habits[h.name] = h;
        }
    }
    inFile.close();
    std::cout << "[INFO] Data kebiasaan berhasil dimuat." << std::endl;

    currentDate = today; // Setelah memuat data lama, perbarui currentDate ke tanggal *sekarang*
}

// --- Fungsi Utama Program ---

// Tampilan "Pohon" Sederhana (Fitur 4)
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


// Menampilkan daftar kebiasaan (Fitur 1.2)
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

// Menambahkan kebiasaan baru (Fitur 1.1)
void addHabit() {
    std::string name = getNonEmptyStringInput("Masukkan nama kebiasaan baru (contoh: Minum 8 gelas air): ");
    
    // Konversi nama ke huruf kecil untuk pengecekan duplikasi yang case-insensitive
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
        newHabit.lastCompletionDate = "N/A"; // Belum pernah diselesaikan
        newHabit.isCompletedToday = false;
        habits[name] = newHabit; // Gunakan nama asli sebagai kunci map
        std::cout << "Kebiasaan '" << name << "' berhasil ditambahkan!" << std::endl;
        saveData();
    }
}

// Menghapus kebiasaan (Fitur 1.3)
void deleteHabit() {
    viewHabits();
    if (habits.empty()) return;

    std::string nameToDelete = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin dihapus: ");
    
    // Mencari kebiasaan dengan nama yang sesuai (case-insensitive)
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

// Mengubah nama kebiasaan (Fitur 1.4)
void renameHabit() {
    viewHabits();
    if (habits.empty()) return;

    std::string oldName = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin diubah: ");
    std::string newName = getNonEmptyStringInput("Masukkan nama kebiasaan yang baru: ");

    // Cari kebiasaan berdasarkan nama lama (case-insensitive)
    Habit* targetHabit = nullptr;
    std::string actualOldName; // Untuk menyimpan nama asli (case-sensitive)
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

    // Cek duplikasi nama baru (case-insensitive)
    std::string lowerNewName = newName;
    std::transform(lowerNewName.begin(), lowerNewName.end(), lowerNewName.begin(), ::tolower);
    bool newNameExists = false;
    for (const auto& pair : habits) {
        if (pair.first == actualOldName) continue; // Jangan bandingkan dengan diri sendiri
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

    // Lakukan perubahan nama
    targetHabit->name = newName;
    habits.erase(actualOldName); // Hapus entri lama
    habits[newName] = *targetHabit; // Tambahkan entri baru
    
    std::cout << "Nama kebiasaan '" << oldName << "' berhasil diubah menjadi '" << newName << "'." << std::endl;
    saveData();
}

// Menandai kebiasaan selesai (Fitur 2.1)
void markHabitCompleted() {
    viewHabits();
    if (habits.empty()) return;

    std::string nameToMark = getNonEmptyStringInput("Masukkan nama kebiasaan yang ingin ditandai selesai hari ini: ");

    // Cari kebiasaan dengan nama yang sesuai (case-insensitive)
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

    // Hitung streak
    if (targetHabit->lastCompletionDate == "N/A" || targetHabit->lastCompletionDate != currentDate) {
        // Jika belum pernah diselesaikan atau terakhir selesai bukan hari ini (mungkin kemarin atau dulu sekali)
        // Cek apakah terakhir selesai adalah kemarin, jika ya, lanjutkan streak
        // Ini bagian yang rumit dengan tanggal. Untuk kesederhanaan saat ini, jika bukan hari ini, anggap streak baru atau lanjut
        // Asumsi sederhana: jika lastCompletionDate sama dengan currentDate saat program terakhir ditutup dan hari ini adalah next day, maka streak lanjut.
        // Jika ada gap hari (misal lastCompletionDate 2 hari lalu atau lebih), streak reset.
        
        // Pendekatan lebih sederhana: Jika lastCompletionDate bukan hari ini, dan belum ditandai hari ini
        // Jika lastCompletionDate adalah hari sebelumnya dari currentDate (misal kemarin), maka streak++
        // Jika tidak, streak reset ke 1.
        
        // Untuk presisi:
        std::tm tmLast = {};
        if (targetHabit->lastCompletionDate != "N/A") {
            std::istringstream ls(targetHabit->lastCompletionDate);
            ls >> std::get_time(&tmLast, "%Y-%m-%d");
        }
        
        std::tm tmCurrent = {};
        std::istringstream cs(currentDate);
        cs >> std::get_time(&tmCurrent, "%Y-%m-%d");

        // Konversi ke time_t untuk perhitungan selisih hari
        std::time_t timeLast = std::mktime(&tmLast);
        std::time_t timeCurrent = std::mktime(&tmCurrent);

        double secondsDiff = std::difftime(timeCurrent, timeLast);
        int daysDiff = static_cast<int>(secondsDiff / (60 * 60 * 24)); // Selisih hari

        if (targetHabit->lastCompletionDate != "N/A" && daysDiff == 1) { // Jika selesai kemarin
            targetHabit->currentStreak++;
        } else if (targetHabit->lastCompletionDate != "N/A" && daysDiff > 1) { // Jika ada gap hari
            targetHabit->currentStreak = 1; // Reset streak
        } else { // Jika belum pernah selesai atau selesai hari ini (tapi belum ditandai)
            targetHabit->currentStreak = 1; // Mulai streak baru
        }
    } else {
        // Jika lastCompletionDate == currentDate, ini sudah ditangani oleh isCompletedToday di atas
        // Jadi tidak perlu update streak lagi
    }
    
    targetHabit->lastCompletionDate = currentDate;
    if (targetHabit->currentStreak > targetHabit->longestStreak) {
        targetHabit->longestStreak = targetHabit->currentStreak;
    }

    std::cout << "\n✅ Kebiasaan '" << targetHabit->name << "' berhasil ditandai selesai hari ini!" << std::endl;
    std::cout << "Streak saat ini: " << targetHabit->currentStreak << " hari." << std::endl;

    // Pesan motivasi acak (Fitur 7)
    std::uniform_int_distribution<> distMessage(0, motivationMessages.size() - 1);
    std::cout << "\n\"" << motivationMessages[distMessage(rng)] << "\"" << std::endl;

    saveData();
}

// Menandai kebiasaan gagal/lewat (Fitur 2.2)
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
    targetHabit->currentStreak = 0; // Reset streak jika gagal

    std::cout << "❌ Kebiasaan '" << targetHabit->name << "' ditandai gagal/lewat hari ini. Streak direset." << std::endl;
    saveData();
}

// Melihat ringkasan harian (Fitur 5.1)
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

// Melihat statistik kebiasaan (Fitur 5.2)
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
            displayHabitTree(h.totalCompletions); // Tampilkan pohon untuk setiap kebiasaan (Fitur 4)
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}


// --- Fungsi Main Game Loop ---
int main() {
    loadData(); // Muat data saat program dimulai

    // Perbarui status kebiasaan berdasarkan tanggal hari ini vs tanggal terakhir disimpan
    std::string today = getCurrentDate();
    if (currentDate != today) {
        std::cout << "[INFO] Hari telah berganti ke " << today << ". Memperbarui status kebiasaan..." << std::endl;
        for (auto& pair : habits) {
            Habit& h = pair.second;
            // Jika kebiasaan belum selesai di tanggal terakhir program berjalan DAN tanggal terakhir selesai adalah tanggal terakhir program berjalan
            // Artinya kebiasaan gagal kemarin malam
            if (h.lastCompletionDate == currentDate && !h.isCompletedToday) {
                h.currentStreak = 0; // Reset streak
            } 
            // Jika lastCompletionDate bukan hari ini DAN lastCompletionDate bukan kemarin
            // maka streak juga direset.
            // Pengecekan ini agak rumit untuk akurasi penuh. Untuk kesederhanaan, jika ada gap hari, kita reset streak
            // Atau jika kebiasaan belum selesai hari ini, streak akan direset saat pertama kali gagal.
            
            h.isCompletedToday = false; // Reset status untuk hari baru
        }
        currentDate = today; // Perbarui tanggal global
        saveData(); // Simpan perubahan status hari ini
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
            case 9: saveData(); break; // Simpan data sebelum keluar
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 9);

    std::cout << "\nTerima kasih telah menggunakan Pohon Kebiasaan Baik! Sampai jumpa lagi." << std::endl;
    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get(); // Menunggu user menekan enter sebelum menutup konsol

    return 0;
}
