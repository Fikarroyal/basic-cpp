// === RailTicket Pro ==============================================
// Kompilasi: g++ -std=c++17 -o rail rail.cpp
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <random>
#include <algorithm>

struct Train { int id; std::string name; };

struct Schedule {
    int id; Train train; std::string date, from, to, tclass;
    double price; int seatsAvail;
    std::string csv() const {
        std::ostringstream oss;
        oss << id << ',' << train.id << ',' << train.name << ','
            << date << ',' << from << ',' << to << ','
            << tclass << ',' << price << ',' << seatsAvail;
        return oss.str();
    }
};

struct Passenger { std::string name, idCard; };

struct Booking {
    std::string pnr; Schedule sched; Passenger pax; int seat;
    double paid;
};

class TicketSystem {
    std::vector<Schedule> schedules;
    int counterPNR{100};
public:
    TicketSystem() { loadSchedule(); }
    void menu() {
        int ch;
        while (true) {
            std::cout << "\n=== RailTicket Pro ===\n"
                      << "1. Lihat Jadwal\n2. Pesan Tiket\n0. Keluar\nPilih: ";
            std::cin >> ch;
            if (ch==1) listSchedule();
            else if (ch==2) bookTicket();
            else break;
        }
    }
private:
    void loadSchedule() {
        std::ifstream fin("schedule.csv");
        std::string line;
        while (getline(fin,line)) {
            std::istringstream iss(line);
            Schedule s;
            std::string tmp;
            getline(iss,tmp,','); s.id=stoi(tmp);
            getline(iss,tmp,','); s.train.id=stoi(tmp);
            getline(iss,s.train.name,',');
            getline(iss,s.date,','); getline(iss,s.from,',');
            getline(iss,s.to,','); getline(iss,s.tclass,',');
            getline(iss,tmp,','); s.price=stod(tmp);
            getline(iss,tmp,','); s.seatsAvail=stoi(tmp);
            schedules.push_back(s);
        }
    }
    void saveSchedule() {
        std::ofstream out("schedule.csv", std::ios::trunc);
        for (auto& s:schedules) out<<s.csv()<<'\n';
    }
    void listSchedule() {
        std::cout << std::left << std::setw(4)<<"ID"<<std::setw(12)<<"Tanggal"
                  <<std::setw(15)<<"Kereta"<<std::setw(6)<<"Dari"
                  <<std::setw(6)<<"Ke"<<std::setw(8)<<"Kelas"
                  <<std::setw(10)<<"Harga"<<std::setw(5)<<"Sisa\n";
        for (auto& s:schedules)
            std::cout<<std::setw(4)<<s.id<<std::setw(12)<<s.date
                     <<std::setw(15)<<s.train.name<<std::setw(6)<<s.from
                     <<std::setw(6)<<s.to<<std::setw(8)<<s.tclass
                     <<std::setw(10)<<s.price<<std::setw(5)<<s.seatsAvail<<"\n";
    }
    static std::string now() {
        time_t t=time(nullptr); char buf[15];
        strftime(buf,sizeof(buf),"%y%m%d%H%M%S",localtime(&t));
        return buf;
    }
    std::string genPNR() { return "P"+now()+std::to_string(counterPNR++); }

    void bookTicket() {
        int sid; listSchedule();
        std::cout<<"Masukkan ID jadwal: "; std::cin>>sid;
        auto it = std::find_if(schedules.begin(), schedules.end(),
                               [&](auto& s){return s.id==sid;});
        if (it==schedules.end()||it->seatsAvail==0){
            std::cout<<"Tidak tersedia.\n";return;}
        Passenger pax;
        std::cout<<"Nama Penumpang : "; std::cin.ignore(); getline(std::cin,pax.name);
        std::cout<<"No Identitas   : "; getline(std::cin,pax.idCard);

        int seat = rand()%100+1;
        double bayar;
        std::cout<<"Harga Rp"<<it->price<<", Bayar: "; std::cin>>bayar;
        if (bayar<it->price){std::cout<<"Kurang!\n";return;}

        Booking b{genPNR(),*it,pax,seat,bayar};
        it->seatsAvail--; saveSchedule();
        saveBooking(b);
        printTicket(b);
    }
    void saveBooking(const Booking& b){
        std::string fname="booking_"+b.sched.date+".csv";
        std::ofstream out(fname,std::ios::app);
        out<<b.pnr<<','<<b.pax.name<<','<<b.pax.idCard<<','
           <<b.sched.id<<','<<b.seat<<','<<b.paid<<'\n';
    }
    void printTicket(const Booking& b){
        std::ostringstream oss;
        oss<<"----------- T I K E T -----------\n"
           <<"PNR   : "<<b.pnr<<"\nNama  : "<<b.pax.name
           <<"\nID    : "<<b.pax.idCard<<"\nKereta: "<<b.sched.train.name
           <<"\nTanggal: "<<b.sched.date<<"  Kursi: "<<b.seat
           <<"\nRute   : "<<b.sched.from<<" - "<<b.sched.to
           <<"\nKelas  : "<<b.sched.tclass
           <<"\nHarga  : Rp"<<b.sched.price<<"\n-------------------------------\n";
        std::cout<<oss.str();
        std::ofstream out(b.pnr+".txt"); out<<oss.str();
        std::cout<<"Tiket tersimpan sebagai "<<b.pnr<<".txt\n";
    }
};

int main(){
    srand((unsigned)time(nullptr));
    TicketSystem ts; ts.menu();
    return 0;
}
