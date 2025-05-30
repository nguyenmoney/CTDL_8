#include "department.h"
#include "csv_handler.h"
#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham tien ich loai bo xoa ky tu '\n' o cuoi chuoi
static void removeNewlineDept(char* str) {
    if (str == NULL) return; // Neu chuoi NULL, khong lam gi ca
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') { // Neu ky tu cuoi cung la '\n', thuc hien xoa
        str[len - 1] = '\0'; // Thay the '\n' bang ky tu ket thuc chuoi '\0'
    }
}

// Ham doc danh sach khoa phong tu file CSV
// Khong bo qua dong tieu de
KhoaPhong* docDanhSachKhoaPhong(const char* tenFile, int* soLuong) {
    FILE* f = fopen(tenFile, "r"); // Mo file de doc
    if (f == NULL) {
        *soLuong = 0; // Khong co khoa phong nao, tra ve NULL
        return NULL;
    }

    KhoaPhong* ds = NULL; // Con tro toi danh sach khoa phong
    int capacity = 0; // Dung luong hien tai cua mang
    *soLuong = 0; // So luong khoa phong hien tai
    char line[500]; // Mang de luu tung dong doc tu file
    char* token; // Con tro de luu tung token khi tach chuoi

    while (fgets(line, sizeof(line), f) != NULL) {
        removeNewlineDept(line);
        if (strlen(line) < 3) continue; // Neu dong qua ngan, bo qua

        if (*soLuong >= capacity) { // Neu so luong hien tai >= dung luong hien tai
            capacity = (capacity == 0) ? 5 : capacity * 2; // Neu dung luong bang 0, dat la 5, neu khac thi nhan doi
            KhoaPhong* temp = realloc(ds, capacity * sizeof(KhoaPhong)); // Cap phat lai bo nho
            if (temp == NULL) {
                perror("Loi cap phat bo nho (realloc docDanhSachKhoaPhong)"); // Neu khong cap phat duoc, bao loi
                free(ds); // Giai phong bo nho da cap phat
                fclose(f); // Dong file
                *soLuong = 0; // Tra ve so luong 0
                return NULL;
            }
            ds = temp; // Cap nhat con tro toi mang moi
        }

        KhoaPhong kp;
        token = strtok(line, ","); // Tach chuoi theo dau phay
        if (token && strlen(token) > 0) { // Ma khoa phong khong duoc rong tu file
			strncpy(kp.maKhoaPhong, token, sizeof(kp.maKhoaPhong) - 1); kp.maKhoaPhong[sizeof(kp.maKhoaPhong) - 1] = '\0';// Chuyen ma khoa phong ve chu hoa
		}// Neu ma khoa phong hop le, sao chep vao struct
        else {
            fprintf(stderr, "CANH BAO (docDanhSachKhoaPhong): Dong khong co maKhoaPhong hop le. Bo qua dong.\n");
            continue; // Bo qua ma khoa phong neu khong hop le
        }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { // Ten khoa phong khong duoc rong tu file
			strncpy(kp.tenKhoaPhong, token, sizeof(kp.tenKhoaPhong) - 1); kp.tenKhoaPhong[sizeof(kp.tenKhoaPhong) - 1] = '\0';// Sao chep ten khoa phong vao struct
        }
        else {
            fprintf(stderr, "Dong co maKhoaPhong '%s' khong co tenKhoaPhong hop le. Bo qua dong.\n", kp.maKhoaPhong);
            continue; // Bo qua dong neu ten khoa phong khong hop le
        }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) { // Vi tri khong duoc rong tu file
			strncpy(kp.viTri, token, sizeof(kp.viTri) - 1); kp.viTri[sizeof(kp.viTri) - 1] = '\0';// Sao chep vi tri vao struct
        }
        else {
            fprintf(stderr, "Dong co maKhoaPhong '%s' khong co viTri hop le. Bo qua dong.\n", kp.maKhoaPhong);
            continue; // Bo qua dong neu vi tri khong hop le
        }

        token = strtok(NULL, "\n");
        if (token && strlen(token) > 0) {
			strncpy(kp.moTa, token, sizeof(kp.moTa) - 1); kp.moTa[sizeof(kp.moTa) - 1] = '\0';// Sao chep mo ta vao struct
        }
        else {
            strcpy(kp.moTa, "Khong co mo ta"); // Gan gia tri mac dinh neu mo ta trong tu file
        }

        ds[*soLuong] = kp; // Luu khoa phong vao mang
        (*soLuong)++; // Tang so luong khoa phong
    }

    fclose(f);
    return ds;
}

