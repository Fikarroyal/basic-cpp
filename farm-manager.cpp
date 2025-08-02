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

struct VegetableType {
    int id;
    std::string name;
    int grow_duration_days;
};

struct PlantingBatch {
    int id;
    int veggie_type_id;
    std::string plant_date;
    std::string harvest_date_est;
    double harvested_quantity;
    bool is_harvested;
};

struct SaleTransaction {
    int id;
    std::string veggie_name;
    std::string sale_date;
    double quantity_sold;
    double price_per_unit;
    double total_price;
};

std::vector<VegetableType> veggieTypes;
std::vector<PlantingBatch> plantingBatches;
std::vector<SaleTransaction> salesTransactions;

int nextVeggieTypeId = 1;
int nextPlantingBatchId = 1;
int nextSaleTransactionId = 1;

const std::string DATA_FILE = "vegetable_farm_data.txt";

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
    outFile << nextVeggieTypeId << "\n";
    outFile << nextPlantingBatchId << "\n";
    outFile << nextSaleTransactionId << "\n";
    outFile << "VEGETABLE_TYPES\n";
    for (const auto& vt : veggieTypes) {
        outFile << vt.id << "|||" << vt.name << "|||" << vt.grow_duration_days << "\n";
    }

    outFile << "PLANTING_BATCHES\n";
    for (const auto& pb : plantingBatches) {
        outFile << pb.id << "|||" << pb.veggie_type_id << "|||" << pb.plant_date << "|||"
                << pb.harvest_date_est << "|||" << pb.harvested_quantity << "|||" << (pb.is_harvested ? "1" : "0") << "\n";
    }

    outFile << "SALE_TRANSACTIONS\n";
    for (const auto& st : salesTransactions) {
        outFile << st.id << "|||" << st.veggie_name << "|||" << st.sale_date << "|||"
                << st.quantity_sold << "|||" << st.price_per_unit << "|||" << st.total_price << "\n";
    }

    outFile.close();
    std::cout << "[INFO] Data pertanian berhasil disimpan." << std::endl;
}

