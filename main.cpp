#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>
using namespace std;

// ==========================================
// DINH NGHIA DU LIEU 
// ==========================================

const int BAC  = 1; 
const int DONG = 2; 
const int NAM  = 4; 
const int TAY  = 8; 

struct OVuong {
    int tuong;       
    bool daDiQua;    
    int trangThaiDi; // 0: Chua buoc den, 1: Dang di toi (Xanh), 2: Quay lui (Cam)
};

struct MeCung {
    int soHang, soCot;                 
    vector<vector<OVuong> > banDo;     
    int hangVao, cotVao;               
    int hangRa, cotRa;                 
};


// ==========================================
// MODULE 1: TAO DU LIEU ME CUNG (DFS)
// ==========================================

void ducTuong(int h, int c, MeCung& mc) {
    mc.banDo[h][c].daDiQua = true;
    int cacHuong[4] = {BAC, DONG, NAM, TAY};
    
    for(int i = 0; i < 4; i++) {
        int viTriDoi = rand() % 4;
        int tam = cacHuong[i];
        cacHuong[i] = cacHuong[viTriDoi];
        cacHuong[viTriDoi] = tam;
    }

    for(int i = 0; i < 4; i++) {
        int huong = cacHuong[i];
        int hangTiep = h, cotTiep = c, huongDoiDien;

        if(huong == BAC) { hangTiep--; huongDoiDien = NAM; }
        else if(huong == DONG) { cotTiep++; huongDoiDien = TAY; }
        else if(huong == NAM) { hangTiep++; huongDoiDien = BAC; }
        else if(huong == TAY) { cotTiep--; huongDoiDien = DONG; }

        if(hangTiep >= 0 && hangTiep < mc.soHang && cotTiep >= 0 && cotTiep < mc.soCot && !mc.banDo[hangTiep][cotTiep].daDiQua) {
            mc.banDo[h][c].tuong &= ~huong;                  
            mc.banDo[hangTiep][cotTiep].tuong &= ~huongDoiDien;  
            ducTuong(hangTiep, cotTiep, mc);                 
        }
    }
}

MeCung taoMeCung() {
    MeCung mc;
    cout<<"nhap kich thuoc ma tran(hang cot):";
    cin>> mc.soHang ; 
	cin>>mc.soCot; 
	cout<<"nhap toa do diem bat dau(hang cot):";
    cin>>mc.hangVao ; 
	cin>> mc.cotVao; 
	cout<<"nhap toa do diem ket thuc(hang cot):";
    cin>> mc.hangRa ; 
	cin>>mc.cotRa;  
    OVuong oMacDinh;
    oMacDinh.tuong = 15;
    oMacDinh.daDiQua = false;
    oMacDinh.trangThaiDi = 0;
    
    mc.banDo.assign(mc.soHang, vector<OVuong>(mc.soCot, oMacDinh));

    srand(time(0)); 
    ducTuong(mc.hangVao, mc.cotVao, mc);

    for(int h = 0; h < mc.soHang; h++) {
        for(int c = 0; c < mc.soCot; c++) {
            mc.banDo[h][c].daDiQua = false;
            mc.banDo[h][c].trangThaiDi = 0; 
        }
    }
    return mc;
}


// ==========================================
// MODULE 3: GIAO DIEN DO HOA (SFML)
// ==========================================

const int KICH_THUOC_O = 50; 
const int DO_DAY_TUONG = 5;  

