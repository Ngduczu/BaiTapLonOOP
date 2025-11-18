#pragma once
#include "Nguoi.cpp"
#include "Sach.cpp"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

class HocSinh : public Nguoi {
private:
    string maHocSinh;
    string lop;
    double diemTrungBinh;
    double diemRenLuyen;
    vector<Sach*> danhSachMuon; // Chỉ lưu trữ con trỏ
    
    // [UPDATE] Biến tạm để lưu ID sách khi load file, tránh xung đột I/O
    vector<string> tempIdSachMuon; 

public:
    HocSinh(string ten, string ns, string dc, string maDD, string email, string gt, string mk,
            string maHS, string lop, double dtb, double drl)
            :Nguoi(ten, ns, dc, maDD, email, gt, mk), maHocSinh(maHS), lop(lop), diemTrungBinh(dtb), diemRenLuyen(drl) {}
    
    ~HocSinh();
    
    void displayInfo() const override {
        cout << "------------------------------------------------------------\n";
        cout << "| THONG TIN HOC SINH (MA HS: " << maHocSinh << ")\n";
        cout << "------------------------------------------------------------\n";
        cout << "| " << left << setw(20) << "Ho va Ten:" << "| " << setw(35) << HoTen << " |\n";
        cout << "| " << left << setw(20) << "Ma CCCD (User):" << "| " << setw(35) << MaCCCD << " |\n";
        cout << "| " << left << setw(20) << "Lop:" << "| " << setw(35) << lop << " |\n";
        cout << "| " << left << setw(20) << "Ngay Sinh:" << "| " << setw(35) << NgayThangNamSinh << " |\n";
        cout << "| " << left << setw(20) << "Gioi Tinh:" << "| " << setw(35) << GioiTinh << " |\n";
        cout << "| " << left << setw(20) << "Dia Chi:" << "| " << setw(35) << DiaChi << " |\n";
        cout << "| " << left << setw(20) << "Email:" << "| " << setw(35) << Email << " |\n";
        cout << "| " << left << setw(20) << "Diem Trung Binh:" << "| " << setw(35) << fixed << setprecision(2) << diemTrungBinh << " |\n";
        cout << "| " << left << setw(20) << "Diem Ren Luyen:" << "| " << setw(35) << fixed << setprecision(2) << diemRenLuyen << " |\n";
        cout << "------------------------------------------------------------\n";
    }
    string getRoleType() const override{ return "HOCSINH";}

    // Định nghĩa (override) các hàm I/O
    void saveData(ofstream& file) const override;
    void loadData(ifstream& file) override;

    // Getters và Setters riêng của HocSinh
    string getMaHocSinh() const { return maHocSinh;}
    string getLop() const { return lop;}
    double getDiemTrungBinh() const;
    double getDiemRenLuyen() const { return diemRenLuyen; }
    
    // [UPDATE] Getter cho biến tạm
    vector<string> getTempIdSachMuon() const { return tempIdSachMuon; }

    void setDiemTrungBinh(double diem) { this->diemTrungBinh = diem;}
    void setLop(string newLop) { this->lop = newLop;}
    void setDiemRenLuyen(double diem) { this->diemRenLuyen = diem;}
    void setThongTinCaNhan(string newTen, string newNgaySinh, string newDiaChi, string newEmail, string newGioiTinh){
        if (!newTen.empty()) this->HoTen = newTen;
        if (!newNgaySinh.empty()) this->NgayThangNamSinh = newNgaySinh;
        if (!newDiaChi.empty()) this->DiaChi = newDiaChi;
        if (!newEmail.empty()) this->Email = newEmail;
        if (!newGioiTinh.empty()) this->GioiTinh = newGioiTinh;
    }
    // Chức năng thư viện
    void muonSach(Sach* sach);
    bool traSach(string idSach); // [UPDATE] Thêm hàm trả sách
    void xemDanhSachMuon() const;
    vector<Sach*> getDanhSachMuon() const { return danhSachMuon; }
};

// --- Triển khai các hàm của HocSinh ---

HocSinh::~HocSinh() {
    danhSachMuon.clear();
}

double HocSinh::getDiemTrungBinh() const {
    return diemTrungBinh;
}

void HocSinh::muonSach(Sach* sach) {
    if(sach == nullptr) return;
    
    for (Sach* s : danhSachMuon) {
        if (s->getIDSach() == sach->getIDSach()) {
            cout << "Ban da muon cuon sach nay roi!" << endl;
            return;
        }
    }
    danhSachMuon.push_back(sach);
    // cout << "Muon sach '" << sach->getTenTacPham() << "' thanh cong!" << endl; // Comment bớt cho đỡ rối khi load
}

// [UPDATE] Triển khai hàm trả sách
bool HocSinh::traSach(string idSach) {
    for (size_t i = 0; i < danhSachMuon.size(); ++i) {
        if (danhSachMuon[i]->getIDSach() == idSach) {
            danhSachMuon.erase(danhSachMuon.begin() + i);
            return true;
        }
    }
    return false;
}

void HocSinh::xemDanhSachMuon() const {
    cout << "\n--- DANH SACH SACH DANG MUON ---" << endl;
    if (danhSachMuon.empty()) {
        cout << "Ban chua muon cuon sach nao." << endl;
        cout << "---------------------------------" << endl;
        return;
    }
    
    cout << "----------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(10) << "ID Sach"
         << "| " << left << setw(30) << "Ten Sach"
         << "| " << left << setw(25) << "Ten Tac Gia"
         << "| " << left << setw(15) << "The Loai" << " |" << endl;
    cout << "----------------------------------------------------------------------------------\n";
    for (Sach* sach : danhSachMuon) {
        if(sach) { // Kiểm tra con trỏ
            cout << "| " << left << setw(10) << sach->getIDSach()
                 << "| " << left << setw(30) << sach->getTenTacPham()
                 << "| " << left << setw(25) << sach->getTenTacGia()
                 << "| " << left << setw(15) << sach->getTheLoai() << " |" << endl;
        }
    }
    cout << "----------------------------------------------------------------------------------\n";
}

void HocSinh::saveData(ofstream& file) const {
    Nguoi::saveData(file); 
    file << maHocSinh << endl;
    file << lop << endl;
    file << diemTrungBinh << endl;
    file << diemRenLuyen << endl;
    file << danhSachMuon.size() << endl;
    for (Sach* sach : danhSachMuon) {
        if(sach) file << sach->getIDSach() << endl;
    }
}

void HocSinh::loadData(ifstream& file) {
    Nguoi::loadData(file);
    getline(file >> ws, maHocSinh);
    getline(file, lop);
    file >> diemTrungBinh;
    file >> diemRenLuyen;
    int soSachMuon;
    file >> soSachMuon;
    file.ignore(numeric_limits<streamsize>::max(), '\n'); 
    
    // [UPDATE] Sửa lỗi load dữ liệu: Đọc ID vào biến tạm
    tempIdSachMuon.clear();
    for (int i = 0; i < soSachMuon; ++i) {
        string idSachMuon;
        getline(file, idSachMuon);
        if(!idSachMuon.empty()) {
            tempIdSachMuon.push_back(idSachMuon);
        }
    }
}