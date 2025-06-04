#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BANG_BAM_DUNG_LUONG_MAC_DINH 101
#define BANG_BAM_NGUONG_TAI_TOI_DA 0.75
#define BANG_BAM_HE_SO_TANG_KICH_THUOC 2

// Cau truc cho mot nut trong danh sach lien ket tai moi bucket
typedef struct NutBam {
    char* khoa;
    void* giaTri;
    struct NutBam* tiepTheo;
} NutBam;

// Cau truc cho bang bam
typedef struct BangBam {
    NutBam** cacBucket;
    int dungLuong;
    int soLuongPhanTu;
} BangBam;

// --- Khai bao cac ham cho bang bam ---

BangBam* taoBangBam(int dungLuongKLT);
/*
 * Ham tao bang bam moi
 * Muc dich: Khoi tao bang bam voi dung luong chi dinh
 * Tham so dau vao:
 *   - dungLuongKLT: dung luong khoi tao (so bucket), neu <= 0 thi dung gia tri mac dinh
 * Gia tri tra ve: Con tro toi bang bam moi tao (NULL neu that bai)
 * Luu y: Bang bam su dung danh sach lien ket de xu ly collision
 */

void giaiPhongBangBam(BangBam* bb);
/*
 * Ham giai phong bo nho cua bang bam
 * Muc dich: Xoa toan bo du lieu va giai phong bo nho da cap phat cho bang bam
 * Tham so dau vao:
 *   - bb: con tro toi bang bam can giai phong
 * Gia tri tra ve: Khong co (void)
 * Luu y: Giai phong tat ca nut, khoa va bang bam, nhung khong giai phong gia tri
 */

unsigned int hamBamChuoi(const char* khoa, int dungLuongBang);
/*
 * Ham bam chuoi thanh chi so bucket
 * Muc dich: Tinh chi so bucket cho khoa chuoi trong bang bam
 * Tham so dau vao:
 *   - khoa: chuoi khoa can bam
 *   - dungLuongBang: so bucket trong bang bam
 * Gia tri tra ve: Chi so bucket (0 den dungLuongBang-1)
 * Luu y: Su dung thuat toan djb2 hash function
 */

int chenVaoBangBam(BangBam* bb, const char* khoa, void* giaTri);
/*
 * Ham chen cap khoa-gia tri vao bang bam
 * Muc dich: Them hoac cap nhat gia tri trong bang bam theo khoa
 * Tham so dau vao:
 *   - bb: con tro toi bang bam
 *   - khoa: chuoi khoa (se duoc sao chep)
 *   - giaTri: con tro toi gia tri can luu
 * Gia tri tra ve: 1 neu thanh cong, 0 neu that bai
 * Luu y: Neu khoa da ton tai, cap nhat gia tri moi; neu chua co, tao nut moi
 */

void* timKiemTrongBangBam(BangBam* bb, const char* khoa);
/*
 * Ham tim kiem gia tri theo khoa trong bang bam
 * Muc dich: Lay gia tri tuong ung voi khoa cho truoc (O(1) trung binh)
 * Tham so dau vao:
 *   - bb: con tro toi bang bam
 *   - khoa: chuoi khoa can tim
 * Gia tri tra ve: Con tro toi gia tri tim thay (NULL neu khong tim thay)
 */

int xoaKhoiBangBam(BangBam* bb, const char* khoa);
/*
 * Ham xoa cap khoa-gia tri khoi bang bam
 * Muc dich: Loai bo nut chua khoa chi dinh ra khoi bang bam
 * Tham so dau vao:
 *   - bb: con tro toi bang bam
 *   - khoa: chuoi khoa can xoa
 * Gia tri tra ve: 1 neu xoa thanh cong, 0 neu khong tim thay khoa
 * Luu y: Chi giai phong bo nho cua nut va khoa, khong giai phong gia tri
 */

int rehashBangBam(BangBam** bb_ptr);
/*
 * Ham tang kich thuoc va sap xep lai bang bam
 * Muc dich: Tang dung luong bang bam khi he so tai vuot nguong, cai thien hieu suat
 * Tham so dau vao:
 *   - bb_ptr: con tro kep toi bang bam (de co the thay doi bang bam)
 * Gia tri tra ve: 1 neu rehash thanh cong, 0 neu that bai
 * Luu y: Tang kich thuoc bang BANG_BAM_HE_SO_TANG_KICH_THUOC lan va phan bo lai tat ca nut
 */

#endif // HASH_TABLE_H