// Ham ghi danh sach khoa phong ra file CSV
// Khong ghi dong tieu de, chi ghi du lieu
void ghiDanhSachKhoaPhongRaFile(const char* tenFile, const KhoaPhong dsKhoaPhong[], int soLuong) {
    FILE* f = fopen(tenFile, "w"); // Mo file o che do ghi
    if (f == NULL) {
        perror("Loi mo file de ghi (ghiDanhSachKhoaPhongRaFile)"); // Bao loi neu khong mo duoc file
        return;
    }
    // Ghi du lieu vao file
    for (int i = 0; i < soLuong; ++i) {
        fprintf(f, "%s,%s,%s,%s\n",
            dsKhoaPhong[i].maKhoaPhong,
            dsKhoaPhong[i].tenKhoaPhong,
            dsKhoaPhong[i].viTri,
            strlen(dsKhoaPhong[i].moTa) == 0 ? "Khong co mo ta" : dsKhoaPhong[i].moTa); // Neu mo ta rong, ghi gia tri mac dinh 'Khong co mo ta'
    }
    fclose(f);
}

// Ham tim kiem khoa phong theo ma ( Tim kiem tuyen tinh )
KhoaPhong* timKhoaPhongTheoMa(const KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhong) {
    if (maKhoaPhong == NULL) return NULL; // Neu ma khoa phong rong, tra ve NULL
	char maKPTimKiemUpper[MAX_LEN_MA_KHOA_PHONG];// Ma khoa phong tim kiem da chuyen ve chu hoa
	strncpy(maKPTimKiemUpper, maKhoaPhong, sizeof(maKPTimKiemUpper) - 1);// Sao chep ma khoa phong vao mang
	maKPTimKiemUpper[sizeof(maKPTimKiemUpper) - 1] = '\0';// Bao dam ket thuc chuoi
    for (int i = 0; maKPTimKiemUpper[i]; i++) maKPTimKiemUpper[i] = toupper(maKPTimKiemUpper[i]); // Chuyen ma khoa phong tim kiem ve chu hoa

	for (int i = 0; i < soLuong; ++i) {// Duyet qua tung khoa phong trong danh sach
        if (strcmp(dsKhoaPhong[i].maKhoaPhong, maKPTimKiemUpper) == 0) {
            return (KhoaPhong*)&dsKhoaPhong[i]; // So sanh ma khoa phong, neu trung thi tra ve con tro toi khoa phong
        }
    }
    return NULL;
}

// Tim khoa phong theo ma trong bang bam
KhoaPhong* timKhoaPhongTheoMaBangBam(BangBam* bbKhoaPhong, const char* maKhoaPhong) {
    if (bbKhoaPhong == NULL || maKhoaPhong == NULL) { // Neu bang bam hoac ma khoa phong rong, tra ve NULL
        return NULL;
    }
	char maKPTimKiem[MAX_LEN_MA_KHOA_PHONG];// Mang de luu ma khoa phong tim kiem da chuyen ve chu hoa
	strncpy(maKPTimKiem, maKhoaPhong, sizeof(maKPTimKiem) - 1);// Sao chep ma khoa phong vao mang
	maKPTimKiem[sizeof(maKPTimKiem) - 1] = '\0';// Bao dam ket thuc chuoi
	for (int k = 0; maKPTimKiem[k]; k++) {// Duyet qua tung ky tu cua ma khoa phong
        maKPTimKiem[k] = toupper(maKPTimKiem[k]); // Chuyen ma khoa phong tim kiem ve chu hoa
    }
    return (KhoaPhong*)timKiemTrongBangBam(bbKhoaPhong, maKPTimKiem); // Tim kiem trong bang bam
}

