#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>

const int TOTAL_WIDTH = 75;

void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}

void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

void drawHangman(int wrongGuesses) {
    std::cout << "\n";
    std::cout << "    +---+\n";
    std::cout << "    |   |\n";
    std::cout << "    " << (wrongGuesses > 0 ? "O" : " ") << "   |\n";
    std::cout << "  " << (wrongGuesses > 2 ? "/" : " ") << (wrongGuesses > 1 ? "|" : " ") << (wrongGuesses > 3 ? "\\" : " ") << "  |\n";
    std::cout << "  " << (wrongGuesses > 4 ? "/" : " ") << " " << (wrongGuesses > 5 ? "\\" : " ") << "  |\n";
    std::cout << "      |\n";
    std::cout << "=========\n";
}

char getInputChar(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || !std::isalpha(value)) {
            std::cout << "Input tidak valid. Harap masukkan satu huruf alfabet.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return std::toupper(value);
        }
    }
}

void playHangman() {
    std::vector<std::string> words = {
        "KOMPUTER", "PROGRAMMING", "ALGORITMA", "DEVELOPER", "INTERNET",
        "KEYBOARD", "MOUSE", "MONITOR", "KONSOL", "KODE", "APLIKASI"
    };

    srand(static_cast<unsigned int>(time(0)));
    std::string secretWord = words[rand() % words.size()];

    std::string guessedWord(secretWord.length(), '_');
    std::vector<char> guessedLetters;
    int wrongGuesses = 0;
    const int maxWrongGuesses = 6;

    bool gameWon = false;

    printSeparator();
    printCenteredText("===== MULAI BERMAIN =====");
    printSeparator();
    std::cout << "\nSaya sudah memilih sebuah kata. Tebaklah kata itu!\n";
    std::cout << "Anda punya " << maxWrongGuesses << " nyawa.\n\n";

    while (wrongGuesses < maxWrongGuesses && !gameWon) {
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

        if (std::find(guessedLetters.begin(), guessedLetters.end(), guess) != guessedLetters.end()) {
            std::cout << "\nAnda sudah mencoba huruf '" << guess << "'. Coba huruf lain!\n";
            continue;
        }

        guessedLetters.push_back(guess);

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

        if (guessedWord == secretWord) {
            gameWon = true;
        }
        std::cout << std::endl;
    }

    printSeparator('*');
    if (gameWon) {
        printCenteredText("SELAMAT! ANDA BERHASIL MENEBAK KATA!");
        printCenteredText("Kata rahasianya adalah: " + secretWord);
    } else {
        printCenteredText("GAMEOVER! ANDA KEHABISAN NYAWA.");
        printCenteredText("Kata rahasianya adalah: " + secretWord);
        drawHangman(maxWrongGuesses);
    }
    printSeparator('*');
    std::cout << std::endl;
}

int main() {
    char playAgain;

    printSeparator('#');
    printCenteredText("------------------------");
    printCenteredText("      GAME HANGMAN!      ");
    printCenteredText("    (TEBAK KATA-KATA)    ");
    printCenteredText("------------------------");
    printSeparator('#');
    std::cout << "\nSelamat datang di game Hangman!\n";
    std::cout << "Tebak kata tersembunyi sebelum Anda kehabisan nyawa!\n\n";

    do {
        playHangman();

        std::cout << "Apakah Anda ingin bermain lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << std::endl;

    } while (playAgain == 'Y');

    printSeparator();
    printCenteredText("Terima kasih sudah bermain!");
    printSeparator();
    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get();

    return 0;
}
