#include "prescription.h"
#include "medicine.h"    
#include "csv_handler.h" 
#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham noi bo de xoa ki tu xuong dong cuoi cung trong chuoi
static void xoaDongDonThuoc(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Ham doc danh sach don thuoc tu file CSV
DonThuocChiTiet* docDanhSachDonThuocChiTiet(const char* tenFile, int* soLuongDongConTro) {
    FILE* f = fopen(tenFile, "r"); // Mo file o che do doc 'r'
    if (f == NULL) {
        *soLuongDongConTro = 0; // Neu mo file that bai, tra ve 0 dong
        return NULL;
    }
    DonThuocChiTiet* ds = NULL; // Con tro toi danh sach don thuoc chi tiet
	int capacity = 0; // Dung luong hien tai cua danh sach
    *soLuongDongConTro = 0; // Khoi tao soLuongDongConTro ve 0
    char line[512]; // Mang de luu tung dong doc duoc
    int lineNumber = 0; // Bien dem so dong da doc

    while (fgets(line, sizeof(line), f) != NULL) {
        lineNumber++;
        xoaDongDonThuoc(line);
        if (strlen(line) < 5) continue; // Bo qua dong rong hoac qua ngan

        if (*soLuongDongConTro >= capacity) { // Neu soLuongDongConTro >= capacity, cap nhat capacity 
            capacity = (capacity == 0) ? 20 : capacity * 2;// tang dung luong len gap 2
            DonThuocChiTiet* temp = realloc(ds, capacity * sizeof(DonThuocChiTiet)); // Cap phat lai bo nho cho danh sach don thuoc chi tiet
            if (temp == NULL) {
                perror("Loi cap phat bo nho"); // Neu realloc that bai, tra ve NULL
                free(ds); fclose(f); *soLuongDongConTro = 0; return NULL; // Giai phong bo nho da cap phat, dong file va tra ve NULL
            }
            ds = temp; // Cap nhat con tro ds toi bo nho moi
        }
        DonThuocChiTiet dtct; // Bien tam de luu tru thong tin don thuoc chi tiet

        char* token; // Bien tam de luu tru tung token khi tach chuoi

        token = strtok(line, ","); // Tach chuoi theo dau phay
        if (token && strlen(token) > 0) dtct.maDonThuocChiTiet = atoi(token); // Chuyen doi chuoi sang so nguyen
        else { // Neu ma don thuoc chi tiet rong, bo qua dong
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Ma don thuoc chi tiet khong hop le. Bo qua dong.\n", lineNumber);
            continue;
        }
        
        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {  // Ma thuoc khong duoc rong tu file
            strncpy(dtct.maThuoc, token, sizeof(dtct.maThuoc) - 1);
            dtct.maThuoc[sizeof(dtct.maThuoc) - 1] = '\0';
        }
        else { // Neu ma thuoc rong, bo qua dong
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Ma thuoc rong cho MaDon %d. Bo qua dong.\n", lineNumber, dtct.maDonThuocChiTiet);
            continue;
        }

        token = strtok(NULL, ",");
        if (token && atoi(token) > 0) {  // So luong phai la so nguyen duong
            dtct.soLuong = atoi(token);
        }
        else { // Neu so luong khong hop le, bo qua dong
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): So luong khong hop le cho MaDon %d, MaThuoc %s. Bo qua dong.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            continue;
        }

        token = strtok(NULL, ",");
        if (token && strlen(token) > 0) {  // Lieu dung khong duoc rong tu file
            strncpy(dtct.lieuDung, token, sizeof(dtct.lieuDung) - 1);
            dtct.lieuDung[sizeof(dtct.lieuDung) - 1] = '\0';
        }
        else { // Neu lieu dung rong, dat gia tri mac dinh la "Chua co"
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Lieu dung rong cho MaDon %d, MaThuoc %s. Dat mac dinh 'Chua co'.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            strcpy(dtct.lieuDung, "Chua co");
        }

        token = strtok(NULL, "\n");
        if (token && strlen(token) > 0) {  // Cach dung khong duoc rong tu file
            strncpy(dtct.cachDung, token, sizeof(dtct.cachDung) - 1);
            dtct.cachDung[sizeof(dtct.cachDung) - 1] = '\0';
        }
        else { // Neu cach dung rong, dat gia tri mac dinh la "Chua co"
            fprintf(stderr, "CANH BAO (docDanhSachDonThuocChiTiet Dong %d): Cach dung rong cho MaDon %d, MaThuoc %s. Dat mac dinh 'Chua co'.\n", lineNumber, dtct.maDonThuocChiTiet, dtct.maThuoc);
            strcpy(dtct.cachDung, "Chua co");
        }

        ds[*soLuongDongConTro] = dtct; // Luu don thuoc chi tiet vao danh sach
        (*soLuongDongConTro)++; // Tang so luong dong da doc
    }
    fclose(f);
    return ds;
}

