#include "Application.cpp"
#include <iostream>

using namespace std;

int main() {
    #ifdef _WIN32
        system("chcp 65001");
    #endif
    
    Application app; // Tạo đối tượng ứng dụng
    app.run();       // Chạy ứng dụng
    
    return 0;
}