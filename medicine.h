#ifndef MEDICINE_H
#define MEDICINE_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_THUOC "data/thuoc.csv"

// --- Khai bao cac ham ---

Thuoc* docDanhSachThuoc(const char* tenFile, int* soLuong);
/*
 * Ham doc danh sach thuoc tu file CSV vao mot mang dong
 * Muc dich: Doc du lieu thuoc tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu thuoc
 *   - soLuong: con tro de luu tru so luong thuoc da doc
 * Gia tri tra ve: Con tro toi mang Thuoc, hoac NULL neu loi (nguoi goi giai phong bo nho)
 * Luu y: Cac truong maThuoc, tenThuoc phai co gia tri
 */

void ghiDanhSachThuocRaFile(const char* tenFile, const Thuoc dsThuoc[], int soLuong);
/*
 * Ham ghi danh sach thuoc (tu mang) vao file CSV (ghi de)
 * Muc dich: Luu toan bo danh sach thuoc vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsThuoc: mang chua danh sach thuoc
 *   - soLuong: so luong thuoc trong mang
 * Gia tri tra ve: Khong co (void)
 * Luu y: Dam bao cac truong khong rong khi ghi
 */

int themThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
    BangBam** bbThuoc_ptr,
    Thuoc thuocMoi, const char* tenFile);
/*
 * Ham them mot loai thuoc moi vao danh sach (trong bo nho, bang bam va file CSV)
 * Muc dich: Them thuoc moi vao mang dong va bang bam, cap nhat file
 * Tham so dau vao:
 *   - dsThuocConTro: con tro toi con tro mang Thuoc (de realloc)
 *   - soLuongConTro: con tro toi bien so luong thuoc
 *   - bbThuoc_ptr: con tro toi con tro BangBam Thuoc
 *   - thuocMoi: thong tin thuoc moi can them (ma thuoc phai duy nhat, cac truong khong duoc rong)
 *   - tenFile: ten file CSV de cap nhat
 * Gia tri tra ve: 1 neu them thanh cong, 0 neu that bai (vi du: trung ma, thong tin rong)
 */

int suaThongTinThuoc(Thuoc dsThuoc[], int soLuong, const char* maThuocCanSua, Thuoc thongTinMoi, const char* tenFile);
/*
 * Ham sua thong tin mot loai thuoc trong danh sach
 * Muc dich: Cap nhat thong tin thuoc theo ma thuoc (chi sua ten thuoc, ma thuoc khong doi)
 * Tham so dau vao:
 *   - dsThuoc: mang thuoc
 *   - soLuong: so luong thuoc
 *   - maThuocCanSua: ma thuoc can sua thong tin
 *   - thongTinMoi: thong tin ten thuoc moi (ma thuoc trong thongTinMoi se bi bo qua)
 *   - tenFile: ten file CSV
 * Gia tri tra ve: 1 neu sua thanh cong, 0 neu khong tim thay hoac loi
 * Luu y: Ten thuoc moi khong duoc rong
 */

int xoaThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
    BangBam* bbThuoc,
    const char* maThuocCanXoa, const char* tenFile);
/*
 * Ham xoa mot loai thuoc khoi danh sach va bang bam
 * Muc dich: Xoa thuoc khoi mang dong, bang bam va cap nhat file
 * Tham so dau vao:
 *   - dsThuocConTro: con tro toi con tro mang Thuoc
 *   - soLuongConTro: con tro toi bien so luong thuoc
 *   - bbThuoc: con tro toi BangBam Thuoc
 *   - maThuocCanXoa: ma thuoc can xoa
 *   - tenFile: ten file CSV
 * Gia tri tra ve: 1 neu xoa thanh cong, 0 neu khong tim thay hoac loi
 */

void inDanhSachThuocConsole(const Thuoc dsThuoc[], int soLuong);
/*
 * Ham in danh sach thuoc ra man hinh console
 * Muc dich: Hien thi toan bo thong tin thuoc duoi dang bang
 * Tham so dau vao:
 *   - dsThuoc: mang chua danh sach thuoc
 *   - soLuong: so luong thuoc can hien thi
 * Gia tri tra ve: Khong co (void)
 */

Thuoc* timThuocTheoMa(const Thuoc dsThuoc[], int soLuong, const char* maThuoc);
/*
 * Ham tim kiem thuoc theo ma (tim kiem tuyen tinh)
 * Muc dich: Tim thuoc cu the trong mang theo ma thuoc
 * Tham so dau vao:
 *   - dsThuoc: mang chua danh sach thuoc
 *   - soLuong: so luong thuoc trong mang
 *   - maThuoc: ma thuoc can tim
 * Gia tri tra ve: Con tro toi thuoc tim thay (NULL neu khong tim thay)
 */

Thuoc* timThuocTheoMaBangBam(BangBam* bbThuoc, const char* maThuoc);
/*
 * Ham tim kiem thuoc theo ma su dung bang bam
 * Muc dich: Tim thuoc nhanh chong thong qua bang bam (O(1))
 * Tham so dau vao:
 *   - bbThuoc: con tro toi BangBam Thuoc
 *   - maThuoc: ma thuoc can tim (se duoc chuan hoa ben trong ham)
 * Gia tri tra ve: Con tro toi Thuoc tim thay, hoac NULL neu khong tim thay
 */

int nhapThongTinThuocTuBanPhim(Thuoc* tConTro,
    BangBam* bbThuocHienCo,
    const Thuoc dsThuocHienCo[], int soLuongThuocHienCo);
/*
 * Ham nhap thong tin cho mot loai thuoc moi tu ban phim
 * Muc dich: Nhap thong tin thuoc moi tu ban phim voi kiem tra hop le
 * Tham so dau vao:
 *   - tConTro: con tro toi struct Thuoc de dien thong tin
 *   - bbThuocHienCo: con tro toi BangBam Thuoc hien co (de kiem tra trung ma)
 *   - dsThuocHienCo: mang cac loai thuoc hien co (de kiem tra trung ten)
 *   - soLuongThuocHienCo: so luong thuoc hien co
 * Gia tri tra ve: 1 neu nhap thanh cong, 0 neu ma/ten bi trung hoac nhap khong hop le
 * Luu y: Ma Thuoc, Ten Thuoc phai duoc nhap va khong rong. Ma Thuoc duy nhat
 */

void timKiemThuocTheoTenTuyenTinh(const Thuoc dsThuoc[], int soLuongThuoc,
    const char* tenThuocTimKiem,
    Thuoc** ketQuaTimKiem, int* soLuongKetQua);
/*
 * Ham tim kiem thuoc theo ten (tim kiem tuyen tinh)
 * Muc dich: Tim kiem thuoc theo ten thuoc (ho tro tim kiem gan dung)
 * Tham so dau vao:
 *   - dsThuoc: mang chua danh sach thuoc de tim kiem
 *   - soLuongThuoc: so luong thuoc trong danh sach
 *   - tenThuocTimKiem: ten thuoc can tim kiem
 *   - ketQuaTimKiem: con tro de tra ve mang ket qua tim duoc
 *   - soLuongKetQua: con tro de tra ve so luong ket qua
 * Gia tri tra ve: Khong co (void), ket qua tra qua tham so
 */

#endif // MEDICINE_H