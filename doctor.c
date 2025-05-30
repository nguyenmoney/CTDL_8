#include "doctor.h"
#include "department.h"
#include "csv_handler.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham tien ich noi bo xoa ky tu xuong dong
static void removeNewlineDoc(char* str) {
	if (str == NULL) return; // Neu chuoi rong, khong lam gi ca
    size_t len = strlen(str); // Tinh do dai chuoi
    if (len > 0 && str[len - 1] == '\n') { // Neu ky tu cuoi cung la '\n', xoa no
        str[len - 1] = '\0'; // Thay the ky tu cuoi cung bang ky tu ket thuc chuoi '\0'
    }
}

// Ham doc danh sach bac si
BacSi* docDanhSachBacSi(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r"); // Mo file de doc
	if (f == NULL) {// Kiem tra an toan mo file
        *soLuong = 0;
        return NULL;
    } 

    BacSi* ds = NULL; // Khai bao con tro danh sach bac si
    int capacity = 0; // Dung luong hien tai cua danh sach
	*soLuong = 0; // khoi tao so luong bac si
    char line[500]; //Mang de luu tung dong doc duoc tu file
    char* token; // Con tro de luu tung token khi tach chuoi

    while (fgets(line, sizeof(line), f) != NULL) { // Doc tung dong tu file
        removeNewlineDoc(line);
        if (strlen(line) < 5) continue; // Neu dong < 5 ky tu, bo qua dong do 

        if (*soLuong >= capacity) { // Neu so luong bac si hien tai lon hon dung luong hien tai
            capacity = (capacity == 0) ? 10 : capacity * 2; // Cap nhat suc chua
            BacSi* temp = realloc(ds, capacity * sizeof(BacSi)); // Cap phat lai bo nho cho danh sach bac si
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachBacSi)"); // Neu cap phat that bai, in thong bao loi
                free(ds); // Giai phong bo nho da cap phat cho ds
                fclose(f); // Dong file
                *soLuong = 0; // Gan soLuong ve 0
                return NULL;
            }
            ds = temp;// Gan con tro ds ve bo nho moi cap phat lai
        }

		BacSi bs; // bien de luu thong tin bac si moi
        token = strtok(line, ","); // Tach dong theo dau phay

		// Doc ma bac si 
        if (token) { strncpy(bs.maBacSi, token, sizeof(bs.maBacSi) - 1); bs.maBacSi[sizeof(bs.maBacSi) - 1] = '\0'; }
        else continue;

		token = strtok(NULL, ","); //Doc ten bac si
        if (token) { strncpy(bs.tenBacSi, token, sizeof(bs.tenBacSi) - 1); bs.tenBacSi[sizeof(bs.tenBacSi) - 1] = '\0'; }
        else bs.tenBacSi[0] = '\0';

		token = strtok(NULL, ","); // Doc chuyen khoa
        if (token) { strncpy(bs.chuyenKhoa, token, sizeof(bs.chuyenKhoa) - 1); bs.chuyenKhoa[sizeof(bs.chuyenKhoa) - 1] = '\0'; }
        else bs.chuyenKhoa[0] = '\0';

        token = strtok(NULL, ",");
        // Neu token khong NULL, sao chep so dien thoai vao struct BacSi
        if (token) { strncpy(bs.soDienThoai, token, sizeof(bs.soDienThoai) - 1); bs.soDienThoai[sizeof(bs.soDienThoai) - 1] = '\0'; }
        else bs.soDienThoai[0] = '\0';

		token = strtok(NULL, ",");// Doc email
        if (token) { strncpy(bs.email, token, sizeof(bs.email) - 1); bs.email[sizeof(bs.email) - 1] = '\0'; }
        else bs.email[0] = '\0';

		token = strtok(NULL, "\n");// Doc ma khoa phong
        if (token) { strncpy(bs.maKhoaPhong, token, sizeof(bs.maKhoaPhong) - 1); bs.maKhoaPhong[sizeof(bs.maKhoaPhong) - 1] = '\0'; }
        else bs.maKhoaPhong[0] = '\0';

        ds[*soLuong] = bs; // Gan bac si moi vao danh sach
        (*soLuong)++; // Tang so luong bac si hien tai
    }

    fclose(f);
    return ds;
}

