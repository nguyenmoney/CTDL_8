#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "structs.h"    
#include "hash_table.h" 
#include <stdio.h>

typedef struct {
    LanKham* dsChoKham;
    int soLuongHienTai;
    int dungLuong;
} HangDoiUuTienMotKhoa;

// --- Khai bao cac ham  ---

void khoiTaoHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk);
/*
 * Ham khoi tao hang doi uu tien cho mot khoa
 * Muc dich: Khoi tao cau truc hang doi uu tien rong cho mot khoa cu the
 * Tham so dau vao:
 *   - hqmk: con tro toi cau truc hang doi uu tien mot khoa
 * Gia tri tra ve: Khong co (void)
 * Luu y: Dat cac gia tri ban dau: dsChoKham = NULL, soLuongHienTai = 0, dungLuong = 0
 */

int themVaoHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham lanKhamMoi);
/*
 * Ham them lan kham vao hang doi uu tien theo khoa
 * Muc dich: Them benh nhan vao hang doi cho theo muc do uu tien va khoa phong
 * Tham so dau vao:
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 *   - maKhoaPhong: ma khoa phong can them vao hang doi
 *   - lanKhamMoi: thong tin lan kham moi can them vao hang doi
 * Gia tri tra ve: 1 neu them thanh cong, 0 neu that bai
 * Luu y: Tu dong sap xep theo muc do uu tien (cap cuu > khan cap > thong thuong...)
 */

int layBenhNhanTiepTheoTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham* lanKhamLayRa);
/*
 * Ham lay benh nhan tiep theo tu hang doi uu tien theo khoa
 * Muc dich: Lay benh nhan co muc do uu tien cao nhat ra khoi hang doi
 * Tham so dau vao:
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 *   - maKhoaPhong: ma khoa phong can lay benh nhan
 *   - lanKhamLayRa: con tro de luu thong tin lan kham lay ra
 * Gia tri tra ve: 1 neu lay thanh cong, 0 neu hang doi rong hoac loi
 * Luu y: Benh nhan duoc lay ra se bi xoa khoi hang doi
 */

int laHangDoiTheoKhoaRong(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong);
/*
 * Ham kiem tra hang doi cua mot khoa co rong hay khong
 * Muc dich: Kiem tra xem con benh nhan nao dang cho kham tai khoa nay khong
 * Tham so dau vao:
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 *   - maKhoaPhong: ma khoa phong can kiem tra
 * Gia tri tra ve: 1 neu hang doi rong, 0 neu con benh nhan hoac loi
 */

void inHangDoiMotKhoaConsole(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong,
    BangBam* bbBenhNhan,
    const BenhNhan dsBenhNhanToanBo[], int soLuongBNTB);
/*
 * Ham in hang doi cua mot khoa ra man hinh console
 * Muc dich: Hien thi danh sach benh nhan dang cho kham tai mot khoa cu the
 * Tham so dau vao:
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 *   - maKhoaPhong: ma khoa phong can hien thi hang doi
 *   - bbBenhNhan: bang bam benh nhan (de lay thong tin benh nhan nhanh)
 *   - dsBenhNhanToanBo: mang tat ca benh nhan
 *   - soLuongBNTB: so luong benh nhan toan bo
 * Gia tri tra ve: Khong co (void)
 * Luu y: Hien thi theo thu tu uu tien (cap cuu truoc, thong thuong sau)
 */

void giaiPhongDuLieuHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk);
/*
 * Ham giai phong bo nho cua hang doi mot khoa
 * Muc dich: Giai phong bo nho da cap phat cho hang doi cua mot khoa cu the
 * Tham so dau vao:
 *   - hqmk: con tro toi cau truc hang doi uu tien mot khoa
 * Gia tri tra ve: Khong co (void)
 * Luu y: Giai phong mang dsChoKham va reset cac gia tri ve 0/NULL
 */

void giaiPhongTatCaHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa);
/*
 * Ham giai phong tat ca hang doi trong bang bam
 * Muc dich: Giai phong bo nho cua tat ca hang doi theo khoa trong he thong
 * Tham so dau vao:
 *   - bbHangDoiTheoKhoa: bang bam cac hang doi theo khoa
 * Gia tri tra ve: Khong co (void)
 * Luu y: Duyet qua tat ca khoa trong bang bam va giai phong tung hang doi
 */

#endif // PRIORITY_QUEUE_H