void veToanBoMeCung(sf::RenderWindow& window, const MeCung& mc) {
    // 1. Xoa sach man hinh cu de chong nhap nhay
    window.clear(sf::Color(15, 20, 35)); 
  
    // 1. Danh dau o Xuat Phat (Xanh la) va Dich (Do)
    
    // O Xuat Phat
    sf::RectangleShape nenXuatPhat(sf::Vector2f(KICH_THUOC_O, KICH_THUOC_O));
    nenXuatPhat.setFillColor(sf::Color(46, 204, 113, 120)); // Xanh la cay (Hoi trong suot)
    nenXuatPhat.setPosition(mc.cotVao * KICH_THUOC_O, mc.hangVao * KICH_THUOC_O);
    window.draw(nenXuatPhat);
    
    // O Dich Den
    sf::RectangleShape nenDichDen(sf::Vector2f(KICH_THUOC_O, KICH_THUOC_O));
    nenDichDen.setFillColor(sf::Color(231, 76, 60, 120)); // Do (Hoi trong suot)
    nenDichDen.setPosition(mc.cotRa * KICH_THUOC_O, mc.hangRa * KICH_THUOC_O);
    window.draw(nenDichDen);

    // 2. Ve tat ca cac vet chan thuat toan
    // Chinh kich thuoc buoc chan nho lai (Vi du: 16x16 pixel de nhin giong 1 vien bi)
    float kichThuocBuocChan = 20.0f;
    sf::RectangleShape buocChan(sf::Vector2f(kichThuocBuocChan, kichThuocBuocChan));
    
    // Tinh toan do lech de canh giua buoc chan vao giua o vuong
    float lechTam = (KICH_THUOC_O - kichThuocBuocChan) / 2.0f;

    for (int h = 0; h < mc.soHang; h++) {
        for (int c = 0; c < mc.soCot; c++) {
            if (mc.banDo[h][c].trangThaiDi == 1) {
                buocChan.setFillColor(sf::Color(100, 255, 200, 200)); // Xanh ngoc
                buocChan.setPosition(c * KICH_THUOC_O + lechTam, h * KICH_THUOC_O + lechTam);
                window.draw(buocChan);
            } 
            else if (mc.banDo[h][c].trangThaiDi == 2) {
                buocChan.setFillColor(sf::Color(255, 140, 100, 200)); // Cam dat
                buocChan.setPosition(c * KICH_THUOC_O + lechTam, h * KICH_THUOC_O + lechTam);
                window.draw(buocChan);
            }
        }
    }
    // 3. Ve toan bo tuong me cung chong len tren (Chuan Vuong Vuc 100%)
    sf::RectangleShape tuongNgang(sf::Vector2f(KICH_THUOC_O, DO_DAY_TUONG));
    tuongNgang.setFillColor(sf::Color(245, 245, 250)); 
    
    sf::RectangleShape tuongDoc(sf::Vector2f(DO_DAY_TUONG, KICH_THUOC_O));
    tuongDoc.setFillColor(sf::Color(245, 245, 250)); 

    // =======================================================
    // [THU THUAT]: Tao "Tru be tong" 4x4 pixel de chot cac goc
    // =======================================================
    sf::RectangleShape truBeTong(sf::Vector2f(DO_DAY_TUONG, DO_DAY_TUONG));
    truBeTong.setFillColor(sf::Color(245, 245, 250)); // Cung mau voi tuong

    for (int h = 0; h < mc.soHang; h++) {
        for (int c = 0; c < mc.soCot; c++) {
            int x = c * KICH_THUOC_O;
            int y = h * KICH_THUOC_O;
            int tuong = mc.banDo[h][c].tuong;

            // Ve tuong ngang va doc
            if ((tuong & BAC) == BAC) { tuongNgang.setPosition(x, y); window.draw(tuongNgang); }
            if ((tuong & NAM) == NAM) { tuongNgang.setPosition(x, y + KICH_THUOC_O - DO_DAY_TUONG); window.draw(tuongNgang); }
            if ((tuong & TAY) == TAY) { tuongDoc.setPosition(x, y); window.draw(tuongDoc); }
            if ((tuong & DONG) == DONG) { tuongDoc.setPosition(x + KICH_THUOC_O - DO_DAY_TUONG, y); window.draw(tuongDoc); }
            
            // Do be tong vao 4 goc cua o vuong de xoa moi lo hong do hoa
            truBeTong.setPosition(x, y); window.draw(truBeTong);                                        // Goc Trai-Tren
            truBeTong.setPosition(x + KICH_THUOC_O - DO_DAY_TUONG, y); window.draw(truBeTong);          // Goc Phai-Tren
            truBeTong.setPosition(x, y + KICH_THUOC_O - DO_DAY_TUONG); window.draw(truBeTong);          // Goc Trai-Duoi
            truBeTong.setPosition(x + KICH_THUOC_O - DO_DAY_TUONG, y + KICH_THUOC_O - DO_DAY_TUONG); window.draw(truBeTong); // Goc Phai-Duoi
        }
    }
    window.display(); 
}


// ==========================================
// MODULE 2: THUAT TOAN TIM DUONG (DFS)
// ==========================================

bool timDuong(MeCung& mc, int h, int c, sf::RenderWindow& window) {
    sf::Event event;
    while (window.pollEvent(event)) { if (event.type == sf::Event::Closed) window.close(); }

    // Tien len 1 buoc
    mc.banDo[h][c].daDiQua = true; 
    mc.banDo[h][c].trangThaiDi = 1; 
    
    veToanBoMeCung(window, mc);
    sf::sleep(sf::milliseconds(50));        

    if (h == mc.hangRa && c == mc.cotRa) return true; 

    int tuong = mc.banDo[h][c].tuong;
    
    if ((tuong & BAC) != BAC && h > 0 && !mc.banDo[h-1][c].daDiQua)
        if (timDuong(mc, h-1, c, window)) return true;
        
    if ((tuong & DONG) != DONG && c < mc.soCot - 1 && !mc.banDo[h][c+1].daDiQua)
        if (timDuong(mc, h, c+1, window)) return true;
        
    if ((tuong & NAM) != NAM && h < mc.soHang - 1 && !mc.banDo[h+1][c].daDiQua)
        if (timDuong(mc, h+1, c, window)) return true;
        
    if ((tuong & TAY) != TAY && c > 0 && !mc.banDo[h][c-1].daDiQua)
        if (timDuong(mc, h, c-1, window)) return true;

    // Quay lui khi gap ngo cut
    mc.banDo[h][c].trangThaiDi = 2; 
    veToanBoMeCung(window, mc);
    sf::sleep(sf::milliseconds(50)); 
    
    return false;
}


// ==========================================
// HAM MAIN
// ==========================================
int main() {
    MeCung baiToan = taoMeCung();

    int chieuRong = baiToan.soCot * KICH_THUOC_O;
    int chieuCao = baiToan.soHang * KICH_THUOC_O;
    sf::RenderWindow window(sf::VideoMode(chieuRong, chieuCao), "Do an Me Cung CTDL&GT - SFML");

    timDuong(baiToan, baiToan.hangVao, baiToan.cotVao, window);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }
    
    return 0;
}

