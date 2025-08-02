#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

const int TOTAL_WIDTH = 75;

void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}

void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

char getInputChar(const std::string& prompt) {
    char value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan karakter.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return std::toupper(value);
        }
    }
}

void playRockPaperScissors() {
    srand(static_cast<unsigned int>(time(0)));

    int playerScore = 0;
    int computerScore = 0;
    char playerChoiceChar;
    int computerChoice;
    std::string choices[] = {"Batu", "Gunting", "Kertas"};

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

        while (playerChoiceChar != 'B' && playerChoiceChar != 'G' && playerChoiceChar != 'K') {
            std::cout << "Pilihan tidak valid. Harap masukkan B, G, atau K.\n";
            playerChoiceChar = getInputChar("Pilihan Anda (B/G/K): ");
        }

        int playerChoice;
        if (playerChoiceChar == 'B') playerChoice = 0;
        else if (playerChoiceChar == 'G') playerChoice = 1;
        else playerChoice = 2;

        computerChoice = rand() % 3;

        std::cout << "\nAnda memilih: " << choices[playerChoice] << std::endl;

        std::cout << "Komputer sedang memilih... ";
        for (int i = 0; i < 3; ++i) {
            std::cout << ". ";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        std::cout << "\nKomputer memilih: " << choices[computerChoice] << std::endl;

        printSeparator('-');
        if (playerChoice == computerChoice) {
            printCenteredText("HASIL: SERI!");
        } else if ((playerChoice == 0 && computerChoice == 1) ||
                   (playerChoice == 1 && computerChoice == 2) ||
                   (playerChoice == 2 && computerChoice == 0)) {
            printCenteredText("HASIL: ANDA MENANG!");
            playerScore++;
        } else {
            printCenteredText("HASIL: KOMPUTER MENANG!");
            computerScore++;
        }
        printSeparator('-');

        std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Anda:"
                  << std::right << playerScore << std::endl;
        std::cout << std::left << std::setw(TOTAL_WIDTH / 2) << "Skor Komputer:"
                  << std::right << computerScore << std::endl;
        printSeparator('=');

        char playAgain;
        std::cout << "Main lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (playAgain == 'T') {
            break;
        }
        std::cout << "\n\n";

    } while (true);

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
    printSeparator('#');
    printCenteredText("===========================");
    printCenteredText("  BATU, GUNTING, KERTAS!   ");
    printCenteredText("===========================");
    printSeparator('#');
    std::cout << "\nSelamat datang di game Batu, Gunting, Kertas!\n";
    std::cout << "Kalahkan komputer dalam duel kecerdikan!\n\n";

    playRockPaperScissors();

    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get();

    return 0;
}
