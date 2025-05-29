#include "priority_queue.h"
#include "patient.h" 
#include "examination.h"
#include "csv_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Ham so sanh cho sap xep---
static int soSanhUuTienLanKham(const void* a, const void* b) {
    const LanKham* lk1 = (const LanKham*)a;
    const LanKham* lk2 = (const LanKham*)b;

    if (lk1->mucDoUuTien < lk2->mucDoUuTien) return -1;
    if (lk1->mucDoUuTien > lk2->mucDoUuTien) return 1;
    if (lk1->gioDangKyThanhCong < lk2->gioDangKyThanhCong) return -1;
    if (lk1->gioDangKyThanhCong > lk2->gioDangKyThanhCong) return 1;
    return 0;
}

// --- Trien khai Merge Sort ---
static void merge(LanKham arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    
    LanKham* L_arr = (LanKham*)malloc(n1 * sizeof(LanKham));
    LanKham* R_arr = (LanKham*)malloc(n2 * sizeof(LanKham));

    if (L_arr == NULL || R_arr == NULL) {
        perror("Loi malloc trong ham merge (priority_queue)");
        if (L_arr) free(L_arr);
        if (R_arr) free(R_arr);
        return;
    }

    for (i = 0; i < n1; i++) L_arr[i] = arr[l + i];
    for (j = 0; j < n2; j++) R_arr[j] = arr[m + 1 + j];

    i = 0; j = 0; k = l;
    while (i < n1 && j < n2) {
        if (soSanhUuTienLanKham(&L_arr[i], &R_arr[j]) <= 0) {
            arr[k] = L_arr[i];
            i++;
        }
        else {
            arr[k] = R_arr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L_arr[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R_arr[j];
        j++;
        k++;
    }
    free(L_arr);
    free(R_arr);
}

static void mergeSort(LanKham arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

// Ham sap xep hang doi mot khoa
static void sapXepHangDoiMotKhoaBangMergeSort(HangDoiUuTienMotKhoa* hqmk) {
    if (hqmk == NULL || hqmk->soLuongHienTai <= 1) {
        return;
    }
    mergeSort(hqmk->dsChoKham, 0, hqmk->soLuongHienTai - 1);
}


// --- Cac ham public cua module---

//Ham khoi tao hang doi cua mot khoa
void khoiTaoHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk) {
    if (hqmk == NULL) return;
    hqmk->dsChoKham = NULL;
    hqmk->soLuongHienTai = 0;
    hqmk->dungLuong = 0;
}

//Ham them vao hang doi cua mot khoa
int themVaoHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham lanKhamMoi) {
	if (bbHangDoiTheoKhoa == NULL || maKhoaPhong == NULL || strlen(maKhoaPhong) == 0) {//Kiem tra tham so hop le
        fprintf(stderr, "Bang bam hang doi hoac maKhoaPhong khong hop le.\n");
        return 0;
    }

	if (lanKhamMoi.trangThai != LK_DANG_CHO) {//Chi them lan kham dang cho vao hang doi
        return 1;
    }
   
	char maKPNorm[MAX_LEN_MA_KHOA_PHONG]; //Chuan hoa ma khoa phong
	strncpy(maKPNorm, maKhoaPhong, sizeof(maKPNorm) - 1);// Sao chep ma khoa phong vao maKPNorm
    maKPNorm[sizeof(maKPNorm) - 1] = '\0';
	for (int i = 0; maKPNorm[i]; ++i) maKPNorm[i] = toupper(maKPNorm[i]);// Chuyen ve chu hoa de so sanh

	HangDoiUuTienMotKhoa* hqmk = (HangDoiUuTienMotKhoa*)timKiemTrongBangBam(bbHangDoiTheoKhoa, maKPNorm);//Tim kiem trong bang bam hang doi theo khoa

	if (hqmk == NULL) {// Neu khong tim thay hang doi cua khoa, khoi tao mot hang doi moi
        hqmk = (HangDoiUuTienMotKhoa*)malloc(sizeof(HangDoiUuTienMotKhoa));
        if (hqmk == NULL) {
            perror("Loi cap phat cho HangDoiUuTienMotKhoa moi");
            return 0;
        }
		khoiTaoHangDoiMotKhoa(hqmk);// Khoi tao hang doi moi

		char* khoaMaKPStored = strdup(maKPNorm);// Sao chep ma khoa phong de luu trong bang bam
		if (khoaMaKPStored == NULL) {// Neu khong sao chep duoc, giai phong bo nho va tra ve loi
            perror("Loi strdup maKhoaPhong cho bang bam hang doi");
            free(hqmk);
            return 0;
        }
		if (!chenVaoBangBam(bbHangDoiTheoKhoa, khoaMaKPStored, hqmk)) {// Chen vao bang bam
            fprintf(stderr, "LOI: Khong the chen hang doi cua khoa %s vao bang bam hang doi.\n", khoaMaKPStored);
            free(khoaMaKPStored); 
            free(hqmk);
            return 0;
        }
    

    }

	for (int i = 0; i < hqmk->soLuongHienTai; ++i) {// Kiem tra xem lan kham da ton tai trong hang doi chua
        if (hqmk->dsChoKham[i].maLanKham == lanKhamMoi.maLanKham) {
            return 1;
        }
    }

	if (hqmk->soLuongHienTai >= hqmk->dungLuong) {// Neu hang doi da day, cap nhat dung luong
		int dungLuongMoi = (hqmk->dungLuong == 0) ? 10 : hqmk->dungLuong * 2;// Tang dung luong len gap doi
		LanKham* temp_ds = realloc(hqmk->dsChoKham, dungLuongMoi * sizeof(LanKham));// Cap phat bo nho cho dsChoKham
        if (temp_ds == NULL) {
            perror("Loi realloc dsChoKham (themVaoHangDoiTheoKhoa)");   
            return 0;
        }
		hqmk->dsChoKham = temp_ds;// Cap nhat con tro dsChoKham
		hqmk->dungLuong = dungLuongMoi;// Cap nhat dung luong moi
    }

	hqmk->dsChoKham[hqmk->soLuongHienTai] = lanKhamMoi;// Them lan kham moi vao cuoi danh sach
	hqmk->soLuongHienTai++;// Tang so luong hien tai cua hang doi

	sapXepHangDoiMotKhoaBangMergeSort(hqmk);// Sap xep lai hang doi sau khi them lan kham moi

    return 1;
}

//Ham goi benh nhan tiep theo(Theo khoa) 
int layBenhNhanTiepTheoTheoKhoa(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong, LanKham* lanKhamLayRa) {
    if (bbHangDoiTheoKhoa == NULL || maKhoaPhong == NULL || strlen(maKhoaPhong) == 0 || lanKhamLayRa == NULL) 
		return 0;// Kiem tra tham so hop le

	char maKPNorm[MAX_LEN_MA_KHOA_PHONG];// Chuan hoa ma khoa phong
    strncpy(maKPNorm, maKhoaPhong, sizeof(maKPNorm) - 1);
    maKPNorm[sizeof(maKPNorm) - 1] = '\0';
	for (int i = 0; maKPNorm[i]; ++i) maKPNorm[i] = toupper(maKPNorm[i]);// Chuyen ve chu hoa de so sanh

	HangDoiUuTienMotKhoa* hqmk = (HangDoiUuTienMotKhoa*)timKiemTrongBangBam(bbHangDoiTheoKhoa, maKPNorm);// Tim kiem trong bang bam hang doi theo khoa

	if (hqmk == NULL || hqmk->soLuongHienTai == 0) {// Neu hang doi rong, ket thuc
        return 0;
    }

	*lanKhamLayRa = hqmk->dsChoKham[0];// Lay lan kham dau tien trong hang doi

	for (int i = 0; i < hqmk->soLuongHienTai - 1; ++i) {// Dich chuyen cac lan kham con lai ve truoc
        hqmk->dsChoKham[i] = hqmk->dsChoKham[i + 1];
    }
	hqmk->soLuongHienTai--;// Giam so luong hien tai cua hang doi

    return 1;
}

//Ham kiem tra hang doi theo khoa co rong khong
int laHangDoiTheoKhoaRong(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong) {
	if (bbHangDoiTheoKhoa == NULL || maKhoaPhong == NULL || strlen(maKhoaPhong) == 0)// Kiem tra tham so hop le
        return 1;

	char maKPNorm[MAX_LEN_MA_KHOA_PHONG];// Chuan hoa ma khoa phong
    strncpy(maKPNorm, maKhoaPhong, sizeof(maKPNorm) - 1);
    maKPNorm[sizeof(maKPNorm) - 1] = '\0';
	for (int i = 0; maKPNorm[i]; ++i) maKPNorm[i] = toupper(maKPNorm[i]);// Chuyen ve chu hoa de so sanh

    HangDoiUuTienMotKhoa* hqmk = (HangDoiUuTienMotKhoa*)timKiemTrongBangBam(bbHangDoiTheoKhoa, maKPNorm);// Tim kiem trong bang bam hang doi theo khoa
    if (hqmk == NULL) return 1;
    return hqmk->soLuongHienTai == 0;
}

void inHangDoiMotKhoaConsole(BangBam* bbHangDoiTheoKhoa, const char* maKhoaPhong,
    BangBam* bbBenhNhan,
    const BenhNhan dsBenhNhanToanBo[], int soLuongBNTB) {

	if (bbHangDoiTheoKhoa == NULL || maKhoaPhong == NULL || strlen(maKhoaPhong) == 0) {// Kiem tra tham so hop le
        printf("Loi: Khong tim thay bang bam hang doi hoac ma khoa phong khong hop le.\n");
        return;
    }

	char maKPHienThi[MAX_LEN_MA_KHOA_PHONG];// Chuan hoa ma khoa phong de hien thi
    strncpy(maKPHienThi, maKhoaPhong, sizeof(maKPHienThi) - 1);
    maKPHienThi[sizeof(maKPHienThi) - 1] = '\0';
	for (int i = 0; maKPHienThi[i]; ++i) maKPHienThi[i] = toupper(maKPHienThi[i]);// Chuyen ve chu hoa de hien thi

	HangDoiUuTienMotKhoa* hqmk = (HangDoiUuTienMotKhoa*)timKiemTrongBangBam(bbHangDoiTheoKhoa, maKPHienThi);// Tim kiem trong bang bam hang doi theo khoa

    printf("\n--- HANG DOI CHO KHAM CUA KHOA: %s ---\n", maKPHienThi);
    if (hqmk == NULL || hqmk->soLuongHienTai == 0) {//
        printf("Hang doi cua khoa %s rong.\n", maKPHienThi);
        return;
    }

    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-7s | %-18s | %-25s | %-15s | %-20s |\n", "STT", "Ma LK", "Ma BN", "Ten Benh Nhan", "Muc Do Uu Tien", "Gio Dang Ky");
    printf("-------------------------------------------------------------------------------------------------------------\n");

	for (int i = 0; i < hqmk->soLuongHienTai; ++i) {// Duyet qua danh sach benh nhan dang cho kham
		const LanKham* lk = &hqmk->dsChoKham[i];// Lay thong tin lan kham
		char tenBNHienThi[101] = "N/A";// Khoi tao ten benh nhan hien thi

		if (strlen(lk->maDinhDanh) > 0) {// Neu ma dinh danh khong rong, tim ten benh nhan
            if (bbBenhNhan) {
                BenhNhan* bn = timBenhNhanTheoMaBangBam(bbBenhNhan, lk->maDinhDanh);
                if (bn) {
                    strncpy(tenBNHienThi, bn->ten, sizeof(tenBNHienThi) - 1);
                    tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
                }
            }
			else if (dsBenhNhanToanBo != NULL) {// Neu khong co bang bam benh nhan, tim trong danh sach benh nhan toan bo
                for (int j = 0; j < soLuongBNTB; ++j) {
                    if (strcmp(dsBenhNhanToanBo[j].maDinhDanh, lk->maDinhDanh) == 0) {
                        strncpy(tenBNHienThi, dsBenhNhanToanBo[j].ten, sizeof(tenBNHienThi) - 1);
                        tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
                        break;
                    }
                }
            }
        }
        
        //xu ly gio dang ky thanh cong
        char gioDangKyStr[30];//
        if (lk->gioDangKyThanhCong != 0) {
            struct tm* tm_info = localtime(&lk->gioDangKyThanhCong);
            if (tm_info != NULL) {
                strftime(gioDangKyStr, sizeof(gioDangKyStr), "%H:%M:%S %d/%m/%Y", tm_info);
            }
            else {
                strcpy(gioDangKyStr, "Loi TGian");
            }
        }
        else {
            strcpy(gioDangKyStr, "Chua DK");
        }

		// Rut gon ten benh nhan
        char tenBNRutGon[26];
        rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, 25);

		// In thong tin benh nhan dang cho kham
        printf("| %-3d | %-7d | %-18s | %-25s | %-15s | %-20s |\n",
            i + 1,
            lk->maLanKham,
            (lk->maDinhDanh[0] != '\0' ? lk->maDinhDanh : "CHUA_GAN"),
            tenBNRutGon,
            mucDoUuTienToString(lk->mucDoUuTien),
            gioDangKyStr);
    }
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("Tong so benh nhan dang cho tai khoa %s: %d\n", maKPHienThi, hqmk->soLuongHienTai);
}