// Ham ghi danh sach ra file
void ghiDanhSachBacSiRaFile(const char* tenFile, const BacSi dsBacSi[], int soLuong) {
    FILE* f = fopen(tenFile, "w"); // Mo file de ghi
	if (f == NULL) {// Kiem tra an toan mo file
        perror("Loi mo file de ghi");
        return;
    }
    for (int i = 0; i < soLuong; ++i) { // Duyet qua tung bac si trong danh sach, ghi tung thuoc tinh vao file
        fprintf(f, "%s,%s,%s,%s,%s,%s\n",
            dsBacSi[i].maBacSi, dsBacSi[i].tenBacSi, dsBacSi[i].chuyenKhoa,
            dsBacSi[i].soDienThoai, dsBacSi[i].email, dsBacSi[i].maKhoaPhong);
    }
    fclose(f);
}

// --- Cac ham tim kiem noi bo theo cac thuoc tinh khong phai khoa chinh ---

//Ham tim bac si theo ten
static BacSi* timBacSiTheoTen(const BacSi dsBacSi[], int soLuong, const char* tenBacSi) {
	if (tenBacSi == NULL || strlen(tenBacSi) == 0) return NULL; // Neu tenBacSi NULL hoac rong, tra ve NULL

    char tenBacSiLower[sizeof(dsBacSi[0].tenBacSi)]; // Mang de luu ten bac si da chuyen ve chu thuong
    strncpy(tenBacSiLower, tenBacSi, sizeof(tenBacSiLower) - 1); // Sao chep ten bac si vao mang
    tenBacSiLower[sizeof(tenBacSiLower) - 1] = '\0';
    for (int k = 0; tenBacSiLower[k]; k++) tenBacSiLower[k] = tolower(tenBacSiLower[k]); // Chuyen ten bac si ve chu thuong

    for (int i = 0; i < soLuong; ++i) {
        char currentTenBSLower[sizeof(dsBacSi[i].tenBacSi)]; // Mang de luu ten bac si hien tai da chuyen ve chu thuong
        strncpy(currentTenBSLower, dsBacSi[i].tenBacSi, sizeof(currentTenBSLower) - 1); // Sao chep ten bac si hien tai vao mang
        currentTenBSLower[sizeof(currentTenBSLower) - 1] = '\0';
        for (int k = 0; currentTenBSLower[k]; k++) currentTenBSLower[k] = tolower(currentTenBSLower[k]); // Chuyen ten bac si hien tai ve chu thuong

        if (strcmp(currentTenBSLower, tenBacSiLower) == 0) {
            return (BacSi*)&dsBacSi[i];
        } // So sanh ten bac si hien tai voi ten can tim, neu trung thi tra ve con tro toi bac si
    }
    return NULL;
}

//Ham chuyen cac ki tu ve chu thuong
static void toLowerString(char* dest, const char* src, size_t dest_size) { 
    // Neu src hoac dest NULL hoac dest_size = 0, khong lam gi ca
    if (src == NULL || dest == NULL || dest_size == 0) return;
    size_t i = 0; // Bien dem de duyet qua tung ky tu cua chuoi src
    for (i = 0; src[i] && i < dest_size - 1; ++i) {
        dest[i] = tolower((unsigned char)src[i]); // Chuyen tung ky tu cua src ve chu thuong va luu vao dest
    }
    dest[i] = '\0';
}

// Ham kiem tra ten cua bac si
static int kiemTraTenBacSi(const char* tenDayDu, const char* tenTimKiemLower) {
	if (tenDayDu == NULL || tenTimKiemLower == NULL) return 0; // `Neu tenDayDu hoac tenTimKiemLower NULL, tra ve 0
    char tenDayDuLower[sizeof(((BacSi*)0)->tenBacSi)]; // Mang de luu ten bac si da chuyen ve chu thuong
    toLowerString(tenDayDuLower, tenDayDu, sizeof(tenDayDuLower)); // Chuyen tenDayDu ve chu thuong
    return (strstr(tenDayDuLower, tenTimKiemLower) != NULL); // Tra ve 1 neu tenDayDuLower chua chuoi tenTimKiemLower, nguoc lai tra ve 0
}

// Ham kiem tra chuyen khoa cua bac si 
static int kiemTraChuyenKhoaBacSi(const char* chuyenKhoaBS, const char* chuyenKhoaTimKiemLower) {
	if (chuyenKhoaBS == NULL || chuyenKhoaTimKiemLower == NULL) return 0;// Neu chuyenKhoaBS hoac chuyenKhoaTimKiemLower NULL, tra ve 0
    char chuyenKhoaBSLower[sizeof(((BacSi*)0)->chuyenKhoa)];
    toLowerString(chuyenKhoaBSLower, chuyenKhoaBS, sizeof(chuyenKhoaBSLower));
    return (strstr(chuyenKhoaBSLower, chuyenKhoaTimKiemLower) != NULL);
}

