#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include "structs.h"
#include "hash_table.h"

#define FILE_DON_THUOC_CHI_TIET "data/donthuoc.csv" 

// --- Khai bao cac ham ---

DonThuocChiTiet* docDanhSachDonThuocChiTiet(const char* tenFile, int* soLuongDongConTro);
/*
 * Ham doc danh sach don thuoc chi tiet tu file CSV
 * Muc dich: Doc du lieu don thuoc chi tiet tu file va tao danh sach dong
 * Tham so dau vao:
 *   - tenFile: ten file CSV chua du lieu don thuoc chi tiet
 *   - soLuongDongConTro: con tro de tra ve so luong dong doc duoc
 * Gia tri tra ve: Con tro toi mang don thuoc chi tiet (NULL neu loi)
 */

void ghiDanhSachDonThuocChiTietRaFile(const char* tenFile, const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong);
/*
 * Ham ghi danh sach don thuoc chi tiet ra file CSV
 * Muc dich: Luu toan bo danh sach don thuoc chi tiet vao file de luu tru lau dai
 * Tham so dau vao:
 *   - tenFile: ten file CSV de ghi du lieu
 *   - dsDonThuocChiTiet: mang chua danh sach don thuoc chi tiet
 *   - soLuongDong: so luong dong trong danh sach
 * Gia tri tra ve: Khong co (void)
 */

int themDongDonThuocChiTiet(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, DonThuocChiTiet dtctMoi, const char* tenFile);
/*
 * Ham them dong don thuoc chi tiet moi
 * Muc dich: Them dong thuoc moi vao don thuoc chi tiet, cap nhat file
 * Tham so dau vao:
 *   - dsDTCTConTro: con tro kep toi mang don thuoc chi tiet
 *   - soLuongDongConTro: con tro toi bien so luong dong
 *   - dtctMoi: thong tin dong don thuoc chi tiet moi
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 */

int xoaDongThuocTrongDon(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, int maDonThuocCanTim, const char* maThuocCanXoa, const char* tenFile);
/*
 * Ham xoa dong thuoc cu the trong don thuoc
 * Muc dich: Xoa mot loai thuoc cu the ra khoi don thuoc theo ma don va ma thuoc
 * Tham so dau vao:
 *   - dsDTCTConTro: con tro kep toi mang don thuoc chi tiet
 *   - soLuongDongConTro: con tro toi bien so luong dong
 *   - maDonThuocCanTim: ma don thuoc chua dong can xoa
 *   - maThuocCanXoa: ma thuoc can xoa khoi don
 *   - tenFile: ten file de cap nhat du lieu
 * Gia tri tra ve: 1 neu xoa thanh cong, 0 neu khong tim thay
 */

int suaDongThuocTrongDon(DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong, int maDonThuocCanTim, const char* maThuocCanSua, DonThuocChiTiet thongTinMoi, const char* tenFile);
/*
 * Ham sua dong thuoc trong don thuoc
 * Muc dich: Cap nhat thong tin dong thuoc (so luong, lieu dung, cach dung)
 * Tham so dau vao:
 *   - dsDonThuocChiTiet: mang don thuoc chi tiet
 *   - soLuongDong: so luong dong trong danh sach
 *   - maDonThuocCanTim: ma don thuoc chua dong can sua
 *   - maThuocCanSua: ma thuoc can sua thong tin
 *   - thongTinMoi: thong tin moi de cap nhat
 *   - tenFile: ten file de luu thay doi
 * Gia tri tra ve: 1 neu sua thanh cong, 0 neu khong tim thay
 */

void inChiTietMotDonThuocConsole(int maDonThuoc,
    const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDongTongCong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB);
/*
 * Ham in chi tiet mot don thuoc cu the ra console
 * Muc dich: Hien thi tat ca thuoc trong mot don thuoc cu the
 * Tham so dau vao:
 *   - maDonThuoc: ma don thuoc can hien thi
 *   - dsDonThuocChiTiet: mang don thuoc chi tiet
 *   - soLuongDongTongCong: tong so dong trong danh sach
 *   - dsThuocToanBo: mang tat ca thuoc (de lay ten thuoc)
 *   - soLuongThuocTB: so luong thuoc toan bo
 * Gia tri tra ve: Khong co (void)
 */

void inTatCaDonThuocChiTietConsole(const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB);
/*
 * Ham in tat ca don thuoc chi tiet ra console
 * Muc dich: Hien thi toan bo danh sach don thuoc chi tiet duoi dang bang
 * Tham so dau vao:
 *   - dsDonThuocChiTiet: mang don thuoc chi tiet
 *   - soLuongDong: so luong dong trong danh sach
 *   - dsThuocToanBo: mang tat ca thuoc (de lay ten thuoc)
 *   - soLuongThuocTB: so luong thuoc toan bo
 * Gia tri tra ve: Khong co (void)
 */

void quanLyCapNhatChiTietDonThuoc(int maDonThuocDeCapNhat,
    DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB,
    BangBam* bbThuoc, const char* tenFileCSV);
/*
 * Ham quan ly cap nhat chi tiet don thuoc (giao dien tong hop)
 * Muc dich: Cung cap giao dien quan ly don thuoc (them, sua, xoa thuoc trong don)
 * Tham so dau vao:
 *   - maDonThuocDeCapNhat: ma don thuoc can quan ly
 *   - dsDTCTConTro: con tro kep toi mang don thuoc chi tiet
 *   - soLuongDongConTro: con tro toi bien so luong dong
 *   - dsThuocToanBo: mang tat ca thuoc (de chon thuoc)
 *   - soLuongThuocTB: so luong thuoc toan bo
 *   - bbThuoc: bang bam thuoc (de tim thuoc nhanh)
 *   - tenFileCSV: ten file de cap nhat du lieu
 * Gia tri tra ve: Khong co (void)
 * Luu y: Ham nay cung cap menu tuong tac cho viec quan ly don thuoc
 */

#endif // PRESCRIPTION_H