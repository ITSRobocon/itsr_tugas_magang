#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <map>
#include <ctime>

using namespace std;

vector<string> kategoriPemasukan = {"Kiriman Ortu", "Gaji", "Investasi"};
vector<string> kategoriPengeluaran = {"Makan & Minum", "Kos & Listrik", "Transportasi", "Hiburan", "Belanja", "Kesehatan", "Kuliah"};

string formatRupiah(long long n) {
    bool negatif = n < 0;
    if (negatif) n = -n;
    string s = to_string(n);
    string hasil = "";
    int hitung = 0;
    for (int i = s.size() - 1; i >= 0; i--) {
        hasil = s[i] + hasil;
        hitung++;
        if (hitung % 3 == 0 && i != 0) {
            hasil = "." + hasil;
        }
    }
    if (negatif) return "Rp -" + hasil;
    return "Rp " + hasil;
}

string bersihkan(string s) {
    for (char &c : s) {
        if (c == '|') c = '-';
    }
    return s;
}

class Transaction {
protected:
    int id;
    string kategori;
    long long nominal;
    string tanggal;   //format YYYY-MM-DD
    string catatan;
public:
    Transaction(int i, string k, long long n, string t, string c) 
    : id(i), kategori(k), nominal(n), tanggal(t), catatan(c) {}
    virtual ~Transaction() {}
    virtual string getJenis() = 0;
    virtual long long pengaruhSaldo() = 0;

    int getId() {
        return id;
    }
    string getKategori() {
        return kategori;
    }
    long long getNominal() {
        return nominal;
    }
    string getCatatan() {
        return catatan;
    }
    string getTanggal() {
        return tanggal;
    }
    string getBulan() {
        return tanggal.substr(0, 7);
    }

    void tampilkan() {
        string tanda = (pengaruhSaldo() > 0) ? "+" : "-";
        cout << "[" << id << "] " << tanggal << " | " << left << setw(11) << getJenis() << " | " 
             << setw(16) << kategori << " | " << tanda << setw(14) << formatRupiah(nominal) 
             << " | " << catatan << endl;
    }
};

class Income : public Transaction {
public:
    Income(int i, string k, long long n, string t, string c) : Transaction(i, k, n, t, c) {}
    string getJenis() override {
        return "Pemasukan";
    }
    long long pengaruhSaldo() override {
        return nominal;
    }
};

class Expense : public Transaction {
public:
    Expense(int i, string k, long long n, string t, string c) : Transaction(i, k, n, t, c) {}
    string getJenis() override {
        return "Pengeluaran";
    }
    long long pengaruhSaldo() override {
        return -nominal;
    }
};

string bacaTeks(string pesan) {
    string s;
    cout << pesan;
    getline(cin, s);
    return s;
}

long long bacaAngka(string pesan) {
    while (true) {
        string s = bacaTeks(pesan);
        string bersih = "";
        for (char c : s) {
            if (c != '.' && c != ',') bersih += c;
        }
        if (!bersih.empty() && bersih.size() <= 12 && bersih.find_first_not_of("0123456789") == string::npos) {
            return stoll(bersih);
        }
        cout << "Masukkan angka saja, contoh 25000 atau 25.000" << endl;
    }
}

string pilihKategori(const vector<string>& daftar) {
    for (size_t i = 0; i < daftar.size(); i++) {
        cout << " " << i + 1 << ". " << daftar[i] << endl;
    }
    long long nomorLain = daftar.size() + 1;
    cout << " " << nomorLain << ". Lainnya" << endl;

    while (true) {
        long long pilihan = bacaAngka("Pilih kategori: ");
        if (pilihan >= 1 && pilihan < nomorLain) {
            return daftar[pilihan - 1];
        } 
        if (pilihan == nomorLain) {
            while (true) {
                string kategoriLain = bersihkan(bacaTeks("Ketik kategori: "));
                if (kategoriLain != "") 
                    return kategoriLain;
                cout << "Kategori tidak boleh kosong." << endl;
            }
        }
        cout << "Pilihan tidak valid." << endl;
    }
}

long long bacaNominal(string pesan) {
    while (true) {
        long long nominal = bacaAngka(pesan);
        if (nominal > 0) return nominal;
        cout << "Nominal harus lebih dari 0" << endl;
    }
}