void loadData() {
    std::ifstream inFile(DATA_FILE);
    if (!inFile.is_open()) {
        std::cout << "[INFO] File data pertanian tidak ditemukan. Membuat data baru." << std::endl;
        return;
    }

    std::string line;
    if (std::getline(inFile, line)) nextVeggieTypeId = std::stoi(line);
    if (std::getline(inFile, line)) nextPlantingBatchId = std::stoi(line);
    if (std::getline(inFile, line)) nextSaleTransactionId = std::stoi(line);
    veggieTypes.clear();
    plantingBatches.clear();
    salesTransactions.clear();
    if (std::getline(inFile, line) && line == "VEGETABLE_TYPES") {
        while (std::getline(inFile, line) && line != "PLANTING_BATCHES") {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 3) {
                VegetableType vt;
                try {
                    vt.id = std::stoi(segments[0]);
                    vt.name = segments[1];
                    vt.grow_duration_days = std::stoi(segments[2]);
                    veggieTypes.push_back(vt);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris VegetableType korup dilewati. Error: " << e.what() << std::endl;
                }
            }
        }
    }

    if (line == "PLANTING_BATCHES") {
        while (std::getline(inFile, line) && line != "SALE_TRANSACTIONS") {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 6) {
                PlantingBatch pb;
                try {
                    pb.id = std::stoi(segments[0]);
                    pb.veggie_type_id = std::stoi(segments[1]);
                    pb.plant_date = segments[2];
                    pb.harvest_date_est = segments[3];
                    pb.harvested_quantity = std::stod(segments[4]);
                    pb.is_harvested = (segments[5] == "1");
                    plantingBatches.push_back(pb);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris PlantingBatch korup dilewati. Error: " << e.what() << std::endl;
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
                    st.veggie_name = segments[1];
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
    std::cout << "[INFO] Data pertanian berhasil dimuat." << std::endl;
}

void addVegetableType() {
    std::cout << "\n--- Tambah Jenis Sayuran Baru ---" << std::endl;
    std::string name = getNonEmptyStringInput("Masukkan nama sayuran (contoh: Bayam, Tomat): ");
    
    std::string lowerName = toLower(name);
    for (const auto& vt : veggieTypes) {
        if (toLower(vt.name) == lowerName) {
            std::cout << "[!] Jenis sayuran '" << name << "' sudah ada." << std::endl;
            return;
        }
    }

    int duration = getValidIntegerInput("Masukkan perkiraan durasi tanam hingga panen (dalam hari): ", 1);

    VegetableType newType = {nextVeggieTypeId++, name, duration};
    veggieTypes.push_back(newType);
    std::cout << "[✅] Jenis sayuran '" << name << "' berhasil ditambahkan." << std::endl;
    saveData();
}

void viewVegetableTypes() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                  DAFTAR JENIS SAYURAN                    " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (veggieTypes.empty()) {
        std::cout << "Belum ada jenis sayuran yang terdaftar." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(30) << "Nama Sayuran"
                  << std::left << std::setw(20) << "Durasi Panen (Hari)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& vt : veggieTypes) {
            std::cout << std::left << std::setw(5) << vt.id
                      << std::left << std::setw(30) << vt.name
                      << std::left << std::setw(20) << vt.grow_duration_days << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void editVegetableType() {
    viewVegetableTypes();
    if (veggieTypes.empty()) return;

    int idToEdit = getValidIntegerInput("Masukkan ID jenis sayuran yang ingin diedit: ");
    VegetableType* targetType = nullptr;
    for (auto& vt : veggieTypes) {
        if (vt.id == idToEdit) {
            targetType = &vt;
            break;
        }
    }

    if (!targetType) {
        std::cout << "[!] Jenis sayuran dengan ID " << idToEdit << " tidak ditemukan." << std::endl;
        return;
    }

    std::cout << "\n--- Edit Jenis Sayuran: " << targetType->name << " ---" << std::endl;
    std::cout << "Nama saat ini: " << targetType->name << std::endl;
    std::string newName = getNonEmptyStringInput("Masukkan nama baru (biarkan kosong untuk tidak mengubah): ");
    if (!newName.empty() && toLower(newName) != toLower(targetType->name)) {
        bool duplicate = false;
        std::string lowerNewName = toLower(newName);
        for (const auto& vt : veggieTypes) {
            if (vt.id != targetType->id && toLower(vt.name) == lowerNewName) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            std::cout << "[!] Nama '" << newName << "' sudah digunakan oleh jenis sayuran lain." << std::endl;
        } else {
            targetType->name = newName;
            std::cout << "[INFO] Nama berhasil diubah." << std::endl;
        }
    }

    std::cout << "Durasi panen saat ini: " << targetType->grow_duration_days << " hari" << std::endl;
    int newDuration = getValidIntegerInput("Masukkan durasi panen baru (0 untuk tidak mengubah): ", 0);
    if (newDuration > 0) {
        targetType->grow_duration_days = newDuration;
        std::cout << "[INFO] Durasi panen berhasil diubah." << std::endl;
    }

    std::cout << "[✅] Jenis sayuran ID " << idToEdit << " berhasil diperbarui." << std::endl;
    saveData();
}

void deleteVegetableType() {
    viewVegetableTypes();
    if (veggieTypes.empty()) return;

    int idToDelete = getValidIntegerInput("Masukkan ID jenis sayuran yang ingin dihapus: ");

    for (const auto& pb : plantingBatches) {
        if (pb.veggie_type_id == idToDelete) {
            std::cout << "[!] Tidak bisa menghapus jenis sayuran ini karena masih ada penanaman yang terkait (ID Penanaman: " << pb.id << ")." << std::endl;
            std::cout << "    Mohon hapus dulu penanaman tersebut atau update jenis sayurannya sebelum menghapus jenis sayuran ini." << std::endl;
            return;
        }
    }

    auto it = std::remove_if(veggieTypes.begin(), veggieTypes.end(),
                             [idToDelete](const VegetableType& vt) {
                                 return vt.id == idToDelete;
                             });
    
    if (it != veggieTypes.end()) {
        veggieTypes.erase(it, veggieTypes.end());
        std::cout << "[✅] Jenis sayuran ID " << idToDelete << " berhasil dihapus." << std::endl;
        saveData();
    } else {
        std::cout << "[!] Jenis sayuran dengan ID " << idToDelete << " tidak ditemukan." << std::endl;
    }
}

void addPlantingBatch() {
    viewVegetableTypes();
    if (veggieTypes.empty()) {
        std::cout << "Anda harus menambahkan jenis sayuran terlebih dahulu sebelum mencatat penanaman." << std::endl;
        return;
    }

    int veggieTypeId = getValidIntegerInput("Masukkan ID jenis sayuran yang ditanam: ");
    VegetableType* selectedType = nullptr;
    for (auto& vt : veggieTypes) {
        if (vt.id == veggieTypeId) {
            selectedType = &vt;
            break;
        }
    }

    if (!selectedType) {
        std::cout << "[!] Jenis sayuran dengan ID " << veggieTypeId << " tidak ditemukan." << std::endl;
        return;
    }

    std::string plantDate = getCurrentDate();
    std::string harvestDateEst = addDaysToDate(plantDate, selectedType->grow_duration_days);

    PlantingBatch newBatch = {nextPlantingBatchId++, selectedType->id, plantDate, harvestDateEst, 0.0, false};
    plantingBatches.push_back(newBatch);
    std::cout << "\n[✅] Penanaman " << selectedType->name << " berhasil dicatat." << std::endl;
    std::cout << "   Tanggal Tanam: " << newBatch.plant_date << std::endl;
    std::cout << "   Estimasi Panen: " << newBatch.harvest_date_est << std::endl;
    saveData();
}

void viewPlantingBatches() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   DAFTAR PENANAMAN                       " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (plantingBatches.empty()) {
        std::cout << "Belum ada penanaman yang dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(20) << "Sayuran"
                  << std::left << std::setw(15) << "Tanam"
                  << std::left << std::setw(15) << "Panen (Est)"
                  << std::left << std::setw(10) << "Hasil"
                  << std::left << std::setw(10) << "Status" << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        for (const auto& pb : plantingBatches) {
            std::string veggieName = "Tidak Diketahui";
            for (const auto& vt : veggieTypes) {
                if (vt.id == pb.veggie_type_id) {
                    veggieName = vt.name;
                    break;
                }
            }
            std::cout << std::left << std::setw(5) << pb.id
                      << std::left << std::setw(20) << veggieName
                      << std::left << std::setw(15) << pb.plant_date
                      << std::left << std::setw(15) << pb.harvest_date_est
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << pb.harvested_quantity
                      << std::left << std::setw(10) << (pb.is_harvested ? "Panen ✅" : "Tumbuh ⏳") << std::endl;
        }
    }
    std::cout << "==========================================================================\n" << std::endl;
}
void updateHarvestResult() {
    viewPlantingBatches();
    if (plantingBatches.empty()) return;

    int idToUpdate = getValidIntegerInput("Masukkan ID penanaman yang sudah panen: ");
    PlantingBatch* targetBatch = nullptr;
    for (auto& pb : plantingBatches) {
        if (pb.id == idToUpdate) {
            targetBatch = &pb;
            break;
        }
    }

    if (!targetBatch) {
        std::cout << "[!] Penanaman dengan ID " << idToUpdate << " tidak ditemukan." << std::endl;
        return;
    }

    if (targetBatch->is_harvested) {
        std::cout << "[!] Penanaman ini sudah ditandai panen sebelumnya." << std::endl;
        return;
    }

    double quantity = getValidDoubleInput("Masukkan kuantitas hasil panen (misal: 10.5 kg): ", 0.0);
    targetBatch->harvested_quantity = quantity;
    targetBatch->is_harvested = true;

    std::cout << "[✅] Hasil panen untuk penanaman ID " << idToUpdate << " berhasil dicatat: " << quantity << " kg." << std::endl;
    saveData();
}

