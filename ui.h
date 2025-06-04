#ifndef UI_H
#define UI_H

#include "structs.h"
#include "patient.h"
#include "examination.h"
#include "department.h"
#include "doctor.h"
#include "medicine.h"
#include "prescription.h"
#include "priority_queue.h"
#include "csv_handler.h" 
#include "hash_table.h" 

// --- Khai bao cac ham giao dien ---

void hienThiMenuChinh();
/*
 * Ham hien thi menu chinh cua chuong trinh
 * Muc dich: In ra man hinh cac lua chon chinh cua he thong quan ly phong kham
 * Tham so dau vao: Khong co
 * Gia tri tra ve: Khong co (void)
 * Luu y: Hien thi menu voi cac lua chon: quan ly benh nhan, lan kham, bac si, thuoc...
 */

void xuLyLuaChonMenuChinh(
    int choice,
    BenhNhan** dsBenhNhan, int* soLuongBenhNhan, BangBam** bbBenhNhan_ptr, const char* fileBN,
    LanKham** dsLanKham, int* soLuongLanKham, BangBam** bbLanKham_ptr, const char* fileLK,
    KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, BangBam** bbKhoaPhong_ptr, const char* fileKP,
    BacSi** dsBacSi, int* soLuongBacSi, BangBam** bbBacSi_ptr, const char* fileBS,
    Thuoc** dsThuoc, int* soLuongThuoc, BangBam** bbThuoc_ptr, const char* fileT,
    DonThuocChiTiet** dsDonThuocChiTiet, int* soLuongDTCT, const char* fileDTCT,
    BangBam* bbHangDoiTheoKhoa
);
/*
 * Ham xu ly lua chon tu menu chinh
 * Muc dich: Dieu huong den cac chuc nang tuong ung theo lua chon cua nguoi dung
 * Tham so dau vao:
 *   - choice: lua chon cua nguoi dung tu menu chinh
 *   - dsBenhNhan: con tro kep toi mang benh nhan
 *   - soLuongBenhNhan: con tro toi bien so luong benh nhan
 *   - bbBenhNhan_ptr: con tro kep toi bang bam benh nhan
 *   - fileBN: ten file CSV benh nhan
 *   - dsLanKham: con tro kep toi mang lan kham
 *   - soLuongLanKham: con tro toi bien so luong lan kham
 *   - bbLanKham_ptr: con tro kep toi bang bam lan kham
 *   - fileLK: ten file CSV lan kham
 *   - dsKhoaPhong: con tro kep toi mang khoa phong
 *   - soLuongKhoaPhong: con tro toi bien so luong khoa phong
 *   - bbKhoaPhong_ptr: con tro kep toi bang bam khoa phong
 *   - fileKP: ten file CSV khoa phong
 *   - dsBacSi: con tro kep toi mang bac si
 *   - soLuongBacSi: con tro toi bien so luong bac si
 *   - bbBacSi_ptr: con tro kep toi bang bam bac si
 *   - fileBS: ten file CSV bac si
 *   - dsThuoc: con tro kep toi mang thuoc
 *   - soLuongThuoc: con tro toi bien so luong thuoc
 *   - bbThuoc_ptr: con tro kep toi bang bam thuoc
 *   - fileT: ten file CSV thuoc
 *   - dsDonThuocChiTiet: con tro kep toi mang don thuoc chi tiet
 *   - soLuongDTCT: con tro toi bien so luong don thuoc chi tiet
 *   - fileDTCT: ten file CSV don thuoc chi tiet
 *   - bbHangDoiTheoKhoa: bang bam hang doi theo khoa
 * Gia tri tra ve: Khong co (void)
 */

 // --- Cac ham menu con ---

void menuQuanLyBenhNhan(BenhNhan** dsBenhNhan, int* soLuongBenhNhan, BangBam** bbBenhNhan_ptr, const char* tenFileCSV);
/*
 * Ham hien thi va xu ly menu quan ly benh nhan
 * Muc dich: Cung cap giao dien quan ly benh nhan (them, sua, xoa, xem, tim kiem)
 * Tham so dau vao:
 *   - dsBenhNhan: con tro kep toi mang benh nhan
 *   - soLuongBenhNhan: con tro toi bien so luong benh nhan
 *   - bbBenhNhan_ptr: con tro kep toi bang bam benh nhan
 *   - tenFileCSV: ten file CSV benh nhan
 * Gia tri tra ve: Khong co (void)
 */

