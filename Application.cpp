#include "HocSinh.cpp"
#include "Sach.cpp"
#include "GiaoVienChuNhiem.cpp"
#include "Admin.cpp"
#include "ThuThu.cpp"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <sstream> 
#include <conio.h>
#include <fstream>
#include <limits>

using namespace std;

typedef pair<string, string> LoginToken;

class Application
{
private:
    Nguoi *currentUser;
    map<string, Sach *> SachMap;  // Key: IDsach
    map<string, Nguoi *> UserMap; // Key: MaCCCD (Username)
    map<LoginToken, Nguoi *> LoginMap;

    const string USER_FILE_IN = "Data/Input/users.txt";
    const string BOOK_FILE_IN = "Data/Input/books.txt";
    const string USER_FILE_OUT = "Data/Input/users.txt";
    const string BOOK_FILE_OUT = "Data/Input/books.txt";

    // Admin chức năng
    void Admin_TaoTaiKhoanMoi();
    void Admin_XemTatCaTaiKhoan() const;
    void Admin_XoaNguoiDung();
    void Admin_SuaThongTinTaiKhoan();
    void Admin_DoiMatKhau(); 
    void Admin_XuatDanhSachTaiKhoanRaFile();
    void Admin_NhapSachTuFile();
    void Admin_NhapNguoiDungTuFile();
    
    // Giao Vien chu nhiem chuc năng
    void GVCN_XemDanhSachThongTinLop() const;
    void GVCN_NhapDiemChoLopChuNhiem();
    void GVCN_SuaThongTinHocSinh();
    void GVCN_SuaThongTinBanThan();
    void GVCN_XuatDanhSachLopRaFile();

    // Chuc Nang Hoc Sinh
    void HS_XemThongTinBanThan();
    void HS_SuaThongTinBanThan();
    void HS_XemDanhSachSachTrongThuVien();
    void HS_DoiMatKhau();
    void HS_MuonSach();
    void HS_TraSach(); // [UPDATE] Thêm hàm này

    // ThuThu chức năng
    void ThuThu_ThemSach();
    void ThuThu_XoaSach();
    void ThuThu_XemTatCaSach() const;
    void ThuThu_XemHocSinhMuonSach() const;
    void ThuThu_XuatTatCaSachRaFile();
    void ThuThu_XuatDanhSachHocSinhMuonSachRaFile();
    void ThuThu_NhapSachTuFile();

    // Hàm hỗ trợ
    void clearConsole() const
    {
#ifdef _WIN32
        system("cls");
#else
        system("clear"); 
#endif
    }
    void printHeader(const string &title) const;
    void pauseExecution() const;

    // Hàm menu
    void showAdminMenu();
    void showGVCNMenu();
    void showHocSinhMenu();
    void showThuThuMenu();

    // Hàm đăng nhập
    void login();
    void logout();

public:
    Application() : currentUser(nullptr) {}

    ~Application()
    {
        for (auto const &[key, val] : SachMap)
        {
            delete val; 
        }
        SachMap.clear();

        for (auto const &[key, val] : UserMap)
        {
            delete val; 
        }
        UserMap.clear();
        LoginMap.clear(); 
    }

    void run();
    void loadData();
    void saveData() const;
};

// --- Triển khai các hàm của Application ---

void Application::printHeader(const string &title) const
{
    clearConsole();
    int width = 53;
    if (title.size() > (size_t)(width))
    {
        width = title.size();
    }
    int len = title.size();
    int space = width - len;

    int leftSpace = space / 2;
    int rightSpace = space - leftSpace;
    
    string border = "============================================================\n";
    if (width > 53) {
        border = "==" + string(width + 2, '=') + "==\n";
    }

    cout << border;
    cout << "==         CHUONG TRINH QUAN LY TRUONG HOC                 ==\n";
    cout << border;
    cout << "== " << string(leftSpace, ' ') << title << string(rightSpace, ' ') << " ==\n";
    cout << border << "\n";
}

void Application::pauseExecution() const
{
    cout << "\nNhan phim bat ky de tiep tuc...";
    _getch();
}

// --- I/O Functions ---

void Application::loadData()
{
    ifstream bookFile(BOOK_FILE_IN);
    if (!bookFile.is_open())
    {
        cout << "Loi: Khong tim thay file " << BOOK_FILE_IN << endl;
    }
    else
    {
        int count;
        bookFile >> count;
        bookFile.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int i = 0; i < count; ++i)
        {
            Sach *newSach = new Sach("", "", "", "", 0.0, 0);
            newSach->loadData(bookFile);
            if (!newSach->getIDSach().empty())
            {
                SachMap[newSach->getIDSach()] = newSach;
            }
            else
            {
                delete newSach; 
            }
        }
        bookFile.close();
    }

    ifstream userFile(USER_FILE_IN);
    if (!userFile.is_open())
    {
        cout << "Loi: Khong tim thay file " << USER_FILE_IN << endl;
        pauseExecution();
        return;
    }

    int count;
    userFile >> count;
    userFile.ignore(numeric_limits<streamsize>::max(), '\n');

    map<HocSinh *, vector<string>> hsMuonSachLinks;

    for (int i = 0; i < count; ++i)
    {
        string role;
        getline(userFile, role);
        Nguoi *newUser = nullptr;

        if (role == "HOCSINH")
        {
            newUser = new HocSinh("", "", "", "", "", "", "", "", "", 0.0, 0.0);
            newUser->loadData(userFile); 

            // [UPDATE] Sửa lỗi đọc file: Lấy list ID từ biến tạm của HocSinh
            HocSinh *hs = dynamic_cast<HocSinh *>(newUser);
            if (hs)
                hsMuonSachLinks[hs] = hs->getTempIdSachMuon();
        }
        else if (role == "GVCN")
        {
            newUser = new GiaoVienChuNhiem("", "", "", "", "", "", "", "", "", "", "", 0, 0.0);
            newUser->loadData(userFile);
        }
        else if (role == "ADMIN")
        {
            newUser = new Admin("", "", "", "", "", "", "", "");
            newUser->loadData(userFile);
        }
        else if (role == "THUTHU")
        {
            newUser = new ThuThu("", "", "", "", "", "", "", "");
            newUser->loadData(userFile);
        }

        if (newUser != nullptr && !newUser->getMaCCCD().empty())
        {
            if (UserMap.count(newUser->getMaCCCD()))
            {
                cout << "Loi: Trung lap tai khoan " << newUser->getMaCCCD() << endl;
                delete newUser;
            }
            else
            {
                UserMap[newUser->getMaCCCD()] = newUser;
                LoginMap[{newUser->getMaCCCD(), newUser->getMatKhau()}] = newUser;
            }
        }
        else if (newUser != nullptr)
        {
            delete newUser; 
        }
    }
    userFile.close();

    for (auto const &[hs, idList] : hsMuonSachLinks)
    {
        for (const string &idSach : idList)
        {
            if (SachMap.count(idSach))
            {
                hs->muonSach(SachMap[idSach]); 
            }
            else
            {
                // cout << "Canh bao: Khong tim thay ID Sach '" << idSach << "' cho hoc sinh '" << hs->getHoTen() << "'." << endl;
            }
        }
    }
    cout << "Tai du lieu thanh cong!" << endl;
    pauseExecution();
}

