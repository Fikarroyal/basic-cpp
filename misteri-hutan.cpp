#include <iostream>   // Input/Output
#include <vector>     // std::vector
#include <string>     // std::string, std::getline
#include <map>        // std::map untuk inventaris
#include <limits>     // std::numeric_limits (untuk membersihkan buffer)
#include <iomanip>    // std::setw, std::left/right (untuk tampilan)

// --- Global Variable / Game State ---
int playerHealth; // Kesehatan pemain
std::map<std::string, int> inventory; // Inventaris pemain (nama item, jumlah)
bool gameOver; // Menandakan apakah game sudah berakhir

// --- Fungsi untuk Membersihkan Buffer Input ---
void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// --- Fungsi untuk Mendapatkan Input Pilihan Angka yang Valid ---
// Menerima prompt, nilai minimum, dan nilai maksimum yang diizinkan
int getValidChoiceInput(const std::string& prompt, int min, int max) {
    int choice;
    while (true) {
        std::cout << prompt;
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cout << "Pilihan tidak valid. Mohon masukkan angka antara " << min << "-" << max << "." << std::endl;
            std::cin.clear();
            clearInputBuffer();
        } else {
            clearInputBuffer(); // Bersihkan buffer setelah input valid
            return choice;
        }
    }
}

// --- Fungsi untuk Menampilkan Status Pemain ---
void displayPlayerStatus() {
    std::cout << "\n------------------- STATUS ANDA -------------------" << std::endl;
    std::cout << "Kesehatan  : " << playerHealth << " HP" << std::endl;
    std::cout << "Inventaris :" << std::endl;
    if (inventory.empty()) {
        std::cout << "  (Kosong)" << std::endl;
    } else {
        for (const auto& item : inventory) {
            std::cout << "  - " << item.first << " (" << item.second << "x)" << std::endl;
        }
    }
    std::cout << "---------------------------------------------------\n" << std::endl;
}

// --- Fungsi untuk Menambahkan Item ke Inventaris ---
void addItemToInventory(const std::string& item, int quantity = 1) {
    inventory[item] += quantity;
    std::cout << "\n[!] Anda mendapatkan " << quantity << "x **" << item << "**!" << std::endl;
    displayPlayerStatus(); // Perbarui status setelah item ditambahkan
}

// --- Fungsi untuk Menghapus Item dari Inventaris ---
bool removeItemFromInventory(const std::string& item, int quantity = 1) {
    if (inventory.count(item) && inventory[item] >= quantity) {
        inventory[item] -= quantity;
        if (inventory[item] == 0) {
            inventory.erase(item); // Hapus entri jika jumlahnya 0
        }
        std::cout << "\n[!] Anda menggunakan " << quantity << "x **" << item << "**." << std::endl;
        displayPlayerStatus(); // Perbarui status setelah item digunakan
        return true;
    }
    std::cout << "\n[!] Anda tidak memiliki cukup **" << item << "**." << std::endl;
    return false; // Item tidak cukup atau tidak ada
}

// --- Fungsi untuk Mengelola Kesehatan Pemain ---
void changePlayerHealth(int amount, const std::string& reason) {
    playerHealth += amount;
    if (amount > 0) {
        std::cout << "\n[+] Kesehatan Anda bertambah " << amount << " HP karena " << reason << "." << std::endl;
    } else {
        std::cout << "\n[-] Kesehatan Anda berkurang " << -amount << " HP karena " << reason << "." << std::endl;
    }
    if (playerHealth > 100) playerHealth = 100; // Batasi kesehatan maksimal
    
    displayPlayerStatus(); // Tampilkan status kesehatan terkini

    if (playerHealth <= 0) {
        gameOver = true; // Set game over menjadi true
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "                      GAME OVER                       " << std::endl;
        std::cout << " " << reason << std::endl;
        std::cout << " Petualangan Anda di Hutan Kegelapan berakhir di sini." << std::endl;
        std::cout << " Coba lagi lain kali untuk menemukan jalan keluar!" << std::endl;
        std::cout << "=======================================================\n" << std::endl;
    }
}

