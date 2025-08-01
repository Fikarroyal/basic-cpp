#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <cctype>

const int TOTAL_WIDTH = 50;
void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}
void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}
void displayBoard(const std::vector<char>& board) {
    std::cout << "\n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "     |     |     \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "  " << board[0] << "  |  " << board[1] << "  |  " << board[2] << "  \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "_____|_____|_____\n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "     |     |     \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "  " << board[3] << "  |  " << board[4] << "  |  " << board[5] << "  \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "_____|_____|_____\n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "     |     |     \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "  " << board[6] << "  |  " << board[7] << "  |  " << board[8] << "  \n";
    std::cout << std::string(TOTAL_WIDTH / 2 - 8, ' ') << "     |     |     \n";
    std::cout << "\n";
}
int getPlayerMove(const std::vector<char>& board, char playerSymbol) {
    int move;
    while (true) {
        std::cout << "Pemain " << playerSymbol << ", masukkan posisi (1-9): ";
        std::cin >> move;

        if (std::cin.fail() || move < 1 || move > 9) {
            std::cout << "Input tidak valid. Harap masukkan angka antara 1 dan 9.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (board[move - 1] != (char)('1' + move - 1)) { 
            std::cout << "Posisi itu sudah terisi. Coba posisi lain.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return move - 1;
        }
    }
}
char checkWinner(const std::vector<char>& board) {
    for (int i = 0; i < 9; i += 3) {
        if (board[i] == board[i+1] && board[i+1] == board[i+2]) return board[i];
    }
    for (int i = 0; i < 3; ++i) {
        if (board[i] == board[i+3] && board[i+3] == board[i+6]) return board[i];
    }
    if (board[0] == board[4] && board[4] == board[8]) return board[0];
    if (board[2] == board[4] && board[4] == board[6]) return board[2];
    bool isDraw = true;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == (char)('1' + i)) {
            isDraw = false;
            break;
        }
    }
    if (isDraw) return 'D';

    return ' ';
}
void playTicTacToe() {
    char currentPlayer = 'X';
    std::vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char winner = ' ';

    do {
        displayBoard(board);
        int move = getPlayerMove(board, currentPlayer);
        board[move] = currentPlayer;

        winner = checkWinner(board);

        if (winner == ' ') {
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

    } while (winner == ' ');
    displayBoard(board);

    printSeparator('*');
    if (winner == 'D') {
        printCenteredText("PERTANDINGAN SERI!");
    } else {
        printCenteredText("SELAMAT! PEMAIN " + std::string(1, winner) + " MENANG!");
    }
    printSeparator('*');
    std::cout << "\n";
}

int main() {
    char playAgain;
    int playerX_wins = 0;
    int playerO_wins = 0;
    int draws = 0;
    printSeparator('#');
    printCenteredText("=====================");
    printCenteredText("    TIC-TAC-TOE!     ");
    printCenteredText("     (X dan O)       ");
    printCenteredText("=====================");
    printSeparator('#');
    std::cout << "\nSelamat datang di game Tic-Tac-Toe!\n";
    std::cout << "Kalahkan lawanmu dengan mensejajarkan 3 simbolmu!\n";
    std::cout << "Pemain X memulai, lalu Pemain O.\n";
    std::cout << "Gunakan angka 1-9 untuk memilih posisi di papan.\n\n";

    do {
        playTicTacToe();
        std::cout << "Apakah Anda ingin bermain lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "\n";

    } while (playAgain == 'Y');

    printSeparator();
    printCenteredText("Terima kasih sudah bermain Tic-Tac-Toe!");
    printSeparator();
    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get(); 
    
    return 0;
}