// Ham tim bac si theo SDT
BacSi* timBacSiTheoSDT(const BacSi dsBacSi[], int soLuong, const char* soDienThoai) {
    if (soDienThoai == NULL || strlen(soDienThoai) == 0) return NULL; // Neu soDienThoai NULL hoac rong, tra ve NULL
    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBacSi[i].soDienThoai, soDienThoai) == 0) {
            return (BacSi*)&dsBacSi[i]; // So sanh soDienThoai cua bac si hien tai voi soDienThoai can tim, neu trung thi tra ve con tro toi bac si
        }
    }
    return NULL;
}

// Ham tim bac si theo Email
BacSi* timBacSiTheoEmail(const BacSi dsBacSi[], int soLuong, const char* email) {
    if (email == NULL || strlen(email) == 0) return NULL; // Neu email NULL hoac rong, tra ve NULL
    char emailLower[sizeof(((BacSi*)0)->email)];
    toLowerString(emailLower, email, sizeof(emailLower)); // Chuyen email can tim ve chu thuong

    for (int i = 0; i < soLuong; ++i) {
        char currentEmailLower[sizeof(dsBacSi[i].email)];
        toLowerString(currentEmailLower, dsBacSi[i].email, sizeof(currentEmailLower)); // Chuyen email cua bac si hien tai ve chu thuong

        if (strcmp(currentEmailLower, emailLower) == 0) {
            return (BacSi*)&dsBacSi[i]; // So sanh email cua bac si hien tai voi email can tim, neu trung thi tra ve con tro toi bac si
        }
    }
    return NULL;
}

//Ham tim bac si linear search
void timKiemBacSiTuyenTinh(const BacSi dsBacSi[], int soLuongBacSi,
    const char* tieuChi, const char* giaTriTimKiem,
    BacSi** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL; // Khoi tao con tro ketQuaTimKiem la NULL
    *soLuongKetQua = 0; // Khoi tao soLuongKetQua la 0
    if (soLuongBacSi == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) {
        return; // Neu soLuongBacSi = 0 hoac giaTriTimKiem, tieuChi NULL hoac rong, khong lam gi ca
    }

    BacSi* dsKetQuaTamThoi = (BacSi*)malloc(soLuongBacSi * sizeof(BacSi)); // Cap phat bo nho cho danh sach ket qua tam thoi
    if (dsKetQuaTamThoi == NULL) {
        perror("Loi cap phat bo nho (timKiemBacSiTuyenTinh: dsKetQuaTamThoi)"); // Neu cap phat that bai, in thong bao loi
        return;
    }
    int count = 0;

    char tieuChiLower[50];
    toLowerString(tieuChiLower, tieuChi, sizeof(tieuChiLower)); // Chuyen tieuChi ve chu thuong

    char giaTriTimKiemLower[sizeof(((BacSi*)0)->tenBacSi)];
    toLowerString(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower)); // Chuyen giaTriTimKiem ve chu thuong

    for (int i = 0; i < soLuongBacSi; ++i) {
        int timThayTrongBacSiNay = 0;
        if (strcmp(tieuChiLower, "ten") == 0) { // Neu tieu chi la "ten", kiem tra ten bac si
            if (kiemTraTenBacSi(dsBacSi[i].tenBacSi, giaTriTimKiemLower)) {
                timThayTrongBacSiNay = 1; // Neu ten bac si chua chuoi tim kiem, gan timThayTrongBacSiNay = 1
            }
        }
        else if (strcmp(tieuChiLower, "chuyenkhoa") == 0) { // Neu tieu chi la "chuyenkhoa", kiem tra chuyen khoa bac si
            if (kiemTraChuyenKhoaBacSi(dsBacSi[i].chuyenKhoa, giaTriTimKiemLower)) {
                timThayTrongBacSiNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "sodienthoai") == 0) {  // Neu tieu chi la "sodienthoai", kiem tra so dien thoai bac si
            if (strcmp(dsBacSi[i].soDienThoai, giaTriTimKiem) == 0) {
                timThayTrongBacSiNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "email") == 0) { // Neu tieu chi la "email", kiem tra email bac si
            char currentEmailLower[sizeof(dsBacSi[i].email)]; // Mang de luu email cua bac si hien tai da chuyen ve chu thuong
            toLowerString(currentEmailLower, dsBacSi[i].email, sizeof(currentEmailLower));
            if (strcmp(currentEmailLower, giaTriTimKiemLower) == 0) { // So sanh email cua bac si hien tai voi giaTriTimKiem
                timThayTrongBacSiNay = 1;
            }
        }

        if (timThayTrongBacSiNay) {
            if (count < soLuongBacSi) {
                dsKetQuaTamThoi[count++] = dsBacSi[i];
            }
            // Neu count < soLuongBacSi, gan bac si hien tai vao dsKetQuaTamThoi
        }
    }

    if (count > 0) { // Neu co ket qua tim thay, cap phat bo nho cho ketQuaTimKiem
        *ketQuaTimKiem = (BacSi*)malloc(count * sizeof(BacSi));
        if (*ketQuaTimKiem == NULL) {
            perror("Loi cap phat bo nho (timKiemBacSiTuyenTinh: ketQuaTimKiem)"); // Neu cap phat that bai, in thong bao loi va giai phong bo nho
            free(dsKetQuaTamThoi);
            *soLuongKetQua = 0; // Gan soLuongKetQua ve 0
            return;
        }
        memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(BacSi)); // Sao chep ket qua tim thay vao ketQuaTimKiem va gan soLuongKetQua
        *soLuongKetQua = count;
    }

    free(dsKetQuaTamThoi); // Giai phong bo nho da cap phat cho dsKetQuaTamThoi
}