void Application::saveData() const
{
    ofstream bookFile(BOOK_FILE_OUT);
    if (!bookFile.is_open())
    {
        cout << "Loi: Khong the ghi file " << BOOK_FILE_OUT << endl;
    }
    else
    {
        bookFile << SachMap.size() << endl;
        for (auto const &[id, sach] : SachMap)
        {
            sach->saveData(bookFile);
        }
        bookFile.close();
    }

    ofstream userFile(USER_FILE_OUT);
    if (!userFile.is_open())
    {
        cout << "Loi: Khong the ghi file " << USER_FILE_OUT << endl;
        pauseExecution();
        return;
    }

    userFile << UserMap.size() << endl;
    for (auto const &[cccd, user] : UserMap)
    {
        userFile << user->getRoleType() << endl; 
        user->saveData(userFile);
    }
    userFile.close();

    cout << "Luu va cap nhat du lieu thanh cong!" << endl;
}

// --- Auth Functions ---

void Application::login()
{
    printHeader("DANG NHAP");
    string cccd, matKhau;
    cout << "Ma CCCD (Username): ";
    getline(cin >> ws, cccd);
    cout << "Mat khau: ";
    getline(cin, matKhau);

    LoginToken token = {cccd, matKhau};
    if (LoginMap.count(token))
    {
        currentUser = LoginMap[token];
        cout << "\nDang nhap thanh cong! Chao mung " << currentUser->getHoTen() << "." << endl;
    }
    else
    {
        cout << "\nSai Ma CCCD hoac mat khau. Vui long thu lai." << endl;
    }
    pauseExecution();
}

void Application::logout()
{
    cout << "Da dang xuat tai khoan " << currentUser->getHoTen() << "." << endl;
    currentUser = nullptr;
    pauseExecution();
}

void Application::run()
{
    loadData();

    bool running = true;
    while (running)
    {
        if (currentUser == nullptr)
        {
            printHeader("MAN HINH CHINH");
            cout << "1. Dang nhap" << endl;
            cout << "0. Luu va Thoat" << endl;
            cout << "\nChon chuc nang: ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            switch (choice)
            {
            case '1':
                login();
                break;
            case '0':
                saveData();
                running = false;
                cout << "Da luu du lieu. Tam biet!" << endl;
                break;
            default:
                cout << "Lua chon khong hop le!" << endl;
                pauseExecution();
            }
        }
        else
        {
            string role = currentUser->getRoleType();
            if (role == "ADMIN")
                showAdminMenu();
            else if (role == "GVCN")
                showGVCNMenu();
            else if (role == "HOCSINH")
                showHocSinhMenu();
            else if (role == "THUTHU")
                showThuThuMenu();
            else
                logout();
        }
    }
}

// --- Menu Functions ---

void Application::showAdminMenu()
{
    printHeader("ADMIN MENU");
    cout << "1. Tao tai khoan moi" << endl;
    cout << "2. Xem tat ca tai khoan" << endl;
    cout << "3. Xoa tai khoan" << endl;
    cout << "4. Sua thong tin tai khoan" << endl;      
    cout << "5. Doi mat khau tai khoan" << endl;      
    cout << "6. Xuat danh sach tai khoan ra file" << endl; 
    // --- [MỚI] Thêm menu ---
    cout << "7. Nhap them SACH tu file" << endl;
    cout << "8. Nhap them NGUOI DUNG tu file" << endl;
    
    cout << "0. Dang xuat" << endl;
    cout << "\nChon chuc nang: ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case '1': Admin_TaoTaiKhoanMoi(); break;
    case '2': Admin_XemTatCaTaiKhoan(); break;
    case '3': Admin_XoaNguoiDung(); break;
    case '4': Admin_SuaThongTinTaiKhoan(); break;
    case '5': Admin_DoiMatKhau(); break;
    case '6': Admin_XuatDanhSachTaiKhoanRaFile(); break;
    
    // --- [MỚI] Thêm case ---
    case '7': Admin_NhapSachTuFile(); break;
    case '8': Admin_NhapNguoiDungTuFile(); break;

    case '0': logout(); break;
    default:
        cout << "Lua chon khong hop le!" << endl;
        pauseExecution();
    }
}

void Application::showGVCNMenu()
{
    printHeader("GVCN MENU");
    cout << "1. Xem danh sach lop chu nhiem" << endl;
    cout << "2. Nhap diem cho lop" << endl;
    cout << "3. Sua thong tin hoc sinh" << endl;
    cout << "4. Sua thong tin ca nhan" << endl; 
    cout << "5. Xuat danh sach lop ra file" << endl;
    cout << "0. Dang xuat" << endl;
    cout << "\nChon chuc nang: ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case '1':
        GVCN_XemDanhSachThongTinLop();
        break;
    case '2':
        GVCN_NhapDiemChoLopChuNhiem();
        break;
    case '3':
        GVCN_SuaThongTinHocSinh();
        break;
    case '4':
        GVCN_SuaThongTinBanThan(); 
        break;
    case '5':
        GVCN_XuatDanhSachLopRaFile();
        break;
    case '0':
        logout();
        break;
    default:
        cout << "Lua chon khong hop le!" << endl;
        pauseExecution();
    }
}

void Application::showHocSinhMenu()
{
    printHeader("HOC SINH MENU");
    cout << "1. Xem thong tin ca nhan" << endl;
    cout << "2. Sua thong tin ca nhan" << endl;
    cout << "3. Xem danh sach sach trong thu vien" << endl;
    cout << "4. Doi mat khau" << endl;
    cout << "5. Muon sach" << endl;
    cout << "6. Tra sach" << endl; // [UPDATE]
    cout << "7. Xem sach dang muon" << endl;
    cout << "8. Dang xuat" << endl;
    cout << "\nChon chuc nang: ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (!hs)
    {
        cout << "Loi: Khong the lay thong tin hoc sinh." << endl;
        logout();
        return;
    }

    switch (choice)
    {
    case '1':
        HS_XemThongTinBanThan();
        break;
    case '2':
        HS_SuaThongTinBanThan();
        break;
    case '3':
        HS_XemDanhSachSachTrongThuVien();
        break;
    case '4':
        HS_DoiMatKhau();
        break;
    case '5':
        HS_MuonSach();
        break;
    case '6':
        HS_TraSach();
        break;
    case '7':
        hs->xemDanhSachMuon();
        pauseExecution();
        break;
    case '8':
        logout();
        break;
    default:
        cout << "Lua chon khong hop le!" << endl;
        pauseExecution();
    }
}

