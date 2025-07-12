#include <iostream>   // Untuk input/output konsol (std::cout, std::cin, std::endl)
#include <vector>     // Untuk menggunakan std::vector (array dinamis)
#include <string>     // Untuk menggunakan std::string dan fungsi-fungsi string (std::getline)
#include <fstream>    // Untuk operasi file input/output (std::ofstream, std::ifstream)
#include <sstream>    // Untuk stringstream (parsing string menjadi angka atau sebaliknya)
#include <chrono>     // Untuk manipulasi waktu dan tanggal (std::chrono::system_clock, std::time_t)
#include <iomanip>    // Untuk manipulasi format output (std::put_time, std::setw, std::fixed, std::setprecision)
#include <algorithm>  // Untuk algoritma seperti std::transform, std::sort, std::remove_if
#include <limits>     // Untuk std::numeric_limits (digunakan untuk membersihkan buffer input)
#include <map>        // Untuk menggunakan std::map (struktur data key-value pair)
#include <utility>    // Untuk std::pair, meskipun sering sudah diinclude oleh header lain, lebih baik eksplisit

// --- Struktur Data ---

// Struktur untuk mendefinisikan jenis sayuran yang dikelola dalam bisnis.
// Setiap jenis sayuran memiliki ID unik, nama, dan durasi tanam hingga panen.
struct VegetableType {
    int id;                 // ID unik untuk jenis sayuran (otomatis generated)
    std::string name;       // Nama sayuran (contoh: "Bayam", "Tomat Cherry")
    int grow_duration_days; // Perkiraan durasi tumbuh dari tanam hingga siap panen (dalam hari)
};

// Struktur untuk mencatat setiap insiden penanaman (satu 'batch' atau kelompok tanam).
// Ini melacak spesifik penanaman sayuran di lahan.
struct PlantingBatch {
    int id;                     // ID unik untuk batch penanaman ini
    int veggie_type_id;         // ID dari jenis sayuran yang ditanam (merujuk ke VegetableType)
    std::string plant_date;     // Tanggal saat penanaman dilakukan (format YYYY-MM-DD)
    std::string harvest_date_est; // Perkiraan tanggal kapan sayuran ini siap dipanen (YYYY-MM-DD)
    double harvested_quantity;  // Kuantitas aktual yang dipanen dari batch ini (misal: dalam kg atau ikat). Default 0 jika belum panen.
    bool is_harvested;          // Status panen: true jika sudah dipanen, false jika masih tumbuh.
};

// Struktur untuk mencatat setiap transaksi penjualan sayuran.
// Ini melacak apa yang dijual, kapan, berapa banyak, dan harganya.
struct SaleTransaction {
    int id;                 // ID unik untuk transaksi penjualan ini
    std::string veggie_name; // Nama sayuran yang terjual (untuk riwayat, bukan ID)
    std::string sale_date;   // Tanggal transaksi penjualan (YYYY-MM-DD)
    double quantity_sold;    // Jumlah sayuran yang terjual (misal: dalam kg atau ikat)
    double price_per_unit;   // Harga jual per unit (misal: harga per kg atau per ikat)
    double total_price;      // Total harga untuk transaksi ini (quantity_sold * price_per_unit)
};

// --- Variabel Global ---

// Vektor untuk menyimpan semua objek VegetableType yang terdaftar.
std::vector<VegetableType> veggieTypes;
// Vektor untuk menyimpan semua objek PlantingBatch yang dicatat.
std::vector<PlantingBatch> plantingBatches;
// Vektor untuk menyimpan semua objek SaleTransaction yang telah terjadi.
std::vector<SaleTransaction> salesTransactions;

// Penghitung ID unik untuk setiap struktur data. Akan diincrement setiap kali objek baru ditambahkan.
int nextVeggieTypeId = 1;
int nextPlantingBatchId = 1;
int nextSaleTransactionId = 1;

// Nama file tempat data akan disimpan dan dimuat.
const std::string DATA_FILE = "vegetable_farm_data.txt";

// --- Fungsi-fungsi Pembantu (Helper Functions) ---

