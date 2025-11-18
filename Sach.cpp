#pragma once
#include<bits/stdc++.h>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Sach{
    private:
        string IDsach;
        string TenSach;
        string TenTacGia;
        string TheLoai;
        double GiaBan;
        int SoLuong;
    public:
        Sach(string id, string tacGia, string ten, string loai, double gia, int sl)
        : IDsach(id), TenTacGia(tacGia), TenSach(ten), TheLoai(loai), GiaBan(gia), SoLuong(sl) {}

        string getIDSach() const { return IDsach; }
        string getTenTacGia() const { return TenTacGia; }
        string getTenTacPham() const { return TenSach; }
        string getTheLoai() const {return TheLoai;}
        int getSoLuong() const {return SoLuong;}

        void setSoLuong(int newSoLuong){ SoLuong = ((newSoLuong >= 0) ? newSoLuong : SoLuong);}

        void displayInfo(){
            cout << "| " << left << setw(10) << IDsach
            << "| " << left << setw(30) << TenSach
            << "| " << left << setw(25) << TenTacGia
            << "| " << left << setw(15) << TheLoai
            << "| " << right << setw(8) << SoLuong << " |" << endl;
        }
        void saveData(ofstream& file) const {
            file << IDsach << endl;
            file << TenTacGia << endl;
            file << TenSach << endl;
            file << TheLoai << endl;
            file << GiaBan << endl;
            file << SoLuong << endl;
        }

        void loadData(ifstream& file) {
            getline(file >> ws, IDsach); // >> ws để tiêu thụ ký tự dòng mới
            getline(file, TenTacGia);
            getline(file, TenSach);
            getline(file, TheLoai);
            file >> GiaBan >> SoLuong;
        }
};