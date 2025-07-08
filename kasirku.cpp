// === MiniPOS: sistem kasir sederhana  ===========================
// Kompilasi: g++ -std=c++17 -o minipos minipos.cpp
// Jalankan : ./minipos
#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>

struct Product {
    int    id{};
    std::string name;
    double price{};
    int    stock{};

    std::string csv() const {
        std::ostringstream oss;
        oss << id << ',' << name << ',' << price << ',' << stock;
        return oss.str();
    }
};

class Inventory {
    std::vector<Product> products;
    const std::string file{"products.csv"};
public:
    Inventory() { load(); }

    void load() {
        products.clear();
        std::ifstream fin(file);
        std::string line;
        while (std::getline(fin, line)) {
            std::istringstream iss(line);
            Product p;
            std::getline(iss, line, ','); p.id = std::stoi(line);
            std::getline(iss, p.name, ',');
            std::getline(iss, line, ','); p.price = std::stod(line);
            std::getline(iss, line, ','); p.stock = std::stoi(line);
            products.push_back(p);
        }
    }
    void save() const {
        std::ofstream fout(file, std::ios::trunc);
        for (auto& p : products) fout << p.csv() << '\n';
    }
    Product* find(int id) {
        for (auto& p : products)
            if (p.id == id) return &p;
        return nullptr;
    }
    void list() const {
        std::cout << "\n== Daftar Produk ==\n";
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20)
                  << "Nama" << std::setw(10) << "Harga"
                  << std::setw(6) << "Stok\n";
        for (auto& p : products)
            std::cout << std::setw(5) << p.id << std::setw(20) << p.name
                      << std::setw(10) << p.price << std::setw(6) << p.stock << '\n';
    }
    void add() {
        Product p;
        std::cout << "ID baru             : "; std::cin >> p.id;
        std::cout << "Nama produk         : "; std::cin.ignore(); std::getline(std::cin, p.name);
        std::cout << "Harga (Rp)          : "; std::cin >> p.price;
        std::cout << "Stok awal           : "; std::cin >> p.stock;
        products.push_back(p);
        save();
        std::cout << "Produk tersimpan.\n";
    }
};

struct CartItem { Product* p; int qty; };
class Cart {
    std::vector<CartItem> items;
public:
    void add(Product* p, int qty) {
        if (p->stock < qty) {
            std::cout << "Stok tidak cukup!\n"; return;
        }
        items.push_back({p, qty});
        p->stock -= qty;
    }
    double total() const {
        double t=0; for(auto& i:items) t+= i.p->price*i.qty; return t;
    }
    void print() const {
        std::cout << "\n--- Keranjang ---\n";
        for (auto& i:items)
            std::cout << i.p->name << " x" << i.qty
                      << " = Rp" << i.p->price*i.qty << '\n';
        std::cout << "Total: Rp" << total() << '\n';
    }
    std::vector<CartItem> get() const { return items; }
};

struct Transaction {
    int id;
    Cart cart;
    std::string datetime;
    double paid{}, change{};
    std::string csv() const {
        std::ostringstream oss;
        oss << id << ',' << datetime << ',' << cart.total()
            << ',' << paid << ',' << change;
        return oss.str();
    }
};

class POSManager {
    Inventory inv;
    std::vector<Transaction> logs;
    int nextTrx{1};
public:
    void mainMenu() {
        int ch;
        while (true) {
            std::cout << "\n=== MiniPOS ===\n"
                      << "1. Transaksi Baru\n"
                      << "2. Kelola Inventori\n"
                      << "3. Laporan Hari Ini\n"
                      << "0. Keluar\n"
                      << "Pilih: "; std::cin >> ch;
            if (ch==1) newSale();
            else if (ch==2) manageInventory();
            else if (ch==3) report();
            else if (ch==0) { inv.save(); break; }
        }
    }
private:
    static std::string now() {
        time_t t=time(nullptr); char buf[20];
        strftime(buf, sizeof(buf), "%F %T", localtime(&t));
        return buf;
    }
    void newSale() {
        Cart cart; int id, qty;
        while (true) {
            inv.list();
            std::cout << "ID produk (0 selesai): "; std::cin >> id;
            if (id==0) break;
            Product* p = inv.find(id);
            if (!p) { std::cout<<"Tidak ada!\n"; continue; }
            std::cout << "Qty: "; std::cin >> qty;
            cart.add(p, qty);
        }
        cart.print();
        double bayar; std::cout << "Uang diterima Rp: "; std::cin >> bayar;
        double kembalian = bayar - cart.total();
        std::cout << "Kembalian: Rp" << kembalian << "\n";

        Transaction trx{nextTrx++, cart, now(), bayar, kembalian};
        logs.push_back(trx);
        saveTransaction(trx);
    }
    void manageInventory() {
        inv.list();
        std::cout << "\n1. Tambah Produk 2. Kembali: "; int c; std::cin>>c;
        if (c==1) inv.add();
    }
    void report() const {
        std::cout << "\n=== Laporan Transaksi ===\n";
        for (auto& t : logs)
            std::cout << "Trx#" << t.id
                      << " [" << t.datetime << "] Total Rp" << t.cart.total() << '\n';
    }
    void saveTransaction(const Transaction& t) const {
        std::string fname="sales_"+t.datetime.substr(0,10)+".csv";
        std::ofstream f("data/"+fname, std::ios::app);
        f << t.csv() << '\n';
    }
};

int main() {
    POSManager pos;
    pos.mainMenu();
    return 0;
}