// Fungsi untuk membersihkan buffer input setelah membaca nilai numerik.
// Ini penting untuk mencegah masalah saat menggunakan std::getline() setelah std::cin >> int/double.
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Fungsi untuk mendapatkan input integer yang valid dari pengguna.
// Memastikan input adalah angka bulat dan berada dalam rentang minVal dan maxVal.
int getValidIntegerInput(const std::string& prompt, int minVal = 0, int maxVal = std::numeric_limits<int>::max()) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail() || value < minVal || value > maxVal) {
            std::cout << "Input tidak valid. Mohon masukkan angka bulat antara " << minVal << " dan " << maxVal << "." << std::endl;
            std::cin.clear();        // Bersihkan flag error pada std::cin
            clearInputBuffer();      // Bersihkan sisa input di buffer
        } else {
            clearInputBuffer();      // Bersihkan sisa newline di buffer
            return value;
        }
    }
}

// Fungsi untuk mendapatkan input double (angka desimal) yang valid dari pengguna.
// Memastikan input adalah angka desimal dan berada dalam rentang minVal dan maxVal.
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

// Fungsi untuk mendapatkan input string yang tidak kosong dari pengguna.
// Memastikan string yang dimasukkan tidak hanya spasi kosong.
std::string getNonEmptyStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        // Memeriksa apakah string kosong atau hanya terdiri dari spasi
        if (input.empty() || input.find_first_not_of(' ') == std::string::npos) {
            std::cout << "Input tidak boleh kosong. Mohon masukkan teks yang valid." << std::endl;
        } else {
            return input;
        }
    }
}

// Fungsi untuk mendapatkan tanggal saat ini dari sistem dan mengembalikannya dalam format YYYY-MM-DD.
std::string getCurrentDate() {
    auto now = std::chrono::system_clock::now(); // Dapatkan waktu saat ini
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now); // Konversi ke time_t
    std::stringstream ss; // Gunakan stringstream untuk memformat tanggal
    ss << std::put_time(std::localtime(&currentTime), "%Y-%m-%d"); // Format ke YYYY-MM-DD
    return ss.str(); // Kembalikan sebagai string
}

// Fungsi untuk menambahkan sejumlah hari ke tanggal yang diberikan.
// Berguna untuk menghitung perkiraan tanggal panen.
std::string addDaysToDate(const std::string& date_str, int days) {
    std::tm tm = {}; // Struktur tm untuk menyimpan komponen waktu
    std::istringstream ss(date_str); // Gunakan stringstream untuk parsing string tanggal
    ss >> std::get_time(&tm, "%Y-%m-%d"); // Parsing string tanggal ke struktur tm
    
    std::time_t time = std::mktime(&tm); // Konversi struktur tm ke time_t (detik sejak epoch)
    time += days * 24 * 60 * 60; // Tambahkan jumlah detik yang setara dengan 'days'
    
    std::tm* new_tm = std::localtime(&time); // Konversi kembali time_t ke struktur tm (waktu lokal)
    std::stringstream new_ss; // Stringstream baru untuk memformat tanggal hasil
    new_ss << std::put_time(new_tm, "%Y-%m-%d"); // Format kembali ke YYYY-MM-DD
    return new_ss.str(); // Kembalikan string tanggal baru
}

// Fungsi untuk mengonversi seluruh string menjadi huruf kecil (lowercase).
// Berguna untuk perbandingan string yang tidak sensitif huruf besar/kecil.
std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(), ::tolower); // Menggunakan std::transform dan ::tolower
    return s;
}

// Fungsi untuk memisahkan string berdasarkan delimiter tertentu dan mengembalikan vektor string.
// Digunakan saat memuat data dari file, di mana data dipisahkan oleh delimiter kustom "|||".
std::vector<std::string> splitString(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do {
        pos = s.find(delimiter, prev); // Cari posisi delimiter
        tokens.push_back(s.substr(prev, pos - prev)); // Ambil substring sebagai token
        prev = pos + delimiter.length(); // Pindahkan posisi awal untuk pencarian berikutnya
    } while (pos != std::string::npos); // Ulangi selama delimiter ditemukan
    return tokens;
}