void Application::showThuThuMenu()
{
    printHeader("THU THU MENU");
    cout << "1. Them sach moi" << endl;
    cout << "2. Xoa sach" << endl;
    cout << "3. Xem tat ca sach trong kho" << endl;
    cout << "4. Xem hoc sinh dang muon sach" << endl;
    cout << "5. Xuat file tat ca sach" << endl;
    cout << "6. Xuat file danh sach muon sach" << endl;
    cout << "7. Nhap them SACH tu file" << endl;
    cout << "0. Dang xuat" << endl;
    cout << "\nChon chuc nang: ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (choice)
    {
    case '1': ThuThu_ThemSach(); break;
    case '2': ThuThu_XoaSach(); break;
    case '3': ThuThu_XemTatCaSach(); break;
    case '4': ThuThu_XemHocSinhMuonSach(); break;
    case '5': ThuThu_XuatTatCaSachRaFile(); break;
    case '6': ThuThu_XuatDanhSachHocSinhMuonSachRaFile(); break;
    case '7': ThuThu_NhapSachTuFile(); break;
    
    case '0': logout(); break;
    default:
        cout << "Lua chon khong hop le!" << endl;
        pauseExecution();
    }
}

// --- Triển khai các chức năng cụ thể ---

// --- ADMIN ---
void Application::Admin_TaoTaiKhoanMoi()
{
    printHeader("TAO TAI KHOAN MOI");
    cout << "Chon loai tai khoan muon tao:\n";
    cout << "1. Hoc Sinh\n";
    cout << "2. Giao Vien Chu Nhiem\n";
    cout << "3. Thu Thu\n";
    cout << "4. Admin\n";
    cout << "0. Quay lai\n";
    cout << "Lua chon: ";
    char choice;
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (choice == '0')
        return;

    cout << "\n--- NHAP THONG TIN TAI KHOAN MOI (Bat buoc nhap tat ca thong tin) ---\n";
    // Thông tin chung
    string ten, ns, dc, cccd, email, gt, mk, ma;
    cout << "Nhap Ma CCCD (lam username): ";
    getline(cin >> ws, cccd);
    if (UserMap.count(cccd))
    {
        cout << "Ma CCCD da ton tai!" << endl;
        pauseExecution();
        return;
    }
    cout << "Nhap mat khau (Khong duoc de trong): ";
    getline(cin >> ws, mk);
    cout << "Nhap ho ten: ";
    getline(cin >> ws, ten);
    do{
        cout << "Nhap ngay thang nam sinh (VD: NN/TT/NNNN): ";
        getline(cin >> ws, ns);
        if (ns.size() != 10 || ns[2] != '/' || ns[5] != '/')
        {
            cout << "Ngay thang nam sinh khong hop le. Vui long nhap lai." << endl;
        }
    } while (ns.size() != 10 || ns[2] != '/' || ns[5] != '/');
    
    cout << "Nhap dia chi: ";
    getline(cin >> ws, dc);
    cout << "Nhap email: ";
    getline(cin >> ws, email);
    cout << "Nhap gioi tinh (Nam/Nu): ";
    getline(cin >> ws, gt);

    Nguoi *newUser = nullptr;

    switch (choice)
    {
    case '1':
    {
        string lop;
        cout << "Nhap Ma Hoc Sinh: ";
        getline(cin >> ws, ma);
        cout << "Nhap Lop: ";
        getline(cin >> ws, lop);
        newUser = new HocSinh(ten, ns, dc, cccd, email, gt, mk, ma, lop, 0.0, 0.0);
        break;
    }
    case '2':
    {
        string lopCN, boMon, hocVi;
        int soNam;
        double luong;
        cout << "Nhap Ma Giao Vien: ";
        getline(cin >> ws, ma);
        cout << "Nhap Lop Chu Nhiem: ";
        getline(cin >> ws, lopCN);
        cout << "Nhap Bo Mon Giang Day: ";
        getline(cin, boMon);
        cout << "Nhap Hoc Vi: ";
        getline(cin, hocVi);
        cout << "Nhap So Nam Kinh Nghiem: ";
        cin >> soNam;
        cout << "Nhap Luong: ";
        cin >> luong;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        newUser = new GiaoVienChuNhiem(ten, ns, dc, cccd, email, gt, mk, ma, lopCN, boMon, hocVi, soNam, luong);
        break;
    }
    case '3':
    {
        cout << "Nhap Ma Thu Thu: ";
        getline(cin >> ws, ma);
        newUser = new ThuThu(ten, ns, dc, cccd, email, gt, mk, ma);
        break;
    }
    case '4':
    {
        cout << "Nhap Ma Admin: ";
        getline(cin >> ws, ma);
        newUser = new Admin(ten, ns, dc, cccd, email, gt, mk, ma);
        break;
    }
    default:
        cout << "Lua chon khong hop le." << endl;
        pauseExecution();
        return;
    }

    UserMap[cccd] = newUser;
    LoginMap[{cccd, mk}] = newUser;
    cout << "Tao tai khoan thanh cong!" << endl;
    cout << "Dang luu vao file..." << endl;
    saveData(); 
    pauseExecution();
}

void Application::Admin_XemTatCaTaiKhoan() const
{
    printHeader("DANH SACH TAT CA TAI KHOAN");
    if (UserMap.empty())
    {
        cout << "Khong co tai khoan nao trong he thong." << endl;
    }
    else
    {
        for (auto const &[cccd, user] : UserMap)
        {
            user->displayInfo(); 
            cout << endl;
        }
    }
    pauseExecution();
}

