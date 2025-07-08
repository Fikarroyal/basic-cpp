#include <iostream>   // Untuk input/output dasar (cout, cin)
#include <vector>     // Untuk menyimpan daftar pertanyaan
#include <string>     // Untuk std::string
#include <iomanip>    // Untuk manipulasi output (setw, left, right)
#include <limits>     // Untuk std::numeric_limits (untuk clear buffer)
#include <algorithm>  // Untuk std::transform (misal: to_upper)
#include <cctype>     // Untuk std::toupper
#include <chrono>     // Untuk std::chrono (mengukur waktu)
#include <thread>     // Untuk std::this_thread::sleep_for (opsional, untuk jeda)

// --- Konfigurasi Tampilan ---
const int TOTAL_WIDTH = 85; // Lebar total tampilan konsol

// Struktur untuk merepresentasikan satu pertanyaan
struct PertanyaanIQ {
    std::string pertanyaan;
    std::vector<std::string> pilihanJawaban;
    char jawabanBenar; // A, B, C, atau D
};

// --- Fungsi Pembantu untuk Tampilan ---
void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}

void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

// Fungsi untuk mendapatkan input karakter (pilihan jawaban) dari pengguna
char getInputChoice(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        // Gunakan getline untuk membaca seluruh baris, lalu ambil karakter pertama
        std::string line;
        std::getline(std::cin >> std::ws, line); 

        if (line.length() == 1 && (std::toupper(line[0]) == 'A' || std::toupper(line[0]) == 'B' || 
                                   std::toupper(line[0]) == 'C' || std::toupper(line[0]) == 'D')) {
            value = std::toupper(line[0]);
            return value;
        } else {
            std::cout << "Pilihan tidak valid. Harap masukkan A, B, C, atau D.\n";
        }
    }
}

// --- Fungsi Inti Game Tebak IQ ---

