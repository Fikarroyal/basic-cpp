#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <map>
#include <utility>

struct FruitType {
    int id;
    std::string name;
    int maturation_period_days;
};

struct CultivationBatch {
    int id;
    int fruit_type_id;
    std::string plant_date;
    std::string harvest_date_est;
    double harvested_quantity;
    bool is_harvested;
};

struct SaleTransaction {
    int id;
    std::string fruit_name;
    std::string sale_date;
    double quantity_sold;
    double price_per_unit;
    double total_price;
};

std::vector<FruitType> fruitTypes;
std::vector<CultivationBatch> cultivationBatches;
std::vector<SaleTransaction> salesTransactions;

int nextFruitTypeId = 1;
int nextCultivationBatchId = 1;
int nextSaleTransactionId = 1;

const std::string DATA_FILE = "fruit_farm_data.txt";

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
            std::cout << "Input tidak valid. Mohon masukkan angka desimal antara " << std::fixed << std::setprecision(2) << minVal << " dan " << maxVal << "." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer();
            return value;
        }
    }
}

std::string getNonEmptyStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        if (input.empty() || input.find_first_not_of(' ') == std::string::npos) {
            std::cout << "Input tidak boleh kosong. Mohon masukkan teks yang valid." << std::endl;
        } else {
            return input;
        }
    }
}

std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d");
    return ss.str();
}

std::string addDaysToDate(const std::string& date_str, int days) {
    std::tm tm = {};
    std::istringstream ss(date_str);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    std::time_t time = std::mktime(&tm);
    time += days * 24 * 60 * 60;
    
    std::tm* new_tm = std::localtime(&time);
    std::stringstream new_ss;
    new_ss << std::put_time(new_tm, "%Y-%m-%d");
    return new_ss.str();
}

std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

std::vector<std::string> splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = s.find(delimiter, prev);
        tokens.push_back(s.substr(prev, pos - prev));
        prev = pos + delimiter.length();
    } while (pos != std::string::npos);
    return tokens;
}

void saveData() {
    std::ofstream outFile(DATA_FILE);
    if (!outFile.is_open()) {
        std::cerr << "ERROR: Gagal membuka file untuk menyimpan data." << std::endl;
        return;
    }

    outFile << nextFruitTypeId << "\n";
    outFile << nextCultivationBatchId << "\n";
    outFile << nextSaleTransactionId << "\n";

    outFile << "FRUIT_TYPES\n";
    for (const auto& ft : fruitTypes) {
        outFile << ft.id << "|||" << ft.name << "|||" << ft.maturation_period_days << "\n";
    }

    outFile << "CULTIVATION_BATCHES\n";
    for (const auto& cb : cultivationBatches) {
        outFile << cb.id << "|||" << cb.fruit_type_id << "|||" << cb.plant_date << "|||"
                << cb.harvest_date_est << "|||" << cb.harvested_quantity << "|||" << (cb.is_harvested ? "1" : "0") << "\n";
    }

    outFile << "SALE_TRANSACTIONS\n";
    for (const auto& st : salesTransactions) {
        outFile << st.id << "|||" << st.fruit_name << "|||" << st.sale_date << "|||"
                << st.quantity_sold << "|||" << st.price_per_unit << "|||" << st.total_price << "\n";
    }

    outFile.close();
    std::cout << "[INFO] Data kebun buah berhasil disimpan." << std::endl;
}