void Application::Admin_XoaNguoiDung()
{
    printHeader("XOA TAI KHOAN NGUOI DUNG");

    // 1. Hiển thị danh sách tài khoản dạng bảng để dễ nhìn
    if (UserMap.empty())
    {
        cout << "Khong co tai khoan nao trong he thong." << endl;
        pauseExecution();
        return;
    }

    cout << "DANH SACH TAI KHOAN HIEN CO:\n";
    cout << "----------------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Ma CCCD"
         << "| " << left << setw(25) << "Ho Ten"
         << "| " << left << setw(35) << "Email"
         << "| " << left << setw(10) << "Vai Tro" << " |\n";
    cout << "----------------------------------------------------------------------------------------------\n";

    for (auto const &[key_cccd, user] : UserMap)
    {
        // Đánh dấu dòng của chính mình để dễ nhận biết (Optional)
        string role = user->getRoleType();
        string displayName = user->getHoTen();
        if (key_cccd == currentUser->getMaCCCD()) {
            displayName += " (Ban)";
        }

        cout << "| " << left << setw(15) << key_cccd
             << "| " << left << setw(25) << displayName
             << "| " << left << setw(35) << user->getEmail()
             << "| " << left << setw(10) << role << " |\n";
    }
    cout << "----------------------------------------------------------------------------------------------\n";

    // 2. Nhập CCCD cần xóa
    string cccd;
    cout << "\nNhap Ma CCCD tai khoan can xoa (Nhan Enter de quay lai): ";
    getline(cin, cccd);

    // Nếu nhập rỗng thì thoát
    if (cccd.empty()) return;

    // 3. Kiểm tra logic: Không cho phép xóa chính mình
    if (cccd == currentUser->getMaCCCD())
    {
        cout << "\n[CANH BAO] Ban khong the tu xoa tai khoan Admin dang dang nhap!" << endl;
        pauseExecution();
        return;
    }

    // 4. Kiểm tra tài khoản có tồn tại không
    if (!UserMap.count(cccd))
    {
        cout << "Loi: Khong tim thay tai khoan voi Ma CCCD: " << cccd << endl;
    }
    else
    {
        // Xác nhận lần cuối
        cout << "Ban co chac chan muon xoa tai khoan [" << UserMap[cccd]->getHoTen() << "]? (y/n): ";
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (confirm == 'y' || confirm == 'Y')
        {
            Nguoi *user = UserMap[cccd];

            // Logic cũ: Trả sách nếu là học sinh trước khi xóa
            HocSinh* hs = dynamic_cast<HocSinh*>(user);
            if(hs) {
                for (Sach* s : hs->getDanhSachMuon()) {
                    if(s) s->setSoLuong(s->getSoLuong() + 1); // Trả lại kho
                }
            }

            // Xóa khỏi map đăng nhập và map user
            LoginMap.erase({cccd, user->getMatKhau()});
            UserMap.erase(cccd);
            
            // Giải phóng bộ nhớ
            delete user;
            
            cout << "Da xoa tai khoan thanh cong!" << endl;
            
            // Gọi hàm lưu dữ liệu (đã được sửa để lưu vào Input folder)
            cout << "Dang cap nhat du lieu he thong..." << endl;
            saveData(); 
        }
        else
        {
            cout << "Da huy thao tac xoa." << endl;
        }
    }
    pauseExecution();
}

void Application::Admin_DoiMatKhau()
{
    printHeader("DOI MAT KHAU TAI KHOAN");
    cout << "=== Danh sach tai khoan trong he thong ===" << endl;
    if (UserMap.empty())
    {
        cout << "Khong co tai khoan nao trong he thong." << endl;
        pauseExecution();
        return;
    }

    cout << "------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Ma CCCD (User)"
        << "| " << left << setw(30) << "Ho Ten"
        << "| " << left << setw(15) << "Vai Tro"
        << " |\n";
    cout << "------------------------------------------------------------------\n";
    for (auto const &[cccd, user] : UserMap)
    {
        cout << "| " << left << setw(15) << cccd
            << "| " << left << setw(30) << user->getHoTen()
            << "| " << left << setw(15) << user->getRoleType() << " |\n";
    }
    cout << "------------------------------------------------------------------\n\n";

    string cccd;
    cout << "Nhap Ma CCCD tai khoan can doi mat khau: ";
    getline(cin, cccd);

    if (!UserMap.count(cccd))
    {
        cout << "Loi: Khong tim thay tai khoan voi Ma CCCD: " << cccd << endl;
        pauseExecution();
        return;
    }

    Nguoi *user = UserMap[cccd];
    string matKhauCu = user->getMatKhau();

    string matKhauMoi;
    cout << "Nhap mat khau moi cho tai khoan '" << user->getHoTen() << "': ";
    getline(cin, matKhauMoi);

    if (matKhauMoi.empty())
    {
        cout << "Mat khau khong the de trong." << endl;
        pauseExecution();
        return;
    }

    LoginMap.erase({cccd, matKhauCu});
    user->setMatKhau(matKhauMoi);
    LoginMap[{cccd, matKhauMoi}] = user;

    cout << "Doi mat khau thanh cong!" << endl;
    cout << "Dang luu thay doi vao file..." << endl;
    saveData();
    pauseExecution();
}

void Application::Admin_SuaThongTinTaiKhoan()
{
    printHeader("SUA THONG TIN TAI KHOAN");

    cout << "=== Danh sach tai khoan trong he thong ===" << endl;
    if (UserMap.empty())
    {
        cout << "Khong co tai khoan nao trong he thong." << endl;
        pauseExecution();
        return;
    }

    cout << "------------------------------------------------------------------\n";
    cout << "| " << left << setw(15) << "Ma CCCD (User)"
         << "| " << left << setw(30) << "Ho Ten"
         << "| " << left << setw(15) << "Vai Tro"
        << " |\n";
    cout << "------------------------------------------------------------------\n";
    for (auto const &[cccd, user] : UserMap)
    {
        cout << "| " << left << setw(15) << cccd
            << "| " << left << setw(30) << user->getHoTen()
            << "| " << left << setw(15) << user->getRoleType() << " |\n";
    }
    cout << "------------------------------------------------------------------\n\n";

    string cccd;
    cout << "Nhap Ma CCCD tai khoan can sua (De trong de huy): ";
    getline(cin, cccd);

    if (cccd.empty())
    {
        return; 
    }

    if (!UserMap.count(cccd))
    {
        cout << "Loi: Khong tim thay tai khoan voi Ma CCCD: " << cccd << endl;
        pauseExecution();
        return;
    }

    Nguoi *user = UserMap[cccd];
    string role = user->getRoleType();

    clearConsole();
    printHeader("SUA TAI KHOAN: " + user->getHoTen());
    cout << "--- THONG TIN HIEN TAI ---" << endl;
    user->displayInfo();

    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de bo qua) ---" << endl;

    string ten, ns, dc, email, gt;
    cout << "Ho Ten (" << user->getHoTen() << "): ";
    getline(cin, ten); 
    cout << "Ngay Sinh (VD: 10/10/2000): ";
    getline(cin, ns);
    cout << "Dia Chi (" << user->getDiaChi() << "): ";
    getline(cin, dc);
    cout << "Email (" << user->getEmail() << "): ";
    getline(cin, email);
    cout << "Gioi Tinh (Nam/Nu): ";
    getline(cin, gt);

    user->setThongTinCoBan(ten, ns, dc, email, gt);

    if (role == "HOCSINH")
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        string lop;
        cout << "Lop (" << hs->getLop() << "): ";
        getline(cin, lop);
        hs->setLop(lop);
    }
    else if (role == "GVCN")
    {
        GiaoVienChuNhiem *gvcn = dynamic_cast<GiaoVienChuNhiem *>(user);
        string lopCN, boMon, hocVi, soNam_str, luong_str; 
        
        cout << "Lop Chu Nhiem (" << gvcn->getLopChuNhiem() << "): ";
        getline(cin, lopCN);
        cout << "Bo Mon Giang Day (" << gvcn->getBoMonGiangDay() << "): ";
        getline(cin, boMon);
        cout << "Hoc Vi (" << gvcn->getHocVi() << "): ";
        getline(cin, hocVi);
        cout << "So Nam Kinh Nghiem (" << gvcn->getSoNamKinhNghiem() << "): ";
        getline(cin, soNam_str); 
        cout << "Luong (" << gvcn->getLuong() << "): ";
        getline(cin, luong_str); 

        gvcn->setLopChuNhiem(lopCN);
        gvcn->setBoMonGiangDay(boMon);
        gvcn->setHocVi(hocVi);
        if(!soNam_str.empty()) gvcn->setSoNamKinhNghiem(stoi(soNam_str)); 
        if(!luong_str.empty()) gvcn->setLuong(stod(luong_str));           
    }
    else if (role == "THUTHU")
    {
        cout << "Khong co thong tin bo sung cho Thu Thu." << endl;
    }
    else if (role == "ADMIN")
    {
        cout << "Khong co thong tin bo sung cho Admin." << endl;
    }

    cout << "\nCap nhat thong tin thanh cong!" << endl;
    saveData(); 
    pauseExecution();
}

