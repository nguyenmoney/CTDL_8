#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
// Cac thu vien khac
#include "structs.h"
#include "csv_handler.h"
#include "patient.h"
#include "department.h"
#include "doctor.h"
#include "medicine.h"
#include "examination.h"
#include "prescription.h"
#include "priority_queue.h"
#include "ui.h"
#include "hash_table.h"
//Khai bao do dai chuoi ma lan kham
#ifndef MA_LAN_KHAM_STR_LEN
#define MA_LAN_KHAM_STR_LEN 12
#endif
//Khai bao ham xu ly chuoi ma lan kham
static void main_intMaLKToString(int maLK, char* maLKStr, size_t bufferSize) { 
    snprintf(maLKStr, bufferSize, "%d", maLK);
}

//--Main--
int main() {
    // --- Khoi tao cac con tro du lieu va so luong ---
    BenhNhan* dsBenhNhan = NULL;
    int soLuongBenhNhan = 0;

    KhoaPhong* dsKhoaPhong = NULL;
    int soLuongKhoaPhong = 0;

    BacSi* dsBacSi = NULL;
    int soLuongBacSi = 0;

    Thuoc* dsThuoc = NULL;
    int soLuongThuoc = 0;

    LanKham* dsLanKham = NULL;
    int soLuongLanKham = 0;

    DonThuocChiTiet* dsDonThuocChiTiet = NULL;
    int soLuongDTCT = 0;

    // --- Khoi tao cac con tro cua bang bam ---
    BangBam* bbBenhNhan = NULL;
    BangBam* bbBacSi = NULL;
    BangBam* bbKhoaPhong = NULL;
    BangBam* bbThuoc = NULL;
    BangBam* bbLanKham = NULL;
    BangBam* bbHangDoiTheoKhoa = NULL;

    printf("Dang khoi tao he thong va tai du lieu...\n");

    // 1. Tai Khoa Phong VA KHOI TAO BANG BAM KHOA PHONG
    dsKhoaPhong = docDanhSachKhoaPhong(FILE_KHOA_PHONG, &soLuongKhoaPhong); 
	if (dsKhoaPhong == NULL && soLuongKhoaPhong == 0) { //Kiem tra xem co du lieu hay khong
		printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Khoa Phong rong.\n", FILE_KHOA_PHONG); //Thong bao neu khong co du lieu
        bbKhoaPhong = taoBangBam(BANG_BAM_DUNG_LUONG_MAC_DINH); 
		if (bbKhoaPhong == NULL) {
            fprintf(stderr, "LOI: Khong the khoi tao bang bam rong cho Khoa Phong.\n");
		}//Neu khong khoi tao duoc bang bam thi thong bao loi
    }
    else if (dsKhoaPhong != NULL) {
		printf("Tai thanh cong %d Khoa Phong tu %s\n", soLuongKhoaPhong, FILE_KHOA_PHONG); //Thong bao neu co du lieu
		int kichThuocBBKP = soLuongKhoaPhong > 0 ? (soLuongKhoaPhong * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH; // Tang kich thuoc bang bam theo so luong khoa phong
        if (kichThuocBBKP < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongKhoaPhong > 0) { 
            kichThuocBBKP = BANG_BAM_DUNG_LUONG_MAC_DINH; 
        }
		bbKhoaPhong = taoBangBam(kichThuocBBKP); //Gan kich thuoc bang bam
        if (bbKhoaPhong == NULL) {
			fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Khoa Phong.\n");//Thong bao neu khong khoi tao duoc bang bam
        }
        else {
            printf("Dang them Khoa Phong vao bang bam...\n");
			for (int i = 0; i < soLuongKhoaPhong; ++i) { //Them tung khoa phong vao bang bam
				if (!chenVaoBangBam(bbKhoaPhong, dsKhoaPhong[i].maKhoaPhong, &dsKhoaPhong[i])) { // Chen khoa phong vao bang bam
                    fprintf(stderr, "LOI khi chen Khoa Phong ID '%s' vao bang bam.\n", dsKhoaPhong[i].maKhoaPhong);
                }
            }
            printf("Da them %d Khoa Phong vao bang bam.\n", bbKhoaPhong->soLuongPhanTu);
        }
	}//Neu co du lieu thi thong bao va them vao bang bam

    // 2. Tai Bac Si VA KHOI TAO BANG BAM BAC SI
    dsBacSi = docDanhSachBacSi(FILE_BAC_SI, &soLuongBacSi); 
    if (dsBacSi == NULL && soLuongBacSi == 0) {
		printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Bac Si rong.\n", FILE_BAC_SI); //Thong bao neu khong co du lieu
		bbBacSi = taoBangBam(BANG_BAM_DUNG_LUONG_MAC_DINH); // Khoi tao bang bam rong
        if (bbBacSi == NULL) {
			fprintf(stderr, "LOI: Khong the khoi tao bang bam rong cho Bac Si.\n"); //Thong bao neu khong khoi tao duoc bang bam
		}//Thong bao neu khong khoi tao duoc bang bam
    }
    else if (dsBacSi != NULL) {
		printf("Tai thanh cong %d Bac Si tu %s\n", soLuongBacSi, FILE_BAC_SI); //Thong bao neu co du lieu
		int kichThuocBBBS = soLuongBacSi > 0 ? (soLuongBacSi * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH; // Tang kich thuoc bang bam theo so luong bac si
        if (kichThuocBBBS < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongBacSi > 0) { 
			kichThuocBBBS = BANG_BAM_DUNG_LUONG_MAC_DINH; // Neu so luong bac si it hon kich thuoc mac dinh thi gan bang mac dinh
        }
		bbBacSi = taoBangBam(kichThuocBBBS); // Khoi tao bang bam bac si voi kich thuoc da tinh toan
        if (bbBacSi == NULL) {
            fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Bac Si.\n"); 
        }
        else {
            printf("Dang them Bac Si vao bang bam...\n"); 
            for (int i = 0; i < soLuongBacSi; ++i) {
				if (!chenVaoBangBam(bbBacSi, dsBacSi[i].maBacSi, &dsBacSi[i])) { // Chen tung bac si vao bang bam
                    fprintf(stderr, "LOI khi chen Bac Si ID '%s' vao bang bam.\n", dsBacSi[i].maBacSi); 
                }
			}
            printf("Da them %d Bac Si vao bang bam.\n", bbBacSi->soLuongPhanTu); 
		}//Thong bao neu them thanh cong bac si vao bang bam
    }

    // 3. Tai Thuoc VA KHOI TAO BANG BAM THUOC
    dsThuoc = docDanhSachThuoc(FILE_THUOC, &soLuongThuoc); 
    if (dsThuoc == NULL && soLuongThuoc == 0) { // Kiem tra neu ds rong hoac bang 0
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Thuoc rong.\n", FILE_THUOC); 
        bbThuoc = taoBangBam(BANG_BAM_DUNG_LUONG_MAC_DINH);
		if (bbThuoc == NULL) fprintf(stderr, "LOI: Khong the khoi tao bang bam rong cho Thuoc.\n");//THong bao neu khong khoi tao duoc bang bam hoac khong co thuoc trong file
    }
    else if (dsThuoc != NULL) {
        printf("Tai thanh cong %d loai Thuoc tu %s\n", soLuongThuoc, FILE_THUOC); //
        int kichThuocBBT = soLuongThuoc > 0 ? (soLuongThuoc * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH;
		if (kichThuocBBT < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongThuoc > 0) kichThuocBBT = BANG_BAM_DUNG_LUONG_MAC_DINH;// Neu so luong thuoc it hon kich thuoc mac dinh thi gan bang mac dinh
		bbThuoc = taoBangBam(kichThuocBBT);// Khoi tao bang bam thuoc voi kich thuoc da tinh toan
        if (bbThuoc == NULL) {
            fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Thuoc.\n");
        }
        else {
            printf("Dang them Thuoc vao bang bam...\n");
            for (int i = 0; i < soLuongThuoc; ++i) {
				if (!chenVaoBangBam(bbThuoc, dsThuoc[i].maThuoc, &dsThuoc[i])) {// Chen tung thuoc vao bang bam
                    fprintf(stderr, "LOI khi chen Thuoc ID '%s' vao bang bam.\n", dsThuoc[i].maThuoc);
                }
            }
            printf("Da them %d Thuoc vao bang bam.\n", bbThuoc->soLuongPhanTu);
		}// Thong bao neu them thanh cong thuoc vao bang bam
    }

    // 4. Tai Benh Nhan VA KHOI TAO BANG BAM BENH NHAN
    dsBenhNhan = docDanhSachBenhNhan(FILE_BENH_NHAN, &soLuongBenhNhan); 
    if (dsBenhNhan == NULL && soLuongBenhNhan == 0) {
		printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Benh Nhan rong.\n", FILE_BENH_NHAN); //Thong bao khong co du lieu
        bbBenhNhan = taoBangBam(BANG_BAM_DUNG_LUONG_MAC_DINH); 
        if (bbBenhNhan == NULL) {
			fprintf(stderr, "LOI: Khong the khoi tao bang bam rong cho Benh Nhan.\n"); //Thong bao neu khong khoi tao duoc bang bam
        }
    }
    else if (dsBenhNhan != NULL) {
        printf("Tai thanh cong %d Benh Nhan tu %s\n", soLuongBenhNhan, FILE_BENH_NHAN); 
        int kichThuocBBBN = soLuongBenhNhan > 0 ? (soLuongBenhNhan * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH; 
        if (kichThuocBBBN < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongBenhNhan > 0) { 
			kichThuocBBBN = BANG_BAM_DUNG_LUONG_MAC_DINH; // Neu so luong benh nhan it hon kich thuoc mac dinh thi gan bang mac dinh
        }
        bbBenhNhan = taoBangBam(kichThuocBBBN); 
        if (bbBenhNhan == NULL) {
            fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Benh Nhan.\n"); 
        }
        else {
            printf("Dang them Benh Nhan vao bang bam...\n"); 
            for (int i = 0; i < soLuongBenhNhan; ++i) {
				if (!chenVaoBangBam(bbBenhNhan, dsBenhNhan[i].maDinhDanh, &dsBenhNhan[i])) { // Chen tung benh nhan vao bang bam
                    fprintf(stderr, "LOI khi chen Benh Nhan ID '%s' vao bang bam.\n", dsBenhNhan[i].maDinhDanh); 
                }
            }
            printf("Da them %d Benh Nhan vao bang bam.\n", bbBenhNhan->soLuongPhanTu); 
		}//Thong bao neu them thanh cong benh nhan vao bang bam
    }

    // 5. Tai Lan Kham VA KHOI TAO BANG BAM LAN KHAM
    dsLanKham = docDanhSachLanKham(FILE_LAN_KHAM, &soLuongLanKham); 
    if (dsLanKham == NULL && soLuongLanKham == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Lan Kham rong.\n", FILE_LAN_KHAM); 
        bbLanKham = taoBangBam(BANG_BAM_DUNG_LUONG_MAC_DINH);
        if (bbLanKham == NULL) fprintf(stderr, "LOI: Khong the khoi tao bang bam rong cho Lan Kham.\n");
	}//Thong bao neu khong co du lieu hoac khong khoi tao duoc bang bam
    else if (dsLanKham != NULL) {
        printf("Tai thanh cong %d Lan Kham tu %s\n", soLuongLanKham, FILE_LAN_KHAM); 
        int kichThuocBBLK = soLuongLanKham > 0 ? (soLuongLanKham * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH;
        if (kichThuocBBLK < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongLanKham > 0) kichThuocBBLK = BANG_BAM_DUNG_LUONG_MAC_DINH;
        bbLanKham = taoBangBam(kichThuocBBLK);
        if (bbLanKham == NULL) {
            fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Lan Kham.\n");
		}//Thong bao neu khong khoi tao duoc bang bam
        else {
            printf("Dang them Lan Kham vao bang bam...\n");
            char maLKStr[MA_LAN_KHAM_STR_LEN];
            for (int i = 0; i < soLuongLanKham; ++i) {
				main_intMaLKToString(dsLanKham[i].maLanKham, maLKStr, sizeof(maLKStr));// Chuyen ma lan kham sang chuoi
				if (!chenVaoBangBam(bbLanKham, maLKStr, &dsLanKham[i])) {// Chen tung lan kham vao bang bam
                    fprintf(stderr, "LOI khi chen Lan Kham ID %d ('%s') vao bang bam.\n", dsLanKham[i].maLanKham, maLKStr);
                }
            }
            printf("Da them %d Lan Kham vao bang bam.\n", bbLanKham->soLuongPhanTu);
		}//Thong bao neu them thanh cong lan kham vao bang bam
    }

    // 6. Tai Chi Tiet Don Thuoc
	dsDonThuocChiTiet = docDanhSachDonThuocChiTiet(FILE_DON_THUOC_CHI_TIET, &soLuongDTCT); // Doc danh sach chi tiet don thuoc tu file
    if (dsDonThuocChiTiet == NULL && soLuongDTCT == 0) {
        printf("Khong tim thay file %s hoac file rong. Khoi tao danh sach Chi Tiet Don Thuoc rong.\n", FILE_DON_THUOC_CHI_TIET); 
    }
    else if (dsDonThuocChiTiet != NULL) {
        printf("Tai thanh cong %d dong Chi Tiet Don Thuoc tu %s\n", soLuongDTCT, FILE_DON_THUOC_CHI_TIET); 
	} // Thong bao neu co du lieu hoac khong co du lieu

    // 7. Khoi tao BangBam cho Hang Doi Theo Khoa
  
    int kichThuocBBHangDoi = soLuongKhoaPhong > 0 ? (soLuongKhoaPhong * BANG_BAM_HE_SO_TANG_KICH_THUOC) : BANG_BAM_DUNG_LUONG_MAC_DINH;
	if (kichThuocBBHangDoi < BANG_BAM_DUNG_LUONG_MAC_DINH && soLuongKhoaPhong > 0) kichThuocBBHangDoi = BANG_BAM_DUNG_LUONG_MAC_DINH;// Neu so luong khoa phong it hon kich thuoc mac dinh thi gan bang mac dinh
    bbHangDoiTheoKhoa = taoBangBam(kichThuocBBHangDoi);
    if (bbHangDoiTheoKhoa == NULL) {
        fprintf(stderr, "LOI: Khong the khoi tao bang bam cho Hang Doi Theo Khoa.\n");
		//Thong bao neu khong khoi tao duoc bang bam
    }
    else {
        printf("Khoi tao bang bam Hang Doi Theo Khoa thanh cong.\n");
	}//Thong bao neu khoi tao duoc bang bam cho hang doi theo khoa

    printf("Hoan tat tai du lieu.\n");
    if (dsLanKham != NULL && soLuongLanKham > 0 && bbHangDoiTheoKhoa != NULL) { 
        printf("Dang nap cac lan kham (trang thai DANG_CHO) vao hang doi theo khoa...\n");
        int countThemVaoHangDoi = 0;
        for (int i = 0; i < soLuongLanKham; ++i) {
            if (dsLanKham[i].trangThai == LK_DANG_CHO && strlen(dsLanKham[i].maKhoaPhong) > 0) {
				if (!themVaoHangDoiTheoKhoa(bbHangDoiTheoKhoa, dsLanKham[i].maKhoaPhong, dsLanKham[i])) { // Them lan kham vao hang doi theo khoa
                    fprintf(stderr, "Loi: Khong the them Lan Kham %d (Khoa: %s) vao hang doi khi khoi tao.\n", dsLanKham[i].maLanKham, dsLanKham[i].maKhoaPhong);
                }
                else {
                    countThemVaoHangDoi++;
                }
            }
        }
        printf("Da nap %d benh nhan vao cac hang doi cho kham theo khoa.\n", countThemVaoHangDoi);
    }
    else if (dsLanKham == NULL || soLuongLanKham == 0) {
        printf("Khong co du lieu Lan Kham de nap vao hang doi.\n");
    }
    else if (bbHangDoiTheoKhoa == NULL) {
        printf("Bang bam Hang Doi Theo Khoa khong kha dung, khong the nap Lan Kham vao hang doi.\n");
	}//Thong bao co hay khong du lieu lan kham de nap vao hang doi theo khoa


	dungManHinhCho(); //Nhan Enter de tiep tuc


	int luaChonMenuChinh;//Man hinh menu chinh
    do {
		hienThiMenuChinh(); //Hien thi menu chinh
        luaChonMenuChinh = nhapSoNguyenCoGioiHan("Vui long nhap lua chon cua ban", 0, 11); //Lua chon cac chuc nang tren man hinh
        if (luaChonMenuChinh < 0) luaChonMenuChinh = 0; 


        if (luaChonMenuChinh >= 0 && luaChonMenuChinh <= 11) { 
            xuLyLuaChonMenuChinh(
                luaChonMenuChinh,
                &dsBenhNhan, &soLuongBenhNhan, &bbBenhNhan, FILE_BENH_NHAN,
                &dsLanKham, &soLuongLanKham, &bbLanKham, FILE_LAN_KHAM,
                &dsKhoaPhong, &soLuongKhoaPhong, &bbKhoaPhong, FILE_KHOA_PHONG,
                &dsBacSi, &soLuongBacSi, &bbBacSi, FILE_BAC_SI,
                &dsThuoc, &soLuongThuoc, &bbThuoc, FILE_THUOC,
                &dsDonThuocChiTiet, &soLuongDTCT, FILE_DON_THUOC_CHI_TIET,
                bbHangDoiTheoKhoa 
            ); //
        }
       
    } while (luaChonMenuChinh != 0);

    // --- Giai phong bo nho ---
    printf("\nDang giai phong bo nho...\n"); 

 
    free(dsBenhNhan); 
    free(dsKhoaPhong); 
    free(dsBacSi); 
    free(dsThuoc); 
    free(dsLanKham); 
    free(dsDonThuocChiTiet); 

   
    giaiPhongBangBam(bbBenhNhan); 
    giaiPhongBangBam(bbBacSi);    
    giaiPhongBangBam(bbKhoaPhong);
    giaiPhongBangBam(bbThuoc);
    giaiPhongBangBam(bbLanKham);
    giaiPhongTatCaHangDoiTheoKhoa(bbHangDoiTheoKhoa); 

    printf("Da giai phong bo nho. Chuong trinh ket thuc.\n"); 

	return EXIT_SUCCESS;//Ket thuc chuong trinh voi ma tra ve thanh cong
}