// Ghi danh sach don thuoc ra file CSV
void ghiDanhSachDonThuocChiTietRaFile(const char* tenFile, const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong) {
    FILE* f = fopen(tenFile, "w"); // Mo file o che do ghi
    if (f == NULL) {
        perror("Loi mo file (ghiDanhSachDonThuocChiTietRaFile)"); // Neu mo file that bai, in thong bao loi
        return;
    }
    // Duyet qua tung dong trong danh sach don thuoc chi tiet, ghi tung dong vao file
    for (int i = 0; i < soLuongDong; ++i){
        const char* maThuocOut = (strlen(dsDonThuocChiTiet[i].maThuoc) > 0) ? dsDonThuocChiTiet[i].maThuoc : "MA_THUOC_KHONG_THOA_MAN";
    int soLuongOut = (dsDonThuocChiTiet[i].soLuong > 0) ? dsDonThuocChiTiet[i].soLuong : 1;
    const char* lieuDungOut = (strlen(dsDonThuocChiTiet[i].lieuDung) > 0) ? dsDonThuocChiTiet[i].lieuDung : "Chua co";
    const char* cachDungOut = (strlen(dsDonThuocChiTiet[i].cachDung) > 0) ? dsDonThuocChiTiet[i].cachDung : "Chua co";

	// Ghi thong tin don thuoc chi tiet vao file
    fprintf(f, "%d,%s,%d,%s,%s\n",
        dsDonThuocChiTiet[i].maDonThuocChiTiet,
        maThuocOut,
        soLuongOut,
        lieuDungOut,
        cachDungOut);
}
fclose(f);
}

// Ham them don thuoc 
int themDongDonThuocChiTiet(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, DonThuocChiTiet dtctMoi, const char* tenFile) {
    // Kiem tra tinh hop le cua don thuoc chi tiet
    if (strlen(dtctMoi.maThuoc) == 0 || dtctMoi.soLuong <= 0 ||
        strlen(dtctMoi.lieuDung) == 0 || strlen(dtctMoi.cachDung) == 0) {
        fprintf(stderr, "LOI (themDongDonThuocChiTiet): Thong tin chi tiet don thuoc khong hop le (truong rong hoac so luong <=0).\n");
        return 0;
    }
    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == dtctMoi.maDonThuocChiTiet &&
            strcmp((*dsDTCTConTro)[i].maThuoc, dtctMoi.maThuoc) == 0) {
            // Neu da ton tai don thuoc chi tiet voi ma va thuoc tuong ung, khong them lai
            return 0;
        }
    }

    DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro + 1) * sizeof(DonThuocChiTiet)); // Cap phat bo nho cho danh sach don thuoc chi tiet
    if (temp == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
    }
    *dsDTCTConTro = temp; // Cap nhat con tro toi bo nho moi
    (*dsDTCTConTro)[*soLuongDongConTro] = dtctMoi; // Them don thuoc chi tiet moi vao danh sach
    (*soLuongDongConTro)++; // Tang so luong dong
    ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro); // Ghi danh sach don thuoc chi tiet ra file
    return 1;
}