void Application::Admin_XuatDanhSachTaiKhoanRaFile()
{
    string OUTPUT_PATH = "Data/Output/";
    string OUTPUT_FILE_NAME;
    printHeader("XUAT DANH SACH TAI KHOAN");
    cout << "Nhap ten file muon xuat (vi du: users.txt): ";
    getline(cin >> ws, OUTPUT_FILE_NAME); 
    OUTPUT_PATH += OUTPUT_FILE_NAME;
    ofstream file(OUTPUT_PATH, ios::out);
    if (!file.is_open())
    {
        cout << "Khong the mo file " << OUTPUT_PATH << " de xuat!" << endl;
        pauseExecution();
        return;
    }

    file << "DANH SACH TAT CA TAI KHOAN (XUAT TU ADMIN)\n";
    file << "===========================================\n\n";

    for (auto const &[cccd, user] : UserMap)
    {
        file << "Vai Tro: " << user->getRoleType() << endl;
        file << "Ten: " << user->getHoTen() << endl;
        file << "Ma CCCD: " << user->getMaCCCD() << endl;
        file << "Email: " << user->getEmail() << endl;
        file << "--------------------------------\n";
    }

    file.close();
    cout << "Xuat file thanh cong -> " << OUTPUT_PATH << endl;
    pauseExecution();
}

void Application::Admin_NhapSachTuFile() {
    printHeader("NHAP SACH TU FILE");
    string filename;
    cout << "Nhap duong dan/ten file (VD: books_new): ";
    getline(cin >> ws, filename);
    string fullPath = "Data/Input/" + filename + ".txt";
    filename = fullPath;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << endl;
        pauseExecution();
        return;
    }

    int count;
    if (!(file >> count)) {
        cout << "Loi: Dinh dang file khong hop le (dong dau tien phai la so luong)." << endl;
        pauseExecution();
        return;
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    int successCount = 0;
    int failCount = 0;

    cout << "\nDang xu ly " << count << " cuon sach...\n";

    for (int i = 0; i < count; ++i) {
        Sach* tempSach = new Sach("", "", "", "", 0.0, 0);
        tempSach->loadData(file);

        if (tempSach->getIDSach().empty()) {
            delete tempSach;
            continue;
        }
        if (SachMap.count(tempSach->getIDSach())) {
            cout << " -> Bo qua: ID " << tempSach->getIDSach() << " da ton tai." << endl;
            delete tempSach;
            failCount++;
        } else {
            SachMap[tempSach->getIDSach()] = tempSach;
            successCount++;
        }
    }

    file.close();
    cout << "\nKet qua: Them thanh cong " << successCount << " sach. Bo qua (trung ID) " << failCount << " sach." << endl;
    cout << "Du lieu da duoc cap nhat vao bo nho (hay chon Luu & Thoat de ghi lai file chinh)." << endl;
    pauseExecution();
}

void Application::Admin_NhapNguoiDungTuFile() {
    printHeader("NHAP NGUOI DUNG TU FILE");
    string filename;
    cout << "Nhap duong dan/ten file (VD: newusers) : ";
    getline(cin >> ws, filename);
    string fullPath = "Data/Input/" + filename + ".txt";
    filename = fullPath;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << endl;
        pauseExecution();
        return;
    }

    int count;
    if (!(file >> count)) {
        cout << "Loi: Dinh dang file khong hop le (dong dau tien phai la so luong)." << endl;
        pauseExecution();
        return;
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    int successCount = 0;
    int failCount = 0;

    map<HocSinh*, vector<string>> tempHSLinks;

    cout << "\nDang xu ly " << count << " tai khoan...\n";

    for (int i = 0; i < count; ++i) {
        string role;
        getline(file, role);
        Nguoi* newUser = nullptr;
        if (role == "HOCSINH") {
            newUser = new HocSinh("", "", "", "", "", "", "", "", "", 0.0, 0.0);
            newUser->loadData(file);

            HocSinh* hs = dynamic_cast<HocSinh*>(newUser);
            if (hs) tempHSLinks[hs] = hs->getTempIdSachMuon();

        } else if (role == "GVCN") {
            newUser = new GiaoVienChuNhiem("", "", "", "", "", "", "", "", "", "", "", 0, 0.0);
            newUser->loadData(file);
        } else if (role == "ADMIN") {
            newUser = new Admin("", "", "", "", "", "", "", "");
            newUser->loadData(file);
        } else if (role == "THUTHU") {
            newUser = new ThuThu("", "", "", "", "", "", "", "");
            newUser->loadData(file);
        }

        if (newUser != nullptr && !newUser->getMaCCCD().empty()) {
            if (UserMap.count(newUser->getMaCCCD())) {
                cout << " -> Bo qua: CCCD " << newUser->getMaCCCD() << " (" << newUser->getHoTen() << ") da ton tai." << endl;
                delete newUser;
                if(role == "HOCSINH") {
                    HocSinh* dummy = (HocSinh*)newUser;
                    tempHSLinks.erase(dummy);
                }
                failCount++;
            } else {
                UserMap[newUser->getMaCCCD()] = newUser;
                LoginMap[{newUser->getMaCCCD(), newUser->getMatKhau()}] = newUser;
                successCount++;
            }
        } else {
            if (newUser) delete newUser;
        }
    }
    for (auto const& [hs, idList] : tempHSLinks) {
        if (UserMap.count(hs->getMaCCCD()) && UserMap[hs->getMaCCCD()] == hs) {
            for (const string& idSach : idList) {
                if (SachMap.count(idSach)) {
                    hs->muonSach(SachMap[idSach]);
                }
            }
        }
    }
    file.close();
    cout << "\nKet qua: Them thanh cong " << successCount << " tai khoan. Bo qua " << failCount << " tai khoan." << endl;
    cout << "Du lieu da duoc cap nhat vao bo nho." << endl;
    pauseExecution();
}

// --- GVCN ---
void Application::GVCN_XemDanhSachThongTinLop() const
{
    GiaoVienChuNhiem *gvcn = dynamic_cast<GiaoVienChuNhiem *>(currentUser);
    if (!gvcn)
        return;

    string lopCN = gvcn->getLopChuNhiem();
    printHeader("DANH SACH HOC SINH LOP " + lopCN);

    bool found = false;
    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && hs->getLop() == lopCN)
        {
            hs->displayInfo();
            cout << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "Lop " << lopCN << " chua co hoc sinh nao." << endl;
    }
    pauseExecution();
}