// --- FUNGSI UNTUK SKENARIO GAME (SETIAP FUNGSI ADALAH SATU LOKASI/PERISTIWA) ---
// Perhatikan: Setiap fungsi akan memeriksa 'gameOver' sebelum melanjutkan
void persimpanganHutan();
void guaMisterius();
void sungaiBerarus();
void perkampunganTua();
void akhirMenang();
void akhirKalah(const std::string& reason); // Fungsi akhirKalah sudah diintegrasikan ke changePlayerHealth


void mulaiPetualangan() {
    if (gameOver) return; // Langsung keluar jika sudah game over

    std::cout << "=======================================================" << std::endl;
    std::cout << "           MISTERI HUTAN KEGELAPAN DIMULAI             " << std::endl;
    std::cout << "=======================================================\n" << std::endl;
    std::cout << "Anda terbangun di tengah hutan yang gelap dan sunyi, tanpa mengingat bagaimana Anda sampai di sini." << std::endl;
    std::cout << "Kabut tebal menyelimuti pepohonan tinggi, dan suara-suara asing terdengar dari kejauhan." << std::endl;
    std::cout << "Anda merasa sedikit pusing dan hanya memiliki sedikit persediaan.\n" << std::endl;
    displayPlayerStatus();

    std::cout << "Apa yang ingin Anda lakukan?" << std::endl;
    std::cout << "1. Mencoba mengingat kembali apa yang terjadi (berisiko)." << std::endl;
    std::cout << "2. Segera mencari jalan keluar dari hutan." << std::endl;

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 2);

    if (choice == 1) {
        std::cout << "\nAnda mencoba memusatkan pikiran, tapi hanya rasa sakit kepala yang Anda dapatkan." << std::endl;
        changePlayerHealth(-5, "memaksakan diri");
        if (gameOver) return; // Penting: cek setelah perubahan kesehatan
        persimpanganHutan();
    } else {
        std::cout << "\nAnda memutuskan untuk tidak membuang waktu dan segera mencari jalan keluar." << std::endl;
        persimpanganHutan();
    }
}

void persimpanganHutan() {
    if (gameOver) return;

    std::cout << "\n======================== LOKASI: PERSIMPANGAN HUTAN ========================" << std::endl;
    std::cout << "Anda tiba di sebuah persimpangan di hutan. Ada tiga jalur yang bisa Anda pilih:" << std::endl;
    std::cout << "1. Jalan setapak ke arah Utara (terlihat gelap dan misterius)." << std::endl;
    std::cout << "2. Jalan setapak ke arah Timur (terdengar suara gemericik air)." << std::endl;
    std::cout << "3. Kembali ke arah Selatan (jalan yang baru saja Anda lewati, terasa semakin membingungkan)." << std::endl;
    displayPlayerStatus();

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 3);

    if (choice == 1) {
        guaMisterius();
    } else if (choice == 2) {
        sungaiBerarus();
    } else {
        std::cout << "\nAnda mencoba kembali ke Selatan, namun hutan terasa semakin membingungkan." << std::endl;
        changePlayerHealth(-10, "tersesat di jalur yang sama");
        if (gameOver) return;
        persimpanganHutan(); // Kembali ke persimpangan lagi
    }
}