//Ham nhap thong tin bac si
int nhapThongTinBacSiTuBanPhim(BacSi* bs,
    BangBam* bbBacSiHienCo,
    const BacSi dsBacSiHienCo[], int soLuongBSHienCo,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong) {
    char bufferMaBS[MAX_LEN_MA_BAC_SI];
    char bufferMaKP[MAX_LEN_MA_KHOA_PHONG];
    BacSi* bsTimThay;

    //Nhap ma bac si
    printf("Nhap Ma Bac Si (VD: BS001, toi da %zu ky tu, se duoc chuyen IN HOA): ", sizeof(bs->maBacSi) - 1);
    if (docDongAnToan(bufferMaBS, sizeof(bufferMaBS), stdin) == NULL || strlen(bufferMaBS) == 0) {
        printf("LOI: Ma Bac Si khong duoc de trong. Huy them bac si.\n"); // Neu ma bac si de trong, in thong bao loi va huy them bac si
        return 0;
    }
    for (int i = 0; bufferMaBS[i]; i++) {
        bufferMaBS[i] = toupper(bufferMaBS[i]); // Chuyen ma bac si ve chu hoa
    }
    // Kiem tra ma bac si da ton tai trong danh sach bac si hien co, neu da ton tai thi in thong bao loi va huy them bac si
    if (bbBacSiHienCo != NULL) {
        if (timKiemTrongBangBam(bbBacSiHienCo, bufferMaBS) != NULL) {
            printf("LOI: Ma Bac Si '%s' da ton tai (kiem tra bang bam). Huy them bac si.\n", bufferMaBS);
            return 0;
        }
    }
    else {
        fprintf(stderr, "LOI HE THONG: Bang bam bac si khong kha dung. Khong the kiem tra trung lap MaBS. Huy them bac si.\n"); // Neu bang bam khong kha dung, in thong bao loi he thong
        return 0;
    }
    strncpy(bs->maBacSi, bufferMaBS, sizeof(bs->maBacSi) - 1); // Sao chep ma bac si vao struct BacSi
    bs->maBacSi[sizeof(bs->maBacSi) - 1] = '\0';

    //Nhap ten bac si
    printf("Nhap Ten Bac Si (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(bs->tenBacSi, sizeof(bs->tenBacSi), stdin) == NULL || strlen(bs->tenBacSi) == 0) {
        printf("LOI: Ten Bac Si khong duoc de trong. Huy them bac si.\n"); // Neu ten bac si de trong, in thong bao loi va huy them bac si
        return 0;
    }

    // Nhap chuyen khoa
    printf("Nhap Chuyen Khoa (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(bs->chuyenKhoa, sizeof(bs->chuyenKhoa), stdin) == NULL || strlen(bs->chuyenKhoa) == 0) {
        printf("LOI: Chuyen khoa khong duoc de trong. Huy them bac si.\n"); // Neu chuyen khoa de trong, in thong bao loi va huy them bac si
        return 0;
    }

    //Nhap so dien thoai
    printf("Nhap So Dien Thoai (KHONG DUOC BO TRONG, chi chua so, KHONG TRUNG): ");
    if (docDongAnToan(bs->soDienThoai, sizeof(bs->soDienThoai), stdin) == NULL || strlen(bs->soDienThoai) == 0) {
        printf("LOI: So Dien Thoai khong duoc de trong. Huy them bac si.\n"); // Neu so dien thoai de trong, in thong bao loi va huy them bac si
        return 0;
    }
    for (int i = 0; bs->soDienThoai[i]; ++i) {
        if (!isdigit(bs->soDienThoai[i])) {
            printf("LOI: So Dien Thoai chi duoc chua cac ky tu so. Huy them bac si.\n"); // Neu so dien thoai co ky tu khac so, in thong bao loi va huy them bac si
            return 0;
        }
    }
    bsTimThay = timBacSiTheoSDT(dsBacSiHienCo, soLuongBSHienCo, bs->soDienThoai);
    if (bsTimThay != NULL) {
        printf("LOI: So Dien Thoai '%s' da duoc su dung boi Bac Si MaBS: %s. Huy them bac si.\n", bs->soDienThoai, bsTimThay->maBacSi); // Neu so dien thoai da ton tai, in thong bao loi va huy them bac si
        return 0;
    }

    //Nhap Email
    printf("Nhap Email (vd: ten@domain.com): ");
    if (docDongAnToan(bs->email, sizeof(bs->email), stdin) == NULL || strlen(bs->email) == 0) {
        printf("LOI: Email khong duoc de trong. Huy them bac si.\n"); // Neu email de trong, in thong bao loi va huy them bac si
        return 0;
    }
    if (strchr(bs->email, '@') == NULL || strchr(bs->email, '.') == NULL) {
        printf("LOI: Dinh dang Email khong hop le. Huy them bac si.\n"); // Neu email khong co '@' hoac '.', in thong bao loi va huy them bac si
        return 0;
    }
    bsTimThay = timBacSiTheoEmail(dsBacSiHienCo, soLuongBSHienCo, bs->email);
    if (bsTimThay != NULL) {
        printf("LOI: Email '%s' da duoc su dung boi Bac Si MaBS: %s. Huy them bac si.\n", bs->email, bsTimThay->maBacSi); // Neu email da ton tai, in thong bao loi va huy them bac si
        return 0;
    }

    //Nhap ma khoa phong
    printf("Nhap Ma Khoa Phong: ");
    if (docDongAnToan(bufferMaKP, sizeof(bufferMaKP), stdin) == NULL || strlen(bufferMaKP) == 0) {
        printf("Ma Khoa Phong khong duoc de trong. Huy them bac si.\n"); // Neu ma khoa phong de trong, in thong bao loi va huy them bac si
        return 0;
    }
    for (int i = 0; bufferMaKP[i]; i++) {
        bufferMaKP[i] = toupper(bufferMaKP[i]); // Chuyen ma khoa phong ve chu hoa
    }
    if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferMaKP) == NULL) {
        printf("Ma Khoa Phong '%s' khong ton tai! Huy them bac si.\n", bufferMaKP); // Neu ma khoa phong khong ton tai, in thong bao loi va huy them bac si
        return 0;
    }
    strncpy(bs->maKhoaPhong, bufferMaKP, sizeof(bs->maKhoaPhong) - 1); // Sao chep ma khoa phong vao struct BacSi
    bs->maKhoaPhong[sizeof(bs->maKhoaPhong) - 1] = '\0';

    return 1;
}