void loadData() {
    std::ifstream inFile(DATA_FILE);
    if (!inFile.is_open()) {
        std::cout << "[INFO] File data kebun buah tidak ditemukan. Membuat data baru." << std::endl;
        return;
    }

    std::string line;
    if (std::getline(inFile, line)) nextFruitTypeId = std::stoi(line);
    if (std::getline(inFile, line)) nextCultivationBatchId = std::stoi(line);
    if (std::getline(inFile, line)) nextSaleTransactionId = std::stoi(line);

    fruitTypes.clear();
    cultivationBatches.clear();
    salesTransactions.clear();

    if (std::getline(inFile, line) && line == "FRUIT_TYPES") {
        while (std::getline(inFile, line) && line != "CULTIVATION_BATCHES") {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 3) {
                FruitType ft;
                try {
                    ft.id = std::stoi(segments[0]);
                    ft.name = segments[1];
                    ft.maturation_period_days = std::stoi(segments[2]);
                    fruitTypes.push_back(ft);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris FruitType korup dilewati. Error: " << e.what() << std::endl;
                }
            }
        }
    }

    if (line == "CULTIVATION_BATCHES") {
        while (std::getline(inFile, line) && line != "SALE_TRANSACTIONS") {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 6) {
                CultivationBatch cb;
                try {
                    cb.id = std::stoi(segments[0]);
                    cb.fruit_type_id = std::stoi(segments[1]);
                    cb.plant_date = segments[2];
                    cb.harvest_date_est = segments[3];
                    cb.harvested_quantity = std::stod(segments[4]);
                    cb.is_harvested = (segments[5] == "1");
                    cultivationBatches.push_back(cb);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris CultivationBatch korup dilewati. Error: " << e.what() << std::endl;
                }
            }
        }
    }

    if (line == "SALE_TRANSACTIONS") {
        while (std::getline(inFile, line)) {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 6) {
                SaleTransaction st;
                try {
                    st.id = std::stoi(segments[0]);
                    st.fruit_name = segments[1];
                    st.sale_date = segments[2];
                    st.quantity_sold = std::stod(segments[3]);
                    st.price_per_unit = std::stod(segments[4]);
                    st.total_price = std::stod(segments[5]);
                    salesTransactions.push_back(st);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris SaleTransaction korup dilewati. Error: " << e.what() << std::endl;
                }
            }
        }
    }

    inFile.close();
    std::cout << "[INFO] Data kebun buah berhasil dimuat." << std::endl;
}

void addFruitType() {
    std::cout << "\n--- Tambah Jenis Buah Baru ---" << std::endl;
    std::string name = getNonEmptyStringInput("Masukkan nama buah (contoh: Apel Fuji, Pisang Cavendish): ");
    
    std::string lowerName = toLower(name);
    for (const auto& ft : fruitTypes) {
        if (toLower(ft.name) == lowerName) {
            std::cout << "[!] Jenis buah '" << name << "' sudah ada." << std::endl;
            return;
        }
    }

    int duration = getValidIntegerInput("Masukkan perkiraan durasi dari tanam/bibit hingga panen pertama (dalam hari): ", 1);

    FruitType newType = {nextFruitTypeId++, name, duration};
    fruitTypes.push_back(newType);
    std::cout << "[✅] Jenis buah '" << name << "' berhasil ditambahkan." << std::endl;
    saveData();
}

void viewFruitTypes() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   DAFTAR JENIS BUAH                      " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (fruitTypes.empty()) {
        std::cout << "Belum ada jenis buah yang terdaftar." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(30) << "Nama Buah"
                  << std::left << std::setw(20) << "Pematangan (Hari)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& ft : fruitTypes) {
            std::cout << std::left << std::setw(5) << ft.id
                      << std::left << std::setw(30) << ft.name
                      << std::left << std::setw(20) << ft.maturation_period_days << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void editFruitType() {
    viewFruitTypes();
    if (fruitTypes.empty()) return;

    int idToEdit = getValidIntegerInput("Masukkan ID jenis buah yang ingin diedit: ");
    FruitType* targetType = nullptr;
    for (auto& ft : fruitTypes) {
        if (ft.id == idToEdit) {
            targetType = &ft;
            break;
        }
    }

    if (!targetType) {
        std::cout << "[!] Jenis buah dengan ID " << idToEdit << " tidak ditemukan." << std::endl;
        return;
    }

    std::cout << "\n--- Edit Jenis Buah: " << targetType->name << " ---" << std::endl;
    std::cout << "Nama saat ini: " << targetType->name << std::endl;
    std::string newName = getNonEmptyStringInput("Masukkan nama baru (biarkan kosong untuk tidak mengubah): ");
    if (!newName.empty() && toLower(newName) != toLower(targetType->name)) {
        bool duplicate = false;
        std::string lowerNewName = toLower(newName);
        for (const auto& ft : fruitTypes) {
            if (ft.id != targetType->id && toLower(ft.name) == lowerNewName) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            std::cout << "[!] Nama '" << newName << "' sudah digunakan oleh jenis buah lain." << std::endl;
        } else {
            targetType->name = newName;
            std::cout << "[INFO] Nama berhasil diubah." << std::endl;
        }
    }

    std::cout << "Periode pematangan saat ini: " << targetType->maturation_period_days << " hari" << std::endl;
    int newDuration = getValidIntegerInput("Masukkan periode pematangan baru (0 untuk tidak mengubah): ", 0);
    if (newDuration > 0) {
        targetType->maturation_period_days = newDuration;
        std::cout << "[INFO] Periode pematangan berhasil diubah." << std::endl;
    }

    std::cout << "[✅] Jenis buah ID " << idToEdit << " berhasil diperbarui." << std::endl;
    saveData();
}

