#ifndef DOCTOR_H
#define DOCTOR_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_BAC_SI "data/bacsi.csv"

BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong);
/*
 * Ham doc danh sach bac si tu file CSV
 * Muc dich: Doc du lieu bac si tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu bac si
 *   - soLuong: con tro de tra ve so luong bac si doc duoc
 * Gia tri tra ve: Con tro toi mang bac si (NULL neu loi)
 */

void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong);
/*
 * Ham ghi danh sach bac si ra file CSV
 * Muc dich: Luu toan bo danh sach bac si vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsBacSi: mang chua danh sach bac si
 *   - soLuong: so luong bac si trong danh sach
 * Gia tri tra ve: Khong co (void)
 */

int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam** bbBacSi_ptr,
    BacSi bacSiMoi, const char* tenFile);
/*
 * Ham them bac si moi vao he thong
 * Muc dich: Them bac si moi vao mang dong va bang bam, cap nhat file
 * Tham so dau vao:
 *   - dsBacSiConTro: con tro kep toi mang bac si (de co the thay doi kich thuoc)
 *   - soLuongConTro: con tro toi bien so luong bac si
 *   - bbBacSi_ptr: con tro kep toi bang bam bac si
 *   - bacSiMoi: thong tin bac si moi can them
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tenFile);
/*
 * Ham sua thong tin bac si hien co
 * Muc dich: Cap nhat thong tin cua bac si theo ma bac si
 * Tham so dau vao:
 *   - dsBacSi: mang chua danh sach bac si
 *   - soLuong: so luong bac si trong danh sach
 *   - maBacSiCanSua: ma bac si can sua
 *   - thongTinMoi: thong tin moi de cap nhat
 *   - dsKhoaPhong: mang khoa phong (de xac thuc ma khoa phong moi)
 *   - soLuongKhoaPhong: so luong khoa phong
 *   - tenFile: ten file de luu thay doi
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 * Luu y: Neu ma bac si (khoa) la bat bien, khong can thay doi truc tiep bang bam
 *        Con tro trong bang bam se van tro den struct da sua doi trong mang
 */

int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam* bbBacSi,
    const char* maBacSiCanXoa, const char* tenFile);
/*
 * Ham xoa bac si khoi he thong
 * Muc dich: Xoa bac si khoi mang dong, bang bam va cap nhat file
 * Tham so dau vao:
 *   - dsBacSiConTro: con tro kep toi mang bac si
 *   - soLuongConTro: con tro toi bien so luong bac si
 *   - bbBacSi: con tro toi bang bam bac si
 *   - maBacSiCanXoa: ma bac si can xoa
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 */

void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong);
/*
 * Ham in danh sach bac si ra man hinh console
 * Muc dich: Hien thi toan bo thong tin bac si duoi dang bang
 * Tham so dau vao:
 *   - dsBacSi: mang chua danh sach bac si
 *   - soLuong: so luong bac si can hien thi
 * Gia tri tra ve: Khong co (void)
 */

int nhapThongTinBacSiTuBanPhim(BacSi* bs,
    BangBam* bbBacSiHienCo,
    const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong);
/*
 * Ham nhap thong tin bac si tu ban phim
 * Muc dich: Nhap thong tin bac si moi tu ban phim voi kiem tra hop le
 * Tham so dau vao:
 *   - bs: con tro toi cau truc bac si de luu thong tin nhap vao
 *   - bbBacSiHienCo: bang bam bac si hien co (de kiem tra trung lap ma bac si)
 *   - dsBacSiHienCo: mang bac si hien co (de kiem tra trung lap cac truong khong phai khoa)
 *   - soLuongBSHienCo: so luong bac si hien co
 *   - dsKhoaPhong: mang khoa phong (de xac thuc ma khoa phong)
 *   - soLuongKhoaPhong: so luong khoa phong
 * Gia tri tra ve: 1 neu nhap thanh cong, 0 neu co loi hoac huy
 */

BacSi* timBacSiTheoMaBangBam(BangBam* bbBacSi, const char* maBacSi);
/*
 * Ham tim bac si theo ma su dung bang bam (O(1))
 * Muc dich: Tim bac si nhanh chong thong qua bang bam
 * Tham so dau vao:
 *   - bbBacSi: con tro toi bang bam bac si
 *   - maBacSi: ma bac si can tim
 * Gia tri tra ve: Con tro toi bac si tim thay (NULL neu khong tim thay)
 */

void timKiemBacSiTuyenTinh(const BacSi dsBacSi[], int soLuongBacSi,
    const char* tieuChi, const char* giaTriTimKiem,
    BacSi** ketQuaTimKiem, int* soLuongKetQua);
/*
 * Ham tim kiem bac si theo nhieu tieu chi
 * Muc dich: Tim kiem bac si theo ma, ten, chuyen khoa, SDT, email hoac ma khoa phong
 * Tham so dau vao:
 *   - dsBacSi: mang chua danh sach bac si de tim kiem
 *   - soLuongBacSi: so luong bac si trong danh sach
 *   - tieuChi: tieu chi tim kiem ("ma", "ten", "chuyenkhoa", "sdt", "email", "makhoa")
 *   - giaTriTimKiem: gia tri can tim kiem
 *   - ketQuaTimKiem: con tro de tra ve mang ket qua tim duoc
 *   - soLuongKetQua: con tro de tra ve so luong ket qua
 * Gia tri tra ve: Khong co (void), ket qua tra qua tham so
 */

BacSi* timBacSiTheoSDT(const BacSi dsBacSi[], int soLuong, const char* soDienThoai);
/*
 * Ham tim bac si theo so dien thoai cu the
 * Muc dich: Tim bac si trong mang theo so dien thoai (kiem tra don le)
 * Tham so dau vao:
 *   - dsBacSi: mang chua danh sach bac si
 *   - soLuong: so luong bac si trong mang
 *   - soDienThoai: so dien thoai can tim
 * Gia tri tra ve: Con tro toi bac si tim thay (NULL neu khong tim thay)
 */

BacSi* timBacSiTheoEmail(const BacSi dsBacSi[], int soLuong, const char* email);
/*
 * Ham tim bac si theo email cu the
 * Muc dich: Tim bac si trong mang theo email (kiem tra don le)
 * Tham so dau vao:
 *   - dsBacSi: mang chua danh sach bac si
 *   - soLuong: so luong bac si trong mang
 *   - email: email can tim
 * Gia tri tra ve: Con tro toi bac si tim thay (NULL neu khong tim thay)
 * Luu y: Cac ham tim kiem cu the nay co the su dung tu UI neu can kiem tra don le,
 *        nhung ham timKiemBacSiTuyenTinh o tren se tong quat hon
 */

#endif // DOCTOR_H