//  Ham nhap thong tin khoa phong tu ban phim
int nhapThongTinKhoaPhongTuBanPhim(KhoaPhong* kp,
    BangBam* bbKhoaPhongHienCo,
    const KhoaPhong dsKhoaPhongHienCo[], int soLuongHienCo) {
	char bufferMa[MAX_LEN_MA_KHOA_PHONG];// Mang de luu ma khoa phong nhap vao
    printf("Nhap Ma Khoa Phong (VD: KHOA_TIMMACH, toi da %zu ky tu, IN HOA, KHONG DUOC BO TRONG): ", sizeof(kp->maKhoaPhong) - 1); // Nhap ma khoa phong
    if (docDongAnToan(bufferMa, sizeof(bufferMa), stdin) == NULL || strlen(bufferMa) == 0) {
        printf("LOI: Ma Khoa Phong khong duoc de trong. Huy them khoa phong.\n"); // Neu ma khoa phong rong, bao loi
        return 0;
    }
    for (int i = 0; bufferMa[i]; i++) {
        bufferMa[i] = toupper(bufferMa[i]); // Chuyen ma khoa phong ve chu hoa
    }

    if (strlen(bufferMa) == 0) { // Kiem tra ma su khi chuan hoa
        printf("LOI: Ma Khoa Phong khong duoc de trong sau khi chuan hoa. Huy them khoa phong.\n");
        return 0;
    }

    if (bbKhoaPhongHienCo != NULL) {
        if (timKiemTrongBangBam(bbKhoaPhongHienCo, bufferMa) != NULL) {// Kiem tra trung lap trong bang bam
            printf("LOI: Ma Khoa Phong '%s' da ton tai (kiem tra bang bam). Huy them khoa phong.\n", bufferMa);
            return 0;
        }
    }
    else { // Neu bang bam hien tai NULL, bao loi he thong
        fprintf(stderr, "LOI HE THONG: Bang bam khoa phong khong kha dung. Khong the kiem tra trung lap MaKhoaPhong. Huy them khoa phong.\n");
        return 0;
    }
    strncpy(kp->maKhoaPhong, bufferMa, sizeof(kp->maKhoaPhong) - 1); // Sao chep ma khoa phong vao struct
    kp->maKhoaPhong[sizeof(kp->maKhoaPhong) - 1] = '\0';

	//Nhap ten khoa phong
    printf("Nhap Ten Khoa Phong (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(kp->tenKhoaPhong, sizeof(kp->tenKhoaPhong), stdin) == NULL || strlen(kp->tenKhoaPhong) == 0) {
        printf("LOI: Ten Khoa Phong khong duoc de trong. Huy them khoa phong.\n"); // Neu nhap ten khoa phong rong, bao loi va huy them khoa phong
        return 0;
    }

	//Nhap vi tri khoa phong
    printf("Nhap Vi Tri (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(kp->viTri, sizeof(kp->viTri), stdin) == NULL || strlen(kp->viTri) == 0) {
        printf("LOI: Vi tri khong duoc de trong. Huy them khoa phong.\n"); // Neu nhap vi tri rong, bao loi va huy them khoa phong
        return 0;
    }

	//Nhap mo ta khoa phong
    printf("Nhap Mo Ta (neu bo trong se la 'Khong co mo ta'): ");
    if (docDongAnToan(kp->moTa, sizeof(kp->moTa), stdin) == NULL || strlen(kp->moTa) == 0) {
        strcpy(kp->moTa, "Khong co mo ta"); // Neu mo ta rong, gan gia tri mac dinh
    }
    return 1;
}
static void dept_toLowerString(char* dest, const char* src, size_t dest_size) {
    if (src == NULL || dest == NULL || dest_size == 0) return; //Neu chuoi nguon NULL, chuoi dich NULL hoac kich thuoc dich 0, khong lam gi ca
    size_t i = 0; // Bien dem de duyet qua tung ky tu cua chuoi src
    for (i = 0; src[i] && i < dest_size - 1; ++i) {
        dest[i] = tolower((unsigned char)src[i]); // Chuyen tung ky tu src ve chu thuong va luu vao dest
    }
    dest[i] = '\0';
}