void viewHarvestedStock() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   STOK PANEN TERSEDIA                    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    std::map<std::string, double> currentStock; 
    
    for (const auto& pb : plantingBatches) {
        if (pb.is_harvested) {
            std::string veggieName = "Tidak Diketahui";
            for (const auto& vt : veggieTypes) {
                if (vt.id == pb.veggie_type_id) {
                    veggieName = vt.name;
                    break;
                }
            }
            currentStock[veggieName] += pb.harvested_quantity;
        }
    }

    for (const auto& st : salesTransactions) {
        currentStock[st.veggie_name] -= st.quantity_sold;
    }

    if (currentStock.empty()) {
        std::cout << "Tidak ada stok panen yang tersedia." << std::endl;
    } else {
        std::cout << std::left << std::setw(30) << "Nama Sayuran"
                  << std::left << std::setw(15) << "Stok (kg)" << std::endl;
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
    std::cout << "\n--- Catat Penjualan Panen ---" << std::endl;
    std::string veggieName = getNonEmptyStringInput("Masukkan nama sayuran yang terjual: ");
    
    std::string lowerVeggieName = toLower(veggieName);
    bool veggieExists = false;
    for(const auto& vt : veggieTypes) {
        if(toLower(vt.name) == lowerVeggieName) {
            veggieExists = true;
            break;
        }
    }
    if(!veggieExists) {
        std::cout << "[!] Sayuran '" << veggieName << "' tidak terdaftar sebagai jenis sayuran." << std::endl;
        std::cout << "    Pastikan nama yang dimasukkan benar dan sayuran sudah didaftarkan." << std::endl;
        return;
    }

    double quantityToSell = getValidDoubleInput("Masukkan kuantitas yang terjual (kg/ikat): ", 0.01);
    double pricePerUnit = getValidDoubleInput("Masukkan harga jual per unit/kg: ", 0.01);

    double availableStock = 0.0;
    for (const auto& pb : plantingBatches) {
        if (pb.is_harvested) {
            std::string currentVeggieName = "Tidak Diketahui";
            for (const auto& vt : veggieTypes) {
                if (vt.id == pb.veggie_type_id) {
                    currentVeggieName = vt.name;
                    break;
                }
            }
            if (toLower(currentVeggieName) == lowerVeggieName) {
                availableStock += pb.harvested_quantity;
            }
        }
    }
    for (const auto& st : salesTransactions) {
        if (toLower(st.veggie_name) == lowerVeggieName) {
            availableStock -= st.quantity_sold;
        }
    }

    if (quantityToSell > availableStock) {
        std::cout << "[!] Stok '" << veggieName << "' tidak cukup. Tersedia: " << std::fixed << std::setprecision(2) << availableStock << " kg." << std::endl;
        return;
    }

    double totalPrice = quantityToSell * pricePerUnit;
    SaleTransaction newSale = {nextSaleTransactionId++, veggieName, getCurrentDate(), quantityToSell, pricePerUnit, totalPrice};
    salesTransactions.push_back(newSale);
    
    std::cout << "\n[✅] Penjualan '" << newSale.veggie_name << "' sebanyak "
              << std::fixed << std::setprecision(2) << newSale.quantity_sold << " kg seharga Rp "
              << std::fixed << std::setprecision(2) << newSale.total_price << " berhasil dicatat." << std::endl;
    saveData();
}

