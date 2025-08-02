#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <limits>
#include <iomanip>

int playerHealth;
std::map<std::string, int> inventory;
bool gameOver;

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

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
            clearInputBuffer();
            return choice;
        }
    }
}

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

void addItemToInventory(const std::string& item, int quantity = 1) {
    inventory[item] += quantity;
    std::cout << "\n[!] Anda mendapatkan " << quantity << "x **" << item << "**!" << std::endl;
    displayPlayerStatus();
}

bool removeItemFromInventory(const std::string& item, int quantity = 1) {
    if (inventory.count(item) && inventory[item] >= quantity) {
        inventory[item] -= quantity;
        if (inventory[item] == 0) {
            inventory.erase(item);
        }
        std::cout << "\n[!] Anda menggunakan " << quantity << "x **" << item << "**." << std::endl;
        displayPlayerStatus();
        return true;
    }
    std::cout << "\n[!] Anda tidak memiliki cukup **" << item << "**." << std::endl;
    return false;
}

void changePlayerHealth(int amount, const std::string& reason) {
    playerHealth += amount;
    if (amount > 0) {
        std::cout << "\n[+] Kesehatan Anda bertambah " << amount << " HP karena " << reason << "." << std::endl;
    } else {
        std::cout << "\n[-] Kesehatan Anda berkurang " << -amount << " HP karena " << reason << "." << std::endl;
    }
    if (playerHealth > 100) playerHealth = 100;
    
    displayPlayerStatus();

    if (playerHealth <= 0) {
        gameOver = true;
        std::cout << "\n=======================================================" << std::endl;
        std::cout << "                      GAME OVER                       " << std::endl;
        std::cout << " " << reason << std::endl;
        std::cout << " Petualangan Anda di Hutan Kegelapan berakhir di sini." << std::endl;
        std::cout << " Coba lagi lain kali untuk menemukan jalan keluar!" << std::endl;
        std::cout << "=======================================================\n" << std::endl;
    }
}

void persimpanganHutan();
void guaMisterius();
void sungaiBerarus();
void perkampunganTua();
void akhirMenang();
void akhirKalah(const std::string& reason);


void mulaiPetualangan() {
    if (gameOver) return;

    std::cout << "=======================================================" << std::endl;
    std::cout << "              MISTERI HUTAN KEGELAPAN DIMULAI              " << std::endl;
    std::cout << "=======================================================\n" << std::endl;
    std::cout << "Anda terbangun di tengah hutan yang gelap dan sunyi, tanpa mengingat bagaimana Anda sampai di sini." << std::endl;
    std::cout << "Kabut tebal menyelimuti pepohonan tinggi, dan suara-suara asing terdengar dari kejauhan." << std::cout << "Anda merasa sedikit pusing dan hanya memiliki sedikit persediaan.\n" << std::endl;
    displayPlayerStatus();

    std::cout << "Apa yang ingin Anda lakukan?" << std::endl;
    std::cout << "1. Mencoba mengingat kembali apa yang terjadi (berisiko)." << std::endl;
    std::cout << "2. Segera mencari jalan keluar dari hutan." << std::endl;

    int choice = getValidChoiceInput("Pilihan Anda: ", 1, 2);

    if (choice == 1) {
        std::cout << "\nAnda mencoba memusatkan pikiran, tapi hanya rasa sakit kepala yang Anda dapatkan." << std::endl;
        changePlayerHealth(-5, "memaksakan diri");
        if (gameOver) return;
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
        persimpanganHutan();
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
        if (playerHealth >= 40) {
            std::cout << "Dengan hati-hati dan langkah mantap, Anda berhasil menyeberang!" << std::endl;
            perkampunganTua();
        } else {
            std::cout << "Jembatan bergoyang kencang, dan Anda terpeleset karena lemas!" << std::endl;
            changePlayerHealth(-30, "jatuh ke sungai dan hanyut sebentar");
            if (gameOver) return;
            std::cout << "Anda berhasil berpegangan, tapi tidak berani melanjutkan. Anda kembali ke tepi." << std::endl;
            sungaiBerarus();
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
            if (removeItemFromInventory("Kunci Kuno")) {
                std::cout << "Klik! Gerbang terbuka!" << std::endl;
                akhirMenang();
            } else {
                std::cout << "Entah kenapa, kunci Anda tidak bisa digunakan. Anda kebingungan." << std::endl;
                perkampunganTua();
            }
        } else {
            std::cout << "Gerbang ini terkunci. Anda tidak punya kunci yang cocok." << std::endl;
            std::cout << "Tiba-tiba, Anda mendengar langkah kaki mendekat dari dalam hutan!" << std::endl;
            changePlayerHealth(-25, "panik karena terjebak");
            if (gameOver) return;
            std::cout << "Anda bersembunyi di balik semak dan harus memikirkan cara lain." << std::endl;
            perkampunganTua();
        }
    } else if (choice == 2) {
        std::cout << "\nAnda menjelajahi perkampungan. Suasana sangat hening." << std::endl;
        std::cout << "Anda menemukan sebuah rumah yang terlihat sedikit terawat. Di dalamnya ada kotak P3K!" << std::endl;
        changePlayerHealth(30, "menemukan kotak P3K");
        std::cout << "Setelah itu, Anda kembali ke gerbang.\n" << std::endl;
        perkampunganTua();
    } else {
        sungaiBerarus();
    }
}

void akhirMenang() {
    if (gameOver) return;

    std::cout << "\n=======================================================" << std::endl;
    std::cout << "                      SELAMAT!                        " << std::endl;
    std::cout << " Anda berhasil membuka gerbang dan melihat jalan raya di kejauhan!" << std::endl;
    std::cout << " Anda telah lolos dari Misteri Hutan Kegelapan!" << std::endl;
    std::cout << " Petualangan Anda berakhir dengan sukses.              " << std::endl;
    std::cout << "=======================================================\n" << std::endl;
    gameOver = true;
}

void akhirKeluarAplikasi() {
    std::cout << "\n******************************************************" << std::endl;
    std::cout << "* TERIMA KASIH SUDAH BERMAIN!                          *" << std::endl;
    std::cout << "* Semoga petualangan Anda selanjutnya lebih beruntung. *" << std::endl;
    std::cout << "******************************************************" << std::endl;
}

int main() {
    char mainLagi;

    std::cout << "*****************************************************" << std::endl;
    std::cout << "* SELAMAT DATANG DI PETUALANGAN TEKS INTERAKTIF!    *" << std::endl;
    std::cout << "* Buat keputusan bijak untuk bertahan hidup!        *" << std::endl;
    std::cout << "*****************************************************" << std::endl;

    do {
        playerHealth = 100;
        inventory.clear();
        gameOver = false;

        mulaiPetualangan();

        if (gameOver) {
            std::cout << "\nIngin mencoba petualangan lain? (Y/N): ";
            std::cin >> mainLagi;
            clearInputBuffer();
        } else {
            mainLagi = 'N';
        }

    } while (mainLagi == 'Y' || mainLagi == 'y');

    akhirKeluarAplikasi();
    std::cout << "\nTekan Enter untuk keluar...";
    std::cin.get();

    return 0;
}