void Application::GVCN_NhapDiemChoLopChuNhiem()
{
    GiaoVienChuNhiem *gvcn = dynamic_cast<GiaoVienChuNhiem *>(currentUser);
    if (!gvcn)
        return;

    string lopCN = gvcn->getLopChuNhiem();
    printHeader("NHAP DIEM CHO LOP " + lopCN);

    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && hs->getLop() == lopCN)
        {
            cout << "--- Nhap diem cho HS: " << hs->getHoTen() << " (MaHS: " << hs->getMaHocSinh() << ") ---" << endl;
            double dtb, drl;
            cout << "Diem trung binh hien tai: " << hs->getDiemTrungBinh() << ". Nhap diem moi: ";
            // [UPDATE] Kiểm tra nhập liệu số
            while(!(cin >> dtb)) {
                cout << "Loi: Vui long nhap so! Nhap lai DTB: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            cout << "Diem ren luyen hien tai: " << hs->getDiemRenLuyen() << ". Nhap diem moi: ";
            while(!(cin >> drl)) {
                cout << "Loi: Vui long nhap so! Nhap lai DRL: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            hs->setDiemTrungBinh(dtb);
            hs->setDiemRenLuyen(drl);
            cout << "Cap nhat diem thanh cong!" << endl << endl;
        }
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    cout << "Da hoan tat nhap diem cho lop." << endl;
    cout << "Dang luu vao file..." << endl;
    saveData(); 
    pauseExecution();
}

void Application::GVCN_SuaThongTinHocSinh()
{
    GiaoVienChuNhiem *gvcn = dynamic_cast<GiaoVienChuNhiem *>(currentUser);
    if (!gvcn)
        return;

    string lopCN = gvcn->getLopChuNhiem();
    printHeader("SUA THONG TIN HOC SINH LOP " + lopCN);

    bool found = false;
    cout << "--- Danh sach hoc sinh lop " << lopCN << " ---" << endl;
    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && hs->getLop() == lopCN)
        {
            cout << "  Ma HS: " << hs->getMaHocSinh() << " | Ten: " << hs->getHoTen() << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Lop khong co hoc sinh." << endl;
        pauseExecution();
        return;
    }
    
    string maHS;
    cout << "\nNhap Ma Hoc Sinh can sua: ";
    getline(cin >> ws, maHS);

    HocSinh *hs_found = nullptr;
    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && hs->getLop() == lopCN && hs->getMaHocSinh() == maHS)
        {
            hs_found = hs;
            break;
        }
    }

    if (hs_found == nullptr)
    {
        cout << "Khong tim thay hoc sinh voi Ma HS: " << maHS << " trong lop cua ban." << endl;
        pauseExecution();
        return;
    }

    clearConsole();
    printHeader("SUA THONG TIN CHO: " + hs_found->getHoTen());
    cout << "--- THONG TIN HIEN TAI ---" << endl;
    hs_found->displayInfo();
    
    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de bo qua) ---" << endl;
    string ten, ns, dc, email, gt;

    cout << "Ho Ten (" << hs_found->getHoTen() << "): ";
    getline(cin, ten);
    cout << "Ngay Sinh (" << hs_found->getNgayThangNamSinh() << "): ";
    getline(cin, ns);
    cout << "Dia Chi (" << hs_found->getDiaChi() << "): ";
    getline(cin, dc);
    cout << "Email (" << hs_found->getEmail() << "): ";
    getline(cin, email);
    cout << "Gioi Tinh (" << hs_found->getGioiTinh() << "): ";
    getline(cin, gt);

    hs_found->setThongTinCoBan(ten, ns, dc, email, gt);
    
    cout << "\nCap nhat thong tin hoc sinh thanh cong!" << endl;
    saveData();
    pauseExecution();
}

void Application::GVCN_SuaThongTinBanThan()
{
    printHeader("SUA THONG TIN CA NHAN (GVCN)");
    
    GiaoVienChuNhiem* gvcn = dynamic_cast<GiaoVienChuNhiem*>(currentUser);
    if(!gvcn) return;
    
    cout << "--- THONG TIN HIEN TAI ---" << endl;
    gvcn->displayInfo();
    
    cout << "\n--- NHAP THONG TIN MOI (Nhan Enter de bo qua) ---" << endl;
    
    string ten, ns, dc, email, gt;
    cout << "Ho Ten (" << gvcn->getHoTen() << "): "; 
    getline(cin, ten);
    cout << "Ngay Sinh (" << gvcn->getNgayThangNamSinh() << "): "; 
    getline(cin, ns);
    cout << "Dia Chi (" << gvcn->getDiaChi() << "): "; 
    getline(cin, dc);
    cout << "Email (" << gvcn->getEmail() << "): "; 
    getline(cin, email);
    cout << "Gioi Tinh (" << gvcn->getGioiTinh() << "): "; 
    getline(cin, gt);
    
    gvcn->setThongTinCoBan(ten, ns, dc, email, gt);

    string boMon, hocVi;
    cout << "Bo Mon Giang Day (" << gvcn->getBoMonGiangDay() << "): "; 
    getline(cin, boMon);
    cout << "Hoc Vi (" << gvcn->getHocVi() << "): "; 
    getline(cin, hocVi);

    if(!boMon.empty()) gvcn->setBoMonGiangDay(boMon);
    if(!hocVi.empty()) gvcn->setHocVi(hocVi);

    cout << "\nCap nhat thong tin thanh cong!" << endl;
    saveData();
    pauseExecution();
}

void Application::GVCN_XuatDanhSachLopRaFile()
{
    GiaoVienChuNhiem *gvcn = dynamic_cast<GiaoVienChuNhiem *>(currentUser);
    if (!gvcn)
        return;

    string lopCN = gvcn->getLopChuNhiem();
    string filename = "Data/Output/export_lop_" + lopCN + ".txt";
    ofstream file(filename);

    if (!file.is_open())
    {
        cout << "Khong thể mo file de xuat!" << endl;
        pauseExecution();
        return;
    }

    printHeader("XUAT DANH SACH LOP " + lopCN);
    file << "DANH SACH HOC SINH LOP " << lopCN << "\n";
    file << "Giao vien chu nhiem: " << gvcn->getHoTen() << "\n";
    file << "===========================================\n\n";
    file << left << setw(25) << "Ho Ten"
        << setw(15) << "Ma HS"
        << setw(30) << "Email"
        << setw(10) << "Diem TB"
        << setw(10) << "Diem RL" << "\n";
    file << "--------------------------------------------------------------------------\n";

    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && hs->getLop() == lopCN)
        {
            file << left << setw(25) << hs->getHoTen()
                << setw(15) << hs->getMaHocSinh()
                << setw(30) << hs->getEmail()
                << setw(10) << fixed << setprecision(2) << hs->getDiemTrungBinh()
                << setw(10) << fixed << setprecision(2) << hs->getDiemRenLuyen() << "\n";
        }
    }

    file.close();
    cout << "Xuat file thanh cong -> " << filename << endl;
    pauseExecution();
}

