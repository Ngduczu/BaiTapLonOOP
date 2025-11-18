#pragma once
#include "Nguoi.cpp"
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class GiaoVienChuNhiem : public Nguoi
{
private:
    string maGiaoVien;
    string lopChuNhiem;
    string boMonGiangDay;
    string hocVi;
    int soNamKinhNghiem;
    double luong;

public:
    // Constructor
    GiaoVienChuNhiem(string ten, string ns, string dc, string maDD, string email, string gt, string mk,
    string maGV, string lop, string boMon, string hv, int soNam, double l)
    : Nguoi(ten, ns, dc, maDD, email, gt, mk),
    maGiaoVien(maGV), lopChuNhiem(lop), boMonGiangDay(boMon),
    hocVi(hv), soNamKinhNghiem(soNam), luong(l) {}

    // Getters
    string getMaGiaoVien() const { return maGiaoVien; }
    string getLopChuNhiem() const { return lopChuNhiem; }
    string getBoMonGiangDay() const { return boMonGiangDay; }
    string getHocVi() const { return hocVi; }

    // setters
    void setLopChuNhiem(string lop) { this->lopChuNhiem = lop; }
    void setBoMonGiangDay(string boMon) { this->boMonGiangDay = boMon; }
    void setHocVi(string hv) { this->hocVi = hv; }
    void setSoNamKinhNghiem(int soNam) { this->soNamKinhNghiem = soNam; }
    void setLuong(double l) { this->luong = l; }
    int getSoNamKinhNghiem() const { return soNamKinhNghiem; }
    double getLuong() const { return luong; }

    // Override các phương thức ảo từ class Nguoi
    string getRoleType() const override
    {
        return "GVCN";
    }

    void displayInfo() const override
    {
        cout << "------------------------------------------------------------\n";
        cout << "| THONG TIN GIAO VIEN CHU NHIEM (MA GV: " << maGiaoVien << ")\n";
        cout << "------------------------------------------------------------\n";
        cout << "| " << left << setw(20) << "Ho va Ten:" << "| " << setw(35) << HoTen << " |\n";
        cout << "| " << left << setw(20) << "Ma CCCD (User):" << "| " << setw(35) << MaCCCD << " |\n";
        cout << "| " << left << setw(20) << "Lop Chu Nhiem:" << "| " << setw(35) << lopChuNhiem << " |\n";
        cout << "| " << left << setw(20) << "Bo Mon Giang Day:" << "| " << setw(35) << boMonGiangDay << " |\n";
        cout << "| " << left << setw(20) << "Hoc Vi:" << "| " << setw(35) << hocVi << " |\n";
        cout << "| " << left << setw(20) << "So Nam Kinh Nghiem:" << "| " << setw(35) << soNamKinhNghiem << " |\n";
        cout << "| " << left << setw(20) << "Luong:" << "| " << setw(35) << fixed << setprecision(0) << luong << " |\n";
        cout << "| " << left << setw(20) << "Ngay Sinh:" << "| " << setw(35) << NgayThangNamSinh << " |\n";
        cout << "| " << left << setw(20) << "Gioi Tinh:" << "| " << setw(35) << GioiTinh << " |\n";
        cout << "| " << left << setw(20) << "Dia Chi:" << "| " << setw(35) << DiaChi << " |\n";
        cout << "| " << left << setw(20) << "Email:" << "| " << setw(35) << Email << " |\n";
        cout << "------------------------------------------------------------\n";
    }

    // Override hàm lưu dữ liệu
    void saveData(ofstream &file) const override
    {
        Nguoi::saveData(file); // Gọi hàm của lớp cha
        file << maGiaoVien << endl;
        file << lopChuNhiem << endl;
        file << boMonGiangDay << endl;
        file << hocVi << endl;
        file << soNamKinhNghiem << endl;
        file << luong << endl;
    }

    // Override hàm tải dữ liệu
    void loadData(ifstream &file) override
    {
        Nguoi::loadData(file); // Gọi hàm của lớp cha
        getline(file >> ws, maGiaoVien);
        getline(file >> ws, lopChuNhiem);
        getline(file >> ws, boMonGiangDay);
        getline(file >> ws, hocVi);
        file >> soNamKinhNghiem;
        file >> luong;
    }
};