#ifndef QLPK_STRUCTS_FULL_H
#define QLPK_STRUCTS_FULL_H

#include <time.h> // Can cho kieu time_t

// Dinh nghia do dai toi da cho cac ma ID dang chuoi
#define MAX_LEN_MA_DINH_DANH_BN 20 // do dai cho ma dinh danh benh nhan
#define MAX_LEN_MA_BAC_SI 20       // do dai cho ma bac si  
#define MAX_LEN_MA_KHOA_PHONG 20   // do dai cho ma khoa phong
#define MAX_LEN_MA_THUOC 30        // do dai cho ma thuoc
// Luu y: maLanKham, maDonThuoc hien tai dang la int, chua phai chuoi

// Enum Trang Thai Lan Kham
typedef enum {
    LK_DANG_CHO,       // benh nhan dang cho kham
    LK_DANG_KHAM,      // benh nhan dang duoc kham
    LK_DA_HOAN_THANH,  // da kham xong va co ket qua
    LK_DA_HUY,         // lan kham bi huy bo
    LK_DA_LO           // benh nhan khong den kham theo hen
} TrangThaiLanKham;

// --- Cau truc Date ---
typedef struct {
    int ngay;   // ngay trong thang (1-31)
    int thang;  // thang trong nam (1-12)
    int nam;    // nam (vi du: 1990, 2025)
} Date;

// --- Enum Muc Do Uu Tien ---
typedef enum {
    CAP_CUU = 0,        // truong hop cap cuu, uu tien cao nhat
    KHAN_CAP,           // truong hop khan cap
    THONG_THUONG,       // kham binh thuong
    TAI_KHAM,           // lich hen tai kham
    UU_TIEN_KHAC,       // cac loai uu tien khac
    SO_MUC_UU_TIEN      // tong so muc uu tien (de lap)
} MucDoUuTien;

// --- Enum Gioi Tinh ---
typedef enum {
    GT_NAM,  // gioi tinh nam
    GT_NU,   // gioi tinh nu
    GT_KHAC  // gioi tinh khac hoac khong xac dinh
} GioiTinh;

// === Cau truc Khoa/Phong Kham ===
typedef struct {
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG];  // ma dinh danh duy nhat cua khoa phong
    char tenKhoaPhong[100];                   // ten day du cua khoa phong
    char viTri[100];                          // vi tri cua khoa phong (tang, day...)
    char moTa[200];                           // mo ta chi tiet ve khoa phong va chuyen mon
} KhoaPhong;

// === Cau truc Bac Si ===
typedef struct {
    char maBacSi[MAX_LEN_MA_BAC_SI];          // ma dinh danh duy nhat cua bac si
    char tenBacSi[100];                       // ho va ten day du cua bac si
    char chuyenKhoa[100];                     // chuyen khoa cua bac si
    char soDienThoai[15];                     // so dien thoai lien lac
    char email[100];                          // dia chi email
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG];  // ma khoa phong ma bac si lam viec
} BacSi;

// === Cau truc Benh Nhan ===
typedef struct {
    char maDinhDanh[MAX_LEN_MA_DINH_DANH_BN]; // ma dinh danh duy nhat cua benh nhan
    char bhyt[16];                            // ma the bao hiem y te (15 chu so)
    char soDienThoai[15];                     // so dien thoai lien lac
    char ten[100];                            // ho va ten day du
    Date ngaySinh;                            // ngay thang nam sinh
    int tuoi;                                 // tuoi hien tai
    GioiTinh gioiTinh;                        // gioi tinh (enum)
    char diaChi[200];                         // dia chi cu tru
    char tieuSuBenhLy[1000];                  // tieu su benh ly va di ung
    time_t ngayTaoHoSo;                       // thoi diem tao ho so (timestamp)
} BenhNhan;

// === Cau truc Lan Kham ===
typedef struct {
    int maLanKham;                            // ma dinh danh duy nhat cua lan kham (so nguyen)
    char maDinhDanh[MAX_LEN_MA_DINH_DANH_BN]; // ma benh nhan (lien ket voi BenhNhan)
    char tenBenhNhan[100];                    // ten benh nhan (de hien thi nhanh)
    char maBacSi[MAX_LEN_MA_BAC_SI];          // ma bac si kham (lien ket voi BacSi)
    char maKhoaPhong[MAX_LEN_MA_KHOA_PHONG];  // ma khoa phong kham (lien ket voi KhoaPhong)
    time_t ngayGioKham;                       // thoi diem kham (timestamp)
    char lyDoKham[500];                       // ly do den kham benh
    char chanDoan[500];                       // ket qua chan doan cua bac si
    char phuongPhapDieuTri[1000];             // phuong phap dieu tri de xuat
    char ghiChuBacSi[500];                    // ghi chu bo sung cua bac si
    Date ngayTaiKham;                         // ngay hen tai kham (neu co)
    int coLichTaiKham;                        // co lich tai kham hay khong (1/0)
    MucDoUuTien mucDoUuTien;                  // muc do uu tien kham (enum)
    time_t gioDangKyThanhCong;                // thoi diem dang ky thanh cong
    int coDonThuoc;                           // co ke don thuoc hay khong (1/0)
    TrangThaiLanKham trangThai;               // trang thai hien tai (enum)
    int maDonThuocChiTiet;                    // ma don thuoc (lien ket voi DonThuocChiTiet)
} LanKham;

// === Cau truc Thuoc ===
typedef struct {
    char maThuoc[MAX_LEN_MA_THUOC]; // ma dinh danh duy nhat cua thuoc
    char tenThuoc[150];             // ten thuong mai hoac ten khoa hoc cua thuoc
} Thuoc;

// === Cau truc Chi Tiet Don Thuoc ===
typedef struct {
    int maDonThuocChiTiet;             // ma don thuoc (lien ket voi LanKham.maDonThuocChiTiet)
    char maThuoc[MAX_LEN_MA_THUOC];    // ma thuoc (lien ket voi Thuoc.maThuoc)
    int soLuong;                       // so luong thuoc ke don
    char lieuDung[100];                // lieu dung su dung (vi du: "500mg", "2 vien/lan")
    char cachDung[100];                // cach su dung thuoc (vi du: "Sau an 30 phut")
} DonThuocChiTiet;
// Luu y: Mot don thuoc co the co nhieu loai thuoc khac nhau
// Cap (maDonThuocChiTiet, maThuoc) co the la duy nhat

#endif // QLPK_STRUCTS_FULL_H