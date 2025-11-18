#pragma once
#include "Nguoi.cpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class ThuThu : public Nguoi {
private:
    string maThuThu; // Mã thủ thư (mã nhân viên)

public:
    ThuThu(string ten, string ns, string dc, string maDD, string email, string gt, string mk,string maTT)
    : Nguoi(ten, ns, dc, maDD, email, gt, mk), maThuThu(maTT) {}
    string getMaThuThu() const { return maThuThu; }
    string getRoleType() const override {return "THUTHU";}

    void displayInfo() const override {
        cout << "------------------------------------------------------------\n";
        cout << "| THONG TIN THU THU (MA NV: " << maThuThu << ")\n";
        cout << "------------------------------------------------------------\n";
        cout << "| " << left << setw(20) << "Ho va Ten:" << "| " << setw(35) << HoTen << " |\n";
        cout << "| " << left << setw(20) << "Ma CCCD (User):" << "| " << setw(35) << MaCCCD << " |\n";
        cout << "| " << left << setw(20) << "Ngay Sinh:" << "| " << setw(35) << NgayThangNamSinh << " |\n";
        cout << "| " << left << setw(20) << "Gioi Tinh:" << "| " << setw(35) << GioiTinh << " |\n";
        cout << "| " << left << setw(20) << "Dia Chi:" << "| " << setw(35) << DiaChi << " |\n";
        cout << "| " << left << setw(20) << "Email:" << "| " << setw(35) << Email << " |\n";
        cout << "------------------------------------------------------------\n";
    }

    void saveData(ofstream& file) const override {
        Nguoi::saveData(file);
        file << maThuThu << endl;
    }

    void loadData(ifstream& file) override {
        Nguoi::loadData(file);
        getline(file >> ws, maThuThu);
    }
};