void guaMisterius() {
    if (gameOver) return;

    std::cout << "\n======================== LOKASI: GUA MISTERIUS ========================" << std::endl;
    std::cout << "Anda memilih jalan ke Utara dan menemukan sebuah gua gelap. Dari dalam gua tercium bau apek dan kelembaban." << std::endl;
    displayPlayerStatus();

    std::cout << "Apa yang ingin Anda lakukan?" << std::endl;
    std::cout << "1. Masuk ke dalam gua (perlu Obor atau berisiko)." << std::endl;
    std::cout << "2. Mencari jalan lain di sekitar gua (mungkin ada sesuatu)." << std::endl;
    std::cout << "3. Kembali ke persimpangan." << std::endl;

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 3);

    if (choice == 1) {
        std::cout << "\nAnda melangkah masuk ke dalam kegelapan gua." << std::endl;
        if (inventory.count("Obor") > 0) {
            std::cout << "Dengan **Obor** Anda, Anda bisa melihat sekeliling." << std::endl;
            std::cout << "Di sudut gua, Anda menemukan sebuah kotak kecil berisi **Kunci Kuno**!" << std::endl;
            addItemToInventory("Kunci Kuno");
            std::cout << "Anda merasa lebih aman dan kembali keluar.\n" << std::endl;
            persimpanganHutan();
        } else {
            std::cout << "Anda tidak memiliki sumber cahaya dan tersandung sesuatu yang tajam!" << std::endl;
            changePlayerHealth(-20, "terluka di dalam kegelapan gua");
            if (gameOver) return;
            std::cout << "Anda segera keluar dari gua dengan panik." << std::endl;
            persimpanganHutan();
        }
    } else if (choice == 2) {
        std::cout << "\nAnda mencoba mencari jalan lain di sekitar gua." << std::endl;
        std::cout << "Setelah beberapa saat, Anda menemukan sebuah **Obor** tua tergeletak di tanah!" << std::endl;
        addItemToInventory("Obor");
        std::cout << "Anda kembali ke persimpangan.\n" << std::endl;
        persimpanganHutan();
    } else {
        persimpanganHutan();
    }
}

void sungaiBerarus() {
    if (gameOver) return;

    std::cout << "\n======================== LOKASI: SUNGAI BERARUS ========================" << std::endl;
    std::cout << "Anda mengikuti suara gemericik air dan tiba di tepi sungai yang berarus deras." << std::endl;
    std::cout << "Ada jembatan gantung tua di seberang, tapi terlihat rapuh." << std::endl;
    displayPlayerStatus();

    std::cout << "Apa yang ingin Anda lakukan?" << std::endl;
    std::cout << "1. Mencoba menyeberangi jembatan (berisiko jika kesehatan rendah)." << std::endl;
    std::cout << "2. Mencari jalan memutar menyusuri sungai." << std::endl;
    std::cout << "3. Kembali ke persimpangan." << std::endl;

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 3);

    if (choice == 1) {
        std::cout << "\nAnda perlahan mulai menyeberangi jembatan yang reyot." << std::endl;
        if (playerHealth >= 40) { // Lebih mudah jika sehat
            std::cout << "Dengan hati-hati dan langkah mantap, Anda berhasil menyeberang!" << std::endl;
            perkampunganTua();
        } else {
            std::cout << "Jembatan bergoyang kencang, dan Anda terpeleset karena lemas!" << std::endl;
            changePlayerHealth(-30, "jatuh ke sungai dan hanyut sebentar");
            if (gameOver) return;
            std::cout << "Anda berhasil berpegangan, tapi tidak berani melanjutkan. Anda kembali ke tepi." << std::endl;
            sungaiBerarus(); // Kembali ke sungai
        }
    } else if (choice == 2) {
        std::cout << "\nAnda memutuskan untuk berjalan memutar menyusuri tepi sungai." << std::endl;
        std::cout << "Perjalanan cukup melelahkan, tapi Anda menemukan jalur rahasia menuju..." << std::endl;
        changePlayerHealth(-15, "kelelahan mencari jalan memutar");
        if (gameOver) return;
        perkampunganTua();
    } else {
        persimpanganHutan();
    }
}

