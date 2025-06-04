#ifndef EXAMINATION_H
#define EXAMINATION_H

#include "structs.h"
#include "patient.h" 
#include "hash_table.h" 
#include "priority_queue.h"

#define FILE_LAN_KHAM "data/lankham.csv"

// --- Khai bao cac ham ---

LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong);
/*
 * Ham doc danh sach cac lan kham tu file CSV
 * Muc dich: Doc du lieu lan kham tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu lan kham
 *   - soLuong: con tro de tra ve so luong lan kham doc duoc
 * Gia tri tra ve: Con tro toi mang lan kham (NULL neu loi)
 * Luu y: Cac truong bat buoc nhu maLanKham, maDinhDanh, maKhoaPhong phai hop le
 *        ChanDoan, PhuongPhapDieuTri khong duoc rong neu trangThai la DaHoanThanh
 */

void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong);
/*
 * Ham ghi danh sach lan kham vao file CSV
 * Muc dich: Luu toan bo danh sach lan kham vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsLanKham: mang chua danh sach lan kham
 *   - soLuong: so luong lan kham trong danh sach
 * Gia tri tra ve: Khong co (void)
 * Luu y: Dam bao cac truong bat buoc khong rong khi ghi
 */

int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong);
/*
 * Ham tao ma lan kham moi, duy nhat
 * Muc dich: Sinh ma lan kham tu dong, dam bao khong trung lap
 * Tham so dau vao:
 *   - dsLanKham: mang chua danh sach lan kham hien co
 *   - soLuong: so luong lan kham hien co
 * Gia tri tra ve: Ma lan kham moi (dang so nguyen)
 */

int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
    BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
    BangBam* bbBenhNhan,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong,
    BangBam* bbKhoaPhong,
    BangBam** bbLanKham_ptr,
    BangBam* bbHangDoiTheoKhoa,
    const char* fileLanKhamCSV, const char* fileBenhNhanCSV);
/*
 * Ham dang ky mot luot kham moi cho benh nhan
 * Muc dich: Tao lan kham moi, them vao he thong va hang doi uu tien
 * Tham so dau vao:
 *   - dsLanKhamConTro: con tro kep toi mang lan kham
 *   - soLuongLanKhamConTro: con tro toi bien so luong lan kham
 *   - dsBenhNhanConTro: con tro kep toi mang benh nhan
 *   - soLuongBenhNhanConTro: con tro toi bien so luong benh nhan
 *   - bbBenhNhan: bang bam benh nhan
 *   - dsKhoaPhong: mang khoa phong
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - bbKhoaPhong: bang bam khoa phong (de xac thuc khoa phong)
 *   - bbLanKham_ptr: con tro kep toi bang bam lan kham
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 *   - fileLanKhamCSV: file CSV lan kham
 *   - fileBenhNhanCSV: file CSV benh nhan
 * Gia tri tra ve: Ma lan kham moi (int) neu thanh cong, -1 neu that bai
 * Luu y: Yeu cau nhap maKhoaPhong
 */

int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV);
/*
 * Ham cap nhat thong tin mot lan kham
 * Muc dich: Sua doi thong tin lan kham da ton tai (trang thai, chan doan, dieu tri...)
 * Tham so dau vao:
 *   - dsLanKham: mang chua danh sach lan kham
 *   - soLuongLanKham: so luong lan kham trong danh sach
 *   - maLanKhamCanCapNhat: ma lan kham can cap nhat
 *   - dsBacSi: mang bac si
 *   - soLuongBacSi: so luong bac si
 *   - dsKhoaPhong: mang khoa phong
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - bbBacSi: bang bam bac si (de tim bac si bang bang bam)
 *   - bbKhoaPhong: bang bam khoa phong (de tim khoa phong bang bang bam)
 *   - fileLanKhamCSV: file CSV de luu thay doi
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 * Luu y: ChanDoan, PhuongPhapDieuTri khong duoc rong neu cap nhat trang thai thanh DaHoanThanh
 */

int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam** bbLanKham_ptr,
    LanKham lanKhamMoi, const char* tenFile);
/*
 * Ham them mot lan kham vao danh sach va bang bam
 * Muc dich: Them lan kham moi vao mang dong va bang bam, cap nhat file
 * Tham so dau vao:
 *   - dsLanKhamConTro: con tro kep toi mang lan kham
 *   - soLuongConTro: con tro toi bien so luong lan kham
 *   - bbLanKham_ptr: con tro kep toi bang bam lan kham
 *   - lanKhamMoi: thong tin lan kham moi can them
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam* bbLanKham,
    int maLanKhamCanXoa, const char* tenFile);
/*
 * Ham xoa mot lan kham khoi danh sach va bang bam
 * Muc dich: Xoa lan kham khoi mang dong, bang bam va cap nhat file
 * Tham so dau vao:
 *   - dsLanKhamConTro: con tro kep toi mang lan kham
 *   - soLuongConTro: con tro toi bien so luong lan kham
 *   - bbLanKham: con tro toi bang bam lan kham
 *   - maLanKhamCanXoa: ma lan kham can xoa
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham,
    const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
    BangBam* bbBenhNhan);
/*
 * Ham in danh sach lan kham ra man hinh console
 * Muc dich: Hien thi toan bo thong tin lan kham duoi dang bang
 * Tham so dau vao:
 *   - dsLanKham: mang chua danh sach lan kham
 *   - soLuongLanKham: so luong lan kham can hien thi
 *   - dsBenhNhan: mang benh nhan
 *   - soLuongBenhNhan: so luong benh nhan
 *   - bbBenhNhan: bang bam benh nhan (de lay ten BN hieu qua)
 * Gia tri tra ve: Khong co (void)
 */