// --- Fungsi Penyimpanan dan Pemuatan Data (Fitur 5: Penyimpanan & Pemuatan Data Otomatis) ---

// Menyimpan semua data (jenis sayuran, batch penanaman, transaksi penjualan) ke dalam file teks.
// Data disimpan dalam format yang terstruktur dengan delimiter kustom "|||".
void saveData() {
    std::ofstream outFile(DATA_FILE); // Buka file dalam mode tulis (akan membuat/menimpa jika sudah ada)
    if (!outFile.is_open()) { // Cek apakah file berhasil dibuka
        std::cerr << "ERROR: Gagal membuka file untuk menyimpan data." << std::endl;
        return;
    }

    // Simpan nilai ID unik berikutnya untuk setiap struktur, agar penomoran ID berlanjut
    outFile << nextVeggieTypeId << "\n";
    outFile << nextPlantingBatchId << "\n";
    outFile << nextSaleTransactionId << "\n";

    // Simpan data VegetableTypes
    outFile << "VEGETABLE_TYPES\n"; // Header seksi
    for (const auto& vt : veggieTypes) {
        outFile << vt.id << "|||" << vt.name << "|||" << vt.grow_duration_days << "\n";
    }

    // Simpan data PlantingBatches
    outFile << "PLANTING_BATCHES\n"; // Header seksi
    for (const auto& pb : plantingBatches) {
        outFile << pb.id << "|||" << pb.veggie_type_id << "|||" << pb.plant_date << "|||"
                << pb.harvest_date_est << "|||" << pb.harvested_quantity << "|||" << (pb.is_harvested ? "1" : "0") << "\n";
    }

    // Simpan data SaleTransactions
    outFile << "SALE_TRANSACTIONS\n"; // Header seksi
    for (const auto& st : salesTransactions) {
        outFile << st.id << "|||" << st.veggie_name << "|||" << st.sale_date << "|||"
                << st.quantity_sold << "|||" << st.price_per_unit << "|||" << st.total_price << "\n";
    }

    outFile.close(); // Tutup file
    std::cout << "[INFO] Data pertanian berhasil disimpan." << std::endl;
}

