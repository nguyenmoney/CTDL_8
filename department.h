#ifndef DEPARTMENT_H
#define DEPARTMENT_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_KHOA_PHONG "data/khoaphong.csv"

// --- Khai bao cac ham ---

KhoaPhong* docDanhSachKhoaPhong(const char* tenFile, int* soLuong);
/*
 * Ham doc danh sach khoa phong tu file CSV
 * Muc dich: Doc du lieu khoa phong tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu khoa phong
 *   - soLuong: con tro de tra ve so luong khoa phong doc duoc
 * Gia tri tra ve: Con tro toi mang khoa phong (NULL neu loi)
 */

void ghiDanhSachKhoaPhongRaFile(const char* tenFile, const KhoaPhong dsKhoaPhong[], int soLuong);
/*
 * Ham ghi danh sach khoa phong ra file CSV
 * Muc dich: Luu toan bo danh sach khoa phong vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsKhoaPhong: mang chua danh sach khoa phong
 *   - soLuong: so luong khoa phong trong danh sach
 * Gia tri tra ve: Khong co (void)
 */

int themKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam** bbKhoaPhong_ptr,
    KhoaPhong khoaPhongMoi, const char* tenFile);
/*
 * Ham them khoa phong moi vao he thong
 * Muc dich: Them khoa phong moi vao mang dong va bang bam, cap nhat file
 * Tham so dau vao:
 *   - dsKhoaPhongConTro: con tro kep toi mang khoa phong (de co the thay doi kich thuoc)
 *   - soLuongConTro: con tro toi bien so luong khoa phong
 *   - bbKhoaPhong_ptr: con tro kep toi bang bam khoa phong
 *   - khoaPhongMoi: thong tin khoa phong moi can them
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

int suaThongTinKhoaPhong(KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhongCanSua, KhoaPhong thongTinMoi, const char* tenFile);
/*
 * Ham sua thong tin khoa phong da ton tai
 * Muc dich: Cap nhat thong tin cua khoa phong theo ma khoa phong
 * Tham so dau vao:
 *   - dsKhoaPhong: mang chua danh sach khoa phong
 *   - soLuong: so luong khoa phong trong danh sach
 *   - maKhoaPhongCanSua: ma khoa phong can sua
 *   - thongTinMoi: thong tin moi de cap nhat
 *   - tenFile: ten file de luu thay doi
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 */

int xoaKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam* bbKhoaPhong,
    const char* maKhoaPhongCanXoa, const char* tenFile);
/*
 * Ham xoa khoa phong khoi he thong
 * Muc dich: Xoa khoa phong khoi mang dong, bang bam va cap nhat file
 * Tham so dau vao:
 *   - dsKhoaPhongConTro: con tro kep toi mang khoa phong
 *   - soLuongConTro: con tro toi bien so luong khoa phong
 *   - bbKhoaPhong: con tro toi bang bam khoa phong
 *   - maKhoaPhongCanXoa: ma khoa phong can xoa
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 */

void inDanhSachKhoaPhongConsole(const KhoaPhong dsKhoaPhong[], int soLuong);
/*
 * Ham in danh sach khoa phong ra man hinh console
 * Muc dich: Hien thi toan bo thong tin khoa phong duoi dang bang
 * Tham so dau vao:
 *   - dsKhoaPhong: mang chua danh sach khoa phong
 *   - soLuong: so luong khoa phong can hien thi
 * Gia tri tra ve: Khong co (void)
 */

KhoaPhong* timKhoaPhongTheoMa(const KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhong);
/*
 * Ham tim khoa phong theo ma bang tim kiem tuyen tinh
 * Muc dich: Tim khoa phong cu the trong mang theo ma khoa phong
 * Tham so dau vao:
 *   - dsKhoaPhong: mang chua danh sach khoa phong
 *   - soLuong: so luong khoa phong trong mang
 *   - maKhoaPhong: ma khoa phong can tim
 * Gia tri tra ve: Con tro toi khoa phong tim thay (NULL neu khong tim thay)
 */

KhoaPhong* timKhoaPhongTheoMaBangBam(BangBam* bbKhoaPhong, const char* maKhoaPhong);
/*
 * Ham tim khoa phong theo ma bang bang bam (O(1))
 * Muc dich: Tim khoa phong nhanh chong thong qua bang bam
 * Tham so dau vao:
 *   - bbKhoaPhong: con tro toi bang bam khoa phong
 *   - maKhoaPhong: ma khoa phong can tim
 * Gia tri tra ve: Con tro toi khoa phong tim thay (NULL neu khong tim thay)
 */

int nhapThongTinKhoaPhongTuBanPhim(KhoaPhong* kp,
    BangBam* bbKhoaPhongHienCo,
    const KhoaPhong dsKhoaPhongHienCo[], int soLuongHienCo);
/*
 * Ham nhap thong tin khoa phong tu ban phim
 * Muc dich: Nhap thong tin khoa phong moi tu ban phim voi kiem tra hop le
 * Tham so dau vao:
 *   - kp: con tro toi cau truc khoa phong de luu thong tin nhap vao
 *   - bbKhoaPhongHienCo: bang bam khoa phong hien co (de kiem tra trung lap)
 *   - dsKhoaPhongHienCo: mang khoa phong hien co (de kiem tra trung lap)
 *   - soLuongHienCo: so luong khoa phong hien co
 * Gia tri tra ve: 1 neu nhap thanh cong, 0 neu co loi hoac huy
 */

void timKiemKhoaPhongTuyenTinh(const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tieuChi, const char* giaTriTimKiem,
    KhoaPhong** ketQuaTimKiem, int* soLuongKetQua);
/*
 * Ham tim kiem khoa phong theo nhieu tieu chi
 * Muc dich: Tim kiem khoa phong theo ma, ten khoa phong, vi tri hoac mo ta
 * Tham so dau vao:
 *   - dsKhoaPhong: mang chua danh sach khoa phong de tim kiem
 *   - soLuongKhoaPhong: so luong khoa phong trong danh sach
 *   - tieuChi: tieu chi tim kiem ("ma", "ten", "vitri", "mota")
 *   - giaTriTimKiem: gia tri can tim kiem
 *   - ketQuaTimKiem: con tro de tra ve mang ket qua tim duoc
 *   - soLuongKetQua: con tro de tra ve so luong ket qua
 * Gia tri tra ve: Khong co (void), ket qua tra qua tham so
 */

#endif // DEPARTMENT_H