#include <iostream>   
#include <string>     
#include <iomanip>    
#include <limits>     
#include <cstdlib>    
#include <ctime>   

const int TOTAL_WIDTH = 70;
void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}
void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

int getInputInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cout << "Input tidak valid. Harap masukkan angka.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

void playGuessingGame() {
    srand(static_cast<unsigned int>(time(0))); 

    int minNum = 1;
    int maxNum = 100;
    int targetNumber = rand() % (maxNum - minNum + 1) + minNum;
    int guess;
    int attempts = 0;
    bool hasGuessedCorrectly = false;

    printSeparator();
    printCenteredText("===== MULAI BERMAIN =====");
    printSeparator();
    std::cout << "\nSaya sudah memilih sebuah angka antara " << minNum << " dan " << maxNum << ".\n";
    std::cout << "Coba tebak angka itu!\n\n";

    while (!hasGuessedCorrectly) {
        attempts++;
        guess = getInputInt("Tebakan Anda (" + std::to_string(attempts) + "): ");

        if (guess < minNum || guess > maxNum) {
            std::cout << "Angka Anda di luar jangkauan " << minNum << " - " << maxNum << ". Coba lagi!\n";
        } else if (guess < targetNumber) {
            std::cout << "Terlalu rendah! Coba angka yang lebih besar.\n";
        } else if (guess > targetNumber) {
            std::cout << "Terlalu tinggi! Coba angka yang lebih kecil.\n";
        } else {
            hasGuessedCorrectly = true;
            printSeparator('*');
            std::cout << std::endl;
            printCenteredText("SELAMAT! TEBAKAN ANDA BENAR!");
            std::cout << std::endl;
            printCenteredText("Anda berhasil menebak angka " + std::to_string(targetNumber) + " dalam " + std::to_string(attempts) + " percobaan.");
            std::cout << std::endl;
            printSeparator('*');
        }
        std::cout << std::endl;
    }
}

int main() {
    char playAgain;
    printSeparator('=');
    printCenteredText("-------------------------");
    printCenteredText("  GAME TEBAK-TEBAK ANGKA   ");
    printCenteredText("-------------------------");
    printSeparator('=');
    std::cout << std::endl;

    do {
        playGuessingGame();

        std::cout << "Apakah Anda ingin bermain lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = toupper(playAgain); 
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