bool semuaAngka(string s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

int jumlahHari(int tahun, int bulan) {
    if (bulan == 2) {
        if ((tahun % 4 == 0 && tahun % 100 != 0) || (tahun % 400 == 0)) {
            return 29; // Tahun kabisat
        } else {
            return 28;
        }
    } else if (bulan == 4 || bulan == 6 || bulan == 9 || bulan == 11) {
        return 30;
    } else {
        return 31;
    }
}

bool tanggalValid(string s) {   // Validasi "YYYY-MM-DD"
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return false;
    string tahunStr = s.substr(0, 4);
    string bulanStr = s.substr(5, 2);
    string hariStr = s.substr(8, 2);
    if (!semuaAngka(tahunStr) || !semuaAngka(bulanStr) || !semuaAngka(hariStr)) return false;
    int tahun = stoi(tahunStr);
    int bulan = stoi(bulanStr);
    int hari = stoi(hariStr);
    if (tahun < 2008 || bulan < 1 || bulan > 12 || hari < 1) return false;
    return hari <= jumlahHari(tahun, bulan);
}

string hariIni() {
    time_t t = time(nullptr);
    tm *waktu = localtime(&t);
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", waktu);
    return string(buffer);
}

string bacaTanggal() {
    while (true) {
        string s = bacaTeks("Tanggal (YYYY-MM-DD, kosong = hari ini): ");
        if (s.empty()) return hariIni();
        if (tanggalValid(s)) return s;
        cout << "Format salah, contoh: 2026-10-10" << endl;
    }
}

string bulanIni() {
    return hariIni().substr(0, 7);
}
long long sisaHariBulanIni() {
    time_t t = time(nullptr);
    tm *waktu = localtime(&t);
    return jumlahHari(waktu->tm_year + 1900, waktu->tm_mon + 1) - waktu->tm_mday + 1;
}

long long hitungSaldo(const vector<Transaction*>& daftar) {
    long long total = 0;
    for (Transaction* t : daftar) {
        total += t->pengaruhSaldo();
    }
    return total;
}

void peringatanMinus(const vector<Transaction*>& daftar) {
    long long saldo = hitungSaldo(daftar);
    if (saldo < 0) {
        cout << "PERINGATAN: Saldo kamu MINUS! " << formatRupiah(saldo) << endl;
    }
}

long long totalKeluarBulan(const vector<Transaction*>& daftar, string bulan) {
    long long total = 0;
    for (Transaction* t : daftar) {
        if (t->getJenis() == "Pengeluaran" && t->getBulan() == bulan) {
            total += t->getNominal();
        }
    }
    return total;
}

void Header(const vector<Transaction*>& daftar) {
    long long saldo = hitungSaldo(daftar);
    cout << "\n==============================\n"
         << " CATATAN KEUANGAN\n"
         << " Saldo              : " << formatRupiah(saldo) << "\n"
         << " Keluar bulan ini   : " << formatRupiah(totalKeluarBulan(daftar, bulanIni())) << "\n";
    if (saldo > 0)
        cout << " Jatah harian aman  : " << formatRupiah(saldo / sisaHariBulanIni()) << " ("
             << sisaHariBulanIni() << " hari lagi)\n";
    cout << "==============================\n";
}

bool bulanValid(string s) {
    return s.size() == 7 && s[4] == '-' && tanggalValid(s + "-01");
}

string bacaBulan() {
    while (true) {
        string s = bacaTeks("Bulan (YYYY-MM, kosong = bulan ini): ");
        if (s.empty()) return bulanIni();
        if (bulanValid(s)) return s;
        cout << "Format salah, contoh: 2026-10" << endl;
    }
}

bool lebihBesar(const pair<string, long long>& a, const pair<string, long long>& b) {
    return a.second > b.second;
}

void cetakRincian(const map<string, long long>& data) {
    vector<pair<string, long long> > urut(data.begin(), data.end());
    sort(urut.begin(), urut.end(), lebihBesar);
    for (size_t i = 0; i < urut.size(); i++) {
        cout << " " << left << setw(18) << urut[i].first << right << setw(15)
             << formatRupiah(urut[i].second) << endl;
    }
    if (urut.empty()) cout << " Belum ada data." << endl;
}

void laporanBulanan(const vector<Transaction*>& daftar, string bulan) {
    long long totalPemasukan = 0, totalPengeluaran = 0;
    map<string, long long> rincianPemasukan, rincianPengeluaran;

    for (Transaction* t : daftar) {
        if (t->getBulan() != bulan) continue;
        if (t->getJenis() == "Pemasukan") {
            totalPemasukan += t->getNominal();
            rincianPemasukan[t->getKategori()] += t->getNominal();
        } else {
            totalPengeluaran += t->getNominal();
            rincianPengeluaran[t->getKategori()] += t->getNominal();
        }
    }
        
    long long selisih = totalPemasukan - totalPengeluaran;
    cout << "\n--- Laporan " << bulan << " ---\n";
    cout << "Total Pemasukan   : " << formatRupiah(totalPemasukan) << endl;
    cout << "Total Pengeluaran : " << formatRupiah(totalPengeluaran) << endl;
    cout << "Selisih           : " << formatRupiah(selisih) << (selisih >= 0 ? " (surplus)" : " (defisit)") << endl;
    cout << "\nRincian Pemasukan:\n";
    cetakRincian(rincianPemasukan);
    cout << "\nRincian Pengeluaran:\n";
    cetakRincian(rincianPengeluaran);
}

bool lebihAwal(Transaction* a, Transaction* b) {
    if (a->getTanggal() != b->getTanggal()) return a->getTanggal() < b->getTanggal();
    return a->getId() < b->getId();
}

void tampilRiwayat(const vector<Transaction*>& daftar, string bulan) {
    vector<Transaction*> hasil;
    for (Transaction* t : daftar) {
        if (bulan == "" || t->getBulan() == bulan) {
            hasil.push_back(t);
        }
    }
    sort(hasil.begin(), hasil.end(), lebihAwal);

    cout << "\n=== Riwayat " << (bulan == "" ? "semua" : bulan) << " ===\n";
    if (hasil.empty()) cout << "Belum ada transaksi." << endl;
    for (Transaction* t : hasil) {
        t->tampilkan();
    }
}

bool hapusTransaksi(vector<Transaction*>& daftar, int id) {
    for (size_t i = 0; i < daftar.size(); ++i) {
        if (daftar[i]->getId() == id) {
            cout << "Dihapus: ";
            daftar[i]->tampilkan();
            delete daftar[i];
            daftar.erase(daftar.begin() + i);
            return true;
        }
    }
    return false;
}

int idBaru(const vector<Transaction*>& daftar) {
    int terbesar = 0;
    for (Transaction* t : daftar) {
        if (t->getId() > terbesar) terbesar = t->getId();
    }
    return terbesar + 1;
}

vector<string> pecah(string baris) {
    vector<string> bagian;
    stringstream ss(baris);
    string potong;
    while (getline(ss, potong, '|')) {
        bagian.push_back(potong);
    }
    return bagian;
}

void simpanTransaksi(const vector<Transaction*>& daftar) {
    ofstream file("transaksi.txt");
    for (Transaction* t : daftar) {
        file << t->getId() << "|" << t->getJenis() << "|" << t->getKategori() << "|" 
        << t->getNominal() << "|" << t->getTanggal() << "|" << t->getCatatan() << endl;
    }
}

void muatTransaksi(vector<Transaction*>& daftar) {
    ifstream file("transaksi.txt");
    string baris;
    while (getline(file, baris)) {
        vector<string> bagian = pecah(baris);
        if (bagian.size() < 5) continue;
        try {
            int id = stoi(bagian[0]);
            string jenis = bagian[1];
            string kategori = bagian[2];
            long long nominal = stoll(bagian[3]);
            string tanggal = bagian[4];
            string catatan = "";
            if (bagian.size() > 5) catatan = bagian[5];
            if (!tanggalValid(tanggal) || nominal <= 0) continue;

            if (jenis == "Pemasukan") daftar.push_back(new Income(id, kategori, nominal, tanggal, catatan));
            else daftar.push_back(new Expense(id, kategori, nominal, tanggal, catatan));
        } catch (...) {
            continue;  // Lewati baris yang tidak valid
        } 
    }
}

int main() {
    vector<Transaction*> daftar;
    muatTransaksi(daftar);
    int pilihan;
    do {
        Header(daftar);
        cout << "1. Tambah Pemasukan" << endl;
        cout << "2. Tambah Pengeluaran" << endl;
        cout << "3. Lihat Transaksi" << endl;
        cout << "4. Laporan Bulanan" << endl;
        cout << "5. Hapus Transaksi" << endl;
        cout << "0. Keluar" << endl;
        pilihan = bacaAngka("Pilih menu: ");
        if (pilihan == 1 || pilihan == 2) {
            long long nominal = bacaNominal("Nominal: ");
            string kategori;
            if (pilihan == 1) kategori = pilihKategori(kategoriPemasukan);
            else kategori = pilihKategori(kategoriPengeluaran);
            string catatan = bersihkan(bacaTeks("Catatan (opsional): "));
            string tanggal = bacaTanggal();

            if (pilihan == 1) {
                daftar.push_back(new Income(idBaru(daftar), kategori, nominal, tanggal, catatan));
            } else {
                daftar.push_back(new Expense(idBaru(daftar), kategori, nominal, tanggal, catatan));
            }
            simpanTransaksi(daftar);
            cout << "Tersimpan." << endl;
            if (pilihan == 2) peringatanMinus(daftar);
        }
            
        else if (pilihan == 3) {
            string filter = bacaTeks("Filter per bulan? (y/n): ");
            if (filter == "y" || filter == "Y") {
                tampilRiwayat(daftar, bacaBulan());
            } else {
                tampilRiwayat(daftar, "");
            }
            cout << "\nSaldo: " << formatRupiah(hitungSaldo(daftar)) << endl;
            peringatanMinus(daftar);
        }

        else if (pilihan == 4) {
            laporanBulanan(daftar, bacaBulan());
        }
        
        else if (pilihan == 5) {
            long long id = bacaAngka("Masukkan ID transaksi yang ingin dihapus (lihat di riwayat): ");
            if (hapusTransaksi(daftar, (int)id)) {
                simpanTransaksi(daftar);
                peringatanMinus(daftar);
            } else {
                cout << "ID transaksi tidak ditemukan." << endl;
            }
        }
    } while (pilihan != 0);

    for (Transaction* t : daftar) {
        delete t;
    }
    return 0;
}