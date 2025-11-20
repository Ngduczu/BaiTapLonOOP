#pragma once
#include "Nguoi.cpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class Admin : public Nguoi {
private:
    string maAdmin; // Mã admin (mã nhân viên)

public:
    // Constructor
    Admin(string ten, string ns, string dc, string maDD, string email, string gt, string mk,string maAD)
    : Nguoi(ten, ns, dc, maDD, email, gt, mk), maAdmin(maAD) {}

    // Getter
    string getMaAdmin() const { return maAdmin; }
    string getRoleType() const override {
        return "ADMIN";
    }

    void displayInfo() const override {
        cout << "------------------------------------------------------------\n";
        cout << "| THONG TIN ADMIN (MA ADMIN: " << maAdmin << ")\n";
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
        file << maAdmin << endl;
    }
    void loadData(ifstream& file) override {
        Nguoi::loadData(file);
        getline(file >> ws, maAdmin);
    }
};