void perkampunganTua() {
    if (gameOver) return;

    std::cout << "\n======================== LOKASI: PERKAMPUNGAN TUA ========================" << std::endl;
    std::cout << "Anda tiba di tepi perkampungan tua yang sepi. Bangunan-bangunannya terlihat usang." << std::endl;
    std::cout << "Ada sebuah gerbang besi besar di ujung jalan, menuju ke luar hutan.\n" << std::endl;
    displayPlayerStatus();

    std::cout << "Apa yang ingin Anda lakukan?" << std::endl;
    std::cout << "1. Mendekati gerbang besi." << std::endl;
    std::cout << "2. Mencari tanda-tanda kehidupan di perkampungan (mungkin ada bantuan atau bahaya)." << std::endl;
    std::cout << "3. Kembali ke sungai." << std::endl;

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 3);

    if (choice == 1) {
        std::cout << "\nAnda mendekati gerbang. Terkunci rapat." << std::endl;
        if (inventory.count("Kunci Kuno") > 0 && inventory["Kunci Kuno"] > 0) {
            std::cout << "Anda mencoba menggunakan **Kunci Kuno**..." << std::endl;
            if (removeItemFromInventory("Kunci Kuno")) { // Pastikan item terhapus
                std::cout << "Klik! Gerbang terbuka!" << std::endl;
                akhirMenang(); // Ini adalah ending kemenangan
            } else { // Seharusnya tidak terjadi karena sudah dicek .count() dan > 0
                std::cout << "Entah kenapa, kunci Anda tidak bisa digunakan. Anda kebingungan." << std::endl;
                perkampunganTua(); // Kembali ke perkampungan
            }
        } else {
            std::cout << "Gerbang ini terkunci. Anda tidak punya kunci yang cocok." << std::endl;
            std::cout << "Tiba-tiba, Anda mendengar langkah kaki mendekat dari dalam hutan!" << std::endl;
            changePlayerHealth(-25, "panik karena terjebak");
            if (gameOver) return;
            std::cout << "Anda bersembunyi di balik semak dan harus memikirkan cara lain." << std::endl;
            perkampunganTua(); // Kembali ke perkampungan
        }
    } else if (choice == 2) {
        std::cout << "\nAnda menjelajahi perkampungan. Suasana sangat hening." << std::endl;
        std::cout << "Anda menemukan sebuah rumah yang terlihat sedikit terawat. Di dalamnya ada kotak P3K!" << std::endl;
        changePlayerHealth(30, "menemukan kotak P3K");
        // Tidak ada cek gameOver di sini karena HP bertambah
        std::cout << "Setelah itu, Anda kembali ke gerbang.\n" << std::endl;
        perkampunganTua();
    } else {
        sungaiBerarus(); // Kembali ke sungai
    }
}

// --- AKHIR GAME (Ending Kemenangan Saja, Kalah sudah di changePlayerHealth) ---

void akhirMenang() {
    if (gameOver) return; // Pastikan tidak ada double ending

    std::cout << "\n=======================================================" << std::endl;
    std::cout << "                      SELAMAT!                        " << std::endl;
    std::cout << " Anda berhasil membuka gerbang dan melihat jalan raya di kejauhan!" << std::endl;
    std::cout << " Anda telah lolos dari Misteri Hutan Kegelapan!" << std::endl;
    std::cout << " Petualangan Anda berakhir dengan sukses.               " << std::endl;
    std::cout << "=======================================================\n" << std::endl;
    gameOver = true; // Set game over karena sudah menang
}

// --- Fungsi Penutup Aplikasi ---
void akhirKeluarAplikasi() {
    std::cout << "\n******************************************************" << std::endl;
    std::cout << "* TERIMA KASIH SUDAH BERMAIN!                        *" << std::endl;
    std::cout << "* Semoga petualangan Anda selanjutnya lebih beruntung. *" << std::endl;
    std::cout << "******************************************************" << std::endl;
}

// --- Fungsi Main Game ---
int main() {
    char mainLagi;

    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI PETUALANGAN TEKS INTERAKTIF!    *" << std::endl;
    std::cout << "* Buat keputusan bijak untuk bertahan hidup!        *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    do {
        // --- Reset Game State untuk Permainan Baru ---
        playerHealth = 100;
        inventory.clear();
        gameOver = false; // Reset status game over

        mulaiPetualangan(); // Mulai cerita

        // Jika game sudah berakhir (menang atau kalah), tanyakan apakah mau main lagi
        if (gameOver) {
            std::cout << "\nIngin mencoba petualangan lain? (Y/N): ";
            std::cin >> mainLagi;
            clearInputBuffer();
        } else {
            // Ini akan terjadi jika pemain keluar dari game dengan cara yang tidak normal
            // Misalnya, jika ada jalur yang belum diimplementasikan mengarah ke sini
            // Untuk skenario ini, kita anggap pemain tidak mau lanjut
            mainLagi = 'N'; 
        }

    } while (mainLagi == 'Y' || mainLagi == 'y');

    akhirKeluarAplikasi(); // Pesan penutup aplikasi
    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get(); // Menunggu user menekan enter sebelum menutup konsol

    return 0;
}