void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
    const char* tieuChi, const char* giaTriTimKiem,
    LanKham** ketQuaTimKiem, int* soLuongKetQua);
/*
 * Ham tim kiem lan kham theo cac tieu chi (tim kiem tuyen tinh)
 * Muc dich: Tim kiem lan kham theo ma, ma benh nhan, ma bac si, ma khoa, trang thai...
 * Tham so dau vao:
 *   - dsLanKham: mang chua danh sach lan kham de tim kiem
 *   - soLuongLanKham: so luong lan kham trong danh sach
 *   - tieuChi: tieu chi tim kiem ("ma", "mabn", "mabs", "makhoa", "trangthai"...)
 *   - giaTriTimKiem: gia tri can tim kiem
 *   - ketQuaTimKiem: con tro de tra ve mang ket qua tim duoc
 *   - soLuongKetQua: con tro de tra ve so luong ket qua
 * Gia tri tra ve: Khong co (void), ket qua tra qua tham so
 */

LanKham* timLanKhamTheoMaBangBam(BangBam* bbLanKham, int maLK);
/*
 * Ham tim lan kham theo ma su dung bang bam
 * Muc dich: Tim lan kham nhanh chong thong qua bang bam
 * Tham so dau vao:
 *   - bbLanKham: bang bam Lan Kham
 *   - maLK: ma lan kham (int) can tim
 * Gia tri tra ve: Con tro toi LanKham neu tim thay, NULL neu khong
 */

int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV);
/*
 * Ham sua chi tiet mot lan kham
 * Muc dich: Cap nhat chi tiet cua lan kham (chan doan, dieu tri, ghi chu...)
 * Tham so dau vao:
 *   - dsLanKham: mang chua danh sach lan kham
 *   - soLuongLanKham: so luong lan kham trong danh sach
 *   - maLanKhamCanSua: ma lan kham can sua
 *   - dsBacSi: mang bac si
 *   - soLuongBacSi: so luong bac si
 *   - dsKhoaPhong: mang khoa phong
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - bbBacSi: bang bam bac si (de tim bac si bang bang bam)
 *   - bbKhoaPhong: bang bam khoa phong (de tim khoa phong bang bang bam)
 *   - fileLanKhamCSV: file CSV de luu thay doi
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 * Luu y: ChanDoan, PhuongPhapDieuTri khong duoc rong neu trang thai la DaHoanThanh
 */

const char* trangThaiLanKhamToString(TrangThaiLanKham tt);
/*
 * Ham chuyen doi enum trang thai lan kham thanh chuoi
 * Muc dich: Chuyen gia tri enum TrangThaiLanKham thanh chuoi de hien thi
 * Tham so dau vao:
 *   - tt: gia tri enum trang thai lan kham
 * Gia tri tra ve: Chuoi mo ta trang thai ("DangCho", "DangKham", "DaHoanThanh"...)
 */

TrangThaiLanKham stringToTrangThaiLanKham(const char* str);
/*
 * Ham chuyen doi chuoi thanh enum trang thai lan kham
 * Muc dich: Chuyen chuoi nhap vao thanh gia tri enum de luu tru
 * Tham so dau vao:
 *   - str: chuoi mo ta trang thai (khong phan biet hoa thuong)
 * Gia tri tra ve: Gia tri enum tuong ung
 */

const char* mucDoUuTienToString(MucDoUuTien md);
/*
 * Ham chuyen doi enum muc do uu tien thanh chuoi
 * Muc dich: Chuyen gia tri enum MucDoUuTien thanh chuoi de hien thi
 * Tham so dau vao:
 *   - md: gia tri enum muc do uu tien
 * Gia tri tra ve: Chuoi mo ta muc do uu tien ("CapCuu", "KhanCap", "ThongThuong"...)
 */

MucDoUuTien stringToMucDoUuTien(const char* str);
/*
 * Ham chuyen doi chuoi thanh enum muc do uu tien
 * Muc dich: Chuyen chuoi nhap vao thanh gia tri enum de luu tru
 * Tham so dau vao:
 *   - str: chuoi mo ta muc do uu tien (khong phan biet hoa thuong)
 * Gia tri tra ve: Gia tri enum tuong ung
 */

#endif // EXAMINATION_H 