// --- HOC SINH ---
void Application::HS_XemThongTinBanThan()
{
    printHeader("THONG TIN CA NHAN");
    currentUser->displayInfo();
    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (hs)
        hs->xemDanhSachMuon();
    pauseExecution();
}

void Application::HS_SuaThongTinBanThan()
{
    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (!hs)
        return;

    printHeader("SUA THONG TIN CA NHAN");
    string ns, dc, gt;

    cout << "Luu y: De trong de bo qua, giu lai gia tri cu.\n";
    cout << "Ngay Sinh hien tai: " << hs->getNgayThangNamSinh() << "\nNhap Ngay Sinh moi (NGAY/THANG/NAM): ";
    getline(cin, ns);
    cout << "Dia Chi hien tai: " << hs->getDiaChi() << "\nNhap Dia Chi moi: ";
    getline(cin, dc);
    hs->setThongTinCaNhan( "" , ns, dc, "", ""); 
    cout << "\nCap nhat thong tin thanh cong!" << endl;
    cout << "Dang luu vao file..." << endl;
    saveData();
    pauseExecution();
}

void Application::HS_XemDanhSachSachTrongThuVien()
{
    printHeader("THU VIEN SACH");
    printHeader("DANH SACH TAT CA SACH");
    cout << "--------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(10) << "ID Sach"
        << "| " << left << setw(30) << "Ten Sach"
        << "| " << left << setw(25) << "Ten Tac Gia"
        << "| " << left << setw(15) << "The Loai"
        << "| " << right << setw(8) << "So Luong" << " |" << endl;
    cout << "--------------------------------------------------------------------------------------\n";
    if (SachMap.empty()) {
        cout << "|                  Khong co sach nao trong thu vien.                               |\n";
    } else {
        for (auto const& [id, sach] : SachMap) {
            sach->displayInfo(); 
        }
    }
    cout << "--------------------------------------------------------------------------------------\n";
    pauseExecution();
}

void Application::HS_MuonSach(){
    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (!hs)
        return;

    printHeader("MUON SACH");
    cout << "--------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(10) << "ID Sach"
        << "| " << left << setw(30) << "Ten Sach"
        << "| " << left << setw(25) << "Ten Tac Gia"
        << "| " << left << setw(15) << "The Loai"
        << "| " << right << setw(8) << "So Luong" << " |" << endl;
    cout << "--------------------------------------------------------------------------------------\n";
    for (auto const &[id, sach] : SachMap)
    {
        sach->displayInfo();
    }
    cout << "--------------------------------------------------------------------------------------\n";

    string idSach;
    cout << "\nNhap ID Sach ban muon muon: ";
    getline(cin >> ws, idSach);

    if (!SachMap.count(idSach))
    {
        cout << "Loi: Khong tim thay sach voi ID: " << idSach << endl;
    }
    else
    {
        if(hs->DaMuonSach(idSach)) {
            cout << "Loi: Ban da muon sach nay truoc do. Vui long khong muon lai." << endl;
            pauseExecution();
            return;
        }
        Sach *sach = SachMap[idSach];
        if (sach->getSoLuong() <= 0)
        {
            cout << "Loi: Sach '" << sach->getTenTacPham() << "' da het hang." << endl;
        }
        else
        {
            hs->muonSach(sach);
            sach->setSoLuong(sach->getSoLuong() - 1); 
            cout << "Muon sach '" << sach->getTenTacPham() << "' thanh cong!" << endl;
            cout << "Dang luu vao file..." << endl;
            saveData(); 
        }
    }
    pauseExecution();
}

void Application::HS_TraSach() {
    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (!hs) return;

    printHeader("TRA SACH");
    
    if(hs->getDanhSachMuon().empty()) {
        cout << "Ban hien khong muon cuon sach nao de tra." << endl;
        pauseExecution();
        return;
    }

    hs->xemDanhSachMuon();

    string idSach;
    cout << "\nNhap ID Sach ban muon tra: ";
    getline(cin >> ws, idSach);

    // Tìm sách trong kho để cộng lại số lượng
    if (!SachMap.count(idSach)) {
        cout << "Loi he thong: Sach nay khong con ton tai trong CSDL thu vien." << endl;
    } else {
        // Gọi hàm traSach bên HocSinh
        bool result = hs->traSach(idSach);
        if(result) {
            // Cộng lại số lượng
            Sach* sach = SachMap[idSach];
            sach->setSoLuong(sach->getSoLuong() + 1);
            cout << "Da tra sach '" << sach->getTenTacPham() << "' thanh cong!" << endl;
            cout << "Dang luu vao file..." << endl;
            saveData();
        } else {
            cout << "Loi: Ban khong muon sach co ID " << idSach << endl;
        }
    }
    pauseExecution();
}

void Application::HS_DoiMatKhau()
{
    HocSinh *hs = dynamic_cast<HocSinh *>(currentUser);
    if (!hs)
        return;

    printHeader("DOI MAT KHAU TAI KHOAN");
    string matKhauCu;
    cout << "Nhap mat khau hien tai: ";
    getline(cin >> ws, matKhauCu);

    if (matKhauCu != hs->getMatKhau())
    {
        cout << "Mat khau hien tai khong dung!" << endl;
        pauseExecution();
        return;
    }

    string matKhauMoi;
    cout << "Nhap mat khau moi: ";
    getline(cin, matKhauMoi);

    if (matKhauMoi.empty())
    {
        cout << "Mat khau khong the de trong." << endl;
        pauseExecution();
        return;
    }

    LoginMap.erase({hs->getMaCCCD(), matKhauCu});
    hs->setMatKhau(matKhauMoi);
    LoginMap[{hs->getMaCCCD(), matKhauMoi}] = hs;

    cout << "Doi mat khau thanh cong!" << endl;
    cout << "Dang luu thay doi vao file..." << endl;
    saveData();
    pauseExecution();
}

// --- THU THU ---
void Application::ThuThu_ThemSach(){
    printHeader("THEM SACH MOI");
    string id, ten, tacGia, loai;
    double gia;
    int sl;

    cout << "Nhap ID Sach: ";
    getline(cin >> ws, id);
    if (SachMap.count(id))
    {
        cout << "Loi: ID Sach da ton tai!" << endl;
        pauseExecution();
        return;
    }
    cout << "Nhap Ten Sach: ";
    getline(cin, ten);
    cout << "Nhap Ten Tac Gia: ";
    getline(cin, tacGia);
    cout << "Nhap The Loai: ";
    getline(cin, loai);
    cout << "Nhap Gia Ban: ";
    cin >> gia;
    cout << "Nhap So Luong: ";
    cin >> sl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Sach *newSach = new Sach(id, tacGia, ten, loai, gia, sl);
    SachMap[id] = newSach;

    cout << "Them sach '" << ten << "' thanh cong!" << endl;
    cout << "Dang luu vao file..." << endl;
    saveData(); 
    pauseExecution();
}

