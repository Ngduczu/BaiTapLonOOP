#pragma once // Ngăn chặn việc include file này nhiều lần
#include <bits/stdc++.h>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class Nguoi{
    protected:
        string HoTen;
        string NgayThangNamSinh;
        string DiaChi;
        string MaCCCD; // Sẽ được dùng làm username đăng nhập
        string Email;
        string GioiTinh;
        string MatKhau;
    public:
        // Sửa lỗi: Bỏ 'Nguoi::' khi định nghĩa constructor bên trong class
        Nguoi(string ten, string ns, string dc, string maDD, string email, string gt, string mk)
        : HoTen(ten), NgayThangNamSinh(ns), DiaChi(dc), MaCCCD(maDD), Email(email), GioiTinh(gt), MatKhau(mk) {}

        virtual ~Nguoi(){} // Destructor ảo

        // getter
        string getHoTen() const {return HoTen;}
        string getDiaChi() const {return DiaChi;}
        string getEmail() const {return Email;}
        string getMaCCCD() const {return MaCCCD;} // Thêm getter này
        string getMatKhau() const {return MatKhau;}
        string getNgayThangNamSinh() const {return NgayThangNamSinh;}
        string getGioiTinh() const {return GioiTinh;}
        
        // setter
        void setThongTinCoBan(string ten, string ns,string dc,string email,string gt){
            this->HoTen = ten;
            this->NgayThangNamSinh = ns;
            this->DiaChi = dc;
            this->Email = email;
            this->GioiTinh = gt;
        }

        void setMatKhau(string newmk){
            this->MatKhau = newmk;
        }
        // Các hàm ảo thuần tuý (pure virtual)
        virtual string getRoleType() const = 0;
        virtual void displayInfo() const = 0;

        // Các hàm ảo (virtual)
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