void viewSaleHistory() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 RIWAYAT TRANSAKSI PENJUALAN              " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (salesTransactions.empty()) {
        std::cout << "Belum ada transaksi penjualan yang dicatat." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(15) << "Tanggal"
                  << std::left << std::setw(25) << "Sayuran"
                  << std::left << std::setw(10) << "Jumlah"
                  << std::left << std::setw(10) << "Harga/Unit"
                  << std::left << std::setw(15) << "Total Harga" << std::endl;
        std::cout << "--------------------------------------------------------------------------------------------------" << std::endl;
        for (const auto& st : salesTransactions) {
            std::cout << std::left << std::setw(5) << st.id
                      << std::left << std::setw(15) << st.sale_date
                      << std::left << std::setw(25) << st.veggie_name
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << st.quantity_sold
                      << std::left << std::fixed << std::setprecision(2) << std::setw(10) << st.price_per_unit
                      << std::left << std::fixed << std::setprecision(2) << std::setw(15) << st.total_price << std::endl;
        }
    }
    std::cout << "==================================================================================================\n" << std::endl;
}

void viewUpcomingHarvests() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                  PANEN MENDATANG                        " << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    std::string today = getCurrentDate();

    std::vector<PlantingBatch> upcoming;
    for (const auto& pb : plantingBatches) {
        if (!pb.is_harvested && pb.harvest_date_est >= today) {
            upcoming.push_back(pb);
        }
    }

    std::sort(upcoming.begin(), upcoming.end(), [](const PlantingBatch& a, const PlantingBatch& b) {
        return a.harvest_date_est < b.harvest_date_est;
    });

    if (upcoming.empty()) {
        std::cout << "Tidak ada panen yang akan datang atau yang belum dipanen." << std::endl;
    } else {
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(20) << "Sayuran"
                  << std::left << std::setw(15) << "Tanam"
                  << std::left << std::setw(15) << "Estimasi Panen" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pb : upcoming) {
            std::string veggieName = "Tidak Diketahui";
            for (const auto& vt : veggieTypes) {
                if (vt.id == pb.veggie_type_id) {
                    veggieName = vt.name;
                    break;
                }
            }
            std::cout << std::left << std::setw(5) << pb.id
                      << std::left << std::setw(20) << veggieName
                      << std::left << std::setw(15) << pb.plant_date
                      << std::left << std::setw(15) << pb.harvest_date_est << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showBestSellingProducts() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   PRODUK TERLARIS (BERDASARKAN JUMLAH TERJUAL)    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    std::map<std::string, double> soldQuantities;
    for (const auto& st : salesTransactions) {
        soldQuantities[st.veggie_name] += st.quantity_sold;
    }

    if (soldQuantities.empty()) {
        std::cout << "Belum ada data penjualan." << std::endl;
    } else {
        std::vector<std::pair<std::string, double>> sortedSales(soldQuantities.begin(), soldQuantities.end());
        
        std::sort(sortedSales.begin(), sortedSales.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second; 
        });

        std::cout << std::left << std::setw(30) << "Nama Sayuran"
                  << std::left << std::setw(15) << "Total Terjual (kg)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : sortedSales) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::left << std::fixed << std::setprecision(2) << std::setw(15) << pair.second << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showTotalHarvestByVegetable() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 TOTAL HASIL PANEN PER JENIS SAYURAN        " << std::endl;
    std::cout << "==========================================================" << std::endl;
    std::map<std::string, double> totalHarvests;
    for (const auto& pb : plantingBatches) {
        if (pb.is_harvested) {
            std::string veggieName = "Tidak Diketahui";
            for (const auto& vt : veggieTypes) {
                if (vt.id == pb.veggie_type_id) {
                    veggieName = vt.name;
                    break;
                }
            }
            totalHarvests[veggieName] += pb.harvested_quantity;
        }
    }

    if (totalHarvests.empty()) {
        std::cout << "Belum ada data hasil panen." << std::endl;
    } else {
        std::vector<std::pair<std::string, double>> sortedHarvests(totalHarvests.begin(), totalHarvests.end());
        std::sort(sortedHarvests.begin(), sortedHarvests.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second;
        });
        std::cout << std::left << std::setw(30) << "Nama Sayuran"
                  << std::left << std::setw(15) << "Total Panen (kg)" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;
        for (const auto& pair : sortedHarvests) {
            std::cout << std::left << std::setw(30) << pair.first
                      << std::left << std::fixed << std::setprecision(2) << std::setw(15) << pair.second << std::endl;
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

void showTotalRevenue() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                    TOTAL PENDAPATAN PENJUALAN            " << std::endl;
    std::cout << "==========================================================" << std::endl;
    double totalRevenue = 0.0;
    for (const auto& st : salesTransactions) {
        totalRevenue += st.total_price;
    }

    std::cout << "Total pendapatan dari semua penjualan: Rp " << std::fixed << std::setprecision(2) << totalRevenue << std::endl;
    std::cout << "==========================================================\n" << std::endl;
}
int main() {
    loadData();

    int choice;
    do {
        std::cout << "\n==========================================================" << std::endl;
        std::cout << "        MANAJER PERTANIAN & DISTRIBUSI SAYURAN            " << std::endl;
        std::cout << "              Tanggal Saat Ini: " << getCurrentDate() << std::endl;
        std::cout << "==========================================================" << std::endl;
        std::cout << "--- Manajemen Jenis Sayuran ---" << std::endl;
        std::cout << "1.  Tambah Jenis Sayuran" << std::endl;
        std::cout << "2.  Lihat Semua Jenis Sayuran" << std::endl;
        std::cout << "3.  Edit Detail Jenis Sayuran" << std::endl;
        std::cout << "4.  Hapus Jenis Sayuran" << std::endl;
        std::cout << "--- Manajemen Penanaman ---" << std::endl;
        std::cout << "5.  Catat Penanaman Baru" << std::endl;
        std::cout << "6.  Lihat Semua Penanaman" << std::endl;
        std::cout << "7.  Update Hasil Panen untuk Penanaman" << std::endl;
        std::cout << "--- Manajemen Stok & Penjualan ---" << std::endl;
        std::cout << "8.  Lihat Stok Panen Tersedia" << std::endl;
        std::cout << "9.  Catat Penjualan Panen" << std::endl;
        std::cout << "10. Lihat Riwayat Penjualan" << std::endl;
        std::cout << "--- Laporan & Statistik ---" << std::endl;
        std::cout << "11. Lihat Panen Mendatang" << std::endl;
        std::cout << "12. Tampilkan Produk Terlaris" << std::endl;
        std::cout << "13. Tampilkan Total Hasil Panen per Jenis Sayuran" << std::endl;
        std::cout << "14. Tampilkan Total Pendapatan Penjualan" << std::endl;
        std::cout << "15. Keluar" << std::endl;
        std::cout << "----------------------------------------------------------" << std::endl;

        choice = getValidIntegerInput("Pilihan Anda: ", 1, 15);
        switch (choice) {
            case 1: addVegetableType(); break;
            case 2: viewVegetableTypes(); break;
            case 3: editVegetableType(); break;
            case 4: deleteVegetableType(); break;
            case 5: addPlantingBatch(); break;
            case 6: viewPlantingBatches(); break;
            case 7: updateHarvestResult(); break;
            case 8: viewHarvestedStock(); break;
            case 9: recordSale(); break;
            case 10: viewSaleHistory(); break;
            case 11: viewUpcomingHarvests(); break;
            case 12: showBestSellingProducts(); break;
            case 13: showTotalHarvestByVegetable(); break;
            case 14: showTotalRevenue(); break;
            case 15: 
                saveData();
                std::cout << "\nTerima kasih telah menggunakan Manajer Pertanian & Distribusi Sayuran! Selamat bertani dan berjualan." << std::endl;
                break;
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 15);
    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get(); 

    return 0;
}
