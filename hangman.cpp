#include <iostream>   // Untuk input/output dasar (cout, cin)
#include <string>     // Untuk std::string
#include <vector>     // Untuk std::vector (daftar kata, huruf yang dicoba)
#include <iomanip>    // Untuk manipulasi output (setw, left, right)
#include <limits>     // Untuk std::numeric_limits (untuk clear buffer)
#include <cstdlib>    // Untuk rand(), srand()
#include <ctime>      // Untuk time()
#include <algorithm>  // Untuk std::transform, std::find
#include <cctype>     // Untuk std::toupper

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

// ASCII Art untuk tampilan Hangman
void drawHangman(int wrongGuesses) {
    std::cout << "\n";
    std::cout << "   +---+\n";
    std::cout << "   |   |\n";
    std::cout << "   " << (wrongGuesses > 0 ? "O" : " ") << "   |\n"; // Kepala
    std::cout << "  " << (wrongGuesses > 2 ? "/" : " ") << (wrongGuesses > 1 ? "|" : " ") << (wrongGuesses > 3 ? "\\" : " ") << "  |\n"; // Badan & Tangan
    std::cout << "  " << (wrongGuesses > 4 ? "/" : " ") << " " << (wrongGuesses > 5 ? "\\" : " ") << "  |\n"; // Kaki
    std::cout << "       |\n";
    std::cout << "=========\n";
}

// Fungsi untuk mendapatkan input karakter dari pengguna
char getInputChar(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || !std::isalpha(value)) { // Pastikan input adalah huruf
            std::cout << "Input tidak valid. Harap masukkan satu huruf alfabet.\n";
            std::cin.clear(); // Hapus flag error
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa input
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
            return std::toupper(value); // Konversi ke huruf kapital
        }
    }
}

// --- Fungsi Game ---

void playHangman() {
    // Daftar kata yang bisa ditebak
    std::vector<std::string> words = {
        "KOMPUTER", "PROGRAMMING", "ALGORITMA", "DEVELOPER", "INTERNET",
        "KEYBOARD", "MOUSE", "MONITOR", "KONSOL", "KODE", "APLIKASI"
    };

    // Pilih kata acak dari daftar
    srand(static_cast<unsigned int>(time(0))); 
    std::string secretWord = words[rand() % words.size()];

    std::string guessedWord(secretWord.length(), '_'); // Kata yang ditampilkan ke pemain (awalnya _ _ _)
    std::vector<char> guessedLetters; // Huruf yang sudah dicoba pemain
    int wrongGuesses = 0; // Jumlah tebakan salah
    const int maxWrongGuesses = 6; // Maksimal nyawa

    bool gameWon = false;

    printSeparator();
    printCenteredText("===== MULAI BERMAIN =====");
    printSeparator();
    std::cout << "\nSaya sudah memilih sebuah kata. Tebaklah kata itu!\n";
    std::cout << "Anda punya " << maxWrongGuesses << " nyawa.\n\n";

    while (wrongGuesses < maxWrongGuesses && !gameWon) {
        // Tampilan status game
        drawHangman(wrongGuesses);
        printSeparator('-');
        std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Kata: " + guessedWord 
                  << "Nyawa Tersisa: " << (maxWrongGuesses - wrongGuesses) << std::endl;
        
        std::string triedLettersStr = "Huruf yang dicoba: ";
        for (char c : guessedLetters) {
            triedLettersStr += c;
            triedLettersStr += " ";
        }
        std::cout << std::left << std::setw(TOTAL_WIDTH) << triedLettersStr << std::endl;
        printSeparator('-');

        char guess = getInputChar("Tebak huruf: ");

        // Cek apakah huruf sudah pernah dicoba
        if (std::find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            std::cout << "\nAnda sudah mencoba huruf '" << guess << "'. Coba huruf lain!\n";
            continue; // Kembali ke awal loop
        }

        // Tambahkan huruf ke daftar yang sudah dicoba
        guessedLetters.push_back(guess);

        // Cek apakah huruf ada di kata rahasia
        bool found = false;
        for (int i = 0; i < secretWord.length(); ++i) {
            if (secretWord[i] == guess) {
                guessedWord[i] = guess;
                found = true;
            }
        }

        if (found) {
            std::cout << "\nBagus! Huruf '" << guess << "' ada di kata.\n";
        } else {
            std::cout << "\nMaaf, huruf '" << guess << "' tidak ada di kata.\n";
            wrongGuesses++;
        }

        // Cek apakah pemain sudah menebak semua huruf
        if (guessedWord == secretWord) {
            gameWon = true;
        }
        std::cout << std::endl; // Spasi setelah setiap tebakan
    }

    // Akhir permainan
    printSeparator('*');
    if (gameWon) {
        printCenteredText("SELAMAT! ANDA BERHASIL MENEBAK KATA!");
        printCenteredText("Kata rahasianya adalah: " + secretWord);
    } else {
        printCenteredText("GAMEOVER! ANDA KEHABISAN NYAWA.");
        printCenteredText("Kata rahasianya adalah: " + secretWord);
        drawHangman(maxWrongGuesses); // Gambar hangman penuh
    }
    printSeparator('*');
    std::cout << std::endl;
}

int main() {
    char playAgain;

    // Header utama game
    printSeparator('#');
    printCenteredText("------------------------");
    printCenteredText("     GAME HANGMAN!      ");
    printCenteredText("   (TEBAK KATA-KATA)    ");
    printCenteredText("------------------------");
    printSeparator('#');
    std::cout << "\nSelamat datang di game Hangman!\n";
    std::cout << "Tebak kata tersembunyi sebelum Anda kehabisan nyawa!\n\n";

    do {
        playHangman();

        std::cout << "Apakah Anda ingin bermain lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
        std::cout << std::endl; // Spasi sebelum mulai game baru atau keluar
        
    } while (playAgain == 'Y');

    printSeparator();
    printCenteredText("Terima kasih sudah bermain!");
    printSeparator();
    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get(); 
    
    return 0;
}
