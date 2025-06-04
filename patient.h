#ifndef PATIENT_H
#define PATIENT_H

#include "structs.h"
#include "hash_table.h"
#include <stdio.h>

#define FILE_BENH_NHAN "data/benhnhan.csv"

// ========== HAM DOC VA GHI FILE ==========

BenhNhan* docDanhSachBenhNhan(const char* tenFile, int* soLuongBenhNhan);
/*
 * Ham doc danh sach benh nhan tu file CSV
 * Muc dich: Doc du lieu benh nhan tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu benh nhan
 *   - soLuongBenhNhan: con tro de tra ve so luong benh nhan doc duoc
 * Gia tri tra ve: Con tro toi mang benh nhan (NULL neu loi)
 */

void ghiDanhSachBenhNhanRaFile(const char* tenFile, const BenhNhan dsBenhNhan[], int soLuongBenhNhan);
/*
 * Ham ghi danh sach benh nhan ra file CSV
 * Muc dich: Luu toan bo danh sach benh nhan vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsBenhNhan: mang chua danh sach benh nhan
 *   - soLuongBenhNhan: so luong benh nhan trong danh sach
 * Gia tri tra ve: Khong co (void)
 */

 // ========== HAM QUAN LY BENH NHAN ==========

int themBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan,
    BangBam** bbBenhNhan_ptr,
    BenhNhan benhNhanMoi, const char* tenFile);
/*
 * Ham them benh nhan moi vao he thong
 * Muc dich: Them benh nhan moi vao mang dong va bang bam, cap nhat file
 * Tham so dau vao:
 *   - dsBenhNhanConTro: con tro kep toi mang benh nhan (de co the thay doi kich thuoc)
 *   - soLuongBenhNhan: con tro toi bien so luong benh nhan
 *   - bbBenhNhan_ptr: con tro kep toi bang bam benh nhan
 *   - benhNhanMoi: thong tin benh nhan moi can them
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

int suaThongTinBenhNhan(BenhNhan dsBenhNhan[], int soLuongBenhNhan, char* maDinhDanhCanSua, BenhNhan thongTinMoi, const char* tenFile);
/*
 * Ham sua thong tin benh nhan da ton tai
 * Muc dich: Cap nhat thong tin cua benh nhan theo ma dinh danh
 * Tham so dau vao:
 *   - dsBenhNhan: mang chua danh sach benh nhan
 *   - soLuongBenhNhan: so luong benh nhan trong danh sach
 *   - maDinhDanhCanSua: ma dinh danh cua benh nhan can sua
 *   - thongTinMoi: thong tin moi de cap nhat
 *   - tenFile: ten file de luu thay doi
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 */

int xoaBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, BangBam* bbBenhNhan, char* maDinhDanhCanXoa, const char* tenFile);
/*
 * Ham xoa benh nhan khoi he thong
 * Muc dich: Xoa benh nhan khoi mang dong, bang bam va cap nhat file
 * Tham so dau vao:
 *   - dsBenhNhanConTro: con tro kep toi mang benh nhan
 *   - soLuongBenhNhan: con tro toi bien so luong benh nhan
 *   - bbBenhNhan: con tro toi bang bam benh nhan
 *   - maDinhDanhCanXoa: ma dinh danh cua benh nhan can xoa
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu khong tim thay
 */

 // ========== HAM HIEN THI VA TIM KIEM ==========

void inDanhSachBenhNhanConsole(const BenhNhan dsBenhNhan[], int soLuongBenhNhan);
/*
 * Ham in danh sach benh nhan ra man hinh console
 * Muc dich: Hien thi toan bo thong tin benh nhan duoi dang bang
 * Tham so dau vao:
 *   - dsBenhNhan: mang chua danh sach benh nhan
 *   - soLuongBenhNhan: so luong benh nhan can hien thi
 * Gia tri tra ve: Khong co (void)
 */

void timKiemBenhNhan(const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
    const char* tieuChi, const char* giaTriTimKiem,
    BenhNhan** ketQuaTimKiem, int* soLuongKetQua);