void deleteFruitType() {
    viewFruitTypes();
    if (fruitTypes.empty()) return;

    int idToDelete = getValidIntegerInput("Masukkan ID jenis buah yang ingin dihapus: ");

    for (const auto& cb : cultivationBatches) {
        if (cb.fruit_type_id == idToDelete) {
            std::cout << "[!] Tidak bisa menghapus jenis buah ini karena masih ada budidaya yang terkait (ID Budidaya: " << cb.id << ")." << std::endl;
            std::cout << "    Mohon hapus dulu budidaya tersebut atau update jenis buahnya sebelum menghapus jenis buah ini." << std::endl;
            return;
        }
    }

    auto it = std::remove_if(fruitTypes.begin(), fruitTypes.end(),
                             [idToDelete](const FruitType& ft) {
                                 return ft.id == idToDelete;
                             });
    
    if (it != fruitTypes.end()) {
        fruitTypes.erase(it, fruitTypes.end());
        std::cout << "[✅] Jenis buah ID " << idToDelete << " berhasil dihapus." << std::endl;
        saveData();
    } else {
        std::cout << "[!] Jenis buah dengan ID " << idToDelete << " tidak ditemukan." << std::endl;
    }
}

void addCultivationBatch() {
    viewFruitTypes();
    if (fruitTypes.empty()) {
        std::cout << "Anda harus menambahkan jenis buah terlebih dahulu sebelum mencatat budidaya." << std::endl;
        return;
    }

    int fruitTypeId = getValidIntegerInput("Masukkan ID jenis buah yang dibudidayakan: ");
    FruitType* selectedType = nullptr;
    for (auto& ft : fruitTypes) {
        if (ft.id == fruitTypeId) {
            selectedType = &ft;
            break;
        }
    }

    if (!selectedType) {
        std::cout << "[!] Jenis buah dengan ID " << fruitTypeId << " tidak ditemukan." << std::endl;
        return;
    }

    std::string plantDate = getCurrentDate();
    std::string harvestDateEst = addDaysToDate(plantDate, selectedType->maturation_period_days);

    CultivationBatch newBatch = {nextCultivationBatchId++, selectedType->id, plantDate, harvestDateEst, 0.0, false};
    cultivationBatches.push_back(newBatch);
    std::cout << "\n[✅] Budidaya " << selectedType->name << " berhasil dicatat." << std::endl;
    std::cout << "   Tanggal Mulai Budidaya: " << newBatch.plant_date << std::endl;
    std::cout << "   Estimasi Panen Pertama: " << newBatch.harvest_date_est << std::endl;
    saveData();
}

void viewCultivationBatches() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   DAFTAR BUDIDAYA BUAH                   " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (cultivationBatches.empty()) {
        std::cout << "Belum ada budidaya buah yang dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(20) << "Buah"
                  << std::left << std::setw(15) << "Mulai Budidaya"
                  << std::left << std::setw(15) << "Panen (Est)"
                  << std::left << std::setw(10) << "Hasil"
                  << std::left << std::setw(10) << "Status" << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        for (const auto& cb : cultivationBatches) {
            std::string fruitName = "Tidak Diketahui";
            for (const auto& ft : fruitTypes) {
                if (ft.id == cb.fruit_type_id) {
                    fruitName = ft.name;
                    break;
                }
            }
            std::cout << std::left << std::setw(5) << cb.id
                      << std::left << std::setw(20) << fruitName
                      << std::left << std::setw(15) << cb.plant_date
                      << std::left << std::setw(15) << cb.harvest_date_est
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << cb.harvested_quantity
                      << std::left << std::setw(10) << (cb.is_harvested ? "Panen ✅" : "Tumbuh ⏳") << std::endl;
        }
    }
    std::cout << "==========================================================================\n" << std::endl;
}