// Ham them bac si
int themBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam** bbBacSi_ptr,
    BacSi bacSiMoi, const char* tenFile) {

    BangBam* bbHienTai = *bbBacSi_ptr;

    if (bbHienTai != NULL) {
        if (timKiemTrongBangBam(bbHienTai, bacSiMoi.maBacSi) != NULL) {
            fprintf(stderr, "Ma Bac Si '%s' da ton tai trong bang bam ngay truoc khi them vao mang!\n", bacSiMoi.maBacSi); // Neu ma bac si da ton tai trong bang bam, in thong bao loi
            return 0;
        }
    }
    else {
        fprintf(stderr, "Bang bam bac si khong kha dung khi them bac si '%s'.\n", bacSiMoi.maBacSi); // Neu bang bam khong kha dung, in thong bao loi
        return 0;
    }

    BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro + 1) * sizeof(BacSi)); // Cap phat bo nho cho danh sach bac si
    if (temp == NULL) {
        perror("Loi cap phat bo nho"); // Neu cap phat that bai, in thong bao loi
        return 0;
    }
    *dsBacSiConTro = temp; // Gan con tro dsBacSiConTro ve bo nho moi cap phat
    (*dsBacSiConTro)[*soLuongConTro] = bacSiMoi; // Gan bac si moi vao cuoi danh sach
    BacSi* conTroToiBsTrongMang = &((*dsBacSiConTro)[*soLuongConTro]); // Con tro toi bac si moi trong mang

    if (!chenVaoBangBam(bbHienTai, bacSiMoi.maBacSi, conTroToiBsTrongMang)) {
        fprintf(stderr, "Khong the chen Bac Si '%s' vao bang bam.\n", bacSiMoi.maBacSi); // Neu chen vao bang bam that bai, in thong bao loi
        return 0;
    }

    (*soLuongConTro)++; // Tang so luong con tro trong mang bac si

    if ((*bbBacSi_ptr)->soLuongPhanTu / (double)(*bbBacSi_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {
        printf("He so tai cua bang bam BAC SI vuot nguong (%d/%d). Dang chuan bi rehash...\n", // Neu so luong phan tu tren dung luong cua bang bam vuot qua nguong, thuc hien rehash
            (*bbBacSi_ptr)->soLuongPhanTu, (*bbBacSi_ptr)->dungLuong);
        if (!rehashBangBam(bbBacSi_ptr)) {
            fprintf(stderr, "Rehash bang bam BAC SI that bai. Chuong trinh co the tiep tuc nhung hieu suat se giam.\n"); // Neu rehash that bai, in thong bao loi
        }
    }

    ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro); // Ghi danh sach bac si ra file
    return 1;
}