// Memuat semua data dari file teks ke dalam memori program saat startup.
// Program akan mencoba membaca data dari file; jika tidak ada, data baru akan dimulai.
void loadData() {
    std::ifstream inFile(DATA_FILE); // Buka file dalam mode baca
    if (!inFile.is_open()) { // Jika file tidak ditemukan, informasikan dan lanjutkan dengan data kosong
        std::cout << "[INFO] File data pertanian tidak ditemukan. Membuat data baru." << std::endl;
        return;
    }

    std::string line;
    // Baca nilai ID unik berikutnya
    if (std::getline(inFile, line)) nextVeggieTypeId = std::stoi(line);
    if (std::getline(inFile, line)) nextPlantingBatchId = std::stoi(line);
    if (std::getline(inFile, line)) nextSaleTransactionId = std::stoi(line);

    // Bersihkan vektor-vektor global sebelum memuat data baru
    veggieTypes.clear();
    plantingBatches.clear();
    salesTransactions.clear();

    // Baca data VegetableTypes
    if (std::getline(inFile, line) && line == "VEGETABLE_TYPES") {
        while (std::getline(inFile, line) && line != "PLANTING_BATCHES") { // Baca hingga menemukan header seksi berikutnya
            std::vector<std::string> segments = splitString(line, "|||"); // Pisahkan baris berdasarkan delimiter
            if (segments.size() == 3) { // Pastikan jumlah segmen benar
                VegetableType vt;
                try { // Gunakan try-catch untuk menangani kemungkinan error konversi string ke angka
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

    // Baca data PlantingBatches (melanjutkan dari posisi terakhir di file)
    if (line == "PLANTING_BATCHES") { // Cek apakah loop di atas berhenti karena menemukan header ini
        while (std::getline(inFile, line) && line != "SALE_TRANSACTIONS") {
            std::vector<std::string> segments = splitString(line, "|||");
            if (segments.size() == 6) {
                PlantingBatch pb;
                try {
                    pb.id = std::stoi(segments[0]);
                    pb.veggie_type_id = std::stoi(segments[1]);
                    pb.plant_date = segments[2];
                    pb.harvest_date_est = segments[3];
                    pb.harvested_quantity = std::stod(segments[4]); // string to double
                    pb.is_harvested = (segments[5] == "1"); // "1" berarti true, "0" berarti false
                    plantingBatches.push_back(pb);
                } catch (const std::exception& e) {
                    std::cerr << "Peringatan: Baris PlantingBatch korup dilewati. Error: " << e.what() << std::endl;
                }
            }
        }
    }

    // Baca data SaleTransactions (melanjutkan dari posisi terakhir di file)
    if (line == "SALE_TRANSACTIONS") { // Cek apakah loop di atas berhenti karena menemukan header ini
        while (std::getline(inFile, line)) { // Baca hingga akhir file
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

    inFile.close(); // Tutup file
    std::cout << "[INFO] Data pertanian berhasil dimuat." << std::endl;
}

// --- Fungsi Manajemen Jenis Sayuran (Fitur 1) ---

// Menambahkan jenis sayuran baru ke sistem. Meminta nama dan durasi tumbuh.
// Ada validasi untuk mencegah duplikasi nama sayuran.
void addVegetableType() {
    std::cout << "\n--- Tambah Jenis Sayuran Baru ---" << std::endl;
    std::string name = getNonEmptyStringInput("Masukkan nama sayuran (contoh: Bayam, Tomat): ");
    
    // Konversi nama input ke lowercase untuk cek duplikasi yang tidak sensitif huruf besar/kecil
    std::string lowerName = toLower(name);
    for (const auto& vt : veggieTypes) {
        if (toLower(vt.name) == lowerName) {
            std::cout << "[!] Jenis sayuran '" << name << "' sudah ada." << std::endl;
            return;
        }
    }

    int duration = getValidIntegerInput("Masukkan perkiraan durasi tanam hingga panen (dalam hari): ", 1);

    VegetableType newType = {nextVeggieTypeId++, name, duration}; // Buat objek baru dengan ID unik
    veggieTypes.push_back(newType); // Tambahkan ke vektor
    std::cout << "[✅] Jenis sayuran '" << name << "' berhasil ditambahkan." << std::endl;
    saveData(); // Simpan perubahan ke file
}

// Menampilkan daftar semua jenis sayuran yang terdaftar dalam format tabel yang rapi.
void viewVegetableTypes() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                  DAFTAR JENIS SAYURAN                    " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (veggieTypes.empty()) {
        std::cout << "Belum ada jenis sayuran yang terdaftar." << std::endl;
    } else {
        // Atur lebar kolom untuk tampilan tabel yang rapi
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

// Mengedit detail jenis sayuran berdasarkan ID. Memungkinkan perubahan nama dan durasi panen.
void editVegetableType() {
    viewVegetableTypes(); // Tampilkan daftar untuk memudahkan pemilihan ID
    if (veggieTypes.empty()) return; // Jika kosong, tidak ada yang bisa diedit

    int idToEdit = getValidIntegerInput("Masukkan ID jenis sayuran yang ingin diedit: ");
    VegetableType* targetType = nullptr; // Pointer untuk menyimpan alamat objek yang akan diedit
    for (auto& vt : veggieTypes) { // Iterasi untuk menemukan jenis sayuran berdasarkan ID
        if (vt.id == idToEdit) {
            targetType = &vt;
            break;
        }
    }

    if (!targetType) { // Jika ID tidak ditemukan
        std::cout << "[!] Jenis sayuran dengan ID " << idToEdit << " tidak ditemukan." << std::endl;
        return;
    }

    std::cout << "\n--- Edit Jenis Sayuran: " << targetType->name << " ---" << std::endl;
    std::cout << "Nama saat ini: " << targetType->name << std::endl;
    std::string newName = getNonEmptyStringInput("Masukkan nama baru (biarkan kosong untuk tidak mengubah): ");
    if (!newName.empty() && toLower(newName) != toLower(targetType->name)) {
        // Cek duplikasi nama baru, kecuali jika itu adalah nama dari objek yang sedang diedit
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
    saveData(); // Simpan perubahan
}

// Menghapus jenis sayuran berdasarkan ID.
// Ada validasi untuk mencegah penghapusan jika jenis sayuran tersebut masih terkait dengan penanaman aktif.
void deleteVegetableType() {
    viewVegetableTypes();
    if (veggieTypes.empty()) return;

    int idToDelete = getValidIntegerInput("Masukkan ID jenis sayuran yang ingin dihapus: ");

    // Cek apakah ada penanaman yang masih menggunakan jenis sayuran ini
    for (const auto& pb : plantingBatches) {
        if (pb.veggie_type_id == idToDelete) {
            std::cout << "[!] Tidak bisa menghapus jenis sayuran ini karena masih ada penanaman yang terkait (ID Penanaman: " << pb.id << ")." << std::endl;
            std::cout << "    Mohon hapus dulu penanaman tersebut atau update jenis sayurannya sebelum menghapus jenis sayuran ini." << std::endl;
            return;
        }
    }

    // Gunakan erase-remove idiom untuk menghapus elemen dari vektor
    auto it = std::remove_if(veggieTypes.begin(), veggieTypes.end(),
                             [idToDelete](const VegetableType& vt) {
                                 return vt.id == idToDelete;
                             });
    
    if (it != veggieTypes.end()) { // Jika elemen ditemukan dan dihapus
        veggieTypes.erase(it, veggieTypes.end());
        std::cout << "[✅] Jenis sayuran ID " << idToDelete << " berhasil dihapus." << std::endl;
        saveData(); // Simpan perubahan
    } else {
        std::cout << "[!] Jenis sayuran dengan ID " << idToDelete << " tidak ditemukan." << std::endl;
    }
}

// --- Fungsi Manajemen Penanaman (Fitur 2: Manajemen Lahan/Bedengan & Penanaman) ---

// Mencatat penanaman baru. Pengguna memilih jenis sayuran, dan tanggal panen estimasi dihitung otomatis.
void addPlantingBatch() {
    viewVegetableTypes(); // Tampilkan jenis sayuran yang ada
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

    if (!selectedType) { // Jika jenis sayuran tidak ditemukan
        std::cout << "[!] Jenis sayuran dengan ID " << veggieTypeId << " tidak ditemukan." << std::endl;
        return;
    }

    std::string plantDate = getCurrentDate(); // Tanggal tanam otomatis adalah tanggal saat ini
    std::string harvestDateEst = addDaysToDate(plantDate, selectedType->grow_duration_days); // Hitung estimasi panen

    PlantingBatch newBatch = {nextPlantingBatchId++, selectedType->id, plantDate, harvestDateEst, 0.0, false};
    plantingBatches.push_back(newBatch);
    std::cout << "\n[✅] Penanaman " << selectedType->name << " berhasil dicatat." << std::endl;
    std::cout << "   Tanggal Tanam: " << newBatch.plant_date << std::endl;
    std::cout << "   Estimasi Panen: " << newBatch.harvest_date_est << std::endl;
    saveData(); // Simpan perubahan
}

// Menampilkan semua batch penanaman yang dicatat, beserta status panennya.
void viewPlantingBatches() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   DAFTAR PENANAMAN                       " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (plantingBatches.empty()) {
        std::cout << "Belum ada penanaman yang dicatat." << std::endl;
    } else {
        // Atur lebar kolom untuk tampilan tabel
        std::cout << std::left << std::setw(5) << "ID"
                  << std::left << std::setw(20) << "Sayuran"
                  << std::left << std::setw(15) << "Tanam"
                  << std::left << std::setw(15) << "Panen (Est)"
                  << std::left << std::setw(10) << "Hasil"
                  << std::left << std::setw(10) << "Status" << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        for (const auto& pb : plantingBatches) {
            std::string veggieName = "Tidak Diketahui"; // Default jika ID jenis sayuran tidak valid
            for (const auto& vt : veggieTypes) { // Cari nama sayuran berdasarkan ID
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

// Memperbarui hasil panen untuk batch penanaman tertentu.
// Memungkinkan pencatatan kuantitas hasil panen dan menandai batch sebagai 'sudah panen'.
void updateHarvestResult() {
    viewPlantingBatches(); // Tampilkan daftar penanaman untuk memilih ID
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
    targetBatch->is_harvested = true; // Tandai sebagai sudah dipanen

    std::cout << "[✅] Hasil panen untuk penanaman ID " << idToUpdate << " berhasil dicatat: " << quantity << " kg." << std::endl;
    saveData(); // Simpan perubahan
}

// --- Fungsi Manajemen Stok & Penjualan Panen (Fitur 3) ---

// Menampilkan ringkasan stok sayuran yang sudah dipanen dan tersedia untuk dijual.
// Menghitung stok berdasarkan hasil panen dikurangi penjualan yang sudah tercatat.
void viewHarvestedStock() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   STOK PANEN TERSEDIA                    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    // Menggunakan std::map untuk menyimpan total stok per nama sayuran
    std::map<std::string, double> currentStock; 
    
    // Agregasi hasil panen dari semua batch yang sudah dipanen
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

    // Kurangi stok dengan jumlah sayuran yang sudah terjual
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
            // Hanya tampilkan sayuran yang memiliki stok positif (lebih dari mendekati nol)
            if (pair.second > 0.001) { 
                std::cout << std::left << std::setw(30) << pair.first
                          << std::left << std::fixed << std::setprecision(2) << std::setw(15) << pair.second << std::endl;
            }
        }
    }
    std::cout << "==========================================================\n" << std::endl;
}

// Mencatat transaksi penjualan baru. Mengurangi stok yang tersedia.
// Ada validasi untuk memastikan stok cukup sebelum penjualan dicatat.
void recordSale() {
    viewHarvestedStock(); // Tampilkan stok yang tersedia untuk referensi
    std::cout << "\n--- Catat Penjualan Panen ---" << std::endl;
    std::string veggieName = getNonEmptyStringInput("Masukkan nama sayuran yang terjual: ");
    
    // Cek apakah nama sayuran yang dimasukkan terdaftar sebagai jenis sayuran
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

    // Hitung ulang stok yang tersedia untuk sayuran spesifik ini
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
    // Kurangi juga dengan penjualan yang sudah ada
    for (const auto& st : salesTransactions) {
        if (toLower(st.veggie_name) == lowerVeggieName) {
            availableStock -= st.quantity_sold;
        }
    }

    // Validasi apakah stok cukup untuk penjualan ini
    if (quantityToSell > availableStock) {
        std::cout << "[!] Stok '" << veggieName << "' tidak cukup. Tersedia: " << std::fixed << std::setprecision(2) << availableStock << " kg." << std::endl;
        return;
    }

    double totalPrice = quantityToSell * pricePerUnit;
    SaleTransaction newSale = {nextSaleTransactionId++, veggieName, getCurrentDate(), quantityToSell, pricePerUnit, totalPrice};
    salesTransactions.push_back(newSale); // Tambahkan transaksi ke vektor
    
    std::cout << "\n[✅] Penjualan '" << newSale.veggie_name << "' sebanyak "
              << std::fixed << std::setprecision(2) << newSale.quantity_sold << " kg seharga Rp "
              << std::fixed << std::setprecision(2) << newSale.total_price << " berhasil dicatat." << std::endl;
    saveData(); // Simpan perubahan
}

// Menampilkan semua riwayat transaksi penjualan dalam format tabel.
void viewSaleHistory() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 RIWAYAT TRANSAKSI PENJUALAN              " << std::endl;
    std::cout << "==========================================================" << std::endl;
    if (salesTransactions.empty()) {
        std::cout << "Belum ada transaksi penjualan yang dicatat." << std::endl;
    } else {
        // Atur lebar kolom untuk tampilan tabel
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

// --- Fungsi Laporan & Statistik (Fitur 4) ---

// Menampilkan daftar penanaman yang diperkirakan akan panen di masa mendatang.
// Diurutkan berdasarkan tanggal panen terdekat.
void viewUpcomingHarvests() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                  PANEN MENDATANG                        " << std::endl;
    std::cout << "==========================================================" << std::endl;
    
    std::string today = getCurrentDate(); // Dapatkan tanggal saat ini

    std::vector<PlantingBatch> upcoming;
    for (const auto& pb : plantingBatches) {
        // Jika belum dipanen dan estimasi tanggal panennya sama atau setelah hari ini
        if (!pb.is_harvested && pb.harvest_date_est >= today) {
            upcoming.push_back(pb);
        }
    }

    // Urutkan penanaman mendatang berdasarkan tanggal estimasi panen (terdekat paling atas)
    std::sort(upcoming.begin(), upcoming.end(), [](const PlantingBatch& a, const PlantingBatch& b) {
        return a.harvest_date_est < b.harvest_date_est;
    });

    if (upcoming.empty()) {
        std::cout << "Tidak ada panen yang akan datang atau yang belum dipanen." << std::endl;
    } else {
        // Tampilkan dalam format tabel
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

// Menampilkan produk sayuran yang paling banyak terjual berdasarkan kuantitas.
void showBestSellingProducts() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                   PRODUK TERLARIS (BERDASARKAN JUMLAH TERJUAL)    " << std::endl;
    std::cout << "==========================================================" << std::endl;

    // Menggunakan map untuk menjumlahkan kuantitas terjual per nama sayuran
    std::map<std::string, double> soldQuantities;
    for (const auto& st : salesTransactions) {
        soldQuantities[st.veggie_name] += st.quantity_sold;
    }

    if (soldQuantities.empty()) {
        std::cout << "Belum ada data penjualan." << std::endl;
    } else {
        // Konversi map ke vektor pair agar bisa diurutkan
        std::vector<std::pair<std::string, double>> sortedSales(soldQuantities.begin(), soldQuantities.end());
        
        // Urutkan dari kuantitas terjual terbanyak ke paling sedikit
        std::sort(sortedSales.begin(), sortedSales.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second; 
        });

        // Tampilkan dalam format tabel
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

// Menampilkan total hasil panen untuk setiap jenis sayuran yang sudah dipanen.
void showTotalHarvestByVegetable() {
    std::cout << "\n==========================================================" << std::endl;
    std::cout << "                 TOTAL HASIL PANEN PER JENIS SAYURAN        " << std::endl;
    std::cout << "==========================================================" << std::endl;

    // Menggunakan map untuk menjumlahkan hasil panen per nama sayuran
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
        // Konversi map ke vektor pair agar bisa diurutkan
        std::vector<std::pair<std::string, double>> sortedHarvests(totalHarvests.begin(), totalHarvests.end());

        // Urutkan dari kuantitas panen terbanyak ke paling sedikit
        std::sort(sortedHarvests.begin(), sortedHarvests.end(), [](const std::pair<const std::string, double>& a, const std::pair<const std::string, double>& b) {
            return a.second > b.second;
        });

        // Tampilkan dalam format tabel
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

// Menampilkan total pendapatan kumulatif dari semua transaksi penjualan.
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


// --- Fungsi Utama (Main Program Loop) ---

int main() {
    loadData(); // Panggil fungsi untuk memuat data dari file saat program pertama kali dijalankan

    int choice;
    do {
        // Tampilan menu utama program
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

        // Mendapatkan pilihan pengguna
        choice = getValidIntegerInput("Pilihan Anda: ", 1, 15);

        // Menjalankan fungsi sesuai pilihan pengguna
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
                saveData(); // Pastikan data disimpan sebelum program keluar
                std::cout << "\nTerima kasih telah menggunakan Manajer Pertanian & Distribusi Sayuran! Selamat bertani dan berjualan." << std::endl;
                break;
            default: std::cout << "Pilihan tidak valid. Silakan coba lagi." << std::endl; break;
        }
    } while (choice != 15); // Loop terus sampai pengguna memilih 15 (Keluar)

    // Menunggu pengguna menekan Enter sebelum menutup konsol (berguna di beberapa IDE/lingkungan)
    std::cout << "Tekan Enter untuk keluar...";
    std::cin.get(); 

    return 0; // Mengindikasikan program berakhir dengan sukses
}