void updateHarvestResult() {
    viewCultivationBatches();
    if (cultivationBatches.empty()) return;

    int idToUpdate = getValidIntegerInput("Masukkan ID budidaya yang sudah panen: ");
    CultivationBatch* targetBatch = nullptr;
    for (auto& cb : cultivationBatches) {
        if (cb.id == idToUpdate) {
            targetBatch = &cb;
            break;
        }
    }

    if (!targetBatch) {
        std::cout << "[!] Budidaya dengan ID " << idToUpdate << " tidak ditemukan." << std::endl;
        return;
    }
    
    std::cout << "Hasil panen saat ini untuk budidaya ID " << targetBatch->id << " adalah " 
              << std::fixed << std::setprecision(2) << targetBatch->harvested_quantity << " kg." << std::endl;
    double quantity = getValidDoubleInput("Masukkan kuantitas hasil panen baru (misal: 100.5 kg, akan ditambahkan ke yang sudah ada): ", 0.0);
    
    targetBatch->harvested_quantity += quantity;
    targetBatch->is_harvested = true;

    std::cout << "[✅] Hasil panen untuk budidaya ID " << idToUpdate << " berhasil dicatat. Total: " << targetBatch->harvested_quantity << " kg." << std::endl;
    saveData();
}

void viewHarvestedStock() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   STOK BUAH TERSEDIA                     " << std::endl;
    std::cout << "==========================================================" << std::endl;

    std::map<std::string, double> currentStock; 
    
    for (const auto& cb : cultivationBatches) {
        if (cb.is_harvested) {
            std::string fruitName = "Tidak Diketahui";
            for (const auto& ft : fruitTypes) {
                if (ft.id == cb.fruit_type_id) {
                    fruitName = ft.name;
                    break;
                }
            }
            currentStock[fruitName] += cb.harvested_quantity;
        }
    }

    for (const auto& st : salesTransactions) {
        currentStock[st.fruit_name] -= st.quantity_sold;
    }

    if (currentStock.empty()) {
        std::cout << "Tidak ada stok buah yang tersedia." << std::endl;
    } else {
        std::cout << std::left << std::setw(30) << "Nama Buah"
                  << std::left << std::setw(15) << "Stok (kg/unit)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : currentStock) {
            if (pair.second > 0.001) { 
                std::cout << std::left << std::setw(30) << pair.first
                          << std::left << std::fixed << std::setprecision(2) << std::setw(15) << pair.second << std::endl;
            }
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void recordSale() {
    viewHarvestedStock();
    std::cout << "\n--- Catat Penjualan Buah ---" << std::endl;
    std::string fruitName = getNonEmptyStringInput("Masukkan nama buah yang terjual: ");
    
    std::string lowerFruitName = toLower(fruitName);
    bool fruitExists = false;
    for(const auto& ft : fruitTypes) {
        if(toLower(ft.name) == lowerFruitName) {
            fruitExists = true;
            break;
        }
    }
    if(!fruitExists) {
        std::cout << "[!] Buah '" << fruitName << "' tidak terdaftar sebagai jenis buah." << std::endl;
        std::cout << "    Pastikan nama yang dimasukkan benar dan buah sudah didaftarkan." << std::endl;
        return;
    }

    double quantityToSell = getValidDoubleInput("Masukkan kuantitas yang terjual (kg/unit): ", 0.01);
    double pricePerUnit = getValidDoubleInput("Masukkan harga jual per unit/kg: ", 0.01);

    double availableStock = 0.0;
    for (const auto& cb : cultivationBatches) {
        if (cb.is_harvested) {
            std::string currentFruitName = "Tidak Diketahui";
            for (const auto& ft : fruitTypes) {
                if (ft.id == cb.fruit_type_id) {
                    currentFruitName = ft.name;
                    break;
                }
            }
            if (toLower(currentFruitName) == lowerFruitName) {
                availableStock += cb.harvested_quantity;
            }
        }
    }
    for (const auto& st : salesTransactions) {
        if (toLower(st.fruit_name) == lowerFruitName) {
            availableStock -= st.quantity_sold;
        }
    }

    if (quantityToSell > availableStock) {
        std::cout << "[!] Stok '" << fruitName << "' tidak cukup. Tersedia: " << std::fixed << std::setprecision(2) << availableStock << " kg/unit." << std::endl;
        return;
    }

    double totalPrice = quantityToSell * pricePerUnit;
    SaleTransaction newSale = {nextSaleTransactionId++, fruitName, getCurrentDate(), quantityToSell, pricePerUnit, totalPrice};
    salesTransactions.push_back(newSale);
    
    std::cout << "\n[✅] Penjualan '" << newSale.fruit_name << "' sebanyak "
              << std::fixed << std::setprecision(2) << newSale.quantity_sold << " kg/unit seharga Rp "
              << std::fixed << std::setprecision(2) << newSale.total_price << " berhasil dicatat." << std::endl;
    saveData();
}

void viewSaleHistory() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 RIWAYAT TRANSAKSI PENJUALAN BUAH         " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (salesTransactions.empty()) {
        std::cout << "Belum ada transaksi penjualan yang dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(25) << "Buah"
                  << std::left << std::setw(10) << "Jumlah"
                  << std::left << std::setw(10) << "Harga/Unit"
                  << std::left << std::setw(15) << "Total Harga" << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        for (const auto& st : salesTransactions) {
            std::cout << std::left << std::setw(5) << st.id
                      << std::left << std::setw(15) << st.sale_date
                      << std::left << std::setw(25) << st.fruit_name
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << st.quantity_sold
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << st.price_per_unit
                      << std::left << std::fixed << std::setprecision(2) << std::setw(15) << st.total_price << std::endl;
        }
    }
    std::cout << "==================================================================================================\n" << std::endl;
}