void menuQuanLyLanKham(
    LanKham** dsLanKham, int* soLuongLanKham, BangBam** bbLanKham_ptr,
    BenhNhan* dsBenhNhan, int soLuongBenhNhan, BangBam* bbBenhNhan,
    BacSi* dsBacSi, int soLuongBacSi, BangBam* bbBacSi,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong, BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV);
/*
 * Ham hien thi va xu ly menu quan ly lan kham
 * Muc dich: Cung cap giao dien quan ly lan kham (dang ky, cap nhat, xem, tim kiem)
 * Tham so dau vao:
 *   - dsLanKham: con tro kep toi mang lan kham
 *   - soLuongLanKham: con tro toi bien so luong lan kham
 *   - bbLanKham_ptr: con tro kep toi bang bam lan kham
 *   - dsBenhNhan: mang benh nhan
 *   - soLuongBenhNhan: so luong benh nhan
 *   - bbBenhNhan: bang bam benh nhan
 *   - dsBacSi: mang bac si
 *   - soLuongBacSi: so luong bac si
 *   - bbBacSi: bang bam bac si
 *   - dsKhoaPhong: mang khoa phong
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - bbKhoaPhong: bang bam khoa phong
 *   - fileLanKhamCSV: ten file CSV lan kham
 * Gia tri tra ve: Khong co (void)
 */

void menuQuanLyBacSi(
    BacSi** dsBacSi, int* soLuongBacSi,
    BangBam** bbBacSi_ptr,
    const char* tenFileCSV,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong, BangBam* bbKhoaPhong
);
/*
 * Ham hien thi va xu ly menu quan ly bac si
 * Muc dich: Cung cap giao dien quan ly bac si (them, sua, xoa, xem, tim kiem)
 * Tham so dau vao:
 *   - dsBacSi: con tro kep toi mang bac si
 *   - soLuongBacSi: con tro toi bien so luong bac si
 *   - bbBacSi_ptr: con tro kep toi bang bam bac si
 *   - tenFileCSV: ten file CSV bac si
 *   - dsKhoaPhong: mang khoa phong (de xac thuc ma khoa phong)
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - bbKhoaPhong: bang bam khoa phong
 * Gia tri tra ve: Khong co (void)
 */

void menuQuanLyThuoc(Thuoc** dsThuoc, int* soLuongThuoc, BangBam** bbThuoc_ptr, const char* tenFileCSV);
/*
 * Ham hien thi va xu ly menu quan ly thuoc
 * Muc dich: Cung cap giao dien quan ly thuoc (them, sua, xoa, xem, tim kiem)
 * Tham so dau vao:
 *   - dsThuoc: con tro kep toi mang thuoc
 *   - soLuongThuoc: con tro toi bien so luong thuoc
 *   - bbThuoc_ptr: con tro kep toi bang bam thuoc
 *   - tenFileCSV: ten file CSV thuoc
 * Gia tri tra ve: Khong co (void)
 */

void menuQuanLyKhoaPhong(KhoaPhong** dsKhoaPhong, int* soLuongKhoaPhong, BangBam** bbKhoaPhong_ptr, const char* tenFileCSV);
/*
 * Ham hien thi va xu ly menu quan ly khoa phong
 * Muc dich: Cung cap giao dien quan ly khoa phong (them, sua, xoa, xem, tim kiem)
 * Tham so dau vao:
 *   - dsKhoaPhong: con tro kep toi mang khoa phong
 *   - soLuongKhoaPhong: con tro toi bien so luong khoa phong
 *   - bbKhoaPhong_ptr: con tro kep toi bang bam khoa phong
 *   - tenFileCSV: ten file CSV khoa phong
 * Gia tri tra ve: Khong co (void)
 */

 // --- Cac ham tien ich giao dien ---