// Ham xoa thuoc trong don thuoc
int xoaDongThuocTrongDon(DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, int maDonThuocCanTim, const char* maThuocCanXoa, const char* tenFile) {
    int viTriCanXoa = -1; // Bien de luu vi tri cua dong can xoa
    char maThuocXoaUpper[MAX_LEN_MA_THUOC]; // Mang de luu ma thuoc can xoa da chuyen sang chu hoa
    strncpy(maThuocXoaUpper, maThuocCanXoa, sizeof(maThuocXoaUpper) - 1); // Sao chep ma thuoc can xoa vao mang
    maThuocXoaUpper[sizeof(maThuocXoaUpper) - 1] = '\0';
    for (int i = 0; maThuocXoaUpper[i]; i++) maThuocXoaUpper[i] = toupper(maThuocXoaUpper[i]); // Chuyen ma thuoc can xoa sang chu hoa

    for (int i = 0; i < *soLuongDongConTro; ++i) {
        if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocCanTim &&
            strcmp((*dsDTCTConTro)[i].maThuoc, maThuocXoaUpper) == 0) {  // Tim thay dong can xoa, luu vi tri
            viTriCanXoa = i;
            break;
        }
    }

    if (viTriCanXoa != -1) {
        for (int i = viTriCanXoa; i < (*soLuongDongConTro - 1); ++i) {
            (*dsDTCTConTro)[i] = (*dsDTCTConTro)[i + 1]; // Dich chuyen cac dong sau vi tri can xoa ve truoc
        }
        (*soLuongDongConTro)--; // Giam so luong dong
        if (*soLuongDongConTro == 0) {
            free(*dsDTCTConTro); // Giai phong bo nho da cap phat neu khong con dong nao
            *dsDTCTConTro = NULL;
        }
        else {
            DonThuocChiTiet* temp = realloc(*dsDTCTConTro, (*soLuongDongConTro) * sizeof(DonThuocChiTiet)); // Cap phat lai bo nho cho danh sach don thuoc chi tiet
            if (temp == NULL && *soLuongDongConTro > 0) {
                perror("Loi realloc (xoaDongThuocTrongDon)");
            }
            else if (*soLuongDongConTro > 0) {
                *dsDTCTConTro = temp; // Cap nhat con tro toi bo nho moi
            }
        }
        ghiDanhSachDonThuocChiTietRaFile(tenFile, *dsDTCTConTro, *soLuongDongConTro); // Ghi danh sach don thuoc chi tiet ra file
        return 1;
    }
    return 0;
}

// Ham Sua thuoc trong don thuoc
int suaDongThuocTrongDon(DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong, int maDonThuocCanTim, const char* maThuocCanSua, DonThuocChiTiet thongTinMoi, const char* tenFile) {
    // Kiem tra tinh hop le cua thong tin moi
    if (thongTinMoi.soLuong <= 0 || strlen(thongTinMoi.lieuDung) == 0 || strlen(thongTinMoi.cachDung) == 0) {
        fprintf(stderr, "LOI (suaDongThuocTrongDon): Thong tin thuoc moi khong hop le (so luong, lieu dung hoac cach dung rong/invalid).\n");
        return 0;
    }
    // Chuyen ma thuoc can sua sang chu hoa de so sanh
    char maThuocSuaUpper[MAX_LEN_MA_THUOC];
    strncpy(maThuocSuaUpper, maThuocCanSua, sizeof(maThuocSuaUpper) - 1);
    maThuocSuaUpper[sizeof(maThuocSuaUpper) - 1] = '\0';
    for (int i = 0; maThuocSuaUpper[i]; i++) maThuocSuaUpper[i] = toupper(maThuocSuaUpper[i]);
    // Duyet qua danh sach don thuoc chi tiet de tim thuoc can sua, neu tim thay thi cap nhat thong tin
	for (int i = 0; i < soLuongDong; ++i) {// Duyet qua danh sach don thuoc chi tiet
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuocCanTim &&
			strcmp(dsDonThuocChiTiet[i].maThuoc, maThuocSuaUpper) == 0) {// Tim thay thuoc can sua
			// Cap nhat thong tin thuoc
            dsDonThuocChiTiet[i].soLuong = thongTinMoi.soLuong;
			strncpy(dsDonThuocChiTiet[i].lieuDung, thongTinMoi.lieuDung, sizeof(dsDonThuocChiTiet[i].lieuDung) - 1);// Sao chep lieu dung moi

            dsDonThuocChiTiet[i].lieuDung[sizeof(dsDonThuocChiTiet[i].lieuDung) - 1] = '\0';
			strncpy(dsDonThuocChiTiet[i].cachDung, thongTinMoi.cachDung, sizeof(dsDonThuocChiTiet[i].cachDung) - 1);// Sao chep cach dung moi
            dsDonThuocChiTiet[i].cachDung[sizeof(dsDonThuocChiTiet[i].cachDung) - 1] = '\0';

            ghiDanhSachDonThuocChiTietRaFile(tenFile, dsDonThuocChiTiet, soLuongDong); // Ghi danh sach don thuoc chi tiet da sua ra file
            return 1;
        }
    }
    return 0;
}

