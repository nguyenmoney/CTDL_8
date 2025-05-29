#include "examination.h"
#include "patient.h"        
#include "csv_handler.h"    
#include "doctor.h"         
#include "department.h"     
#include "priority_queue.h" 
#include "hash_table.h"     
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#define TEN_BN_LK_CONSOLE_WIDTH
#define TEN_BN_LK_CONSOLE_WIDTH 25 // So ki tu toi da ten benh nhan trong console     
#define LY_DO_KHAM_LK_CONSOLE_WIDTH 20  // So ki tu toi da ly do kham trong console
#define CHAN_DOAN_LK_CONSOLE_WIDTH 15   // So ki tu toi da chan doan trong console
#define GHI_CHU_BS_LK_CONSOLE_WIDTH 15  // So ki tu toi da ghi chu bac si trong console
#define MA_BAC_SI_LK_CONSOLE_WIDTH 15   // So ki tu toi da ma bac si trong console
#define MA_LAN_KHAM_STR_LEN 12 

// Ham noi bo chuyen ma lan kham sang dang string
static void intMaLKToString(int maLK, char* maLKStr, size_t bufferSize) {
    snprintf(maLKStr, bufferSize, "%d", maLK);
}
// Ham tien ich de so sanh hai chuoi khong phan biet hoa thuong
//Chuyen enum sang string de ghi vao file CSV
// Chuyen string sang enum de doc tu file CSV
int my_strcasecmp(const char* s1, const char* s2) {
    int c1, c2;
    do {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}
// Cac ham chuyen doi tu Enum sang String va nguoc lai
const char* trangThaiLanKhamToString(TrangThaiLanKham tt) {
    switch (tt) {
    case LK_DANG_CHO: return "DangCho";
    case LK_DANG_KHAM: return "DangKham";
    case LK_DA_HOAN_THANH: return "DaHoanThanh";
    case LK_DA_HUY: return "DaHuy";
    case LK_DA_LO: return "DaLo";
    default: return "KhongXacDinh";
    }
}

TrangThaiLanKham stringToTrangThaiLanKham(const char* str) {
    if (str == NULL || strlen(str) == 0) return LK_DANG_CHO;
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr) - 1); lowerStr[sizeof(lowerStr) - 1] = '\0';
    for (int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (my_strcasecmp(lowerStr, "dangcho") == 0) return LK_DANG_CHO;
    if (my_strcasecmp(lowerStr, "dangkham") == 0) return LK_DANG_KHAM;
    if (my_strcasecmp(lowerStr, "dahoanthanh") == 0) return LK_DA_HOAN_THANH;
    if (my_strcasecmp(lowerStr, "dahuy") == 0) return LK_DA_HUY;
    if (my_strcasecmp(lowerStr, "dalo") == 0) return LK_DA_LO;
    return LK_DANG_CHO;
}

const char* mucDoUuTienToString(MucDoUuTien md) {
    switch (md) {
    case CAP_CUU: return "CapCuu";
    case KHAN_CAP: return "KhanCap";
    case THONG_THUONG: return "ThongThuong";
    case TAI_KHAM: return "TaiKham";
    case UU_TIEN_KHAC: return "UuTienKhac";
    default: return "KhongXacDinh";
    }
}

MucDoUuTien stringToMucDoUuTien(const char* str) {
    if (str == NULL || strlen(str) == 0) return THONG_THUONG;
    char lowerStr[50];
    strncpy(lowerStr, str, sizeof(lowerStr) - 1); lowerStr[sizeof(lowerStr) - 1] = '\0';
    for (int i = 0; lowerStr[i]; i++) lowerStr[i] = tolower(lowerStr[i]);

    if (my_strcasecmp(lowerStr, "capcuu") == 0) return CAP_CUU;
    if (my_strcasecmp(lowerStr, "khancap") == 0) return KHAN_CAP;
    if (my_strcasecmp(lowerStr, "thongthuong") == 0) return THONG_THUONG;
    if (my_strcasecmp(lowerStr, "taikham") == 0) return TAI_KHAM;
    if (my_strcasecmp(lowerStr, "uutienkhac") == 0) return UU_TIEN_KHAC;
    return THONG_THUONG;
}
// Ham doc danh sach lan kham tu file CSV
LanKham* docDanhSachLanKham(const char* tenFile, int* soLuong) {
	FILE* f = fopen(tenFile, "r"); // Mo file de doc
    if (f == NULL) {// Kiem tra file rong
		*soLuong = 0;//Cap nhat so luong ve 0
        return NULL;
    }

	LanKham* ds = NULL; //Khoi tao mot danh sach lan kham rong
    int capacity = 0;// Suc chua cua danh sach
	*soLuong = 0; // So luong lan kham hien tai
	char line[2048]; //Buffer de doc tung dong
	int lineNumber = 0;// Bien dem so dong da doc

	while (fgets(line, sizeof(line), f) != NULL) {// Doc tung dong trong file
        lineNumber++;
		char orgLine[2048];//Tao dong goc de giu lai dong goc
		strncpy(orgLine, line, sizeof(orgLine) - 1);// Sao chep dong vao orgLine
		orgLine[sizeof(orgLine) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
		xoaKyTuXuongDong(orgLine);// Xoa ky tu xuong dong tiep theo

        xoaKyTuXuongDong(line);
        if (strlen(line) < 10) {//Bo qua dong qua ngan
            if (strlen(line) > 0) {
                fprintf(stderr, "CANH BAO (docDSLK Dong %d): Dong qua ngan, bo qua: \"%s\"\n", lineNumber, orgLine);
            }
            continue;
        }

		if (*soLuong >= capacity) {// Neu danh sach da day, cap nhat suc chua
            capacity = (capacity == 0) ? 20 : capacity * 2; // Cap nhat suc chua *2
			LanKham* temp = realloc(ds, capacity * sizeof(LanKham));// Cap phat bo nho moi
			if (temp == NULL) {// Kiem tra cap phat thanh cong
                perror("Loi cap phat bo nho cho docDanhSachLanKham");
                free(ds); fclose(f); *soLuong = 0; return NULL;
            }
			ds = temp;// Cap nhat con tro ds
        }

        LanKham lk;
        //Khoi tao ve gia tri an toan
        lk.maLanKham = 0;                lk.maDinhDanh[0] = '\0';        lk.tenBenhNhan[0] = '\0';
        lk.maBacSi[0] = '\0';            lk.maKhoaPhong[0] = '\0';       lk.ngayGioKham = 0;
        lk.lyDoKham[0] = '\0';           lk.chanDoan[0] = '\0';          lk.phuongPhapDieuTri[0] = '\0';
        lk.ghiChuBacSi[0] = '\0';        lk.ngayTaiKham.ngay = -1;       lk.ngayTaiKham.thang = -1;
        lk.ngayTaiKham.nam = -1;         lk.coLichTaiKham = 0;           lk.mucDoUuTien = THONG_THUONG;
        lk.gioDangKyThanhCong = 0;       lk.coDonThuoc = 0;              lk.trangThai = LK_DANG_CHO;
        lk.maDonThuocChiTiet = -1;

        char mucDoUuTienStr[50] = "";    char trangThaiStr[50] = "";
        long tempTimestamp;              char* token;
		char* lineCopyForStrtok = strdup(line);// Tao mot ban sao cua dong de dung strtok
        if (!lineCopyForStrtok) { perror("Loi strdup lineCopyForStrtok"); continue; }
        char* p = lineCopyForStrtok;

		token = strtok(p, ","); // Doc ma lan kham
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { lk.maLanKham = atoi(token); }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): MaLanKham ('%s') khong hop le. Bo qua dong: \"%s\"\n", lineNumber, token ? token : "NULL", orgLine); free(lineCopyForStrtok); continue; }

		
        token = strtok(NULL, ",");// Doc ma dinh danh benh nhan
        if (token && strlen(token) > 0) { strncpy(lk.maDinhDanh, token, sizeof(lk.maDinhDanh) - 1); lk.maDinhDanh[sizeof(lk.maDinhDanh) - 1] = '\0'; }
		else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): Thieu maDinhDanh cho MaLK %d. Bo qua dong.\n", lineNumber, lk.maLanKham); free(lineCopyForStrtok); continue; }
        // Neu ma dinh danh rong, bao loi va bo qua dong

		token = strtok(NULL, ",");// Doc ten benh nhan
        if (token) { strncpy(lk.tenBenhNhan, token, sizeof(lk.tenBenhNhan) - 1); lk.tenBenhNhan[sizeof(lk.tenBenhNhan) - 1] = '\0'; }
		else { strcpy(lk.tenBenhNhan, "Khong Biet"); }// Neu ten benh nhan rong, gan mac dinh la "Khong Biet"

		
        token = strtok(NULL, ","); // Doc ma bac si
        if (token && strlen(token) > 0) { strncpy(lk.maBacSi, token, sizeof(lk.maBacSi) - 1); lk.maBacSi[sizeof(lk.maBacSi) - 1] = '\0'; }
        else { strcpy(lk.maBacSi, "CHUA_GAN"); }// Neu ma bac si rong, gan mac dinh la "CHUA_GAN"

		token = strtok(NULL, ",");// Doc ma khoa phong
        if (token && strlen(token) > 0) { strncpy(lk.maKhoaPhong, token, sizeof(lk.maKhoaPhong) - 1); lk.maKhoaPhong[sizeof(lk.maKhoaPhong) - 1] = '\0'; }
        else { fprintf(stderr, "LOI DOC CSV (LanKham dong %d): Thieu maKhoaPhong cho MaLK %d. Bo qua dong.\n", lineNumber, lk.maLanKham); free(lineCopyForStrtok); continue; }
		// Neu ma khoa phong rong, bao loi va bo qua dong

        token = strtok(NULL, ",");// Doc ngay
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { tempTimestamp = atol(token); if (tempTimestamp >= 0) lk.ngayGioKham = (time_t)tempTimestamp; }
       // Neu ngay gio kham khong hop le, bao loi va bo qua dong

        token = strtok(NULL, ",");// Doc ly do kham
        if (token && strlen(token) > 0) { strncpy(lk.lyDoKham, token, sizeof(lk.lyDoKham) - 1); lk.lyDoKham[sizeof(lk.lyDoKham) - 1] = '\0'; }
		else { strcpy(lk.lyDoKham, "Khong ro ly do"); } // Neu ly do kham rong, gan mac dinh là "Khong ro ly do"

        
		token = strtok(NULL, ","); // Doc chan doan
        if (token && strlen(token) > 0) { strncpy(lk.chanDoan, token, sizeof(lk.chanDoan) - 1); lk.chanDoan[sizeof(lk.chanDoan) - 1] = '\0'; }
		else { strcpy(lk.chanDoan, "CHUA_KHAM"); } // Neu chan doan rong, gan mac dinh là "CHUA_KHAM"

		token = strtok(NULL, ","); // Doc phuong phap dieu tri
        if (token && strlen(token) > 0) { strncpy(lk.phuongPhapDieuTri, token, sizeof(lk.phuongPhapDieuTri) - 1); lk.phuongPhapDieuTri[sizeof(lk.phuongPhapDieuTri) - 1] = '\0'; }
		else { strcpy(lk.phuongPhapDieuTri, "CHUA_KHAM"); } // Neu phuong phap dieu tri rong, gan mac dinh là "CHUA_KHAM"

		token = strtok(NULL, ","); // Doc ghi chu bac si
        if (token) { strncpy(lk.ghiChuBacSi, token, sizeof(lk.ghiChuBacSi) - 1); lk.ghiChuBacSi[sizeof(lk.ghiChuBacSi) - 1] = '\0'; }
		else { strcpy(lk.ghiChuBacSi, "Khong"); } // Neu ghi chu bac si rong, gan mac dinh là "Khong"

		// Doc ngay, thang, nam tai kham va co lich tai kham, neu khong hop le thi gan mac dinh
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.ngay = atoi(token); else lk.ngayTaiKham.ngay = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.thang = atoi(token); else lk.ngayTaiKham.thang = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.ngayTaiKham.nam = atoi(token); else lk.ngayTaiKham.nam = -1;
        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coLichTaiKham = atoi(token); else lk.coLichTaiKham = 0;

        token = strtok(NULL, ","); // mucDoUuTien
        if (token && strlen(token) > 0) { lk.mucDoUuTien = stringToMucDoUuTien(token); }

        token = strtok(NULL, ","); // gioDangKyThanhCong
        if (token && strlen(token) > 0 && strspn(token, "0123456789") == strlen(token)) { tempTimestamp = atol(token); if (tempTimestamp >= 0) lk.gioDangKyThanhCong = (time_t)tempTimestamp; }

        token = strtok(NULL, ","); if (token && strlen(token) > 0 && strspn(token, "01") == strlen(token)) lk.coDonThuoc = atoi(token); else lk.coDonThuoc = 0;

        token = strtok(NULL, ","); // trangThai (phai la token cuoi cung truoc maDonThuoc)
        if (token && strlen(token) > 0) { lk.trangThai = stringToTrangThaiLanKham(token); }

        token = strtok(NULL, "\n"); // maDonThuocChiTiet
        if (token && strlen(token) > 0 && strspn(token, "-0123456789") == strlen(token)) lk.maDonThuocChiTiet = atoi(token); else lk.maDonThuocChiTiet = -1;

		// Kiem tra cac truong bat buoc
		if (lk.trangThai == LK_DA_HOAN_THANH) {// Neu trang thai la Da Hoan Thanh
			if (strlen(lk.chanDoan) == 0 || strcmp(lk.chanDoan, "CHUA_KHAM") == 0) {// Neu chan doan rong/mac dinh
				fprintf(stderr, "CANH BAO (docDSLK Dong %d): ChanDoan rong/mac dinh cho MaLK %d (DaHoanThanh). Dat 'CHUA_NHAP_CD_DOC'.\n", lineNumber, lk.maLanKham);// Canh bao loi
				strcpy(lk.chanDoan, "CHUA_NHAP_CD_DOC");// Dat lai chan doan
            }
			if (strlen(lk.phuongPhapDieuTri) == 0 || strcmp(lk.phuongPhapDieuTri, "CHUA_KHAM") == 0) {// Neu phuong phap dieu tri rong/mac dinh
				fprintf(stderr, "CANH BAO (docDSLK Dong %d): PPDieuTri rong/mac dinh cho MaLK %d (DaHoanThanh). Dat 'CHUA_NHAP_PPTT_DOC'.\n", lineNumber, lk.maLanKham);
                strcpy(lk.phuongPhapDieuTri, "CHUA_NHAP_PPTT_DOC");
            }
        }

		free(lineCopyForStrtok);// Giai phong bo nho da cap phat cho lineCopyForStrtok
		ds[*soLuong] = lk;// Luu lan kham vao danh sach
		(*soLuong)++;// Tang so luong lan kham
    }
    fclose(f);
	return ds;// Tra ve danh sach lan kham
}