void viewUpcomingHarvests() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                  PANEN BUAH MENDATANG                    " << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    std::string today = getCurrentDate();

    std::vector<CultivationBatch> upcoming;
    for (const auto& cb : cultivationBatches) {
        if (!cb.is_harvested && cb.harvest_date_est >= today) {
            upcoming.push_back(cb);
        }
    }

    std::sort(upcoming.begin(), upcoming.end(), [](const CultivationBatch& a, const CultivationBatch& b) {
        return a.harvest_date_est < b.harvest_date_est;
    });

    if (upcoming.empty()) {
        std::cout << "Tidak ada panen buah yang akan datang atau yang belum dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(20) << "Buah"
                  << std::left << std::setw(15) << "Mulai Budidaya"
                  << std::left << std::setw(15) << "Estimasi Panen" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& cb : upcoming) {
            std::string fruitName = "Tidak Diketahui";
            for (const auto& ft : fruitTypes) {
                if (ft.id == cb.fruit_type_id) {
                    fruitName = ft.name;
                    break;
                }
            }
            std::cout << std::left << std::setw(5) << cb.id
                      << std::left << std::setw(20) << fruitName
                      << std::left << std::setw(15) << cb.plant_date
                      << std::left << std::setw(15) << cb.harvest_date_est << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showBestSellingProducts() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   BUAH TERLARIS (BERDASARKAN JUMLAH TERJUAL)    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    std::map<std::string, double> soldQuantities;
    for (const auto& st : salesTransactions) {
        soldQuantities[st.fruit_name] += st.quantity_sold;
    }

    if (soldQuantities.empty()) {
        std::cout << "Belum ada data penjualan buah." << std::endl;
    } else {
        std::vector<std::pair<std::string, double>> sortedSales(soldQuantities.begin(), soldQuantities.end());
        
        std::sort(sortedSales.begin(), sortedSales.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second; 
        });

        std::cout << std::left << std::setw(30) << "Nama Buah"
                  << std::left << std::setw(20) << "Total Terjual (kg/unit)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : sortedSales) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::left << std::fixed << std::setprecision(2) << std::setw(20) << pair.second << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showTotalHarvestByFruit() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 TOTAL HASIL PANEN PER JENIS BUAH        " << std::endl;
    std::cout << "==========================================================" << std::endl;

    std::map<std::string, double> totalHarvests;
    for (const auto& cb : cultivationBatches) {
        if (cb.is_harvested) {
            std::string fruitName = "Tidak Diketahui";
            for (const auto& ft : fruitTypes) {
                if (ft.id == cb.fruit_type_id) {
                    fruitName = ft.name;
                    break;
                }
            }
            totalHarvests[fruitName] += cb.harvested_quantity;
        }
    }

    if (totalHarvests.empty()) {
        std::cout << "Belum ada data hasil panen buah." << std::endl;
    } else {
        std::vector<std::pair<std::string, double>> sortedHarvests(totalHarvests.begin(), totalHarvests.end());

        std::sort(sortedHarvests.begin(), sortedHarvests.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second;
        });

        std::cout << std::left << std::setw(30) << "Nama Buah"
                  << std::left << std::setw(20) << "Total Panen (kg/unit)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : sortedHarvests) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::left << std::fixed << std::setprecision(2) << std::setw(20) << pair.second << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showTotalRevenue() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                    TOTAL PENDAPATAN PENJUALAN BUAH       " << std::endl;
    std::cout << "==========================================================" << std::endl;
    double totalRevenue = 0.0;
    for (const auto& st : salesTransactions) {
        totalRevenue += st.total_price;
    }

    std::cout << "Total pendapatan dari semua penjualan buah: Rp " << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
    std::cout << "==========================================================\n" << std::endl;
}