// Ham in don thuoc ra man hinh - 1 don thuoc 
void inChiTietMotDonThuocConsole(int maDonThuoc,
    const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDongTongCong, // Tong so luong dong trong danh sach don thuoc chi tiet
    // Tim thuoc theo ma trong danh sach toan bo
    const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- CHI TIET DON THUOC MA: %d ---\n", maDonThuoc); // In tieu de don thuoc
    int stt = 0;
    int timThayDon = 0;

    for (int i = 0; i < soLuongDongTongCong; ++i) {
        if (dsDonThuocChiTiet[i].maDonThuocChiTiet == maDonThuoc) {
            if (!timThayDon) { // Neu chua tim thay don thuoc, in tieu de bang
                printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
                printf("| %-4s | %-20s | %-35s | %-10s | %-25s | %-25s |\n", "STT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
                printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

                timThayDon = 1; // Danh dau da tim thay don thuoc
            }
            stt++; // Tang so thu tu
            const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc); // Tim thuoc theo ma trong danh sach toan bo
            char tenThuocHienThi[151] = "Khong tim thay ten"; // Khoi tao ten thuoc hien thi mac dinh
            // Neu tim thay thuoc, sao chep ten thuoc vao chuoi
            if (thuocInfo != NULL) {
                strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
                tenThuocHienThi[150] = '\0';
            }
            // In thong tin chi tiet don thuoc
            printf("| %-4d | %-20.20s | %-35.35s | %-10d | %-25.25s | %-25.25s |\n",
                stt,
                dsDonThuocChiTiet[i].maThuoc,
                tenThuocHienThi,
                dsDonThuocChiTiet[i].soLuong,
                dsDonThuocChiTiet[i].lieuDung,
                dsDonThuocChiTiet[i].cachDung);
        }
    }

    if (!timThayDon) { // Neu khong tim thay don thuoc, in thong bao loi
        printf("Khong co chi tiet nao cho don thuoc ma %d hoac don thuoc khong ton tai.\n", maDonThuoc);
    }
    else { // Neu tim thay don thuoc, in dong ket thuc bang
        printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    }
}