void Application::ThuThu_XoaSach()
{
    printHeader("XOA SACH");
    string id;
    cout << "Nhap ID Sach can xoa: ";
    getline(cin >> ws, id);

    if (!SachMap.count(id))
    {
        cout << "Loi: Khong tim thay sach voi ID: " << id << endl;
    }
    else
    {
        bool dangMuon = false;
        for (auto const &[cccd, user] : UserMap) {
            HocSinh* hs = dynamic_cast<HocSinh*>(user);
            if(hs) {
                for (Sach* s : hs->getDanhSachMuon()) {
                    if (s && s->getIDSach() == id) {
                        cout << "Loi: Sach nay dang duoc hoc sinh " << hs->getHoTen() << " muon. Khong the xoa." << endl;
                        dangMuon = true;
                        break;
                    }
                }
            }
            if(dangMuon) break;
        }

        if(!dangMuon) {
            Sach *sach = SachMap[id];
            SachMap.erase(id);
            delete sach; 
            cout << "Da xoa sach thanh cong!" << endl;
            cout << "Dang luu vao file..." << endl;
            saveData(); 
        }
    }
    pauseExecution();
}

void Application::ThuThu_XemTatCaSach() const
{
    printHeader("DANH SACH TAT CA SACH");
    cout << "--------------------------------------------------------------------------------------\n";
    cout << "| " << left << setw(10) << "ID Sach"
        << "| " << left << setw(30) << "Ten Sach"
        << "| " << left << setw(25) << "Ten Tac Gia"
        << "| " << left << setw(15) << "The Loai"
        << "| " << right << setw(8) << "So Luong"
        << " |" << endl;
    cout << "--------------------------------------------------------------------------------------\n";

    if (SachMap.empty())
    {
        cout << "|                  Khong co sach nao trong thu vien.                               |\n";
    }
    else
    {
        for (auto const &[id, sach] : SachMap)
        {
            sach->displayInfo(); 
        }
    }
    cout << "--------------------------------------------------------------------------------------\n";
    pauseExecution();
}

void Application::ThuThu_XemHocSinhMuonSach() const
{
    printHeader("THONG KE SACH DANG DUOC MUON");
    bool found = false;
    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && !hs->getDanhSachMuon().empty())
        {
            cout << "\n--- Hoc Sinh: " << hs->getHoTen() << " (Lop: " << hs->getLop() << ") ---" << endl;
            hs->xemDanhSachMuon(); 
            found = true;
        }
    }
    if (!found)
    {
        cout << "Chua co hoc sinh nao muon sach." << endl;
    }
    pauseExecution();
}

void Application::ThuThu_XuatTatCaSachRaFile()
{
    ofstream file("Data/Output/export_books.txt", ios::out);
    if (!file.is_open())
    {
        cout << "Khong thể mo file de xuat!" << endl;
        pauseExecution();
        return;
    }

    printHeader("XUAT FILE DANH SACH SACH");
    file << "DANH SACH TAT CA SACH TRONG THU VIEN\n";
    file << "======================================\n\n";
    file << left << setw(12) << "ID Sach"
        << setw(32) << "Ten Sach"
        << setw(27) << "Ten Tac Gia"
        << setw(17) << "The Loai"
        << setw(10) << "So Luong" << "\n";
    file << "------------------------------------------------------------------------------------------\n";

    for (auto const &[id, sach] : SachMap)
    {
        file << left << setw(12) << sach->getIDSach()
            << setw(32) << sach->getTenTacPham()
            << setw(27) << sach->getTenTacGia()
            << setw(17) << sach->getTheLoai()
            << setw(10) << sach->getSoLuong() << "\n";
    }

    file.close();
    cout << "Xuat file thanh cong -> Data/Output/export_books.txt" << endl;
    pauseExecution();
}

void Application::ThuThu_XuatDanhSachHocSinhMuonSachRaFile()
{
    ofstream file("Data/Output/export_borrows.txt");
    if (!file.is_open())
    {
        cout << "Khong thể mo file de xuat!" << endl;
        pauseExecution();
        return;
    }

    printHeader("XUAT FILE DANH SACH MUON SACH");
    file << "DANH SACH HOC SINH DANG MUON SACH\n";
    file << "=================================\n\n";

    for (auto const &[cccd, user] : UserMap)
    {
        HocSinh *hs = dynamic_cast<HocSinh *>(user);
        if (hs && !hs->getDanhSachMuon().empty())
        {
            file << "--- Hoc Sinh: " << hs->getHoTen() << " (Lop: " << hs->getLop() << ") ---\n";
            file << left << setw(12) << "ID Sach"
                << setw(32) << "Ten Sach"
                << setw(27) << "Ten Tac Gia" << "\n";
            file << "--------------------------------------------------------------\n";

            for (Sach *sach : hs->getDanhSachMuon())
            {
                if (sach)
                {
                    file << left << setw(12) << sach->getIDSach()
                        << setw(32) << sach->getTenTacPham()
                        << setw(27) << sach->getTenTacGia() << "\n";
                }
            }
            file << "\n";
        }
    }

    file.close();
    cout << "Xuat file thanh cong -> Data/Output/export_borrows.txt" << endl;
    pauseExecution();
}

void Application::ThuThu_NhapSachTuFile() {
    printHeader("NHAP SACH TU FILE");
    string filename;
    cout << "Nhap duong dan/ten file (VD: newbooks) :";
    getline(cin >> ws, filename);
    string fullPath = "Data/Input/" + filename + ".txt";
    filename = fullPath;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Loi: Khong the mo file " << filename << endl;
        pauseExecution();
        return;
    }

    int count;
    if (!(file >> count)) {
        cout << "Loi: Dinh dang file khong hop le (dong dau tien phai la so luong)." << endl;
        pauseExecution();
        return;
    }
    file.ignore(numeric_limits<streamsize>::max(), '\n');

    int successCount = 0;
    int failCount = 0;

    cout << "\nDang xu ly " << count << " cuon sach...\n";

    for (int i = 0; i < count; ++i) {
        Sach* tempSach = new Sach("", "", "", "", 0.0, 0);
        tempSach->loadData(file);

        if (tempSach->getIDSach().empty()) {
            delete tempSach;
            continue;
        }

        // Kiểm tra trùng ID trong SachMap hiện tại
        if (SachMap.count(tempSach->getIDSach())) {
            cout << " -> Bo qua: ID " << tempSach->getIDSach() << " da ton tai." << endl;
            delete tempSach;
            failCount++;
        } else {
            SachMap[tempSach->getIDSach()] = tempSach;
            successCount++;
        }
    }

    file.close();
    cout << "\nKet qua: Them thanh cong " << successCount << " sach. Bo qua (trung ID) " << failCount << " sach." << endl;
    cout << "Dang luu du lieu vao he thong..." << endl;
    saveData(); 
    
    pauseExecution();
}