void xoaManHinhConsole();
/*
 * Ham xoa man hinh console
 * Muc dich: Xoa toan bo noi dung hien thi tren man hinh console
 * Tham so dau vao: Khong co
 * Gia tri tra ve: Khong co (void)
 * Luu y: Su dung system("cls") tren Windows hoac system("clear") tren Linux/Mac
 */

void dungManHinhCho();
/*
 * Ham dung man hinh cho nguoi dung nhan phim
 * Muc dich: Tam dung chuong trinh cho nguoi dung doc thong tin truoc khi tiep tuc
 * Tham so dau vao: Khong co
 * Gia tri tra ve: Khong co (void)
 * Luu y: Hien thi thong bao "Nhan Enter de tiep tuc..." va cho nguoi dung nhan phim
 */

int nhapSoNguyenCoGioiHan(const char* prompt, int min, int max);
/*
 * Ham nhap so nguyen voi gioi han gia tri
 * Muc dich: Nhap so nguyen tu ban phim voi kiem tra trong khoang cho phep
 * Tham so dau vao:
 *   - prompt: thong bao nhap lieu hien thi cho nguoi dung
 *   - min: gia tri toi thieu cho phep
 *   - max: gia tri toi da cho phep
 * Gia tri tra ve: So nguyen hop le trong khoang [min, max]
 * Luu y: Lap lai yeu cau nhap neu gia tri khong hop le
 */

int nhapSoNguyenKhongAm(const char* prompt);
/*
 * Ham nhap so nguyen khong am
 * Muc dich: Nhap so nguyen khong am tu ban phim voi kiem tra hop le
 * Tham so dau vao:
 *   - prompt: thong bao nhap lieu hien thi cho nguoi dung
 * Gia tri tra ve: So nguyen khong am (>= 0)
 * Luu y: Lap lai yeu cau nhap neu gia tri am hoac khong hop le
 */

void inThongDiepLoi(const char* message);
/*
 * Ham in thong diep loi
 * Muc dich: Hien thi thong bao loi voi dinh dang noi bat
 * Tham so dau vao:
 *   - message: noi dung thong bao loi
 * Gia tri tra ve: Khong co (void)
 * Luu y: Co the su dung mau sac hoac ky tu dac biet de lam noi bat thong bao loi
 */

void inThongDiepThanhCong(const char* message);
/*
 * Ham in thong diep thanh cong
 * Muc dich: Hien thi thong bao thanh cong voi dinh dang noi bat
 * Tham so dau vao:
 *   - message: noi dung thong bao thanh cong
 * Gia tri tra ve: Khong co (void)
 * Luu y: Co the su dung mau sac xanh hoac ky tu dac biet de lam noi bat thong bao thanh cong
 */

void hienThiThongTinBangBam(BangBam* bb, const char* tenBangBam);
/*
 * Ham hien thi thong tin bang bam
 * Muc dich: Hien thi cac thong so cua bang bam (kich thuoc, so phan tu, ty le tai...)
 * Tham so dau vao:
 *   - bb: con tro toi bang bam can hien thi thong tin
 *   - tenBangBam: ten mo ta cua bang bam
 * Gia tri tra ve: Khong co (void)
 * Luu y: Hien thi dung luong, so luong phan tu, he so tai va thong tin debug khac
 */

void menuXemThongTinBangBam(
    BangBam* bbBenhNhan,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    BangBam* bbThuoc,
    BangBam* bbLanKham
);
/*
 * Ham hien thi menu xem thong tin cac bang bam
 * Muc dich: Cung cap giao dien xem thong tin debug cua tat ca bang bam trong he thong
 * Tham so dau vao:
 *   - bbBenhNhan: bang bam benh nhan
 *   - bbBacSi: bang bam bac si
 *   - bbKhoaPhong: bang bam khoa phong
 *   - bbThuoc: bang bam thuoc
 *   - bbLanKham: bang bam lan kham
 * Gia tri tra ve: Khong co (void)
 * Luu y: Cho phep nguoi dung chon xem thong tin bang bam nao hoac xem tat ca
 */

#endif // UI_H