// Ham in tat ca thuoc ra man hinh
void inTatCaDonThuocChiTietConsole(const DonThuocChiTiet dsDonThuocChiTiet[], int soLuongDong,
    const Thuoc dsThuocToanBo[], int soLuongThuocTB) {
    printf("\n--- TAT CA CHI TIET CAC DON THUOC ---\n"); // In tieu de
    if (soLuongDong == 0) {
        printf("Khong co chi tiet don thuoc nao.\n"); // Neu khong co don thuoc, in thong bao 
        return;
    }
    // In tieu de bang
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-4s | %-20s | %-35s | %-10s | %-25s | %-25s |\n", "STT", "Ma Thuoc", "Ten Thuoc", "So Luong", "Lieu Dung", "Cach Dung");
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < soLuongDong; ++i) {
        const Thuoc* thuocInfo = timThuocTheoMa(dsThuocToanBo, soLuongThuocTB, dsDonThuocChiTiet[i].maThuoc);
        char tenThuocHienThi[151] = "N/A";
        if (thuocInfo != NULL) { // Neu tim thay thuoc, sao chep ten thuoc vao chuoi
            strncpy(tenThuocHienThi, thuocInfo->tenThuoc, 150);
            tenThuocHienThi[150] = '\0';
        }
        // In thong tin chi tiet don thuoc
        printf("| %-4d | %-20.20s | %-35.35s | %-10d | %-25.25s | %-25.25s |\n",
            dsDonThuocChiTiet[i].maDonThuocChiTiet,
            dsDonThuocChiTiet[i].maThuoc,
            tenThuocHienThi,
            dsDonThuocChiTiet[i].soLuong,
            dsDonThuocChiTiet[i].lieuDung,
            dsDonThuocChiTiet[i].cachDung);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------------------\n");
}


