#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>

struct TiketKereta {
    std::string nama;
    std::string nomorIdentitas;
    std::string keretaApi;
    std::string berangkat;
    std::string waktuBerangkat;
    std::string kodeBooking;
    std::string tipePenumpang;
    std::string noTempatDuduk;
    std::string perkiraanTiba;
    std::string waktuTiba;
    std::string checkInCode;
};

std::string getCurrentDateTimeForCheckin() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%d-%m-%Y %H:%M", &tstruct);
    return buf;
}

std::string generateBarcodePlaceholder() {
    std::string barcode_str = "";
    for (int i = 0; i < 40; ++i) { 
        barcode_str += "#";
    }
    return barcode_str;
}

void cetakTiket(const TiketKereta& tiket) {
    std::cout << "\n\n";
    const int total_width = 90; 

    std::cout << std::string(total_width, '=') << std::endl;
    
    const int keretapi_indent = 15;
    const int boardingpass_start = 65;

    std::cout << std::left << std::setw(keretapi_indent) << " " << "KERETA API" 
              << std::setw(boardingpass_start - keretapi_indent - std::string("KERETA API").length()) << " "
              << std::right << "BOARDING PASS" << std::endl;
    std::cout << std::string(total_width, '=') << std::endl; 
    std::cout << std::endl;

    const int label_kiri_width = 28; 
    const int value_kiri_width = 28; 
    const int label_kanan_width = 30; 

    std::cout << std::left << std::setw(label_kiri_width) << "nama / name" << ": "
              << std::left << std::setw(value_kiri_width) << tiket.nama
              << std::left << std::setw(label_kanan_width) << "kodebooking / bookingcode" << ": "
              << tiket.kodeBooking << std::endl;

    std::cout << std::string(total_width, '=') << std::endl;
    std::cout << std::left << std::setw(label_kiri_width) << "nomor identitas / id number" << ": "
              << std::left << std::setw(value_kiri_width) << tiket.nomorIdentitas
              << std::left << std::setw(label_kanan_width) << "tipe penumpang / pax type" << ": "
              << tiket.tipePenumpang << std::endl;
    
    std::cout << std::string(total_width, '=') << std::endl;

    std::cout << std::left << std::setw(label_kiri_width) << "kereta api / train" << ": "
              << std::left << std::setw(value_kiri_width) << tiket.keretaApi
              << std::left << std::setw(label_kanan_width) << "no tempat duduk / seat number" << ": "
              << tiket.noTempatDuduk << std::endl;

    std::cout << std::string(total_width, '=') << std::endl;

    std::cout << std::left << std::setw(label_kiri_width) << "berangkat / departure" << ": "
              << std::left << std::setw(value_kiri_width) << tiket.berangkat
              << std::left << std::setw(label_kanan_width) << "perkiraan tiba / eta" << ": "
              << tiket.perkiraanTiba << std::endl;

    std::cout << std::left << std::setw(label_kiri_width + 2) << " " 
              << std::left << std::setw(value_kiri_width) << tiket.waktuBerangkat
              << std::left << std::setw(label_kanan_width + 2) << " " 
              << tiket.waktuTiba << std::endl;

    std::cout << std::string(total_width, '=') << std::endl;
    const int barcode_length = generateBarcodePlaceholder().length();
    const int barcode_align_offset = total_width - barcode_length - 5; 
    std::cout << std::setw(barcode_align_offset) << " " << generateBarcodePlaceholder() << std::endl;
    std::cout << std::setw(barcode_align_offset) << " " << "CGMR1_xxxxxxxx" << std::endl; 
    std::cout << std::right << std::setw(total_width) << "check in at " << tiket.checkInCode << std::endl; 

    std::cout << std::string(total_width, '=') << std::endl;
    std::cout << "\n\n";
}

std::string getInputString(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin >> std::ws, input); 
    return input;
}

int main() {
    srand(static_cast<unsigned int>(time(0))); 

    TiketKereta tiketSaya;

    std::cout << "================================================" << std::endl;
    std::cout << "  SISTEM PENCETAKAN TIKET KERETA API INDONESIA  " << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Silakan masukkan data penumpang dan perjalanan:" << std::endl;

    tiketSaya.nama = getInputString("Nama Penumpang: ");
    tiketSaya.nomorIdentitas = getInputString("Nomor Identitas (KTP/Paspor): ");
    tiketSaya.keretaApi = getInputString("Nama Kereta Api / Nomor Kereta (Contoh: ARGO ANGGREK PAGI / 2): ");
    tiketSaya.berangkat = getInputString("Stasiun Keberangkatan (Contoh: GAMBIR(GMR)): ");
    tiketSaya.waktuBerangkat = getInputString("Waktu Keberangkatan (Contoh: 28 JUNI 2017; 09:30 WIB): ");

    int randomNumBooking = rand() % 900 + 100; 
    tiketSaya.kodeBooking = "WIK" + std::to_string(randomNumBooking);

    tiketSaya.tipePenumpang = getInputString("Tipe Penumpang (Contoh: UMUM): ");

    std::string kelasOptions[] = {"EKS", "BIS", "EKO"}; 
    std::string kelasPilih = kelasOptions[rand() % 3]; 
    int nomorGerbong = rand() % 10 + 1; 
    char hurufKursi = 'A' + (rand() % 4); 
    int nomorKursi = rand() % 30 + 1; 
    tiketSaya.noTempatDuduk = kelasPilih + " " + std::to_string(nomorGerbong) + "; " + std::to_string(nomorKursi) + std::string(1, hurufKursi);

    tiketSaya.perkiraanTiba = getInputString("Stasiun Tujuan (Contoh: SURABAYA PASAR TURI(SBI)): ");
    tiketSaya.waktuTiba = getInputString("Waktu Perkiraan Tiba (Contoh: 28 JUNI 2017; 18:30 WIB): ");

    std::string randomCheckInPrefix = "CGMR1_";
    for(int i = 0; i < 8; ++i) { 
        randomCheckInPrefix += std::to_string(rand() % 10);
    }
    tiketSaya.checkInCode = randomCheckInPrefix + " " + getCurrentDateTimeForCheckin();
    std::cout << "\nData telah dimasukkan. Mencetak tiket..." << std::endl;
    cetakTiket(tiketSaya);

    std::cout << "\nProgram selesai. Tekan Enter untuk keluar.";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
    std::cin.get(); 
    
    return 0;
}
