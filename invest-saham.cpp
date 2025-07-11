#include <iostream>   // Input/Output
#include <vector>     // std::vector
#include <string>     // std::string, std::getline
#include <iomanip>    // std::fixed, std::setprecision, std::setw, std::left/right
#include <limits>     // std::numeric_limits (untuk membersihkan buffer)
#include <random>     // Untuk generator angka acak yang lebih baik (mt19937, uniform_real_distribution)
#include <algorithm>  // Untuk std::remove_if

// --- Struktur Data untuk Saham Perusahaan ---
struct CompanyStock {
    std::string symbol; // Contoh: "AAPL", "GOOG"
    std::string name;   // Contoh: "Apple Inc."
    double currentPrice;
};

// --- Struktur Data untuk Portofolio Pemain (Saham yang Dimiliki) ---
struct OwnedStock {
    std::string symbol;
    int quantity;
    double averageBuyPrice; // Harga beli rata-rata per saham
};

// --- Struktur Data untuk Riwayat Transaksi ---
struct Transaction {
    std::string type; // "Beli" atau "Jual"
    std::string symbol;
    int quantity;
    double pricePerShare;
    double totalAmount;
    int day; // Hari ke berapa transaksi terjadi
};

// --- Generator Angka Acak yang Lebih Baik ---
std::mt19937 rng(static_cast<unsigned int>(std::time(0))); // Seed generator dengan waktu saat ini

// --- Fungsi untuk Membersihkan Buffer Input ---
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Fungsi untuk Mendapatkan Input Angka Integer yang Valid ---
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

