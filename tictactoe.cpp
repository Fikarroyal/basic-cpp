#include <iostream>   // Untuk input/output dasar (cout, cin)
#include <vector>     // Untuk menyimpan papan permainan
#include <string>     // Untuk std::string
#include <iomanip>    // Untuk manipulasi output (setw, left, right)
#include <limits>     // Untuk std::numeric_limits (untuk clear buffer)
#include <algorithm>  // Untuk std::transform (toupper)
#include <cctype>     // Untuk std::toupper

// --- Konfigurasi Tampilan ---
const int TOTAL_WIDTH = 50; // Lebar total tampilan konsol (disesuaikan untuk papan Tic-Tac-Toe)

// Fungsi untuk mencetak garis pemisah
void printSeparator(char c = '=', int length = TOTAL_WIDTH) {
    std::cout << std::string(length, c) << std::endl;
}

// Fungsi untuk mencetak teks di tengah
void printCenteredText(const std::string& text) {
    int padding = (TOTAL_WIDTH - text.length()) / 2;
    std::cout << std::string(padding, ' ') << text << std::string(padding, ' ') << std::endl;
}

// Fungsi untuk menampilkan papan Tic-Tac-Toe
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

// Fungsi untuk mendapatkan input posisi dari pemain
int getPlayerMove(const std::vector<char>& board, char playerSymbol) {
    int move;
    while (true) {
        std::cout << "Pemain " << playerSymbol << ", masukkan posisi (1-9): ";
        std::cin >> move;

        if (std::cin.fail() || move < 1 || move > 9) {
            std::cout << "Input tidak valid. Harap masukkan angka antara 1 dan 9.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (board[move - 1] != (char)('1' + move - 1)) { // Cek apakah kotak sudah terisi
            std::cout << "Posisi itu sudah terisi. Coba posisi lain.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Pastikan buffer bersih
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Buang sisa newline
            return move - 1; // Mengembalikan indeks array (0-8)
        }
    }
}

// Fungsi untuk mengecek pemenang
char checkWinner(const std::vector<char>& board) {
    // Cek baris
    for (int i = 0; i < 9; i += 3) {
        if (board[i] == board[i+1] && board[i+1] == board[i+2]) return board[i];
    }
    // Cek kolom
    for (int i = 0; i < 3; ++i) {
        if (board[i] == board[i+3] && board[i+3] == board[i+6]) return board[i];
    }
    // Cek diagonal
    if (board[0] == board[4] && board[4] == board[8]) return board[0];
    if (board[2] == board[4] && board[4] == board[6]) return board[2];

    // Cek seri (jika semua kotak terisi dan tidak ada pemenang)
    bool isDraw = true;
    for (int i = 0; i < 9; ++i) {
        if (board[i] == (char)('1' + i)) { // Jika masih ada angka (kotak kosong)
            isDraw = false;
            break;
        }
    }
    if (isDraw) return 'D'; // 'D' untuk Draw (Seri)

    return ' '; // Belum ada pemenang
}

// --- Fungsi Game ---
void playTicTacToe() {
    char currentPlayer = 'X';
    std::vector<char> board = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    char winner = ' '; // 'X', 'O', 'D' (Draw), atau ' ' (belum ada)

    do {
        displayBoard(board);
        int move = getPlayerMove(board, currentPlayer);
        board[move] = currentPlayer;

        winner = checkWinner(board);

        if (winner == ' ') { // Jika belum ada pemenang, ganti giliran
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }

    } while (winner == ' '); // Lanjutkan selama belum ada pemenang

    // Permainan berakhir
    displayBoard(board); // Tampilkan papan terakhir

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

    // Header utama game
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
        playTicTacToe(); // Jalankan satu ronde permainan
        
        // Update skor (ini bisa diperluas di sini jika mau melacak skor antar ronde)
        // Saat ini, skor hanya untuk satu ronde. Jika mau melacak total,
        // fungsi playTicTacToe harus mengembalikan siapa pemenangnya.
        // Untuk kesederhanaan, saya akan tambahkan variabel skor global atau dikembalikan dari playTicTacToe
        // Untuk sekarang, anggap ini satu permainan selesai.

        std::cout << "Apakah Anda ingin bermain lagi? (Y/T): ";
        std::cin >> playAgain;
        playAgain = std::toupper(playAgain); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        std::cout << "\n";
        
        // Reset papan jika bermain lagi
        // (Ini sudah ditangani karena board diinisialisasi ulang di setiap panggilan playTicTacToe)

    } while (playAgain == 'Y');

    printSeparator();
    printCenteredText("Terima kasih sudah bermain Tic-Tac-Toe!");
    printSeparator();
    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.get(); 
    
    return 0;
}