// Ham xuat danh sach lan kham ra file CSV
void ghiDanhSachLanKhamRaFile(const char* tenFile, const LanKham dsLanKham[], int soLuong) {
	FILE* f = fopen(tenFile, "w");// Mo file de ghi
	if (f == NULL) {// Kiem tra file rong
        perror("Loi mo file de ghi (ghiDanhSachLanKhamRaFile)");
        return;
    }
	for (int i = 0; i < soLuong; ++i) {// Duyet qua tung lan kham
		const char* maKhoaPhongOut = (strlen(dsLanKham[i].maKhoaPhong) > 0) ? dsLanKham[i].maKhoaPhong : "KP_KHONG_THOA_MAN";// Neu ma khoa phong rong, gan mac dinh
		const char* lyDoKhamOut = (strlen(dsLanKham[i].lyDoKham) > 0) ? dsLanKham[i].lyDoKham : "Khong ro";//  Neu ly do kham rong, gan mac dinh
		const char* chanDoanOut = (strlen(dsLanKham[i].chanDoan) > 0) ? dsLanKham[i].chanDoan : "CHUA_KHAM";//  Neu chan doan rong, gan mac dinh
		const char* ppdtOut = (strlen(dsLanKham[i].phuongPhapDieuTri) > 0) ? dsLanKham[i].phuongPhapDieuTri : "CHUA_KHAM";// Neu phuong phap dieu tri rong, gan mac dinh

		if (dsLanKham[i].trangThai == LK_DA_HOAN_THANH) {// Neu trang thai la Da Hoan Thanh
			if (strcmp(chanDoanOut, "CHUA_KHAM") == 0 || strlen(chanDoanOut) == 0) chanDoanOut = "CHUA_NHAP_CD_GHI"; // Neu chan doan rong, gan mac dinh
			if (strcmp(ppdtOut, "CHUA_KHAM") == 0 || strlen(ppdtOut) == 0) ppdtOut = "CHUA_NHAP_PPTT_GHI";// Neu phuong phap dieu tri rong, gan mac dinh
        }

		fprintf(f, "%d,%s,%s,%s,%s,%ld,%s,%s,%s,%s,%d,%d,%d,%d,%s,%ld,%d,%s,%d\n",//Ghi tung dong vao file
            dsLanKham[i].maLanKham,
            (strlen(dsLanKham[i].maDinhDanh) > 0 ? dsLanKham[i].maDinhDanh : "BN_KHONG_THOA_MAN"),
            (strlen(dsLanKham[i].tenBenhNhan) > 0 ? dsLanKham[i].tenBenhNhan : "Ten Khong Biet"),
            (strlen(dsLanKham[i].maBacSi) == 0 ? "CHUA_GAN" : dsLanKham[i].maBacSi),
            maKhoaPhongOut,
            (long)dsLanKham[i].ngayGioKham,
            lyDoKhamOut,
            chanDoanOut,
            ppdtOut,
            (strlen(dsLanKham[i].ghiChuBacSi) == 0 ? "Khong" : dsLanKham[i].ghiChuBacSi),
            dsLanKham[i].ngayTaiKham.ngay, dsLanKham[i].ngayTaiKham.thang, dsLanKham[i].ngayTaiKham.nam,
            dsLanKham[i].coLichTaiKham,
            mucDoUuTienToString(dsLanKham[i].mucDoUuTien),
            (long)dsLanKham[i].gioDangKyThanhCong,
            dsLanKham[i].coDonThuoc,
            trangThaiLanKhamToString(dsLanKham[i].trangThai),
            dsLanKham[i].maDonThuocChiTiet
        );
    }
    fclose(f);
}