// Ham kiem tra mot chuoi co chua chuoi con khac khong (khong phan biet hoa thuong)
static int kiemTraChuoiConKhongPhanBietHoaThuong(const char* chuoiGoc, const char* chuoiTimKiem) {
    if (chuoiGoc == NULL || chuoiTimKiem == NULL) return 0;

	char chuoiGocLower[201]; // Mang de luu chuoi goc da chuyen ve chu thuong
	char chuoiTimKiemLower[201]; // Mang de luu chuoi tim kiem da chuyen ve chu thuong

    dept_toLowerString(chuoiGocLower, chuoiGoc, sizeof(chuoiGocLower)); // Chuyen chuoi goc ve chu thuong
    dept_toLowerString(chuoiTimKiemLower, chuoiTimKiem, sizeof(chuoiTimKiemLower)); // Chuyen chuoi tim kiem ve chu thuong

    return (strstr(chuoiGocLower, chuoiTimKiemLower) != NULL); // Su dung strstr de kiem tra chuoi con co ton tai trong chuoi goc hay khong
}
// Ham tim kiem khoa phong theo tieu chi va gia tri tim kiem
void timKiemKhoaPhongTuyenTinh(const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    const char* tieuChi, const char* giaTriTimKiem,
    KhoaPhong** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL;
    *soLuongKetQua = 0;
    if (soLuongKhoaPhong == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) { // Neu danh sach khoa phong rong, gia tri tim kiem va tieu chi rong, tra ve NULL
        return;
    }

    KhoaPhong* dsKetQuaTamThoi = (KhoaPhong*)malloc(soLuongKhoaPhong * sizeof(KhoaPhong)); // Cap phat bo nho cho danh sach ket qua tam thoi
    if (dsKetQuaTamThoi == NULL) {
        perror("Loi cap phat bo nho"); // Neu khong cap phat duoc, bao loi va tra ve NULL
        return;
    }
    int count = 0;

    char tieuChiLower[50];
	dept_toLowerString(tieuChiLower, tieuChi, sizeof(tieuChiLower));// Chuyen tieu chi ve chu thuong de so sanh khong phan biet hoa thuong

	for (int i = 0; i < soLuongKhoaPhong; ++i) {// Duyet qua tung khoa phong trong danh sach
        int timThayTrongKhoaPhongNay = 0;
        if (strcmp(tieuChiLower, "tenkhoaphong") == 0) { // Ham tim kiem theo ten khoa phong
            if (kiemTraChuoiConKhongPhanBietHoaThuong(dsKhoaPhong[i].tenKhoaPhong, giaTriTimKiem)) {
                timThayTrongKhoaPhongNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "mota") == 0) { // Ham tim kiem theo mo ta
            if (kiemTraChuoiConKhongPhanBietHoaThuong(dsKhoaPhong[i].moTa, giaTriTimKiem)) {
                timThayTrongKhoaPhongNay = 1;
            }
        }
        else if (strcmp(tieuChiLower, "vitri") == 0) { // Ham tim kiem theo vi tri
            if (kiemTraChuoiConKhongPhanBietHoaThuong(dsKhoaPhong[i].viTri, giaTriTimKiem)) {
                timThayTrongKhoaPhongNay = 1;
            }
        }

        if (timThayTrongKhoaPhongNay) {
            if (count < soLuongKhoaPhong) { // Neu count chua vuot qua so luong khoa phong hien tai, luu vao danh sach ket qua tam thoi
                dsKetQuaTamThoi[count++] = dsKhoaPhong[i];
            }
        }
    }

    if (count > 0) {
        *ketQuaTimKiem = (KhoaPhong*)malloc(count * sizeof(KhoaPhong)); // Cap phat bo nho cho ket qua tim kiem
        if (*ketQuaTimKiem == NULL) {
            perror("Loi cap phat bo nho (timKiemKhoaPhongTuyenTinh: ketQuaTimKiem)"); // Neu khong cap phat duoc, bao loi va tra ve NULL
            free(dsKetQuaTamThoi); // Giai phong bo nho tam thoi
            *soLuongKetQua = 0; // Tra ve so luong ket qua 0
            return;
        }
        memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(KhoaPhong)); // Sao chep danh sach ket qua tam thoi vao ket qua tim kiem
        *soLuongKetQua = count; // Cap nhat so luong ket qua tim kiem
    }

    free(dsKetQuaTamThoi); // Giai phong bo nho tam thoi
}
// Ham them khoa phong vao danh sach khoa phong va bang bam
int themKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam** bbKhoaPhong_ptr,
    KhoaPhong khoaPhongMoi, const char* tenFile) {
    BangBam* bbHienTai = *bbKhoaPhong_ptr;

    if (bbHienTai != NULL) {
        if (timKiemTrongBangBam(bbHienTai, khoaPhongMoi.maKhoaPhong) != NULL) { // Ham kiem tra xem ma khoa phong co ton tai trong bang bam hay khong
            fprintf(stderr, "LOI NGHIEM TRONG: Ma Khoa Phong '%s' da ton tai trong bang bam (logic themKhoaPhong)!\n", khoaPhongMoi.maKhoaPhong);
            return 0;
        }
    }
    else { // Neu bang bam hien tai NULL, bao loi he thong
        fprintf(stderr, "LOI HE THONG: Bang bam khoa phong khong kha dung (NULL) khi them khoa phong '%s'.\n", khoaPhongMoi.maKhoaPhong);
        return 0;
    }

    KhoaPhong* temp = realloc(*dsKhoaPhongConTro, (*soLuongConTro + 1) * sizeof(KhoaPhong)); // Cap phat bo nho cho mang khoa phong
    if (temp == NULL) {
        perror("Loi cap phat bo nho (realloc themKhoaPhong)"); // Neu khong cap phat duoc, bao loi
        return 0;
    }
    *dsKhoaPhongConTro = temp; // Cap nhat con tro toi mang moi
    (*dsKhoaPhongConTro)[*soLuongConTro] = khoaPhongMoi; // Sao chep khoa phong moi vao vi tri cuoi cung cua mang
    KhoaPhong* conTroToiKPTrongMang = &((*dsKhoaPhongConTro)[*soLuongConTro]); // Con tro toi khoa phong moi trong mang

    if (!chenVaoBangBam(bbHienTai, khoaPhongMoi.maKhoaPhong, conTroToiKPTrongMang)) {
        fprintf(stderr, "LOI NGHIEM TRONG: Khong the chen Khoa Phong '%s' vao bang bam.\n", khoaPhongMoi.maKhoaPhong); // Neu chen vao bang bam that bai, bao loi
        return 0;
    }

    (*soLuongConTro)++; // Thanh cong them khoa phong thi tang so luong khoa phong
    // Neu so luong phan tu tren bang bam vuot qua nguong thi thong bao chuan bi thuc hien rehash
    if ((*bbKhoaPhong_ptr)->soLuongPhanTu / (double)(*bbKhoaPhong_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {
        printf("Thong bao: He so tai cua bang bam KHOA PHONG vuot nguong (%d/%d). Dang chuan bi rehash...\n",
            (*bbKhoaPhong_ptr)->soLuongPhanTu, (*bbKhoaPhong_ptr)->dungLuong);
        if (!rehashBangBam(bbKhoaPhong_ptr)) {
            fprintf(stderr, "LOI: Rehash bang bam KHOA PHONG that bai. Chuong trinh co the tiep tuc nhung hieu suat se giam.\n"); // Bao loi neu rehash that bai
        }
    }

    ghiDanhSachKhoaPhongRaFile(tenFile, *dsKhoaPhongConTro, *soLuongConTro); // Ghi danh sach khoa phong ra file
    return 1;
}

// Ham sua thong tin khoa phong
int suaThongTinKhoaPhong(KhoaPhong dsKhoaPhong[], int soLuong, const char* maKhoaPhongCanSua, KhoaPhong thongTinMoi, const char* tenFile) {
    int indexCanSua = -1; // Bien luu vi tri khoa phong can sua
    char maCanSuaUpper[MAX_LEN_MA_KHOA_PHONG]; // Mang de luu ma khoa phong can sua da chuyen ve chu hoa
    strncpy(maCanSuaUpper, maKhoaPhongCanSua, sizeof(maCanSuaUpper) - 1); // Sao chep ma khoa phong can sua vao mang
    maCanSuaUpper[sizeof(maCanSuaUpper) - 1] = '\0';
    for (int i = 0; maCanSuaUpper[i]; i++) maCanSuaUpper[i] = toupper(maCanSuaUpper[i]); // Chuyen ma khoa phong can sua ve chu hoa

    for (int i = 0; i < soLuong; ++i) {
        if (strcmp(dsKhoaPhong[i].maKhoaPhong, maCanSuaUpper) == 0) { // So sanh ma khoa phong can sua voi ma khoa phong trong danh sach, neu trung thi luu vi tri
            indexCanSua = i;
            break;
        }
    }

    if (indexCanSua == -1) {
        printf("Khong tim thay khoa phong voi ma '%s' de sua.\n", maKhoaPhongCanSua); // Neu khong tim thay khoa phong can sua, bao loi
        return 0;
    }

    // Kiem tra cac truong bat buoc cua thongTinMoi
	if (strlen(thongTinMoi.tenKhoaPhong) == 0) {// Neu ten khoa phong moi rong, bao loi
        printf("LOI: Ten Khoa Phong moi khong duoc de trong. Sua that bai.\n");
        return 0;
    }

	if (strlen(thongTinMoi.viTri) == 0) {// Neu vi tri moi rong, bao loi
        printf("LOI: Vi tri moi khong duoc de trong. Sua that bai.\n");
        return 0;
    }
	if (strlen(thongTinMoi.moTa) == 0) {// Neu mo ta moi rong, gan gia tri mac dinh la "Khong co mo ta"
        strcpy(thongTinMoi.moTa, "Khong co mo ta");
    }

    // Ma Khoa Phong khong duoc sua, chi sua cac truong con lai
    strcpy(dsKhoaPhong[indexCanSua].tenKhoaPhong, thongTinMoi.tenKhoaPhong);
    strcpy(dsKhoaPhong[indexCanSua].viTri, thongTinMoi.viTri);
    strcpy(dsKhoaPhong[indexCanSua].moTa, thongTinMoi.moTa);

    ghiDanhSachKhoaPhongRaFile(tenFile, dsKhoaPhong, soLuong); // Ghi danh sach khoa phong da sua ra file
    return 1;
}

// Ham xoa khoa phong khoi danh sach khoa phong va bang bam
int xoaKhoaPhong(KhoaPhong** dsKhoaPhongConTro, int* soLuongConTro,
    BangBam* bbKhoaPhong,
    const char* maKhoaPhongCanXoa, const char* tenFile) {
    char maXoaUpper[MAX_LEN_MA_KHOA_PHONG]; // Mang de luu ma khoa phong can xoa da chuyen ve chu hoa
    strncpy(maXoaUpper, maKhoaPhongCanXoa, sizeof(maXoaUpper) - 1); // Sao chep ma khoa phong can xoa vao mang
    maXoaUpper[sizeof(maXoaUpper) - 1] = '\0';
    for (int k = 0; maXoaUpper[k]; k++) maXoaUpper[k] = toupper(maXoaUpper[k]); // Chuyen ma khoa phong can xoa ve chu hoa

    int removedFromHashTable = 0;
    if (bbKhoaPhong != NULL) { // Neu bang bam khoa phong khong NULL, kiem tra xem ma khoa phong can xoa co ton tai trong bang bam khong
        if (timKiemTrongBangBam(bbKhoaPhong, maXoaUpper) != NULL) {
			if (xoaKhoiBangBam(bbKhoaPhong, maXoaUpper)) { // Neu xoa thanh cong khoi bang bam, cap nhat bien removedFromHashTable
                removedFromHashTable = 1;
            }
            else {
                fprintf(stderr, "LOI: Khong the xoa Khoa Phong '%s' khoi bang bam mac du da tim thay.\n", maXoaUpper); // Neu xoa that bai, bao loi
                return 0;
            }
        }
    }
    else {
        fprintf(stderr, "LOI HE THONG: Bang bam khoa phong khong kha dung khi xoa.\n"); // Neu bang bam khoa phong NULL, bao loi he thong
        return 0;
    }

    int viTriCanXoa = -1;
    for (int i = 0; i < *soLuongConTro; ++i) {
        if (strcmp((*dsKhoaPhongConTro)[i].maKhoaPhong, maXoaUpper) == 0) { // So sanh ma khoa phong can xoa voi ma khoa phong trong danh sach, neu trung thi luu vi tri
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        if (!removedFromHashTable && timKiemTrongBangBam(bbKhoaPhong, maXoaUpper) == NULL) { // Neu khong xoa khoi bang bam va ma khoa phong can xoa khong ton tai trong bang bam, bao loi
            fprintf(stderr, "CANH BAO: Khoa Phong '%s' tim thay trong mang nhung khong co trong bang bam truoc khi xoa khoi mang.\n", maXoaUpper);
        }

        for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
            (*dsKhoaPhongConTro)[i] = (*dsKhoaPhongConTro)[i + 1]; // Dich trai cac khoa phong sau vi tri can xoa
        }
        (*soLuongConTro)--; // Giam so luong khoa phong di 1

        if (*soLuongConTro == 0) {
            free(*dsKhoaPhongConTro); // Neu so luong khoa phong con lai bang 0, giai phong bo nho
            *dsKhoaPhongConTro = NULL; // Cap nhat con tro ve NULL de tranh viec truy cap vao bo nho da giai phong
        }
        else {
            KhoaPhong* temp = realloc(*dsKhoaPhongConTro, (*soLuongConTro) * sizeof(KhoaPhong)); // Cap phat lai bo nho cho mang khoa phong
            if (temp == NULL && *soLuongConTro > 0) {
                perror("Loi realloc khi xoa khoa phong (thu nho mang)"); // Neu khong cap phat duoc, bao loi
            }
            else if (*soLuongConTro > 0) {
                *dsKhoaPhongConTro = temp; // Cap nhat con tro toi mang moi
            }
        }
        ghiDanhSachKhoaPhongRaFile(tenFile, *dsKhoaPhongConTro, *soLuongConTro); // Ghi danh sach khoa phong da xoa ra file
        return 1;
    }

    if (removedFromHashTable && viTriCanXoa == -1) {
        fprintf(stderr, "LOI DONG BO: Da xoa Khoa Phong '%s' khoi bang bam nhung khong tim thay trong mang.\n", maXoaUpper); // Neu da xoa khoi bang bam nhung khong tim thay trong mang, bao loi dong bo
        return 0;
    }

    printf("Khong tim thay khoa phong voi ma '%s' trong he thong de xoa.\n", maXoaUpper); // Neu khong tim thay khoa phong can xoa trong mang, bao loi
    return 0;
}

// Ham in danh sach khoa phong ra console
void inDanhSachKhoaPhongConsole(const KhoaPhong dsKhoaPhong[], int soLuong) {
    printf("\n--- DANH SACH KHOA PHONG ---\n"); // In tieu de
    if (soLuong == 0) {
        printf("Khong co khoa phong nao trong danh sach.\n"); // Neu danh sach khoa phong rong, bao thong bao
        return;
    }
    // In tieu de bang
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-30s | %-20s | %-30s |\n", "STT", "Ma Khoa Phong", "Ten Khoa Phong", "Vi Tri", "Mo Ta");
    printf("-------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuong; ++i) {
        char moTaDisplay[31]; // 30 chars + null
        rutGonChuoiHienThi(moTaDisplay, sizeof(moTaDisplay), dsKhoaPhong[i].moTa, 30); // Su dung csv_handler
        // In thong tin khoa phong theo dinh dang bang
        printf("| %-5d | %-20s | %-30s | %-20s | %-30s |\n",
            i + 1,
            dsKhoaPhong[i].maKhoaPhong,
            dsKhoaPhong[i].tenKhoaPhong,
            dsKhoaPhong[i].viTri,
            moTaDisplay);
    }
    printf("-------------------------------------------------------------------------------------------------------------------------\n");
}