//Ham sua thong tin bac si
int suaThongTinBacSi(BacSi dsBacSi[], int soLuong, const char* maBacSiCanSua, BacSi thongTinMoi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tenFile) {
    int indexCanSua = -1; // Khoi tao bien de kiem tra xem co tim thay bac si can sua hay khong
    char maCanSuaUpper[MAX_LEN_MA_BAC_SI]; // Mang de luu ma bac si can sua da chuyen ve chu hoa
    strncpy(maCanSuaUpper, maBacSiCanSua, sizeof(maCanSuaUpper) - 1); // Sao chep ma bac si can sua vao mang
    maCanSuaUpper[sizeof(maCanSuaUpper) - 1] = '\0';
    for (int i = 0; maCanSuaUpper[i]; i++) maCanSuaUpper[i] = toupper(maCanSuaUpper[i]); // Chuyen ma bac si can sua ve chu hoa

    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsBacSi[i].maBacSi, maCanSuaUpper) == 0) {
            indexCanSua = i; // So sanh ma bac si hien tai voi ma can sua, neu trung thi gan indexCanSua = i
            break;
        }
    }

    if (indexCanSua == -1) {
        printf("Khong tim thay bac si voi ma '%s' de sua.\n", maBacSiCanSua); // Neu khong tim thay bac si can sua, in thong bao loi
        return 0;
    }

    BacSi* bsTimThay; // Con tro de luu bac si tim thay khi kiem tra trung lap

    //Sua ten bac si
    if (strlen(thongTinMoi.tenBacSi) == 0) {
        printf("Ten Bac Si khong duoc de trong.\n"); return 0; // Neu ten bac si moi de trong, in thong bao loi
    }

    //Sua so dien thoai
    if (strlen(thongTinMoi.soDienThoai) == 0) {
        printf("So Dien Thoai khong duoc de trong.\n"); return 0; // Neu so dien thoai moi de trong, in thong bao loi
    }
    for (int i = 0; thongTinMoi.soDienThoai[i]; ++i) {
        if (!isdigit(thongTinMoi.soDienThoai[i])) {
            printf("So Dien Thoai moi chi duoc chua cac ky tu so.\n"); return 0; // Neu so dien thoai moi co ky tu khac so, in thong bao loi
        }
    }
    bsTimThay = timBacSiTheoSDT(dsBacSi, soLuong, thongTinMoi.soDienThoai);
    if (bsTimThay != NULL && strcmp(bsTimThay->maBacSi, maCanSuaUpper) != 0) {
        printf("Loi: So Dien Thoai '%s' da duoc su dung boi Bac Si MaBS: %s.\n", thongTinMoi.soDienThoai, bsTimThay->maBacSi); // Neu so dien thoai moi da ton tai, in thong bao loi
        return 0;
    }

    //Sua email
    if (strlen(thongTinMoi.email) == 0) {
        printf("Email khong duoc de trong.\n"); return 0; // Neu email moi de trong, in thong bao loi
    }
    if (strchr(thongTinMoi.email, '@') == NULL || strchr(thongTinMoi.email, '.') == NULL) {
        printf("Dinh dang Email moi khong hop le.\n"); return 0; // Neu email moi khong co '@' hoac '.', in thong bao loi
    }
    bsTimThay = timBacSiTheoEmail(dsBacSi, soLuong, thongTinMoi.email);
    if (bsTimThay != NULL && strcmp(bsTimThay->maBacSi, maCanSuaUpper) != 0) {
        printf("Loi: Email '%s' da duoc su dung boi Bac Si MaBS: %s.\n", thongTinMoi.email, bsTimThay->maBacSi); // Neu email moi da ton tai, in thong bao loi
        return 0;
    }

    //Sua ma khoa phong
    char maKPMoiUpper[MAX_LEN_MA_KHOA_PHONG];
    if (strlen(thongTinMoi.maKhoaPhong) == 0) {
        printf("Ma Khoa Phong khong duoc de trong.\n"); return 0; // Neu ma khoa phong moi de trong, in thong bao loi
    }
    strncpy(maKPMoiUpper, thongTinMoi.maKhoaPhong, sizeof(maKPMoiUpper) - 1); // Sao chep ma khoa phong moi vao mang roi chuyen ve chu hoa
    maKPMoiUpper[sizeof(maKPMoiUpper) - 1] = '\0';
    for (int i = 0; maKPMoiUpper[i]; i++) maKPMoiUpper[i] = toupper(maKPMoiUpper[i]);

        if (timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKPMoiUpper) == NULL) {
            printf("Loi: Ma Khoa Phong moi '%s' khong ton tai!\n", maKPMoiUpper); // Neu ma khoa phong moi khong ton tai, in thong bao loi
            return 0;
        }
    strcpy(thongTinMoi.maKhoaPhong, maKPMoiUpper); // Sao chep ma khoa phong moi da chuyen ve chu hoa vao struct BacSi
    // Sao chep thong tin moi vao struct BacSi tai vi tri can sua
    strcpy(dsBacSi[indexCanSua].tenBacSi, thongTinMoi.tenBacSi);
    strcpy(dsBacSi[indexCanSua].chuyenKhoa, thongTinMoi.chuyenKhoa);
    strcpy(dsBacSi[indexCanSua].soDienThoai, thongTinMoi.soDienThoai);
    strcpy(dsBacSi[indexCanSua].email, thongTinMoi.email);
    strcpy(dsBacSi[indexCanSua].maKhoaPhong, thongTinMoi.maKhoaPhong);

    ghiDanhSachBacSiRaFile(tenFile, dsBacSi, soLuong); // Ghi danh sach bac si da sua thong tin ra file
    return 1;
}