// --- Fungsi untuk Mendapatkan Input Angka Double yang Valid ---
double getValidDoubleInput(const std::string& prompt, double minVal = 0.0, double maxVal = std::numeric_limits<double>::max()) {
    double value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cout << "Input tidak valid. Mohon masukkan angka (desimal) antara " << minVal << " dan " << maxVal << "." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

// --- Fungsi untuk Menampilkan Daftar Saham yang Tersedia ---
void displayMarket(const std::vector<CompanyStock>& stocks) {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   PASAR SAHAM SAAT INI                   " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::cout << std::left << std::setw(5) << "No."
              << std::left << std::setw(10) << "Symbol"
              << std::left << std::setw(25) << "Nama Perusahaan"
              << std::left << std::setw(15) << "Harga/Saham" << std::endl;
    std::cout << "----------------------------------------------------------" << std::endl;
    for (size_t i = 0; i < stocks.size(); ++i) {
        std::cout << std::left << std::setw(5) << i + 1 << ".";
        std::cout << std::left << std::setw(10) << stocks[i].symbol;
        std::cout << std::left << std::setw(25) << stocks[i].name;
        std::cout << "Rp " << std::fixed << std::setprecision(2) << stocks[i].currentPrice << std::endl;
    }
    std::cout << "==========================================================\n" << std::endl;
}

// --- Fungsi untuk Menampilkan Portofolio Pemain ---
void displayPortfolio(const std::vector<OwnedStock>& portfolio, const std::vector<CompanyStock>& marketStocks) {
    std::cout << "\n=====================================================================" << std::endl;
    std::cout << "                          PORTOFOLIO ANDA                            " << std::endl;
    std::cout << "=====================================================================" << std::endl;
    if (portfolio.empty()) {
        std::cout << "Anda belum memiliki saham apapun. Ayo mulai berinvestasi!" << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "No."
                  << std::left << std::setw(10) << "Symbol"
                  << std::left << std::setw(10) << "Qty"
                  << std::left << std::setw(18) << "Harga Beli Rata-rata"
                  << std::left << std::setw(15) << "Harga Terkini"
                  << std::left << std::setw(15) << "Untung/Rugi" << std::endl;
        std::cout << "---------------------------------------------------------------------" << std::endl;
        for (size_t i = 0; i < portfolio.size(); ++i) {
            double currentPrice = 0.0;
            // Cari harga terkini dari saham di pasar
            for (const auto& ms : marketStocks) {
                if (ms.symbol == portfolio[i].symbol) {
                    currentPrice = ms.currentPrice;
                    break;
                }
            }
            double profitLoss = (currentPrice - portfolio[i].averageBuyPrice) * portfolio[i].quantity;

            std::cout << std::left << std::setw(5) << i + 1 << ".";
            std::cout << std::left << std::setw(10) << portfolio[i].symbol;
            std::cout << std::left << std::setw(10) << portfolio[i].quantity;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << std::setw(15) << portfolio[i].averageBuyPrice;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << std::setw(12) << currentPrice;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << profitLoss << std::endl;
        }
    }
    std::cout << "=====================================================================\n" << std::endl;
}

// --- Fungsi untuk Menampilkan Riwayat Transaksi ---
void displayTransactionHistory(const std::vector<Transaction>& history) {
    std::cout << "\n==================================================================================" << std::endl;
    std::cout << "                             RIWAYAT TRANSAKSI                                    " << std::endl;
    std::cout << "==================================================================================" << std::endl;
    if (history.empty()) {
        std::cout << "Belum ada transaksi tercatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "Hari"
                  << std::left << std::setw(10) << "Tipe"
                  << std::left << std::setw(10) << "Symbol"
                  << std::left << std::setw(10) << "Qty"
                  << std::left << std::setw(15) << "Harga/Saham"
                  << std::left << std::setw(18) << "Total Amount" << std::endl;
        std::cout << "----------------------------------------------------------------------------------" << std::endl;
        for (const auto& tr : history) {
            std::cout << std::left << std::setw(5) << tr.day;
            std::cout << std::left << std::setw(10) << tr.type;
            std::cout << std::left << std::setw(10) << tr.symbol;
            std::cout << std::left << std::setw(10) << tr.quantity;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << std::setw(12) << tr.pricePerShare;
            std::cout << "Rp " << std::fixed << std::setprecision(2) << tr.totalAmount << std::endl;
        }
    }
    std::cout << "==================================================================================\n" << std::endl;
}


// --- Fungsi Utama Program ---
int main() {
    // --- Data Awal ---
    double playerCash = 10000000.00; // Modal awal pemain: Rp 10.000.000
    std::vector<CompanyStock> marketStocks = {
        {"TECH", "Teknologi Maju", 10000.00},
        {"FINA", "Keuangan Prima", 8000.00},
        {"CONS", "Konsumsi Rakyat", 5000.00},
        {"ENER", "Energi Hijau", 12000.00},
        {"HEAL", "Kesehatan Sejahtera", 7500.00}
    };
    std::vector<OwnedStock> playerPortfolio;
    std::vector<Transaction> transactionHistory;
    int currentDay = 0; // Menghitung hari simulasi

    // Untuk fluktuasi harga saham
    std::uniform_real_distribution<> distrib(-0.05, 0.05); // Harga bisa naik/turun 5%

    int pilihan;
    char lanjutSimulasi;

    // --- Sambutan Awal ---
    std::cout << "**********************************************************" << std::endl;
    std::cout << "*   SELAMAT DATANG DI VIRTUAL STOCK MARKET SIMULATOR!    *" << std::endl;
    std::cout << "*  Mulai petualangan investasimu tanpa risiko sungguhan. *" << std::endl;
    std::cout << "**********************************************************" << std::endl;

    do {
        currentDay++;
        std::cout << "\n-----------------------------------------------------------" << std::endl;
        std::cout << "                  HARI PERDAGANGAN KE-" << currentDay << std::endl;
        std::cout << "-----------------------------------------------------------" << std::endl;

        // --- Fluktuasi Harga Saham ---
        for (auto& stock : marketStocks) {
            double changePercentage = distrib(rng); // Acak perubahan harga
            stock.currentPrice *= (1.0 + changePercentage);
            // Batasi harga minimum agar tidak menjadi nol atau negatif
            if (stock.currentPrice < 100.00) {
                stock.currentPrice = 100.00; // Harga minimum Rp 100
            }
        }

        // --- Tampilkan Ringkasan Keuangan ---
        double totalPortfolioValue = 0.0;
        for (const auto& owned : playerPortfolio) {
            for (const auto& market : marketStocks) {
                if (owned.symbol == market.symbol) {
                    totalPortfolioValue += (owned.quantity * market.currentPrice);
                    break;
                }
            }
        }
        double netWorth = playerCash + totalPortfolioValue;

        std::cout << "\n------------- Ringkasan Keuangan Anda ------------" << std::endl;
        std::cout << std::left << std::setw(20) << "Kas Tersedia" << ": Rp " << std::fixed << std::setprecision(2) << playerCash << std::endl;
        std::cout << std::left << std::setw(20) << "Nilai Portofolio" << ": Rp " << std::fixed << std::setprecision(2) << totalPortfolioValue << std::endl;
        std::cout << std::left << std::setw(20) << "Total Kekayaan Bersih" << ": Rp " << std::fixed << std::setprecision(2) << netWorth << std::endl;
        std::cout << "--------------------------------------------------\n" << std::endl;


        std::cout << "----------------- MENU INVESTASI -----------------" << std::endl;
        std::cout << "1. Lihat Pasar Saham" << std::endl;
        std::cout << "2. Beli Saham" << std::endl;
        std::cout << "3. Jual Saham" << std::endl;
        std::cout << "4. Lihat Portofolio Anda" << std::endl;
        std::cout << "5. Lihat Riwayat Transaksi" << std::endl;
        std::cout << "6. Lanjutkan ke Hari Berikutnya (Akhiri Putaran)" << std::endl;
        std::cout << "7. Keluar dari Simulator" << std::endl;
        pilihan = getValidIntegerInput("Pilih opsi (1-7): ", 1, 7);

        switch (pilihan) {
            case 1: { // Lihat Pasar Saham
                displayMarket(marketStocks);
                break;
            }
            case 2: { // Beli Saham
                displayMarket(marketStocks);
                int stockIndex = getValidIntegerInput("Masukkan nomor saham yang ingin Anda beli: ", 1, marketStocks.size());
                CompanyStock& selectedStock = marketStocks[stockIndex - 1];

                int quantity = getValidIntegerInput("Berapa lembar saham " + selectedStock.symbol + " yang ingin Anda beli? ", 1);
                double cost = quantity * selectedStock.currentPrice;

                if (playerCash >= cost) {
                    playerCash -= cost;

                    // Update portfolio
                    bool found = false;
                    for (auto& owned : playerPortfolio) {
                        if (owned.symbol == selectedStock.symbol) {
                            // Hitung harga beli rata-rata baru
                            owned.averageBuyPrice = ((owned.averageBuyPrice * owned.quantity) + cost) / (owned.quantity + quantity);
                            owned.quantity += quantity;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        playerPortfolio.push_back({selectedStock.symbol, quantity, selectedStock.currentPrice});
                    }

                    // Catat transaksi
                    transactionHistory.push_back({"Beli", selectedStock.symbol, quantity, selectedStock.currentPrice, cost, currentDay});
                    std::cout << "✅ Anda berhasil membeli " << quantity << " lembar saham " << selectedStock.symbol << "." << std::endl;
                } else {
                    std::cout << "Maaf, kas Anda tidak cukup untuk membeli saham ini." << std::endl;
                }
                break;
            }
            case 3: { // Jual Saham
                displayPortfolio(playerPortfolio, marketStocks);
                if (playerPortfolio.empty()) {
                    std::cout << "Anda tidak memiliki saham untuk dijual." << std::endl;
                    break;
                }

                int portfolioIndex = getValidIntegerInput("Masukkan nomor saham dari portofolio Anda yang ingin Anda jual: ", 1, playerPortfolio.size());
                OwnedStock& ownedStock = playerPortfolio[portfolioIndex - 1];
                
                int quantityToSell = getValidIntegerInput("Berapa lembar saham " + ownedStock.symbol + " yang ingin Anda jual? (Anda memiliki " + std::to_string(ownedStock.quantity) + "): ", 1, ownedStock.quantity);
                
                double currentStockPrice = 0.0;
                for (const auto& ms : marketStocks) {
                    if (ms.symbol == ownedStock.symbol) {
                        currentStockPrice = ms.currentPrice;
                        break;
                    }
                }

                double proceeds = quantityToSell * currentStockPrice;
                playerCash += proceeds;
                ownedStock.quantity -= quantityToSell;

                // Catat transaksi
                transactionHistory.push_back({"Jual", ownedStock.symbol, quantityToSell, currentStockPrice, proceeds, currentDay});
                std::cout << "✅ Anda berhasil menjual " << quantityToSell << " lembar saham " << ownedStock.symbol << " seharga Rp " << std::fixed << std::setprecision(2) << proceeds << "." << std::endl;

                // Hapus saham dari portofolio jika kuantitasnya 0
                playerPortfolio.erase(std::remove_if(playerPortfolio.begin(), playerPortfolio.end(),
                                                     [](const OwnedStock& s){ return s.quantity == 0; }),
                                      playerPortfolio.end());
                break;
            }
            case 4: { // Lihat Portofolio
                displayPortfolio(playerPortfolio, marketStocks);
                break;
            }
            case 5: { // Lihat Riwayat Transaksi
                displayTransactionHistory(transactionHistory);
                break;
            }
            case 6: { // Lanjutkan ke Hari Berikutnya
                std::cout << "\nMelanjutkan ke hari perdagangan berikutnya..." << std::endl;
                lanjutSimulasi = 'Y'; // Set agar loop utama berjalan lagi
                break;
            }
            case 7: { // Keluar
                lanjutSimulasi = 'N'; // Set agar loop utama berhenti
                std::cout << "\n***********************************************************" << std::endl;
                std::cout << "* TERIMA KASIH TELAH BERMAIN VIRTUAL STOCK MARKET!        *" << std::endl;
                std::cout << "* Kekayaan Akhir Anda: Rp " << std::fixed << std::setprecision(2) << netWorth << " *" << std::endl;
                std::cout << "***********************************************************" << std::endl;
                break;
            }
        }

        // Jika bukan opsi Lanjutkan Hari atau Keluar, tanyakan ingin lanjut atau tidak
        if (pilihan != 6 && pilihan != 7) {
            std::cout << "\nLanjutkan ke hari berikutnya atau lakukan aksi lain? (Y/N): ";
            std::cin >> lanjutSimulasi;
            clearInputBuffer();
        }

    } while (lanjutSimulasi == 'Y' || lanjutSimulasi == 'y');

    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
