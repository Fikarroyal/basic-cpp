#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <random>
#include <algorithm>

struct CompanyStock {
    std::string symbol;
    std::string name;
    double currentPrice;
};

struct OwnedStock {
    std::string symbol;
    int quantity;
    double averageBuyPrice;
};

struct Transaction {
    std::string type;
    std::string symbol;
    int quantity;
    double pricePerShare;
    double totalAmount;
    int day;
};

std::mt19937 rng(static_cast<unsigned int>(std::time(0)));

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

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

void displayMarket(const std::vector<CompanyStock>& stocks) {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                     PASAR SAHAM SAAT INI                       " << std::endl;
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

void displayPortfolio(const std::vector<OwnedStock>& portfolio, const std::vector<CompanyStock>& marketStocks) {
    std::cout << "\n=====================================================================" << std::endl;
    std::cout << "                            PORTOFOLIO ANDA                           " << std::endl;
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

void displayTransactionHistory(const std::vector<Transaction>& history) {
    std::cout << "\n==================================================================================" << std::endl;
    std::cout << "                               RIWAYAT TRANSAKSI                                " << std::endl;
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


int main() {
    double playerCash = 10000000.00;
    std::vector<CompanyStock> marketStocks = {
        {"TECH", "Teknologi Maju", 10000.00},
        {"FINA", "Keuangan Prima", 8000.00},
        {"CONS", "Konsumsi Rakyat", 5000.00},
        {"ENER", "Energi Hijau", 12000.00},
        {"HEAL", "Kesehatan Sejahtera", 7500.00}
    };
    std::vector<OwnedStock> playerPortfolio;
    std::vector<Transaction> transactionHistory;
    int currentDay = 0;

    std::uniform_real_distribution<> distrib(-0.05, 0.05);

    int pilihan;
    char lanjutSimulasi;

    std::cout << "**********************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI VIRTUAL STOCK MARKET SIMULATOR!    *" << std::endl;
    std::cout << "* Mulai petualangan investasimu tanpa risiko sungguhan. *" << std::endl;
    std::cout << "**********************************************************" << std::endl;

    do {
        currentDay++;
        std::cout << "\n-----------------------------------------------------------" << std::endl;
        std::cout << "                   HARI PERDAGANGAN KE-" << currentDay << std::endl;
        std::cout << "-----------------------------------------------------------" << std::endl;

        for (auto& stock : marketStocks) {
            double changePercentage = distrib(rng);
            stock.currentPrice *= (1.0 + changePercentage);
            if (stock.currentPrice < 100.00) {
                stock.currentPrice = 100.00;
            }
        }

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
            case 1: {
                displayMarket(marketStocks);
                break;
            }
            case 2: {
                displayMarket(marketStocks);
                int stockIndex = getValidIntegerInput("Masukkan nomor saham yang ingin Anda beli: ", 1, marketStocks.size());
                CompanyStock& selectedStock = marketStocks[stockIndex - 1];

                int quantity = getValidIntegerInput("Berapa lembar saham " + selectedStock.symbol + " yang ingin Anda beli? ", 1);
                double cost = quantity * selectedStock.currentPrice;

                if (playerCash >= cost) {
                    playerCash -= cost;

                    bool found = false;
                    for (auto& owned : playerPortfolio) {
                        if (owned.symbol == selectedStock.symbol) {
                            owned.averageBuyPrice = ((owned.averageBuyPrice * owned.quantity) + cost) / (owned.quantity + quantity);
                            owned.quantity += quantity;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        playerPortfolio.push_back({selectedStock.symbol, quantity, selectedStock.currentPrice});
                    }

                    transactionHistory.push_back({"Beli", selectedStock.symbol, quantity, selectedStock.currentPrice, cost, currentDay});
                    std::cout << "✅ Anda berhasil membeli " << quantity << " lembar saham " << selectedStock.symbol << "." << std::endl;
                } else {
                    std::cout << "Maaf, kas Anda tidak cukup untuk membeli saham ini." << std::endl;
                }
                break;
            }
            case 3: {
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

                transactionHistory.push_back({"Jual", ownedStock.symbol, quantityToSell, currentStockPrice, proceeds, currentDay});
                std::cout << "✅ Anda berhasil menjual " << quantityToSell << " lembar saham " << ownedStock.symbol << " seharga Rp " << std::fixed << std::setprecision(2) << proceeds << "." << std::endl;

                playerPortfolio.erase(std::remove_if(playerPortfolio.begin(), playerPortfolio.end(),
                                                     [](const OwnedStock& s){ return s.quantity == 0; }),
                                     playerPortfolio.end());
                break;
            }
            case 4: {
                displayPortfolio(playerPortfolio, marketStocks);
                break;
            }
            case 5: {
                displayTransactionHistory(transactionHistory);
                break;
            }
            case 6: {
                std::cout << "\nMelanjutkan ke hari perdagangan berikutnya..." << std::endl;
                lanjutSimulasi = 'Y';
                break;
            }
            case 7: {
                lanjutSimulasi = 'N';
                std::cout << "\n***********************************************************" << std::endl;
                std::cout << "* TERIMA KASIH TELAH BERMAIN VIRTUAL STOCK MARKET!        *" << std::endl;
                std::cout << "* Kekayaan Akhir Anda: Rp " << std::fixed << std::setprecision(2) << netWorth << " *" << std::endl;
                std::cout << "***********************************************************" << std::endl;
                break;
            }
        }

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