/*
 * Ham tim kiem benh nhan theo nhieu tieu chi
 * Muc dich: Tim kiem benh nhan theo ma dinh danh, BHYT, SDT hoac ten
 * Tham so dau vao:
 *   - dsBenhNhan: mang chua danh sach benh nhan de tim kiem
 *   - soLuongBenhNhan: so luong benh nhan trong danh sach
 *   - tieuChi: tieu chi tim kiem ("madinhdanh", "bhyt", "sodienthoai", "ten")
 *   - giaTriTimKiem: gia tri can tim kiem
 *   - ketQuaTimKiem: con tro de tra ve mang ket qua tim duoc
 *   - soLuongKetQua: con tro de tra ve so luong ket qua
 * Gia tri tra ve: Khong co (void), ket qua tra qua tham so
 */

 // ========== HAM NHAP DU LIEU ==========

int nhapThongTinBenhNhanTuBanPhim(BenhNhan* bn, const BenhNhan dsBenhNhanHienCo[], int soLuongHienCo, BangBam* bbBenhNhan);
/*
 * Ham nhap thong tin benh nhan tu ban phim
 * Muc dich: Nhap thong tin benh nhan moi tu ban phim voi kiem tra hop le va trung lap
 * Tham so dau vao:
 *   - bn: con tro toi cau truc benh nhan de luu thong tin nhap vao
 *   - dsBenhNhanHienCo: mang benh nhan hien co (de kiem tra trung lap BHYT, SDT)
 *   - soLuongHienCo: so luong benh nhan hien co trong mang
 *   - bbBenhNhan: bang bam benh nhan (de kiem tra trung lap ma dinh danh nhanh)
 * Gia tri tra ve: 1 neu nhap thanh cong, 0 neu co loi hoac huy
 * Luu y: Kiem tra trung lap ma dinh danh, BHYT, so dien thoai
 *        Yeu cau nhap day du cac truong bat buoc (ma, ten, ngay sinh, tuoi)
 *        Tu dong gan thoi gian tao ho so
 */

 // ========== HAM TIM KIEM NHANH ==========

const BenhNhan* timBenhNhanTheoMaChuoi(const BenhNhan dsBenhNhan[], int soLuong, const char* maDD_str);
/*
 * Ham tim benh nhan theo ma dinh danh bang tim kiem tuyen tinh
 * Muc dich: Tim benh nhan cu the trong mang theo ma dinh danh
 * Tham so dau vao:
 *   - dsBenhNhan: mang chua danh sach benh nhan
 *   - soLuong: so luong benh nhan trong mang
 *   - maDD_str: ma dinh danh can tim (dang chuoi)
 * Gia tri tra ve: Con tro toi benh nhan tim thay (NULL neu khong tim thay)
 */

BenhNhan* timBenhNhanTheoMaBangBam(BangBam* bb, const char* maDD_str);
/*
 * Ham tim benh nhan theo ma dinh danh bang bang bam (O(1))
 * Muc dich: Tim benh nhan nhanh chong thong qua bang bam
 * Tham so dau vao:
 *   - bb: con tro toi bang bam benh nhan
 *   - maDD_str: ma dinh danh can tim (dang chuoi)
 * Gia tri tra ve: Con tro toi benh nhan tim thay (NULL neu khong tim thay)
 */

 // ========== HAM CHUYEN DOI DU LIEU ==========

const char* gioiTinhToString(GioiTinh gt);
/*
 * Ham chuyen doi enum gioi tinh thanh chuoi
 * Muc dich: Chuyen gia tri enum GioiTinh thanh chuoi de hien thi
 * Tham so dau vao:
 *   - gt: gia tri enum gioi tinh (GT_NAM, GT_NU, GT_KHAC)
 * Gia tri tra ve: Chuoi mo ta gioi tinh ("Nam", "Nu", "Khac")
 */

GioiTinh stringToGioiTinh(const char* str);
/*
 * Ham chuyen doi chuoi thanh enum gioi tinh
 * Muc dich: Chuyen chuoi nhap vao thanh gia tri enum de luu tru
 * Tham so dau vao:
 *   - str: chuoi mo ta gioi tinh ("nam", "nu", "khac" - khong phan biet hoa thuong)
 * Gia tri tra ve: Gia tri enum tuong ung (GT_KHAC neu khong nhan ra)
 */

#endif // PATIENT_H