//Ham xoa bac si
int xoaBacSi(BacSi** dsBacSiConTro, int* soLuongConTro,
    BangBam* bbBacSi,
    const char* maBacSiCanXoa, const char* tenFile) {
    char maXoaUpper[MAX_LEN_MA_BAC_SI]; // Mang de luu ma bac si can xoa da chuyen ve chu hoa
    // Sao chep ma bac si can xoa vao mang, chuyen ve chu hoa
    strncpy(maXoaUpper, maBacSiCanXoa, sizeof(maXoaUpper) - 1);
    maXoaUpper[sizeof(maXoaUpper) - 1] = '\0';
    for (int k = 0; maXoaUpper[k]; k++) maXoaUpper[k] = toupper(maXoaUpper[k]);

    int removedFromHashTable = 0; // Bien kiem tra xem da xoa khoi bang bam hay chua
    // Neu bang bam bac si khong NULL, ma bac si can xoa ton tai trong bang bam, thuc hien xoa khoi bang bam
    if (bbBacSi != NULL) {
        if (timKiemTrongBangBam(bbBacSi, maXoaUpper) != NULL) {
            if (xoaKhoiBangBam(bbBacSi, maXoaUpper)) {
                removedFromHashTable = 1; // Neu xoa khoi bang bam thanh cong, gan removedFromHashTable = 1
            }
            else {
                fprintf(stderr, " Khong the xoa Bac Si '%s' mac du da tim thay.\n", maXoaUpper); // Neu xoa khoi bang bam that bai, in thong bao loi
                return 0;
            }
        }
    }
    else {
        fprintf(stderr, "Bac si khong kha dung khi xoa.\n"); // Neu bang bam bac si NULL, in thong bao loi
        return 0;
    }

    int viTriCanXoa = -1; // Khoi tao vi tri can xoa la -1, neu tim thay thi se gan lai
    for (int i = 0; i < *soLuongConTro; ++i) {
        if (strcmp((*dsBacSiConTro)[i].maBacSi, maXoaUpper) == 0) {
            viTriCanXoa = i; // So sanh ma bac si hien tai voi ma can xoa, neu trung thi gan viTriCanXoa = i
            break;
        }
    }
    // Neu tim thay vi tri can xoa trong mang, neu khong xoa khoi bang bam va ma bac si can xoa khong ton tai trong bang bam, in thong bao loi
    if (viTriCanXoa != -1) {
        if (!removedFromHashTable && timKiemTrongBangBam(bbBacSi, maXoaUpper) == NULL) {
            fprintf(stderr, "Bac Si '%s' tim thay trong mang nhung khong co trong bang bam truoc khi xoa khoi mang.\n", maXoaUpper);
        }

        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) { // Dich chuyen cac phan tu sau vi tri can xoa ve truoc
            (*dsBacSiConTro)[i] = (*dsBacSiConTro)[i + 1];
        }
        (*soLuongConTro)--; // Giam so luong con tro trong mang bac si

        if (*soLuongConTro == 0) {
            free(*dsBacSiConTro); // Neu so luong con tro = 0, giai phong bo nho
            *dsBacSiConTro = NULL; // Gan con tro dsBacSiConTro ve NULL
        }
        else {
            BacSi* temp = realloc(*dsBacSiConTro, (*soLuongConTro) * sizeof(BacSi)); // Cap phat lai bo nho cho danh sach bac si
            if (temp == NULL && *soLuongConTro > 0) { // Neu cap phat that bai va so luong con tro > 0, in thong bao loi
                perror("Loi khi xoa bac si");
            }
            else if (*soLuongConTro > 0) { // Neu cap phat thanh cong va so luong con tro > 0, gan con tro dsBacSiConTro ve bo nho moi
                *dsBacSiConTro = temp;
            }
        }
        ghiDanhSachBacSiRaFile(tenFile, *dsBacSiConTro, *soLuongConTro); // Ghi danh sach bac si da xoa ra file
        return 1;
    }
    //
    if (removedFromHashTable && viTriCanXoa == -1) { // Neu da xoa khoi bang bam nhung khong tim thay trong mang, in thong bao loi
        fprintf(stderr, "Da xoa Bac Si '%s' khoi bang bam nhung khong tim thay trong mang.\n", maXoaUpper);
        return 0;
    }

    printf("Khong tim thay bac si voi ma '%s' trong he thong de xoa.\n", maXoaUpper); // Neu khong tim thay bac si can xoa trong mang, in thong bao loi
    return 0;
}

