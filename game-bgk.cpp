#include <iostream>   // Untuk input/output dasar (cout, cin)
#include <string>     // Untuk std::string
#include <iomanip>    // Untuk manipulasi output (setw, left, right)
#include <limits>     // Untuk std::numeric_limits (untuk clear buffer)
#include <cstdlib>    // Untuk rand(), srand()
#include <ctime>      // Untuk time()
#include <thread>     // Untuk std::this_thread::sleep_for
#include <chrono>     // Untuk std::chrono::milliseconds

// --- Konfigurasi Tampilan ---
const int TOTAL_WIDTH = 75; // Lebar total tampilan konsol

// Fungsi untuk mencetak garis pemisah
void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}

// Fungsi untuk mencetak teks di tengah
void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

// Fungsi untuk mendapatkan input karakter dari pengguna
char getInputChar(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan karakter.\n";
            std::cin.clear(); // Hapus flag error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa input
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
            return std::toupper(value); // Konversi ke huruf kapital
        }
    }
}

// --- Fungsi Game ---

void playRockPaperScissors() {
    // Inisialisasi generator angka acak
    srand(static_cast<unsigned int>(time(0))); 

    int playerScore = 0;
    int computerScore = 0;
    char playerChoiceChar;
    int computerChoice; // 0=Batu, 1=Gunting, 2=Kertas
    std::string choices[] = {"Batu", "Gunting", "Kertas"}; // Untuk mapping pilihan angka ke string

    do {
        printSeparator('-');
        printCenteredText("RONDE BARU");
        printSeparator('-');
        std::cout << "\nMasukkan pilihan Anda:\n";
        std::cout << "  (B)atu\n";
        std::cout << "  (G)unting\n";
        std::cout << "  (K)ertas\n";
        printSeparator('-');

        playerChoiceChar = getInputChar("Pilihan Anda (B/G/K): ");

        // Validasi pilihan pemain
        while (playerChoiceChar != 'B' && playerChoiceChar != 'G' && playerChoiceChar != 'K') {
            std::cout << "Pilihan tidak valid. Harap masukkan B, G, atau K.\n";
            playerChoiceChar = getInputChar("Pilihan Anda (B/G/K): ");
        }

        // Konversi pilihan pemain ke angka (0=Batu, 1=Gunting, 2=Kertas)
        int playerChoice;
        if (playerChoiceChar == 'B') playerChoice = 0;
        else if (playerChoiceChar == 'G') playerChoice = 1;
        else playerChoice = 2; // Kertas

        // Pilihan Komputer
        computerChoice = rand() % 3; // Angka acak 0, 1, atau 2

        std::cout << "\nAnda memilih: " << choices[playerChoice] << std::endl;

        // Simulasi "suit"
        std::cout << "Komputer sedang memilih... ";
        for (int i = 0; i < 3; ++i) {
            std::cout << ". ";
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Jeda 0.5 detik
        }
        std::cout << "\nKomputer memilih: " << choices[computerChoice] << std::endl;

        printSeparator('-');
        // Menentukan pemenang
        if (playerChoice == computerChoice) {
            printCenteredText("HASIL: SERI!");
        } else if ((playerChoice == 0 && computerChoice == 1) || // Batu vs Gunting
                   (playerChoice == 1 && computerChoice == 2) || // Gunting vs Kertas
                   (playerChoice == 2 && computerChoice == 0)) { // Kertas vs Batu
            printCenteredText("HASIL: ANDA MENANG!");
            playerScore++;
        } else {
            printCenteredText("HASIL: KOMPUTER MENANG!");
            computerScore++;
        }
        printSeparator('-');

        // Tampilkan skor saat ini
        std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Anda:" 
                  << std::right << playerScore << std::endl;
        std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Komputer:" 
                  << std::right << computerScore << std::endl;
        printSeparator('=');

        char playAgain;
        std::cout << "Main lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
        
        if (playAgain == 'T') {
            break; // Keluar dari loop permainan
        }
        std::cout << "\n\n"; // Spasi sebelum ronde baru

    } while (true); // Loop akan berhenti jika pemain memilih 'T'

    printSeparator();
    printCenteredText("--- SKOR AKHIR ---");
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Anda:" 
              << std::right << playerScore << std::endl;
    std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Komputer:" 
              << std::right << computerScore << std::endl;
    printSeparator();
    
    if (playerScore > computerScore) {
        printCenteredText("SELAMAT! ANDA PEMENANGNYA!");
    } else if (computerScore > playerScore) {
        printCenteredText("KOMPUTER MEMENANGKAN PERTANDINGAN!");
    } else {
        printCenteredText("PERTANDINGAN SERI!");
    }
    printSeparator();
}

int main() {
    // Header utama game
    printSeparator('#');
    printCenteredText("===========================");
    printCenteredText("  BATU, GUNTING, KERTAS!   ");
    printCenteredText("===========================");
    printSeparator('#');
    std::cout << "\nSelamat datang di game Batu, Gunting, Kertas!\n";
    std::cout << "Kalahkan komputer dalam duel kecerdikan!\n\n";

    // Panggil fungsi game
    playRockPaperScissors();

    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get(); // Menunggu Enter terakhir sebelum menutup konsol
    
    return 0;
}