// Ham cap nhat chi tiet 1 don thuoc
void quanLyCapNhatChiTietDonThuoc(int maDonThuocDeCapNhat, // Ma don thuoc can cap nhat
    DonThuocChiTiet** dsDTCTConTro, int* soLuongDongConTro, // Con tro toi danh sach don thuoc chi tiet va so luong dong hien tai
    const Thuoc dsThuocToanBo[], int soLuongThuocTB, // Danh sach toan bo thuoc va so luong thuoc
    BangBam* bbThuoc,
    const char* tenFileCSV) {
    int choice; // Bien lua chon cua nguoi dung
    char buffer[128];
    // Kiem tra tinh hop le cua tham so dau vao, neu khong hop le thi bao loi va ket thuc
    if (dsThuocToanBo == NULL || soLuongThuocTB == 0 || bbThuoc == NULL) {
        printf("LOI: Khong co du lieu thuoc hoac bang bam thuoc khong kha dung de ke don.\n");
        printf("Vui long cap nhat danh sach thuoc va khoi tao bang bam thuoc truoc.\n");
        return;
    }

    do { // Lap lai cho den khi nguoi dung chon thoat
        inChiTietMotDonThuocConsole(maDonThuocDeCapNhat, *dsDTCTConTro, *soLuongDongConTro, dsThuocToanBo, soLuongThuocTB);
        printf("\n--- QUAN LY DON THUOC MA: %d ---\n", maDonThuocDeCapNhat);
        printf("1. Them thuoc vao don\n");
        printf("2. Sua thong tin thuoc trong don\n");
        printf("3. Xoa thuoc khoi don\n");
        printf("0. Quay lai\n");
        printf("Lua chon cua ban: ");

        choice = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 3); // Doc lua chon cua nguoi dung, cho phep lua chon tu 0 den 3
        if (choice < 0) choice = -1; // Neu nhap khong hop le, thoat khoi vong lap

        switch (choice) {

        case 1: { // Them thuoc
            xoaManHinhConsole();
            DonThuocChiTiet dtctMoi;// Bien tam de luu tru thong tin don thuoc chi tiet moi
            dtctMoi.maDonThuocChiTiet = maDonThuocDeCapNhat; // Gan ma don thuoc chi tiet moi

            printf("\n--- Danh Sach Thuoc Hien Co (co the tim theo Ma hoac Ten) ---\n"); // In danh sach thuoc hien co
            inDanhSachThuocConsole(dsThuocToanBo, soLuongThuocTB);

            printf("Nhap Ma Thuoc can them (IN HOA, KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap ma thuoc
            // Doc ma thuoc tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(dtctMoi.maThuoc, sizeof(dtctMoi.maThuoc), stdin) == NULL || strlen(dtctMoi.maThuoc) == 0) {
                printf("LOI: Ma Thuoc khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; dtctMoi.maThuoc[i]; i++) dtctMoi.maThuoc[i] = toupper(dtctMoi.maThuoc[i]);
            // Kiem tra xem ma thuoc co ton tai trong bang bam thuoc khong, neu khong ton tai thi bao loi va ket thuc
            if (timThuocTheoMaBangBam(bbThuoc, dtctMoi.maThuoc) == NULL) {
                printf("Loi: Ma Thuoc '%s' khong ton tai trong danh muc thuoc! Huy them.\n", dtctMoi.maThuoc);
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            int thuocDaCoTrongDon = 0; // Bien kiem tra xem thuoc da co trong don thuoc chua
            // Duyet qua danh sach don thuoc chi tiet hien tai de kiem tra xem thuoc da co trong don thuoc chua, neu co thi bao loi va ket thuc
            for (int i = 0; i < *soLuongDongConTro; ++i) {
                if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocDeCapNhat &&
                    strcmp((*dsDTCTConTro)[i].maThuoc, dtctMoi.maThuoc) == 0) {
                    thuocDaCoTrongDon = 1;
                    break;
                }
            }
            if (thuocDaCoTrongDon) { // Neu thuoc da co trong don thuoc, bao loi va ket thuc
                printf("Loi: Thuoc '%s' da ton tai trong don thuoc %d. Hay sua so luong/thong tin neu can.\n", dtctMoi.maThuoc, maDonThuocDeCapNhat);
                dungManHinhCho();
                xoaManHinhConsole();
                break;

            }


            printf("Nhap So Luong (phai > 0): "); // Yeu cau nguoi dung nhap so luong
            // Doc so luong tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &dtctMoi.soLuong) != 1 || dtctMoi.soLuong <= 0) {
                printf("LOI: So luong khong hop le hoac khong phai la so nguyen duong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap Lieu Dung (KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap lieu dung
            // Doc lieu dung tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(dtctMoi.lieuDung, sizeof(dtctMoi.lieuDung), stdin) == NULL || strlen(dtctMoi.lieuDung) == 0) {
                printf("LOI: Lieu dung khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            printf("Nhap Cach Dung (KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap cach dung
            // Doc cach dung tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(dtctMoi.cachDung, sizeof(dtctMoi.cachDung), stdin) == NULL || strlen(dtctMoi.cachDung) == 0) {
                printf("LOI: Cach dung khong duoc de trong. Huy them.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            if (themDongDonThuocChiTiet(dsDTCTConTro, soLuongDongConTro, dtctMoi, tenFileCSV)) { // Goi ham them don thuoc chi tiet, neu thanh cong thi in thong bao
                printf("Them thuoc vao don thanh cong!\n");
                dungManHinhCho();
                xoaManHinhConsole();
            }
            else {
                printf("Them thuoc vao don THAT BAI (co the do loi logic hoac du lieu khong hop le da duoc kiem tra).\n"); // Neu them that bai, in thong bao loi
                xoaManHinhConsole();
            }
            dungManHinhCho();
            break;
        }
        case 2: { // Sua thuoc

            char maThuocSua[MAX_LEN_MA_THUOC];
            DonThuocChiTiet thongTinMoiSua;
            printf("Nhap Ma Thuoc trong don can sua (IN HOA, KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap ma thuoc can sua
            // Neu ma thuoc khong hop le thi bao loi va ket thuc
            if (docDongAnToan(maThuocSua, sizeof(maThuocSua), stdin) == NULL || strlen(maThuocSua) == 0) {
                printf("LOI: Ma Thuoc can sua khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; maThuocSua[i]; i++) maThuocSua[i] = toupper(maThuocSua[i]); // Chuyen ma thuoc sang chu hoa
            // Kiem tra xem ma thuoc co ton tai trong danh sach toan bo thuoc khong, neu khong ton tai thi bao loi va ket thuc
            int timThayThuocTrongDon = 0;
            for (int i = 0; i < *soLuongDongConTro; ++i) {
                if ((*dsDTCTConTro)[i].maDonThuocChiTiet == maDonThuocDeCapNhat && strcmp((*dsDTCTConTro)[i].maThuoc, maThuocSua) == 0) {
                    timThayThuocTrongDon = 1;
                    break;
                }
            }
            if (!timThayThuocTrongDon) { // Neu khong tim thay thuoc trong don thuoc, bao loi va ket thuc
                printf("Thuoc '%s' khong co trong don thuoc %d. Huy sua.\n", maThuocSua, maDonThuocDeCapNhat);
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap So Luong moi (phai > 0): "); // Yeu cau nguoi dung nhap so luong moi
            // Doc so luong moi tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || sscanf(buffer, "%d", &thongTinMoiSua.soLuong) != 1 || thongTinMoiSua.soLuong <= 0) {
                printf("LOI: So luong moi khong hop le. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            printf("Nhap Lieu Dung moi (KHONG DUOC BO TRONG): ");// Yeu cau nguoi dung nhap lieu dung moi
            // Doc lieu dung moi tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(thongTinMoiSua.lieuDung, sizeof(thongTinMoiSua.lieuDung), stdin) == NULL || strlen(thongTinMoiSua.lieuDung) == 0) {
                printf("LOI: Lieu dung moi khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            printf("Nhap Cach Dung moi (KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap cach dung moi
            // Doc cach dung moi tu nguoi dung, neu khong hop le thi bao loi va ket thuc
            if (docDongAnToan(thongTinMoiSua.cachDung, sizeof(thongTinMoiSua.cachDung), stdin) == NULL || strlen(thongTinMoiSua.cachDung) == 0) {
                printf("LOI: Cach dung moi khong duoc de trong. Huy sua.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }

            if (suaDongThuocTrongDon(*dsDTCTConTro, *soLuongDongConTro, maDonThuocDeCapNhat, maThuocSua, thongTinMoiSua, tenFileCSV)) {
                printf("Sua thong tin thuoc thanh cong!\n"); // Goi ham sua don thuoc chi tiet, neu thanh cong thi in thong bao
                dungManHinhCho();
                xoaManHinhConsole();
            }
            else {
                printf("Sua thong tin thuoc THAT BAI.\n"); // Neu sua that bai, in thong bao loi
                dungManHinhCho();
                xoaManHinhConsole();
            }
            dungManHinhCho();
            break;
        }
        case 3: { // Xoa thuoc
            xoaManHinhConsole();
            char maThuocXoa[MAX_LEN_MA_THUOC];
            printf("Nhap Ma Thuoc trong don can xoa (IN HOA, KHONG DUOC BO TRONG): "); // Yeu cau nguoi dung nhap ma thuoc can xoa
            // Neu ma thuoc khong hop le thi bao loi va ket thuc
            if (docDongAnToan(maThuocXoa, sizeof(maThuocXoa), stdin) == NULL || strlen(maThuocXoa) == 0) {
                printf("LOI: Ma Thuoc can xoa khong duoc de trong. Huy xoa.\n");
                dungManHinhCho();
                xoaManHinhConsole();
                break;
            }
            for (int i = 0; maThuocXoa[i]; i++) maThuocXoa[i] = toupper(maThuocXoa[i]);

            if (xoaDongThuocTrongDon(dsDTCTConTro, soLuongDongConTro, maDonThuocDeCapNhat, maThuocXoa, tenFileCSV)) {
                printf("Xoa thuoc khoi don thanh cong!\n"); // Goi ham xoa thuoc khoi don, neu thanh cong thi in thong bao
                dungManHinhCho();
                xoaManHinhConsole();

            }
            else {
                printf("Xoa thuoc khoi don THAT BAI (co the khong tim thay thuoc '%s' trong don %d).\n", maThuocXoa, maDonThuocDeCapNhat); // Neu xoa that bai, in thong bao loi
                xoaManHinhConsole();

            }
            dungManHinhCho();
            break;
        }
        case 0: //Ra ngoai menu
            break; // Neu nguoi dung chon 0, thoat khoi menu cap nhat chi tiet don thuoc
        default:
            inThongDiepLoi("Lua chon khong hop le. Vui long chon lai."); // Neu lua chon khong hop le, in thong bao loi
            dungManHinhCho();
            xoaManHinhConsole();
            break;
        }
    } while (choice != 0);  // Lap lai cho den khi nguoi dung chon thoat
}