// Ham in danh sach bac si
void inDanhSachBacSiConsole(const BacSi dsBacSi[], int soLuong) {
    printf("\n--- DANH SACH BAC SI ---\n"); // In tieu de danh sach bac si
    if (soLuong == 0) {
        printf("Khong co bac si nao trong danh sach.\n"); // Neu so luong bac si = 0, in thong bao khong co bac si
        return;
    }
    // In tieu de bang
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n", "STT", "Ma BS", "Ten Bac Si", "Chuyen Khoa", "So Dien Thoai", "Email", "Ma Khoa Phong");
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < soLuong; ++i) {
        printf("| %-5d | %-10s | %-30s | %-20s | %-15s | %-25s | %-20s |\n",
            i + 1, dsBacSi[i].maBacSi, dsBacSi[i].tenBacSi, dsBacSi[i].chuyenKhoa,
            dsBacSi[i].soDienThoai, dsBacSi[i].email, dsBacSi[i].maKhoaPhong);
    } // In thong tin tung bac si trong danh sach
    printf("---------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

// Ham tim bac si theo ma trong bang bam
BacSi* timBacSiTheoMaBangBam(BangBam* bbBacSi, const char* maBacSi) {
    if (bbBacSi == NULL || maBacSi == NULL) { // Neu bang bam bac si NULL hoac ma bac si NULL, tra ve NULL
        return NULL;
    }
    char maBSTimKiem[MAX_LEN_MA_BAC_SI];
    strncpy(maBSTimKiem, maBacSi, sizeof(maBSTimKiem) - 1); // Sao chep ma bac si can tim vao mang
    maBSTimKiem[sizeof(maBSTimKiem) - 1] = '\0';
    for (int k = 0; maBSTimKiem[k]; k++) {
        maBSTimKiem[k] = toupper(maBSTimKiem[k]); // Chuyen ma bac si can tim ve chu hoa
    }
    return (BacSi*)timKiemTrongBangBam(bbBacSi, maBSTimKiem); // Tra ve con tro toi bac si tim thay trong bang bam, neu khong tim thay thi tra ve NULL
}