//-----Cac ham giai phong bo nho------

// Ham giai phong du lieu hang doi cua mot khoa
void giaiPhongDuLieuHangDoiMotKhoa(HangDoiUuTienMotKhoa* hqmk) {
    if (hqmk != NULL) {
        free(hqmk->dsChoKham);
        hqmk->dsChoKham = NULL;
        hqmk->soLuongHienTai = 0;
        hqmk->dungLuong = 0;
    }
}

//Ham giai phong tat cả hang doi
void giaiPhongTatCaHangDoiTheoKhoa(BangBam* bbHangDoiTheoKhoa) {
    if (bbHangDoiTheoKhoa == NULL || bbHangDoiTheoKhoa->cacBucket == NULL) {
        return;
    }

    for (int i = 0; i < bbHangDoiTheoKhoa->dungLuong; ++i) {
        NutBam* nutHienTai = bbHangDoiTheoKhoa->cacBucket[i];
        while (nutHienTai != NULL) {
            HangDoiUuTienMotKhoa* hqmk = (HangDoiUuTienMotKhoa*)nutHienTai->giaTri;
            if (hqmk != NULL) {
                giaiPhongDuLieuHangDoiMotKhoa(hqmk);
                free(hqmk);
            }
            nutHienTai = nutHienTai->tiepTheo;
        }
    }
    giaiPhongBangBam(bbHangDoiTheoKhoa);
}