// Ham tao ma lan kham moi
int taoMaLanKhamMoi(const LanKham dsLanKham[], int soLuong) {// Tao ma lan kham moi tu danh sach hien tai
	if (soLuong == 0) return 1;// Neu danh sach rong, tra ve 1
	int maxId = 0;// Khoi tao maxId la 0
	for (int i = 0; i < soLuong; ++i) {// Duyet qua tung lan kham
		if (dsLanKham[i].maLanKham > maxId) maxId = dsLanKham[i].maLanKham;// Neu ma lan kham lon hon maxId, cap nhat maxId
    }
	return maxId + 1;// Tra ve ma lan kham moi
}
// Tim kiem lan kham theo ma trong bang bam
LanKham* timLanKhamTheoMaBangBam(BangBam* bbLanKham, int maLK) {
	if (bbLanKham == NULL) return NULL;// Kiem tra bang bam co ton tai khong
	char maLKStr[MA_LAN_KHAM_STR_LEN];// Chuan bi chuoi de chuyen ma lan kham
	intMaLKToString(maLK, maLKStr, sizeof(maLKStr));// Chuyen ma lan kham sang chuoi
	return (LanKham*)timKiemTrongBangBam(bbLanKham, maLKStr);// Tim kiem trong bang bam
}
// Tim kiem lan kham theo tuyen tinh
void timKiemLanKham(const LanKham dsLanKham[], int soLuongLanKham,
    const char* tieuChi, const char* giaTriTimKiem,
    LanKham** ketQuaTimKiem, int* soLuongKetQua) {
    *ketQuaTimKiem = NULL;
    *soLuongKetQua = 0;
    if (soLuongLanKham == 0 || giaTriTimKiem == NULL || tieuChi == NULL || strlen(giaTriTimKiem) == 0) {
        return;
	}// Kiem tra tham so hop le

    LanKham* dsKQTemp = (LanKham*)malloc(soLuongLanKham * sizeof(LanKham));
    if (dsKQTemp == NULL) {
        perror("Loi cap phat bo nho");
        return;
	}// Cap phat bo nho cho ket qua tim kiem
	int count = 0;// Bien dem so ket qua tim thay

	char tieuChiLower[50];// Chuan bi chuoi de chuyen tieu chi sang chu thuong
	strncpy(tieuChiLower, tieuChi, sizeof(tieuChiLower) - 1);// Sao chep tieu chi vao chuoi
	tieuChiLower[sizeof(tieuChiLower) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
	for (int idx = 0; tieuChiLower[idx]; idx++) tieuChiLower[idx] = tolower(tieuChiLower[idx]);// Chuyen tieu chi sang chu thuong

	char giaTriTimKiemLower[100];// Chuoi chuyen gia tri tim kiem sang chu thuong
	strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower) - 1);// Sao chep gia tri tim kiem vao chuoi
	giaTriTimKiemLower[sizeof(giaTriTimKiemLower) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
	if (strcmp(tieuChiLower, "malankham") != 0) {
        for (int idx = 0; giaTriTimKiemLower[idx]; idx++) giaTriTimKiemLower[idx] = tolower(giaTriTimKiemLower[idx]);
	}// Chuyen gia tri tim kiem sang chu thuong neu khong phai la ma lan kham


	for (int i = 0; i < soLuongLanKham; ++i) {// Duyet qua tung lan kham trong danh sach
		int timThayTrongLanKhamNay = 0;// Bien kiem tra co tim thay trong lan kham nay hay khong

        if (strcmp(tieuChiLower, "malankham") == 0) {
            if (dsLanKham[i].maLanKham == atoi(giaTriTimKiem)) {
                timThayTrongLanKhamNay = 1;
			}// Neu tieu chi la ma lan kham, so sanh voi gia tri tim kiem, neu tim thay thi cap nhat bien timThayTrongLanKhamNay
        }
		else if (strcmp(tieuChiLower, "madinhdanhbn") == 0) {// Neu tieu chi la ma dinh danh benh nhan
			char maDDBNLower[MAX_LEN_MA_DINH_DANH_BN + 1];// Chuoi de chuyen ma dinh danh sang chu thuong
			strncpy(maDDBNLower, dsLanKham[i].maDinhDanh, sizeof(maDDBNLower) - 1);//Sao chep ma dinh danh vao chuoi
			maDDBNLower[sizeof(maDDBNLower) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
			for (int k = 0; maDDBNLower[k]; k++) maDDBNLower[k] = tolower(maDDBNLower[k]);// Chuyen ma dinh danh sang chu thuong
            if (strcmp(maDDBNLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
			}// Neu ma dinh danh benh nhan trong lan kham hien tai trung voi gia tri tim kiem, cap nhat bien timThayTrongLanKhamNay
        }
		else if (strcmp(tieuChiLower, "trangthai") == 0) {// Neu tieu chi la trang thai
			char trangThaiLower[50];// Chuoi de chuyen trang thai sang chu thuong
			strncpy(trangThaiLower, trangThaiLanKhamToString(dsLanKham[i].trangThai), sizeof(trangThaiLower) - 1);// Sao chep trang thai sang chuoi
			trangThaiLower[sizeof(trangThaiLower) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
            for (int k = 0; trangThaiLower[k]; k++)  trangThaiLower[k] = tolower(trangThaiLower[k]);// Chuyen trang thai sang chu thuong
            if (strcmp(trangThaiLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
			}// Neu trang thai trong lan kham hien tai trung voi gia tri tim kiem, cap nhat bien timThayTrongLanKhamNay
        }
		else if (strcmp(tieuChiLower, "bacsi") == 0) {// Neu tieu chi la bac si
			char maBacSiLower[MAX_LEN_MA_BAC_SI + 1];// Chuoi de chuyen ma bac si sang chu thuong
			strncpy(maBacSiLower, dsLanKham[i].maBacSi, sizeof(maBacSiLower) - 1);// Sao chep ma bac si vao chuoi
			maBacSiLower[sizeof(maBacSiLower) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
			for (int k = 0; maBacSiLower[k]; k++) maBacSiLower[k] = tolower(maBacSiLower[k]);// Chuyen ma bac si sang chu thuong
            if (strstr(maBacSiLower, giaTriTimKiemLower) != NULL) {
                timThayTrongLanKhamNay = 1;
			}// Neu ma bac si trong lan kham hien tai chua gia tri tim kiem, cap nhat bien timThayTrongLanKhamNay
        }
		else if (strcmp(tieuChiLower, "makhoaphong") == 0) {// Neu tieu chi la ma khoa phong
			char maLKLower[MAX_LEN_MA_KHOA_PHONG + 1];// Chuoi de chuyen ma khoa phong sang chu thuong
			strncpy(maLKLower, dsLanKham[i].maKhoaPhong, sizeof(maLKLower) - 1);// Sao chep ma khoa phong vao chuoi
			maLKLower[sizeof(maLKLower) - 1] = '\0';//Bao dam ket thuc chuoi, tranh tran bo nho
			for (int k = 0; maLKLower[k]; k++) maLKLower[k] = tolower(maLKLower[k]);// Chuyen ma khoa phong sang chu thuong
            if (strcmp(maLKLower, giaTriTimKiemLower) == 0) {
                timThayTrongLanKhamNay = 1;
			}// Neu ma khoa phong trong lan kham hien tai trung voi gia tri tim kiem, cap nhat bien timThayTrongLanKhamNay
        }


        if (timThayTrongLanKhamNay) {
            dsKQTemp[count++] = dsLanKham[i];
		}// Neu tim thay trong lan kham hien tai, luu lan kham vao ket qua tam
    }

	if (count > 0) {// Neu co ket qua tim thay
		*ketQuaTimKiem = (LanKham*)malloc(count * sizeof(LanKham));// Cap phat bo nho cho ket qua tim kiem
		if (*ketQuaTimKiem != NULL) {// Neu cap phat thanh cong
			memcpy(*ketQuaTimKiem, dsKQTemp, count * sizeof(LanKham));// Sao chep ket qua tim kiem vao con tro ket qua
			*soLuongKetQua = count;// Cap nhat so luong ket qua tim kiem
        }
        else {
            perror("Loi cap phat bo nho");
            *soLuongKetQua = 0;
		}// Neu khong cap phat duoc bo nho, cap nhat so luong ket qua ve 0
    }
	free(dsKQTemp);// Giai phong bo nho ket qua tam
}
// Ham dang ky lan kham moi
int dangKyKhamMoi(LanKham** dsLanKhamConTro, int* soLuongLanKhamConTro,
    BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhanConTro,
    BangBam* bbBenhNhan,
    KhoaPhong* dsKhoaPhong, int soLuongKhoaPhong,
    BangBam* bbKhoaPhong,
    BangBam** bbLanKham_ptr,
    BangBam* bbHangDoiTheoKhoa,
    const char* fileLanKhamCSV, const char* fileBenhNhanCSV) {
	char maBN_input[MAX_LEN_MA_DINH_DANH_BN]; // Chuoi de nhap ma dinh danh benh nhan
	char maBN_upper[MAX_LEN_MA_DINH_DANH_BN]; // Chuoi de chuyen ma dinh danh sang chu hoa
	BenhNhan* bnTimThay = NULL;// Bien de luu benh nhan tim thay

	printf("Nhap Ma Dinh Danh Benh Nhan (KHONG BO TRONG): ");// Yeu cau nhap ma dinh danh benh nhan
	if (docDongAnToan(maBN_input, sizeof(maBN_input), stdin) == NULL || strlen(maBN_input) == 0) {// Kiem tra neu nhap rong
        printf("LOI: Ma Dinh Danh Benh Nhan khong duoc de trong. Huy dang ky.\n");
        return -1;
    }
	strncpy(maBN_upper, maBN_input, sizeof(maBN_upper) - 1);// Sao chep ma dinh danh vao chuoi
	maBN_upper[sizeof(maBN_upper) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
	for (int k = 0; maBN_upper[k]; k++) maBN_upper[k] = toupper(maBN_upper[k]);// Chuyen ma dinh danh sang chu hoa

	if (bbBenhNhan != NULL) {
        bnTimThay = timBenhNhanTheoMaBangBam(bbBenhNhan, maBN_upper);
	}// Neu bang bam benh nhan khong NULL, tim kiem trong bang bam

    if (bnTimThay == NULL && dsBenhNhanConTro != NULL && *dsBenhNhanConTro != NULL) {
        bnTimThay = (BenhNhan*)timBenhNhanTheoMaChuoi(*dsBenhNhanConTro, *soLuongBenhNhanConTro, maBN_upper);
	}// Neu khong tim thay trong bang bam, tim kiem trong danh sach benh nhan

	if (bnTimThay == NULL) {// Neu van khong tim thay
        printf("Benh nhan voi Ma Dinh Danh %s khong ton tai.\n", maBN_upper);
		char choice_str[10];// Chuoi de luu lua chon cua nguoi dung
        printf("Ban co muon tao moi ho so benh nhan khong? (c/k): ");
        docDongAnToan(choice_str, sizeof(choice_str), stdin);// Doc lua chon cua nguoi dung
		if (strlen(choice_str) > 0 && tolower(choice_str[0]) == 'c') {// Neu nguoi dung chon tao moi
			BenhNhan bnMoiTam;// Bien tam de luu thong tin benh nhan moi
            printf("--- Nhap Thong Tin Benh Nhan Moi ---\n");
			if (nhapThongTinBenhNhanTuBanPhim(&bnMoiTam, *dsBenhNhanConTro, *soLuongBenhNhanConTro, bbBenhNhan)) {// Neu nhap thong tin thanh cong

				if (!themBenhNhan(dsBenhNhanConTro, soLuongBenhNhanConTro, &bbBenhNhan, bnMoiTam, fileBenhNhanCSV)) {// Neu them benh nhan thanh cong
					printf("Loi: Khong thể tao benh nhan moi.\n");// Bao loi neu khong the them benh nhan
                    return -1;
                }
				bnTimThay = timBenhNhanTheoMaBangBam(bbBenhNhan, bnMoiTam.maDinhDanh);// Tim lai benh nhan moi trong bang bam
				if (bnTimThay == NULL) {// Neu van khong tim thay trong bang bam
					bnTimThay = (BenhNhan*)timBenhNhanTheoMaChuoi(*dsBenhNhanConTro, *soLuongBenhNhanConTro, bnMoiTam.maDinhDanh);// Tim lai trong danh sach benh nhan
                }
				if (bnTimThay == NULL) {// Neu van khong tim thay
					fprintf(stderr, "LOI NGHIEM TRONG: Khong tim thay benh nhan %s ngay sau khi tao.\n", bnMoiTam.maDinhDanh);// Bao loi neu khong tim thay benh nhan
                    return -1;
                }
                printf("Da tao benh nhan moi voi Ma Dinh Danh: %s\n", bnTimThay->maDinhDanh);
				strncpy(maBN_upper, bnTimThay->maDinhDanh, sizeof(maBN_upper) - 1); // Sao chep ma dinh danh benh nhan moi vao chuoi
				maBN_upper[sizeof(maBN_upper) - 1] = '\0';//Bao dam ket thuc chuoi, tranh tran bo nho

            }
            else {
                printf("Huy tao benh nhan moi. Dang ky kham bi huy.\n");
                return -1;
            }
        }
        else {
            printf("Dang ky kham bi huy do benh nhan khong ton tai va khong tao moi.\n");
            return -1;
        }
    }
    // Chon khoa phong
	char maKP_input[MAX_LEN_MA_KHOA_PHONG];// Chuoi de nhap ma khoa phong
	char maKP_upper_norm[MAX_LEN_MA_KHOA_PHONG]; // Chuoi de chuyen ma khoa phong sang chu hoa
	KhoaPhong* kpChon = NULL;// Bien de luu khoa phong chon 

    do {
        printf("\nDanh sach Khoa Phong hien co:\n");
		if (dsKhoaPhong && soLuongKhoaPhong > 0) {// Neu danh sach khoa phong khong rong
			inDanhSachKhoaPhongConsole(dsKhoaPhong, soLuongKhoaPhong);// In danh sach khoa phong
        }
		else {// Neu danh sach khoa phong rong
            printf("Khong co khoa phong nao trong he thong.\n");
            printf("Vui long them khoa phong truoc khi dang ky kham. Huy dang ky.\n");
			return -1;// Bao loi
        }

        printf("Nhap Ma Khoa Phong muon dang ky kham (KHONG DUOC BO TRONG): ");
		if (docDongAnToan(maKP_input, sizeof(maKP_input), stdin) == NULL || strlen(maKP_input) == 0) {// Kiem tra neu nhap rong
            printf("LOI: Ma Khoa Phong khong duoc de trong. Huy dang ky.\n");
			return -1;// Bao loi
        }
		strncpy(maKP_upper_norm, maKP_input, sizeof(maKP_upper_norm) - 1);// Sao chep ma khoa phong vao chuoi
		maKP_upper_norm[sizeof(maKP_upper_norm) - 1] = '\0';// Bao dam ket thuc chuoi, tranh tran bo nho
		for (int k = 0; maKP_upper_norm[k]; k++) maKP_upper_norm[k] = toupper(maKP_upper_norm[k]);//Chuyen ma khoa phong sang chu hoa

        if (bbKhoaPhong) {
			kpChon = timKhoaPhongTheoMaBangBam(bbKhoaPhong, maKP_upper_norm);// Tim kiem khoa phong trong bang bam
        }
        else if (dsKhoaPhong) {
			kpChon = timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, maKP_upper_norm);// Tim kiem khoa phong trong danh sach
        }

        if (kpChon == NULL) {
			printf("LOI: Ma Khoa Phong '%s' khong ton tai. Vui long chon lai.\n", maKP_upper_norm);// Bao loi neu khong tim thay khoa phong
        }
	} while (kpChon == NULL);// Lap lai neu khong tim thay khoa phong

	LanKham lkMoi; // Bien de luu thong tin lan kham moi
	lkMoi.maLanKham = taoMaLanKhamMoi(*dsLanKhamConTro, *soLuongLanKhamConTro);// Tao ma lan kham moi
	// Sao chep thong tin benh nhan vao lan kham moi
	strncpy(lkMoi.maDinhDanh, bnTimThay->maDinhDanh, sizeof(lkMoi.maDinhDanh) - 1);
	lkMoi.maDinhDanh[sizeof(lkMoi.maDinhDanh) - 1] = '\0';

	// Sao chep ten benh nhan vao lan kham moi
	strncpy(lkMoi.tenBenhNhan, bnTimThay->ten, sizeof(lkMoi.tenBenhNhan) - 1);
	lkMoi.tenBenhNhan[sizeof(lkMoi.tenBenhNhan) - 1] = '\0';

	// Sao chep ma khoa phong vao lan kham moi
    strncpy(lkMoi.maKhoaPhong, kpChon->maKhoaPhong, sizeof(lkMoi.maKhoaPhong) - 1);
    lkMoi.maKhoaPhong[sizeof(lkMoi.maKhoaPhong) - 1] = '\0';

    printf("Nhap Ly Do Kham (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(lkMoi.lyDoKham, sizeof(lkMoi.lyDoKham), stdin) == NULL || strlen(lkMoi.lyDoKham) == 0) {
        printf("LOI: Ly do kham khong duoc de trong. Huy dang ky.\n");
		return -1;// Bao loi neu ly do kham rong    
    }

	char mucDoUTStr[30];// Chuoi de luu muc do uu tien
    printf("Nhap Muc Do Uu Tien (CapCuu, KhanCap, ThongThuong, TaiKham, UuTienKhac) [ThongThuong]: ");
	docDongAnToan(mucDoUTStr, sizeof(mucDoUTStr), stdin);// Doc muc do uu tien tu ban phim
	lkMoi.mucDoUuTien = (strlen(mucDoUTStr) == 0) ? THONG_THUONG : stringToMucDoUuTien(mucDoUTStr);// Chuyen chuoi muc do uu tien sang enum

	// Cap nhat cac truong con lai trong lan kham moi
    lkMoi.gioDangKyThanhCong = time(NULL);
    lkMoi.trangThai = LK_DANG_CHO;
    strcpy(lkMoi.maBacSi, "CHUA_GAN");
    lkMoi.ngayGioKham = 0;
    strcpy(lkMoi.chanDoan, "CHUA_KHAM");
    strcpy(lkMoi.phuongPhapDieuTri, "CHUA_KHAM");
    strcpy(lkMoi.ghiChuBacSi, "-1");
    lkMoi.ngayTaiKham.ngay = -1; lkMoi.ngayTaiKham.thang = -1; lkMoi.ngayTaiKham.nam = -1;
    lkMoi.coLichTaiKham = 0;
    lkMoi.coDonThuoc = 0;
    lkMoi.maDonThuocChiTiet = -1;

    if (themLanKham(dsLanKhamConTro, soLuongLanKhamConTro, bbLanKham_ptr, lkMoi, fileLanKhamCSV)) {
        // Them vao hang doi cua khoa cu the
        if (bbHangDoiTheoKhoa) {
			if (!themVaoHangDoiTheoKhoa(bbHangDoiTheoKhoa, lkMoi.maKhoaPhong, lkMoi)) {// Neu them vao hang doi khoa that bai
                fprintf(stderr, "LOI: Khong them duoc Lan Kham %d vao hang doi khoa %s.\n", lkMoi.maLanKham, lkMoi.maKhoaPhong);
                fprintf(stderr, "Xoa Lan Kham %d vua them.\n", lkMoi.maLanKham);

				if (!xoaLanKham(dsLanKhamConTro, soLuongLanKhamConTro, *bbLanKham_ptr, lkMoi.maLanKham, fileLanKhamCSV)) {// Neu xoa khong thanh cong
                    fprintf(stderr, "LOI ROLLBACK: Khong the xoa Lan Kham %d khoi dsLanKham/bbLanKham.\n", lkMoi.maLanKham);

                }
                else {
					printf("Da xoa Lan Kham %d.\n", lkMoi.maLanKham);// Bao da xoa thanh cong   
                }
                return -1;
            }
        }
        else {
            fprintf(stderr, "CANH BAO: Bang bam hang doi theo khoa khong kha dung. Lan kham %d chua duoc them vao hang doi khoa phong.\n", lkMoi.maLanKham);
        }
        printf("Dang ky kham thanh cong!\n");
        printf("  Ma Lan Kham: %d\n", lkMoi.maLanKham);
        printf("  Benh Nhan: %s (Ma: %s)\n", lkMoi.tenBenhNhan, lkMoi.maDinhDanh);
        printf("  Khoa Phong: %s (Ma: %s)\n", kpChon->tenKhoaPhong, lkMoi.maKhoaPhong);
        printf("  Muc Do Uu Tien: %s\n", mucDoUuTienToString(lkMoi.mucDoUuTien));
        return lkMoi.maLanKham;
    }
    else {
        printf("Loi: Khong the them lan kham vao he thong.\n");
        return -1;
    }
}
// Ham them lan kham
int themLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam** bbLanKham_ptr,
    LanKham lanKhamMoi, const char* tenFile) {
	if (bbLanKham_ptr == NULL || *bbLanKham_ptr == NULL) {// Kiem tra bang bam co ton tai khong
        fprintf(stderr, "LOI HE THONG.\n");
        return 0;
    }
    if (strlen(lanKhamMoi.maDinhDanh) == 0 || strlen(lanKhamMoi.maKhoaPhong) == 0 || strlen(lanKhamMoi.lyDoKham) == 0) {
        fprintf(stderr, "LOI: Thong tin lan kham moi khong hop le!\n");
        return 0;
	}// Kiem tra thong tin lan kham moi co hop le khong

	char maLKStr[MA_LAN_KHAM_STR_LEN];// Chuoi de luu ma lan kham
	intMaLKToString(lanKhamMoi.maLanKham, maLKStr, sizeof(maLKStr));// Chuyen ma lan kham sang chuoi

	if (timKiemTrongBangBam(*bbLanKham_ptr, maLKStr) != NULL) {
        fprintf(stderr, "LOI: Ma Lan Kham %d (%s) da ton tai!\n", lanKhamMoi.maLanKham, maLKStr);
        return 0;// Kiem tra xem ma lan kham da ton tai trong bang bam chua
    }

	LanKham* temp = realloc(*dsLanKhamConTro, (*soLuongConTro + 1) * sizeof(LanKham));// Cap phat bo nho cho danh sach lan kham
    if (temp == NULL) {
        perror("Loi cap phat bo nho");
        return 0;
	}// Neu cap phat thanh cong, gan lai con tro
	*dsLanKhamConTro = temp;// Cap nhat con tro danh sach lan kham
	(*dsLanKhamConTro)[*soLuongConTro] = lanKhamMoi;// Gan lan kham moi vao vi tri cuoi cung trong danh sach
	LanKham* conTroToiLKTrongMang = &((*dsLanKhamConTro)[*soLuongConTro]);// Tao con tro toi lan kham moi trong mang

	if (!chenVaoBangBam(*bbLanKham_ptr, maLKStr, conTroToiLKTrongMang)) {// Chen lan kham moi vao bang bam
        fprintf(stderr, "LOI NGHIEM TRONG: Khong the chen Lan Kham %d (%s) vao bang bam.\n", lanKhamMoi.maLanKham, maLKStr);

        return 0;
	}// Neu chen khong thanh cong, bao loi

	(*soLuongConTro)++;// Tang so luong con tro trong danh sach lan kham

    if ((*bbLanKham_ptr)->soLuongPhanTu / (double)(*bbLanKham_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {// Neu so luong phan tu tren bang bam vuot qua nguong, thuc hien rehash
        if (!rehashBangBam(bbLanKham_ptr)) {
            fprintf(stderr, "LOI: Rehash bang bam LAN KHAM that bai.\n");
		}// Neu rehash that bai, bao loi
	}

	ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);// Ghi danh sach lan kham ra file
    return 1;
}

// Ham xoa lan kham
int xoaLanKham(LanKham** dsLanKhamConTro, int* soLuongConTro,
    BangBam* bbLanKham,
    int maLanKhamCanXoa, const char* tenFile) {
	if (bbLanKham == NULL) {// Kiem tra bang bam co ton tai khong
        fprintf(stderr, "LOI HE THONG: Bang bam Lan Kham khong kha dung khi xoa.\n");
        return 0;
    }
	char maLKStr[MA_LAN_KHAM_STR_LEN];// Chuoi de luu ma lan kham
	intMaLKToString(maLanKhamCanXoa, maLKStr, sizeof(maLKStr));// Chuyen ma lan kham can xoa sang chuoi
	int removedFromHashTable = 0;// Bien kiem tra da xoa khoi bang bam hay chua

	if (timKiemTrongBangBam(bbLanKham, maLKStr) != NULL) {// Kiem tra xem ma lan kham co ton tai trong bang bam khong
		if (xoaKhoiBangBam(bbLanKham, maLKStr)) {// Neu xoa khoi bang bam thanh cong
            removedFromHashTable = 1;
        }
		else {// Neu xoa khong thanh cong
            fprintf(stderr, "LOI: Khong the xoa Lan Kham %d (%s) khoi bang bam.\n", maLanKhamCanXoa, maLKStr);
            return 0;
        }
    }

	int viTri = -1;// Bien de luu vi tri cua lan kham can xoa trong danh sach
	for (int i = 0; i < *soLuongConTro; ++i) {// Duyet qua danh sach lan kham
        if ((*dsLanKhamConTro)[i].maLanKham == maLanKhamCanXoa) {
            viTri = i;
            break;
		}// Neu tim thay lan kham can xoa, cap nhat vi tri
    }
	if (viTri != -1) {// Neu tim thay lan kham can xoa trong danh sach
        if (!removedFromHashTable && timKiemTrongBangBam(bbLanKham, maLKStr) == NULL) {

            fprintf(stderr, "CANH BAO: Lan Kham %d tim thay trong mang nhung khong co trong bang bam truoc khi xoa (hoac da duoc xoa khoi bang bam).\n", maLanKhamCanXoa);
		}// Neu khong tim thay trong bang bam, bao canh bao

		for (int i = viTri; i < (*soLuongConTro - 1); ++i) {// Duyet tu vi tri can xoa den cuoi danh sach
            (*dsLanKhamConTro)[i] = (*dsLanKhamConTro)[i + 1];
        }
		(*soLuongConTro)--;// Giam so luong con tro trong danh sach
        if (*soLuongConTro == 0) {
            free(*dsLanKhamConTro); *dsLanKhamConTro = NULL;
        }// Neu so luong con tro ve 0, giai phong bo nho
        else {
            LanKham* temp_ds = realloc(*dsLanKhamConTro, (*soLuongConTro) * sizeof(LanKham));
            if (temp_ds == NULL && *soLuongConTro > 0) {
                perror("Loi cap phat bo nho");
            }
            else if (*soLuongConTro > 0) *dsLanKhamConTro = temp_ds;

		}// Neu cap phat lai thanh cong, cap nhat con tro danh sach lan kham
        ghiDanhSachLanKhamRaFile(tenFile, *dsLanKhamConTro, *soLuongConTro);
        return 1;
    }
    if (removedFromHashTable && viTri == -1) {
        fprintf(stderr, "LOI DONG BO: Da xoa Lan Kham %d khoi bang bam nhung khong tim thay trong mang.\n", maLanKhamCanXoa);
    }// Neu da xoa khoi bang bam nhung khong tim thay trong mang, bao loi dong bo
    return 0;
}

// Ham sua thong tin lan kham
int capNhatThongTinLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanCapNhat,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV) {
	int indexLK = -1;// Bien de luu vi tri cua lan kham can cap nhat
    for (int i = 0; i < soLuongLanKham; ++i) {
        if (dsLanKham[i].maLanKham == maLanKhamCanCapNhat) {
            indexLK = i;
            break;
		}// Duyet qua danh sach lan kham de tim vi tri cua lan kham can cap nhat
    }

    if (indexLK == -1) {
        printf("Khong tim thay Lan Kham voi Ma: %d\n", maLanKhamCanCapNhat);
        return 0;
	}// Neu khong tim thay lan kham can cap nhat, bao loi

	LanKham* lk = &dsLanKham[indexLK];// Lay con tro toi lan kham can cap nhat
	TrangThaiLanKham trangThaiCu = lk->trangThai;// Luu trang thai cu cua lan kham
	char chanDoanCu[sizeof(lk->chanDoan)];// Bien tam de luu chan doan cu
	strcpy(chanDoanCu, lk->chanDoan);// Sao chep chan doan cu vao bien tam

    if (lk->trangThai == LK_DA_HOAN_THANH || lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
        printf("Lan kham (Ma: %d) da o trang thai '%s'. Khong the cap nhat theo quy trinh nay.\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
	}// Kiem tra trang thai cua lan kham, neu da hoan thanh, huy hoac lo, bao loi

    if (lk->trangThai != LK_DANG_CHO && lk->trangThai != LK_DANG_KHAM) {
        printf("Lan kham (Ma: %d) khong o trang thai phu hop de cap nhat (hien tai: '%s').\n",
            lk->maLanKham, trangThaiLanKhamToString(lk->trangThai));
        return 0;
	}// Kiem tra neu trang thai la dang cho hoac dang kham, neu khong bao loi


    printf("\n--- CAP NHAT THONG TIN LAN KHAM MA: %d (Benh nhan: %s, Khoa: %s) ---\n", lk->maLanKham, lk->tenBenhNhan, lk->maKhoaPhong);

	char denKhamChoice_str[10];// Chuoi de luu lua chon den kham
    printf("Benh nhan co den kham khong? (c/k): ");
    docDongAnToan(denKhamChoice_str, sizeof(denKhamChoice_str), stdin);

    if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'k') {

        strcpy(lk->ghiChuBacSi, "Benh nhan khong den");
	}// Neu nguoi dung chon khong den kham, cap nhat ghi chu bac si

    else if (strlen(denKhamChoice_str) > 0 && tolower(denKhamChoice_str[0]) == 'c') {
        lk->trangThai = LK_DA_HOAN_THANH;

        // Nhap ma bac si
        char maBacSiNhap[MAX_LEN_MA_BAC_SI];
        const BacSi* bacSiTimThay = NULL;
        do {
            printf("Nhap Ma Bac Si kham: ");
            if (docDongAnToan(maBacSiNhap, sizeof(maBacSiNhap), stdin) == NULL || strlen(maBacSiNhap) == 0) {
                inThongDiepLoi("Ma Bac Si khong duoc de trong. Vui long nhap lai.");

                continue;
            }
			for (int i = 0; maBacSiNhap[i]; i++) maBacSiNhap[i] = toupper(maBacSiNhap[i]);// Chuyen ma bac si nhap vao chu hoa

			bacSiTimThay = NULL;// Reset bien tim thay bac si
            if (bbBacSi != NULL) {
                bacSiTimThay = timBacSiTheoMaBangBam(bbBacSi, maBacSiNhap);
			}// Neu bang bam bac si khong NULL, tim kiem trong bang bam
            else {

                inThongDiepLoi("He thong du lieu Bac Si (Bang Bam) khong kha dung. Khong the xac thuc Ma Bac Si.");

                strcpy(lk->maBacSi, "CHUA_GAN_LOI_BB");
                inThongDiepLoi("Khong the gan Ma Bac Si do loi he thong.");
                break;
			}// Neu khong tim thay trong bang bam, tim kiem trong danh sach bac si

            if (bacSiTimThay != NULL) {
                strncpy(lk->maBacSi, bacSiTimThay->maBacSi, sizeof(lk->maBacSi) - 1);
                lk->maBacSi[sizeof(lk->maBacSi) - 1] = '\0';
                printf("Da chon Bac Si: %s (Ma: %s)\n", bacSiTimThay->tenBacSi, bacSiTimThay->maBacSi);
                break;
			}// Neu tim thay bac si trong bang bam, cap nhat ma bac si

            else {

                printf("CANH BAO: Ma Bac Si '%s' khong tim thay trong he thong.\n", maBacSiNhap);
                printf("Ban co chac chan muon su dung Ma Bac Si '%s' nay (co the khong ton tai)? (c/k): ", maBacSiNhap);
                char confirmBS[10];
                docDongAnToan(confirmBS, sizeof(confirmBS), stdin);
                if (strlen(confirmBS) > 0 && tolower(confirmBS[0]) == 'c') {
                    strncpy(lk->maBacSi, maBacSiNhap, sizeof(lk->maBacSi) - 1);
                    lk->maBacSi[sizeof(lk->maBacSi) - 1] = '\0';
                    inThongDiepThanhCong("Da ghi nhan Ma Bac Si theo y nguoi dung.");
                    break;
				}// Neu nguoi dung xac nhan su dung ma bac si khong ton tai, cap nhat ma bac si theo y nguoi dung
                inThongDiepLoi("Vui long nhap lai Ma Bac Si hoac chon mot Ma Bac Si ton tai.");
            }
        } while (1);

		// Nhap Chan Doan
        printf("Nhap Chan Doan: ");
        docDongAnToan(lk->chanDoan, sizeof(lk->chanDoan), stdin);
        if (strlen(lk->chanDoan) == 0) {
            printf("LOI: Chan Doan khong duoc de trong cho lan kham da hoan thanh. Cap nhat that bai.\n");
            lk->trangThai = trangThaiCu;
            return 0;
		}// Neu chan doan rong, bao loi va khong cap nhat

		// Nhap Phuong Phap Dieu Tri
        printf("Nhap Phuong Phap Dieu Tri: ");
        docDongAnToan(lk->phuongPhapDieuTri, sizeof(lk->phuongPhapDieuTri), stdin);
        if (strlen(lk->phuongPhapDieuTri) == 0) {
            printf("LOI: Phuong Phap Dieu Tri khong duoc de trong cho lan kham da hoan thanh. Cap nhat that bai.\n");
            strcpy(lk->chanDoan, chanDoanCu);
            lk->trangThai = trangThaiCu;
            return 0;
		}// Neu phuong phap dieu tri rong, bao loi va khong cap nhat

		// Nhap Ghi Chu Bac Si
        printf("Nhap Ghi Chu Bac Si: ");
        docDongAnToan(lk->ghiChuBacSi, sizeof(lk->ghiChuBacSi), stdin);
        if (strlen(lk->ghiChuBacSi) == 0) {
            strcpy(lk->ghiChuBacSi, "Khong");
		}// Neu ghi chu rong, gan gia tri mac dinh

		char taiKhamChoice_str_capnhat[10];// Chuoi de luu lua chon co lich tai kham
        printf("Co lich tai kham khong? (c/k): ");
        docDongAnToan(taiKhamChoice_str_capnhat, sizeof(taiKhamChoice_str_capnhat), stdin);
		if (strlen(taiKhamChoice_str_capnhat) > 0 && tolower(taiKhamChoice_str_capnhat[0]) == 'c') {// Neu nguoi dung chon co lich tai kham
			// Cap nhat thong tin lich tai kham
            lk->coLichTaiKham = 1;
            char bufferNgayTK[30];
            int ngay, thang, nam;
            int nhapNgayThanhCong = 0;
            do {
                printf("Nhap Ngay Tai Kham (dd mm yyyy, vd: 20 12 2025): ");
                docDongAnToan(bufferNgayTK, sizeof(bufferNgayTK), stdin);
				if (sscanf(bufferNgayTK, "%d %d %d", &ngay, &thang, &nam) == 3) {// Kiem tra dinh dang ngay tai kham

					if (ngay >= 1 && ngay <= 31 && thang >= 1 && thang <= 12 && nam >= 2024 && nam <= 2100) {// Kiem tra ngay, thang, nam hop le

                        time_t now = lk->ngayGioKham == 0 ? time(NULL) : lk->ngayGioKham;
                        struct tm partsTK = { 0 };
                        partsTK.tm_mday = ngay;
                        partsTK.tm_mon = thang - 1;
                        partsTK.tm_year = nam - 1900; // 
                        time_t thoiGianTK = mktime(&partsTK);

						// Kiem tra xem ngay tai kham co sau ngay kham hien tai hay khong
                        if (difftime(thoiGianTK, now) > 0) {
                            lk->ngayTaiKham.ngay = ngay;
                            lk->ngayTaiKham.thang = thang;
                            lk->ngayTaiKham.nam = nam;
                            nhapNgayThanhCong = 1;
                        }
                        else {
                            inThongDiepLoi("Ngay tai kham phai sau ngay kham/ngay hien tai.");
                        }
                    }
                    else {
                        inThongDiepLoi("Ngay, thang, hoac nam tai kham khong hop le.");
                    }
                }
                else {
                    inThongDiepLoi("Dinh dang ngay tai kham khong dung. Vui long nhap lai.");
                }
            } while (!nhapNgayThanhCong);
        }
		else {// Neu nguoi dung chon khong co lich tai kham
            lk->coLichTaiKham = 0;
            lk->ngayTaiKham.ngay = -1;
            lk->ngayTaiKham.thang = -1;
            lk->ngayTaiKham.nam = -1;
        }

		char donThuocChoice_str_capnhat[10];// Chuoi de luu lua chon co don thuoc
        printf("Co ke don thuoc khong? (c/k): ");
        docDongAnToan(donThuocChoice_str_capnhat, sizeof(donThuocChoice_str_capnhat), stdin);
		if (strlen(donThuocChoice_str_capnhat) > 0 && tolower(donThuocChoice_str_capnhat[0]) == 'c') {// Neu nguoi dung chon co don thuoc
			// Cap nhat thong tin don thuoc
            lk->coDonThuoc = 1;
            lk->maDonThuocChiTiet = lk->maLanKham;
        }
		else {// Neu nguoi dung chon khong co don thuoc
			// Cap nhat thong tin don thuoc
            lk->coDonThuoc = 0;
            lk->maDonThuocChiTiet = -1;
        }
        printf("Da cap nhat thong tin lan kham thanh cong (trang thai: Da Hoan Thanh).\n");
    }
    else {
        printf("Lua chon khong hop le. Khong thay doi.\n");
        return 0;
    }

    ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
    return 1;
}

// Ham sua chi tiet lan kham duoc chon
int suaChiTietLanKham(LanKham dsLanKham[], int soLuongLanKham, int maLanKhamCanSua,
    const BacSi dsBacSi[], int soLuongBacSi,
    const KhoaPhong dsKhoaPhong[], int soLuongKhoaPhong,
    BangBam* bbBacSi,
    BangBam* bbKhoaPhong,
    const char* fileLanKhamCSV) {

	int indexLK = -1;// Bien de luu vi tri cua lan kham can sua
	for (int i = 0; i < soLuongLanKham; ++i) {// Duyet qua danh sach lan kham
        if (dsLanKham[i].maLanKham == maLanKhamCanSua) {
            indexLK = i;
            break;
		} // Neu tim thay lan kham can sua, cap nhat vi tri
	}

	if (indexLK == -1) {// Neu khong tim thay lan kham can sua
		char errorMessage[100];// Bien de luu thong diep loi
		snprintf(errorMessage, sizeof(errorMessage), "Khong tim thay Lan Kham voi Ma: %d de sua.", maLanKhamCanSua);// Tao thong diep loi
		inThongDiepLoi(errorMessage);// In thong diep loi
        return 0;
    }
	LanKham* lkCanSua = &dsLanKham[indexLK];// Lay con tro toi lan kham can sua
	LanKham lkTamSua = *lkCanSua;// Sao chep lan kham can sua vao bien tam de sua

	int luaChonSuaNoiBo;// Bien luu lua chon sua noi bo
	char bufferNhap[512];// Buffer de luu nhap tu ban phim

	do {// Vong lap de cho nguoi dung sua thong tin
        xoaManHinhConsole();
        printf("\n--- SUA CHI TIET LAN KHAM (Ma LK: %d, Benh Nhan: %s, Khoa: %s, Trang Thai: %s) ---\n",
            lkTamSua.maLanKham, lkTamSua.tenBenhNhan, lkTamSua.maKhoaPhong, trangThaiLanKhamToString(lkTamSua.trangThai));
		// Hien thi thong tin hien tai cua lan kham
        printf("  1. Ma Bac Si        (Hien tai: %s)\n", (strlen(lkTamSua.maBacSi) > 0 && strcmp(lkTamSua.maBacSi, "CHUA_GAN") != 0) ? lkTamSua.maBacSi : "CHUA_GAN");
        printf("  2. Ma Khoa Phong    (Hien tai: %s)\n", (strlen(lkTamSua.maKhoaPhong) > 0 && strcmp(lkTamSua.maKhoaPhong, "CHUA_GAN") != 0) ? lkTamSua.maKhoaPhong : "CHUA_GAN");
        printf("  3. Ly Do Kham       (Hien tai: %s)\n", strlen(lkTamSua.lyDoKham) > 0 ? lkTamSua.lyDoKham : "Khong ro");
        printf("  4. Chan Doan        (Hien tai: %s)\n", (strlen(lkTamSua.chanDoan) > 0 && strcmp(lkTamSua.chanDoan, "CHUA_KHAM") != 0) ? lkTamSua.chanDoan : "CHUA_KHAM/CHUA_NHAP");
        printf("  5. Phuong Phap Dieu Tri (Hien tai: %s)\n", (strlen(lkTamSua.phuongPhapDieuTri) > 0 && strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM") != 0) ? lkTamSua.phuongPhapDieuTri : "CHUA_KHAM/CHUA_NHAP");
        printf("  6. Ghi Chu Bac Si   (Hien tai: %s)\n", (strlen(lkTamSua.ghiChuBacSi) > 0 && strcmp(lkTamSua.ghiChuBacSi, "-1") != 0) ? lkTamSua.ghiChuBacSi : "Khong co");
        printf("  7. Thay doi Trang Thai\n");
        printf("-------------------------------------------------\n");
        printf("  0. HOAN TAT SUA DOI VA LUU\n");
        printf("  9. Huy bo tat ca thay doi va quay lai\n");

		// Nhap lua chon sua noi bo
        luaChonSuaNoiBo = nhapSoNguyenCoGioiHan("Lua chon cua ban", 0, 9);
        if (luaChonSuaNoiBo < 0) { luaChonSuaNoiBo = 9; }

        switch (luaChonSuaNoiBo) {
        case 1: // Sua ma bac si
            printf("Nhap Ma Bac Si moi (KHONG DUOC BO TRONG): ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) == 0) {
                inThongDiepLoi("Ma Bac Si moi khong duoc de trong. Thao tac huy.");
                break;
			}// Neu ma bac si moi khong de trong, bao loi
			for (int k = 0; bufferNhap[k]; ++k) bufferNhap[k] = toupper(bufferNhap[k]); // Chuyen ma bac si nhap vao chu hoa
			const BacSi* bsKiemTra = NULL;// Bien de kiem tra ma bac si moi
			if (bbBacSi != NULL) {// Neu bang bam bac si khong NULL, tim kiem trong bang bam
                bsKiemTra = timBacSiTheoMaBangBam(bbBacSi, bufferNhap); 
            }
            else {
                inThongDiepLoi("He thong du lieu Bac Si khong kha dung. Khong the kiem tra MaBS moi.");
                break;
            }
            if (bsKiemTra != NULL) {
                printf("LOI: Ma Bac Si moi '%s' da ton tai. Vui long chon ma khac.\n", bufferNhap);
            }
            else {

				if (bbBacSi != NULL) {// Neu bang bam bac si khong NULL, xoa ma bac si cu khoi bang bam
                    xoaKhoiBangBam(bbBacSi, lkTamSua.maBacSi);
                }
				strncpy(lkTamSua.maBacSi, bufferNhap, sizeof(lkTamSua.maBacSi) - 1);// Sao chep ma bac si moi vao lan kham tam sua
				lkTamSua.maBacSi[sizeof(lkTamSua.maBacSi) - 1] = '\0';// Bao dam ket thuc chuoi
                inThongDiepThanhCong("Ma Bac Si se duoc cap nhat thanh '%s' khi ban luu thay doi.", lkTamSua.maBacSi);
            }
            break;
        case 2: // Sua ma khoa phong
            printf("Nhap Ma Khoa Phong moi: ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) > 0) {
				for (int k = 0; bufferNhap[k]; ++k) bufferNhap[k] = toupper(bufferNhap[k]);// Chuyen ma khoa phong nhap vao chu hoa
                // Neu bang bam khoa phong khong NULL, tim kiem trong bang bam
				const KhoaPhong* kp = bbKhoaPhong ? timKhoaPhongTheoMaBangBam(bbKhoaPhong, bufferNhap) : timKhoaPhongTheoMa(dsKhoaPhong, soLuongKhoaPhong, bufferNhap);
                if (kp) strcpy(lkTamSua.maKhoaPhong, kp->maKhoaPhong);// Neu tim thay khoa phong trong bang bam, cap nhat ma khoa phong
				else printf("CANH BAO: Ma Khoa Phong '%s' khong tim thay. Giu nguyen ma cu.\n", bufferNhap);// Neu khong tim thay, bao canh bao
            }
            else printf("LOI: Ma Khoa Phong khong duoc de trong. Giu nguyen ma cu.\n");
            break;
        case 3: // Sua ly do kham
            printf("Nhap Ly Do Kham moi: ");
            docDongAnToan(lkTamSua.lyDoKham, sizeof(lkTamSua.lyDoKham), stdin);
			if (strlen(lkTamSua.lyDoKham) == 0) {
                printf("LOI: Ly Do Kham khong duoc de trong. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.lyDoKham, lkCanSua->lyDoKham);
            }//Neu ly do kham rong, bao loi va khoi phuc gia tri cu
            break;
        case 4: // Sua chuan doan
            printf("Nhap Chan Doan moi : ");
            docDongAnToan(lkTamSua.chanDoan, sizeof(lkTamSua.chanDoan), stdin);
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH && strlen(lkTamSua.chanDoan) == 0) {
                printf("LOI: Chan Doan khong duoc de trong khi Da Hoan Thanh. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.chanDoan, lkCanSua->chanDoan);
            }// Neu chan doan rong khi da hoan thanh, bao loi va khoi phuc gia tri cu
            else if (strlen(lkTamSua.chanDoan) == 0) {
                strcpy(lkTamSua.chanDoan, "CHUA_KHAM");
            }//Neu chan doan rong, gan gia tri mac dinh
            break;
        case 5: // Sua phuong phap dieu tri
            printf("Nhap Phuong Phap Dieu Tri moi:  ");
            docDongAnToan(lkTamSua.phuongPhapDieuTri, sizeof(lkTamSua.phuongPhapDieuTri), stdin);
            if (lkTamSua.trangThai == LK_DA_HOAN_THANH && strlen(lkTamSua.phuongPhapDieuTri) == 0) {
                printf("LOI: Phuong Phap Dieu Tri khong duoc de trong khi Da Hoan Thanh. Khoi phuc gia tri cu.\n");
                strcpy(lkTamSua.phuongPhapDieuTri, lkCanSua->phuongPhapDieuTri);
			}// Neu phuong phap dieu tri rong khi da hoan thanh, bao loi va khoi phuc gia tri cu

            else if (strlen(lkTamSua.phuongPhapDieuTri) == 0) {
                strcpy(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM");
			}// Neu phuong phap dieu tri rong, gan gia tri mac dinh
            break;
        case 6: // Sua ghi chu
            printf("Nhap Ghi Chu Bac Si moi:  ");
            docDongAnToan(lkTamSua.ghiChuBacSi, sizeof(lkTamSua.ghiChuBacSi), stdin);
            if (strlen(lkTamSua.ghiChuBacSi) == 0) strcpy(lkTamSua.ghiChuBacSi, "Khong");//Neu ghi chu rong, gan gia tri mac dinh
            break;
        case 7: // Sua trang thai
            printf("Chon Trang Thai moi (DangCho, DangKham, DaHoanThanh, DaHuy, DaLo): ");
            docDongAnToan(bufferNhap, sizeof(bufferNhap), stdin);
            if (strlen(bufferNhap) > 0) {
				TrangThaiLanKham ttMoi = stringToTrangThaiLanKham(bufferNhap);// Chuyen chuoi nhap vao trang thai lan kham

                if (ttMoi == LK_DANG_CHO && my_strcasecmp(bufferNhap, "DangCho") != 0 && strlen(bufferNhap) > 0) {
                    printf("CANH BAO: Trang thai nhap khong hop le. Giu nguyen trang thai cu.\n");
                }// Neu trang thai nhap khong hop le, bao canh bao
                else {
					lkTamSua.trangThai = ttMoi;// Cap nhat trang thai moi

                    if (ttMoi == LK_DANG_CHO || ttMoi == LK_DANG_KHAM) {
                        strcpy(lkTamSua.chanDoan, "CHUA_KHAM");
                        strcpy(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM");
                        strcpy(lkTamSua.maBacSi, "CHUA_GAN");

                    }//Neu trang thai moi la DangCho hoac DangKham, reset chan doan, phuong phap dieu tri va ma bac si
                }
            }
            else {
                printf("LOI: Trang thai khong duoc de trong. Giu nguyen trang thai cu.\n");
			}// Neu trang thai rong, bao loi va giu trang thai cu
            break;
        case 0: // Hoan tat va luu
           
			if (strlen(lkTamSua.maDinhDanh) == 0) {// Kiem tra ma dinh danh benh nhan
                inThongDiepLoi("LOI HE THONG: Ma Dinh Danh Benh Nhan bi rong. Vui long bao cao loi nay.");
                luaChonSuaNoiBo = -1;
                dungManHinhCho();
                break;
            }
			if (strlen(lkTamSua.maKhoaPhong) == 0 || strcmp(lkTamSua.maKhoaPhong, "CHUA_GAN") == 0) {// Kiem tra ma khoa phong
                printf("LOI KHI LUU: Ma Khoa Phong khong duoc de trong/CHUA_GAN. Vui long sua lai.\n");
                luaChonSuaNoiBo = -1; dungManHinhCho(); break;
            }
			if (strlen(lkTamSua.lyDoKham) == 0) {// Kiem tra ly do kham
                printf("LOI KHI LUU: Ly Do Kham khong duoc de trong. Vui long sua lai.\n");
                luaChonSuaNoiBo = -1; dungManHinhCho(); break;
            }

			if (lkTamSua.trangThai == LK_DA_HOAN_THANH) {// Kiem tra trang thai da hoan thanh
                if (strlen(lkTamSua.chanDoan) == 0 || strcmp(lkTamSua.chanDoan, "CHUA_KHAM") == 0 || my_strcasecmp(lkTamSua.chanDoan, "CHUA_NHAP_CD_DOC") == 0 || my_strcasecmp(lkTamSua.chanDoan, "CHUA_NHAP_CD_GHI") == 0) {
                    printf("LOI KHI LUU: Chan Doan khong duoc de trong/mac dinh cho lan kham da hoan thanh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
                if (strlen(lkTamSua.phuongPhapDieuTri) == 0 || strcmp(lkTamSua.phuongPhapDieuTri, "CHUA_KHAM") == 0 || my_strcasecmp(lkTamSua.phuongPhapDieuTri, "CHUA_NHAP_PPTT_DOC") == 0 || my_strcasecmp(lkTamSua.phuongPhapDieuTri, "CHUA_NHAP_PPTT_GHI") == 0) {
                    printf("LOI KHI LUU: Phuong Phap Dieu Tri khong duoc de trong/mac dinh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
                if (strlen(lkTamSua.maBacSi) == 0 || strcmp(lkTamSua.maBacSi, "CHUA_GAN") == 0) {
                    printf("LOI KHI LUU: Ma Bac Si khong duoc de trong/CHUA_GAN cho lan kham da hoan thanh. Vui long sua lai.\n");
                    luaChonSuaNoiBo = -1; dungManHinhCho(); break;
                }
            }
			*lkCanSua = lkTamSua;// Cap nhat lan kham can sua voi thong tin da sua
            ghiDanhSachLanKhamRaFile(fileLanKhamCSV, dsLanKham, soLuongLanKham);
            printf("THANH CONG: Da luu cac thay doi cho Lan Kham Ma: %d.\n", maLanKhamCanSua);

            break;
        case 9: // Huy bo
            printf("Da huy bo tat ca thay doi.\n");

            return 0;
        default:
            printf("LOI: Lua chon khong hop le.\n");
            dungManHinhCho();
            break;
        }
		if (luaChonSuaNoiBo >= 1 && luaChonSuaNoiBo <= 7 && luaChonSuaNoiBo != 0 && luaChonSuaNoiBo != 9) {// Neu lua chon sua noi bo hop le
            printf("Thong tin da duoc cap nhat tam thoi. Chon truong khac de sua hoac Luu/Huy.\n");
            dungManHinhCho();
        }
    } while (luaChonSuaNoiBo != 0 && luaChonSuaNoiBo != 9);

    return (luaChonSuaNoiBo == 0);
}
// Ham in danh sach lan kham ra man hinh 
void inDanhSachLanKhamConsole(const LanKham dsLanKham[], int soLuongLanKham,
    const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
    BangBam* bbBenhNhan) {
    printf("\n--- DANH SACH LAN KHAM BENH ---\n");
    if (soLuongLanKham == 0) {
        printf("Chua co lan kham nao.\n");
        return;
	}// Neu khong co lan kham nao, bao thong bao
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-7s | %-18s | %-25s | %-12s | %-15s | %-18s | %-20s | %-20s | %-10s | %-10s | %-15s | %-20s |\n",
        "STT", "Ma LK", "Ma BN", "Ten Benh Nhan", "Trang Thai", "Muc Do UT", "Ma Khoa Phong", "Ly Do Kham", "Chan Doan", "Tai Kham", "Don Thuoc", "Ma Bac Si", "Ghi Chu BS");
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");


	for (int i = 0; i < soLuongLanKham; ++i) {// Duyet qua danh sach lan kham
        const LanKham* lk = &dsLanKham[i];
        char tenBNHienThi[101] = "N/A";
        char tenBNRutGon[26];
        char lyDoKhamRutGon[21];
        char chanDoanRutGon[21];
        char ghiChuBSRutGon[21];
        char maBacSiDisplay[16];
       

        // Lay ten benh nhan tu ma dinh danh
        if (bbBenhNhan != NULL && strlen(lk->maDinhDanh) > 0) {
            BenhNhan* bn = timBenhNhanTheoMaBangBam(bbBenhNhan, lk->maDinhDanh);
            if (bn != NULL) {
                strncpy(tenBNHienThi, bn->ten, sizeof(tenBNHienThi) - 1);
                tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
			}// Neu tim thay benh nhan trong bang bam, lay ten benh nhan
        }

        else if (dsBenhNhan != NULL && strlen(lk->maDinhDanh) > 0) {
			const BenhNhan* bnTuyenTinh = timBenhNhanTheoMaChuoi(dsBenhNhan, soLuongBenhNhan, lk->maDinhDanh);// Tim benh nhan theo ma dinh danh
            if (bnTuyenTinh) {
                strncpy(tenBNHienThi, bnTuyenTinh->ten, sizeof(tenBNHienThi) - 1);
                tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
			}// Neu tim thay benh nhan trong danh sach, lay ten benh nhan
        }

        else if (strlen(lk->tenBenhNhan) > 0 && strcmp(lk->tenBenhNhan, "Khong Biet") != 0) {
            strncpy(tenBNHienThi, lk->tenBenhNhan, sizeof(tenBNHienThi) - 1);
            tenBNHienThi[sizeof(tenBNHienThi) - 1] = '\0';
		}// Neu ten benh nhan trong lan kham khac, lay ten benh nhan tu lan kham

		rutGonChuoiHienThi(tenBNRutGon, sizeof(tenBNRutGon), tenBNHienThi, 25);// Rut gon ten benh nhan de hien thi
		rutGonChuoiHienThi(lyDoKhamRutGon, sizeof(lyDoKhamRutGon), lk->lyDoKham, 20);// Rut gon ly do kham de hien thi


		char ngayTaiKhamStr[12] = "-";// Chuoi de luu ngay tai kham
        if (lk->coLichTaiKham && lk->ngayTaiKham.ngay > 0) {
            snprintf(ngayTaiKhamStr, sizeof(ngayTaiKhamStr), "%02d/%02d/%04d", lk->ngayTaiKham.ngay, lk->ngayTaiKham.thang, lk->ngayTaiKham.nam);
		}// Neu co lich tai kham, dinh dang ngay tai kham
        
	   // Rut gon chuoi hien thi ngay tai kham
        const char* cdDisplay = "N/A";
        const char* ppdtDisplay = "N/A";
        const char* bsDisplay = "N/A";
        const char* gcDisplay = "Khong";

		// Rut gon chan doan, phuong phap dieu tri va ghi chu bac si
        if (lk->trangThai == LK_DA_HOAN_THANH || lk->trangThai == LK_DA_HUY || lk->trangThai == LK_DA_LO) {
            cdDisplay = (strlen(lk->chanDoan) > 0 && strcmp(lk->chanDoan, "CHUA_KHAM") != 0) ? lk->chanDoan : "CHUA_NHAP";
            bsDisplay = (strlen(lk->maBacSi) > 0 && strcmp(lk->maBacSi, "CHUA_GAN") != 0) ? lk->maBacSi : "N/A";
            gcDisplay = (strlen(lk->ghiChuBacSi) > 0 && strcmp(lk->ghiChuBacSi, "Khong") != 0) ? lk->ghiChuBacSi : "Khong";
        }
        rutGonChuoiHienThi(chanDoanRutGon, sizeof(chanDoanRutGon), cdDisplay, 20);
        rutGonChuoiHienThi(maBacSiDisplay, sizeof(maBacSiDisplay), bsDisplay, 15);
        rutGonChuoiHienThi(ghiChuBSRutGon, sizeof(ghiChuBSRutGon), gcDisplay, 20);


        printf("| %-3d | %-7d | %-18s | %-25s | %-12s | %-15s | %-18s | %-20s | %-20s | %-10s | %-10s | %-15s | %-20s |\n",
            i + 1,
            lk->maLanKham,
            lk->maDinhDanh,
            tenBNRutGon,
            trangThaiLanKhamToString(lk->trangThai),
            mucDoUuTienToString(lk->mucDoUuTien),
            lk->maKhoaPhong,
            lyDoKhamRutGon,
            chanDoanRutGon,
            ngayTaiKhamStr,
            lk->coDonThuoc ? "Co" : "Khong",
            maBacSiDisplay,
            ghiChuBSRutGon
        );
    }
    printf("-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