void runIQTester() {
    // Daftar pertanyaan (25 soal)
    std::vector<PertanyaanIQ> questions = {
        {"1. Jika beberapa bulan tidak memiliki 31 hari, berapa banyak yang memiliki 28 hari?", 
         {"A. 1", "B. 2", "C. Semua", "D. Tidak ada"}, 'C'},
        
        {"2. Lanjutkan deret angka ini: 2, 4, 8, 16, ?",
         {"A. 20", "B. 24", "C. 32", "D. 64"}, 'C'},
        
        {"3. Manakah yang berbeda dari yang lain?",
         {"A. Apel", "B. Jeruk", "C. Pisang", "D. Kentang"}, 'D'},
        
        {"4. Jika semua Slorps adalah Boops, dan semua Boops adalah Kroops, apakah semua Slorps pasti Kroops?",
         {"A. Ya", "B. Tidak", "C. Tergantung", "D. Tidak bisa ditentukan"}, 'A'},
        
        {"5. Pilih gambar yang akan mengisi deret ini: Segitiga, Kotak, Pentagon, ?",
         {"A. Heksagon", "B. Lingkaran", "C. Oval", "D. Trapesium"}, 'A'},

        {"6. Jika Anda melempar koin 3 kali, berapa probabilitas mendapatkan tepat 2 kepala?",
         {"A. 1/8", "B. 3/8", "C. 1/2", "D. 5/8"}, 'B'},
        
        {"7. Berapakah 1/2 dari 2/3 dari 3/4 dari 4/5 dari 5/6 dari 6/7 dari 7/8 dari 8/9 dari 9/10 dari 100?",
         {"A. 10", "B. 5", "C. 1", "D. 20"}, 'A'},

        {"8. Apa yang harus Anda tambahkan pada 120,4 untuk mendapatkan 250?",
         {"A. 129,6", "B. 130,4", "C. 120,6", "D. 129,4"}, 'A'},

        {"9. Pilihlah kata yang paling cocok untuk mengisi titik-titik: 'Dokter' berhubungan dengan 'Pasien' seperti 'Pengacara' berhubungan dengan '.......'?",
         {"A. Pengadilan", "B. Klien", "C. Hukum", "D. Hakim"}, 'B'},
        
        {"10. Jika 5 mesin dapat membuat 5 produk dalam 5 menit, berapa lama waktu yang dibutuhkan 100 mesin untuk membuat 100 produk?",
         {"A. 1 menit", "B. 5 menit", "C. 10 menit", "D. 100 menit"}, 'B'},

        {"11. Manakah angka yang ganjil?",
         {"A. 2, 4, 6", "B. 1, 3, 5", "C. 12, 14, 16", "D. 20, 22, 24"}, 'B'},

        {"12. Jika 'Makan' adalah 'Nasi', maka 'Minum' adalah '...'",
         {"A. Lapar", "B. Air", "C. Sendok", "D. Kenyang"}, 'B'},

        {"13. Mana yang bukan mamalia?",
         {"A. Paus", "B. Kelelawar", "C. Burung Hantu", "D. Kucing"}, 'C'},

        {"14. Jika kemarin adalah besoknya hari Minggu, hari apa sekarang?",
         {"A. Jumat", "B. Sabtu", "C. Minggu", "D. Senin"}, 'A'},

        {"15. Apa kelanjutan dari deret huruf ini: A, C, E, G, ?",
         {"A. H", "B. I", "C. J", "D. K"}, 'B'},
        
        {"16. Sebuah jam jatuh dari ketinggian 10 meter. Apa yang tidak rusak?",
         {"A. Kaca", "B. Jarum", "C. Rantai", "D. Angka"}, 'C'}, 

        {"17. Manakah yang terberat?",
         {"A. 1 kg bulu", "B. 1 kg besi", "C. Sama berat", "D. Tergantung tempat"}, 'C'},

        {"18. Jika Anda memiliki 3 apel dan Anda mengambil 2, berapa banyak apel yang Anda miliki?",
         {"A. 1", "B. 2", "C. 3", "D. 5"}, 'B'},

        {"19. Pilihlah kata yang paling tidak cocok: 'Sedih', 'Gembira', 'Senang', 'Bahagia'",
         {"A. Sedih", "B. Gembira", "C. Senang", "D. Bahagia"}, 'A'},

        {"20. Seorang anak laki-laki memiliki 10 jari di kedua tangannya. Berapa banyak jari yang dia miliki?",
         {"A. 10", "B. 20", "C. 5", "D. 2"}, 'A'},
        
        // 5 Soal Baru
        {"21. Jika Anda melihat angka 6, 10, 14, 18, berapa angka selanjutnya?",
         {"A. 20", "B. 22", "C. 24", "D. 26"}, 'B'},

        {"22. Jika A = Z, B = Y, C = X, maka E = ?",
         {"A. U", "B. V", "C. W", "D. X"}, 'V'}, 

        {"23. Manakah yang termasuk buah?",
         {"A. Brokoli", "B. Wortel", "C. Tomat", "D. Kentang"}, 'C'}, 

        {"24. Jika suatu kode 'KUE' adalah 'LVF', maka 'SUSU' adalah '...'",
         {"A. TVTV", "B. TUTU", "C. TWTW", "D. UWUW"}, 'A'}, 

        {"25. Seorang pria berdiri di satu sisi sungai. Anjingnya berada di sisi lain. Pria itu memanggil anjingnya, yang segera menyeberangi sungai tanpa jembatan atau perahu. Bagaimana anjing itu melakukannya?",
         {"A. Sungai itu beku", "B. Anjing itu bisa terbang", "C. Sungai itu kering", "D. Anjing itu berenang"}, 'C'} 
    };

    int score = 0;
    std::vector<bool> correctAnswers(questions.size(), false);
    bool timeUp = false;
    int questionsAnswered = 0; 

    // --- Pengaturan Waktu ---
    const int TIME_LIMIT_SECONDS = 300; // 5 menit
    auto startTime = std::chrono::high_resolution_clock::now();

    printSeparator();
    printCenteredText("===== MULAI TES IQ =====");
    printSeparator();
    // Baris ini yang diubah/dihapus sesuai permintaan
    std::cout << "\nJawablah pertanyaan-pertanyaan ini dengan memilih A, B, C, atau D.\n";
    std::cout << "Tes ini memiliki batas waktu, jadi fokuslah!\n"; // Memberi sedikit hint tentang waktu
    std::cout << "Tekan Enter untuk memulai tes...\n\n";

    std::cin.get(); // Menunggu Enter untuk memulai timer

    for (int i = 0; i < questions.size(); ++i) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
        
        if (elapsed.count() >= TIME_LIMIT_SECONDS) {
            timeUp = true;
            break; // Waktu habis, hentikan tes
        }
        questionsAnswered++; // Menambah jumlah soal yang dijawab

        printSeparator('-');
        // Tampilkan sisa waktu
        int timeLeft = TIME_LIMIT_SECONDS - static_cast<int>(elapsed.count());
        std::cout << std::left << std::setw(TOTAL_WIDTH - 20) << "Pertanyaan " + std::to_string(i + 1) + ":"
                  << "Waktu Sisa: " << timeLeft << " detik" << std::endl;
        std::cout << questions[i].pertanyaan << "\n\n";

        for (const auto& pilihan : questions[i].pilihanJawaban) {
            std::cout << "  " << pilihan << "\n";
        }
        std::cout << "\n";

        char playerChoice = getInputChoice("Pilihan Anda: ");

        if (playerChoice == questions[i].jawabanBenar) {
            std::cout << "\nJawaban Anda BENAR!\n";
            score += 10; // Setiap jawaban benar bernilai 10 poin
            correctAnswers[i] = true;
        } else {
            std::cout << "\nJawaban Anda SALAH. Jawaban benar adalah: " << questions[i].jawabanBenar << "\n";
        }
        std::cout << std::endl;
        printSeparator('-');
        
        // Hanya jeda jika bukan pertanyaan terakhir dan waktu belum habis
        if (i < questions.size() - 1 && !timeUp) {
            std::cout << "Tekan Enter untuk pertanyaan selanjutnya...";
            std::cin.get();
            std::cout << "\n";
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto totalDuration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);

    // --- Hasil Tes ---
    printSeparator('*');
    printCenteredText("--- HASIL TES IQ ANDA ---");
    printSeparator('*');
    std::cout << std::endl;

    if (timeUp) {
        printCenteredText("WAKTU HABIS! Tes dihentikan.");
        std::cout << "\n";
    }

    double maxScore = questions.size() * 10;
    double percentage = (static_cast<double>(score) / maxScore) * 100.0;

    std::cout << "Total Soal: " << questions.size() << "\n";
    std::cout << "Soal Dijawab: " << questionsAnswered << "\n";
    std::cout << "Total Skor Anda: " << score << " dari " << static_cast<int>(maxScore) << "\n";
    std::cout << std::fixed << std::setprecision(2); // Format persentase 2 angka desimal
    std::cout << "Persentase Skor: " << percentage << "%\n"; // Tampilan persentase
    std::cout << "Waktu yang digunakan: " << totalDuration.count() << " detik\n\n";

    // Estimasi IQ sederhana (disesuaikan untuk 25 soal, max 250 poin)
    std::string iqEstimate;
    if (percentage == 100.0) { 
        iqEstimate = "Sangat Cerdas (Estimasi IQ: 145+)";
    } else if (percentage >= 90.0) { 
        iqEstimate = "Cerdas Luar Biasa (Estimasi IQ: 130-144)";
    } else if (percentage >= 70.0) { 
        iqEstimate = "Cerdas (Estimasi IQ: 115-129)";
    } else if (percentage >= 50.0) { 
        iqEstimate = "Rata-rata (Estimasi IQ: 90-114)";
    } else {
        iqEstimate = "Perlu Latihan (Estimasi IQ: Di bawah 90)";
    }

    printCenteredText("Hasil Estimasi IQ Anda: " + iqEstimate);
    std::cout << std::endl;

    // Ringkasan jawaban
    printSeparator('-');
    printCenteredText("RINGKASAN JAWABAN");
    printSeparator('-');
    for (int i = 0; i < questions.size(); ++i) {
        // Hanya tampilkan ringkasan untuk soal yang sudah dijawab
        if (i < questionsAnswered) { 
            std::cout << "Q" << std::setw(2) << std::left << (i + 1) << ": ";
            // Ambil bagian awal pertanyaan saja agar tidak terlalu panjang
            std::string q_snippet = questions[i].pertanyaan.substr(questions[i].pertanyaan.find(". ") + 2); // Hapus "Q#. "
            q_snippet = q_snippet.substr(0, std::min((int)q_snippet.length(), TOTAL_WIDTH - 20)); // Potong agar pas

            std::cout << std::left << std::setw(TOTAL_WIDTH - 20) << q_snippet; 
            if (correctAnswers[i]) {
                std::cout << "[BENAR]\n";
            } else {
                std::cout << "[SALAH] Jawaban benar: " << questions[i].jawabanBenar << "\n";
            }
        }
    }
    printSeparator();
    std::cout << std::endl;
}

int main() {
    char playAgain;

    // Header utama aplikasi
    printSeparator('#');
    printCenteredText("===========================");
    printCenteredText("   GAME TEBAK TEST IQ!   ");
    printCenteredText("==========================="); // Header lebih ringkas
    printSeparator('#');
    std::cout << "\nSelamat datang di Tes IQ mini ini!\n";
    std::cout << "Ini hanyalah hiburan, bukan tes IQ profesional.\n\n";

    do {
        runIQTester();

        std::cout << "Apakah Anda ingin mencoba lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "\n";
        
    } while (playAgain == 'Y');

    printSeparator();
    printCenteredText("Terima kasih sudah mencoba Tes IQ ini!");
    printSeparator();
    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get(); 
    
    return 0;
}
