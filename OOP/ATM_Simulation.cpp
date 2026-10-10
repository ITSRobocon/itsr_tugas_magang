#include<iostream>
#include<string>
#include<iomanip>

using namespace std;

class Rekening{
private:
    string noRek;
    string nama;
    string pin;
    double saldo;

public:
    void setNoRek(string nomor){
        if(nomor.size() == 10){
            noRek = nomor;
        } else {
            cout << "[ERROR] Nomor rekening tidak valid!" << endl;
        }
    }

    void setNama(string n){
        nama = n;
    }

    void setPin(string p){
        pin = p;
    }

    void setSaldo(double saldo_awal){
        if (saldo_awal >= 0){
            saldo = saldo_awal;
        } else {
            cout << "[ERROR] Saldo tidak boleh negatif!" << endl;
        }
    }

    Rekening(string no, string n, string p, double s){
        setNoRek(no);
        setNama(n);
        setPin(p);
        setSaldo(s);
    }

    string getNoRek(){
        return noRek;
    }

    string getNama(){
        return nama;
    }

    double getSaldo(){
        return saldo;
    }

    void tfsaldoTujuan(double jumlah){
        saldo += jumlah;
    }

    bool verifPin(){
        string pin_bank;
        for (int i = 0; i < 3; i++){
            cout << "Silakan Masukkan PIN Anda: " << endl;
            cin >> pin_bank;
            if (pin_bank == pin){
                return true;
            } else {
                if (i == 2){
                    cout << "PIN yang Anda masukkan salah." << endl;
                } else {
                    cout << "PIN yang Anda masukkan salah. Silakan coba lagi." << endl;
                    cout << "Sisa percobaan: " << 2 - i << endl;
                }
            }
        }
        return false;
    }

    void setorTunai (double setoran){
        if (setoran <= 0){
            cout << "[GAGAL] Jumlah setoran harus lebih dari 0." << endl;
            return;
        }
        saldo += setoran;
        cout << "[BERHASIL] Rp " << fixed << setprecision(2) << setoran << " berhasil masuk." << endl;
        cout << "Saldo: Rp " << fixed << setprecision(2) << saldo << endl;
               
    }

    void tarikTunai (double tarik){
        if (tarik <= 0){
            cout << "[GAGAL] Jumlah tarik tunai harus lebih dari 0." << endl;
            return;
        }
        if (tarik > saldo){
            cout << "[GAGAL] Saldo tidak mencukupi." << endl;
            cout << "Saldo: Rp " << fixed << setprecision(2) << saldo << endl;
            return;
        }
        saldo -= tarik;
        cout << "[BERHASIL] Rp " << fixed << setprecision(2) << tarik << " berhasil ditarik." << endl;
        cout << "Saldo: Rp " << fixed << setprecision(2) << saldo << endl;
    }

    void transfer (Rekening& tujuan, double jumlah){
        cout << "Transfer Rp " << fixed << setprecision(2) << jumlah << " dari " << nama << " ke " << tujuan.getNama() << "..." << endl;
        if (jumlah <= 0){
            cout << "[GAGAL] Jumlah transfer harus lebih dari 0." << endl;
            return;
        }
        if (saldo >= jumlah){
            saldo -= jumlah;
            tujuan.tfsaldoTujuan(jumlah);
            cout << "[BERHASIL] Transaksi Berhasil" << endl;
            cout << "Saldo: Rp " << fixed << setprecision(2) << saldo << endl;
        } else {
            cout << "[GAGAL] Saldo Anda tidak mencukupi." << endl;
        }
    }

    void informasi (){
        cout << "=== INFORMASI REKENING ===" << endl;
        cout << "Nomor Rekening : " << noRek << endl;
        cout << "Nama Pemilik   : " << nama << endl;
        cout << "Saldo          : Rp " << fixed << setprecision(2) << saldo << endl;
    }

    void menu(){
        cout << "====== MENU UTAMA ======" << endl;
        cout << "1. Setor Tunai" << endl;
        cout << "2. Tarik Tunai" << endl;
        cout << "3. Transfer" << endl;
        cout << "4. Informasi Rekening" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilihan Anda: ";
    }
};

int main (){
    Rekening rek1("1234567890", "Rafa", "9923", 1000000);
    Rekening rek2("5163629119", "Naura", "2712", 500000);

    int pilihan;
    double nominal;

    cout << "Silakan Masukkan Kartu Anda" << endl;

    if (!rek1.verifPin()){
        cout << "Kartu Anda ditahan oleh mesin demi keamanan. Silakan hubungi kantor cabang terdekat." << endl;
        return 0;
    }
    do {
        rek1.menu();
        cin >> pilihan;

        switch (pilihan){
            case 1:
                cout << "Masukkan nominal setor tunai: Rp ";
                cin >> nominal;
                rek1.setorTunai(nominal);
                break;
            case 2:
                cout << "Masukkan nominal tarik tunai: Rp ";
                cin >> nominal;
                rek1.tarikTunai(nominal);
                break;
            case 3:
                cout << "Masukkan nominal transfer ke " << rek2.getNama() << ": Rp ";
                cin >> nominal;
                rek1.transfer(rek2, nominal);
                break;
            case 4:
                rek1.informasi();
                break;
            case 5:
                cout << "TERIMA KASIH" << endl << "Silakan ambil kartu ATM Anda." << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
        cout << endl;
    } while (pilihan != 5);

    return 0;
}