int main() {
    loadData();

    int choice;
    do {
        std::cout << "\n==========================================================" << std::endl;
        std::cout << "            MANAJER KEBUN & DISTRIBUSI BUAH                " << std::endl;
        std::cout << "              Tanggal Saat Ini: " << getCurrentDate() << std::endl;
        std::cout << "==========================================================" << std::endl;
        std::cout << "--- Manajemen Jenis Buah ---" << std::endl;
        std::cout << "1.  Tambah Jenis Buah" << std::endl;
        std::cout << "2.  Lihat Semua Jenis Buah" << std::endl;
        std::cout << "3.  Edit Detail Jenis Buah" << std::endl;
        std::cout << "4.  Hapus Jenis Buah" << std::endl;
        std::cout << "--- Manajemen Budidaya Buah ---" << std::endl;
        std::cout << "5.  Catat Budidaya Baru" << std::endl;
        std::cout << "6.  Lihat Semua Budidaya" << std::endl;
        std::cout << "7.  Update Hasil Panen Buah untuk Budidaya" << std::endl;
        std::cout << "--- Manajemen Stok & Penjualan ---" << std::endl;
        std::cout << "8.  Lihat Stok Buah Tersedia" << std::endl;
        std::cout << "9.  Catat Penjualan Buah" << std::endl;
        std::cout << "10. Lihat Riwayat Penjualan Buah" << std::endl;
        std::cout << "--- Laporan & Statistik ---" << std::endl;
        std::cout << "11. Lihat Panen Buah Mendatang" << std::endl;
        std::cout << "12. Tampilkan Buah Terlaris" << std::endl;
        std::cout << "13. Tampilkan Total Hasil Panen per Jenis Buah" << std::endl;
        std::cout << "14. Tampilkan Total Pendapatan Penjualan Buah" << std::endl;
        std::cout << "15. Keluar" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;

        choice = getValidIntegerInput("Pilihan Anda: ", 1, 15);

        switch (choice) {
            case 1: addFruitType(); break;
            case 2: viewFruitTypes(); break;
            case 3: editFruitType(); break;
            case 4: deleteFruitType(); break;
            case 5: addCultivationBatch(); break;
            case 6: viewCultivationBatches(); break;
            case 7: updateHarvestResult(); break;
            case 8: viewHarvestedStock(); break;
            case 9: recordSale(); break;
            case 10: viewSaleHistory(); break;
            case 11: viewUpcomingHarvests(); break;
            case 12: showBestSellingProducts(); break;
            case 13: showTotalHarvestByFruit(); break;
            case 14: showTotalRevenue(); break;
            case 15: 
                saveData();
                std::cout << "\nTerima kasih telah menggunakan Manajer Kebun & Distribusi Buah! Selamat berkebun dan berjualan." << std::endl;
                break;
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 15);

    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get(); 

    return 0;
}
