#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class Nguoi{
    protected:
        string HoTen;
        string NgayThangNamSinh;
        string DiaChi;
        string MaCCCD; 
        string Email;
        string GioiTinh;
        string MatKhau;
    public:
        Nguoi(string ten, string ns, string dc, string maDD, string email, string gt, string mk)
        : HoTen(ten), NgayThangNamSinh(ns), DiaChi(dc), MaCCCD(maDD), Email(email), GioiTinh(gt), MatKhau(mk) {}

        virtual ~Nguoi(){}

        // getter
        string getHoTen() const {return HoTen;}
        string getDiaChi() const {return DiaChi;}
        string getEmail() const {return Email;}
        string getMaCCCD() const {return MaCCCD;}
        string getMatKhau() const {return MatKhau;}
        string getNgayThangNamSinh() const {return NgayThangNamSinh;}
        string getGioiTinh() const {return GioiTinh;}
        
        // --- [ĐÃ SỬA] Setter có kiểm tra rỗng ---
        void setThongTinCoBan(string ten, string ns, string dc, string email, string gt){
            if (!ten.empty()) this->HoTen = ten;
            if (!ns.empty()) this->NgayThangNamSinh = ns;
            if (!dc.empty()) this->DiaChi = dc;
            if (!email.empty()) this->Email = email;
            if (!gt.empty()) this->GioiTinh = gt;
        }

        void setMatKhau(string newmk){
            if (!newmk.empty()) this->MatKhau = newmk;
        }
        
        virtual string getRoleType() const = 0;
        virtual void displayInfo() const = 0;

        virtual void saveData(ofstream& file) const {
            file << HoTen << endl;
            file << NgayThangNamSinh << endl;
            file << DiaChi << endl;
            file << MaCCCD << endl;
            file << Email << endl;
            file << GioiTinh << endl;
            file << MatKhau << endl;
        }

        virtual void loadData(ifstream& file) {
            getline(file >> ws, HoTen);
            getline(file,NgayThangNamSinh);
            getline(file, DiaChi);
            getline(file, MaCCCD);
            getline(file, Email);
            getline(file, GioiTinh);
            getline(file, MatKhau);
        }
};