#include "medicine.h"
#include "csv_handler.h" 
#include "hash_table.h"  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Ham noi bo 
static void xoaDongMoiMed(char* str) {
	if (str == NULL) return; // Neu do dai chuoi la 0 thi return
	size_t len = strlen(str); // Lay do dai cua chuoi
	if (len > 0 && str[len - 1] == '\n') { // Neu ky tu cuoi cung la '\n'
		str[len - 1] = '\0'; // Thay the bang ky tu ket thuc chuoi '\0'
	}
}
static void med_toLowerString(char* dest, const char* src, size_t dest_size) {
	if (src == NULL || dest == NULL || dest_size == 0) return; // Kiem tra con tro va kich thuoc
	size_t i = 0;
	// Chuyen chuoi nguon sang chuoi thuong trong chuoi dich
	for (i = 0; src[i] && i < dest_size - 1; ++i) {
		dest[i] = tolower((unsigned char)src[i]);
	}
	dest[i] = '\0';
}
// Ham tim thuoc theo ten 
static Thuoc* timThuocTheoTen(const Thuoc dsThuoc[], int soLuong, const char* tenThuoc) {
	if (tenThuoc == NULL || strlen(tenThuoc) == 0) return NULL; // Kiem tra ten thuoc NULL hoac rong
	char tenThuocLower[sizeof(((Thuoc*)0)->tenThuoc)];
	med_toLowerString(tenThuocLower, tenThuoc, sizeof(tenThuocLower)); // Chuyen ten thuoc sang chu thuong

	for (int i = 0; i < soLuong; ++i) { // Duyet qua danh sach thuoc
		char currentTenThuocLower[sizeof(dsThuoc[i].tenThuoc)];
		med_toLowerString(currentTenThuocLower, dsThuoc[i].tenThuoc, sizeof(currentTenThuocLower)); // Chuyen ten thuoc hien tai sang chu thuong

		if (strcmp(currentTenThuocLower, tenThuocLower) == 0) { // Neu ten thuoc hien tai chuyen sang thuong bang voi ten thuoc can tim kiem
			return (Thuoc*)&dsThuoc[i]; // Tra ve con tro toi thuoc tim thay
		}
	}
	return NULL;
}

// Ham doc danh sach thuoc tu file CSV
Thuoc* docDanhSachThuoc(const char* tenFile, int* soLuong) {
	FILE* f = fopen(tenFile, "r"); // Mo file de doc 
	if (f == NULL) { // Neu mo file that bai
		*soLuong = 0; // Gan soLuong ve 0
		return NULL; // Tra ve NULL
	}

	Thuoc* ds = NULL; // Con tro toi danh sach thuoc
	int capacity = 0; // Dung luong hien tai cua danh sach thuoc
	*soLuong = 0; // Gan soLuong ve 0 de bat dau
	char line[256]; // Mang de luu tung dong doc duoc tu file
	int lineNumber = 0; // Bien dem so dong da doc

	while (fgets(line, sizeof(line), f) != NULL) { // Doc tung dong tu file
		lineNumber++; // Tang bien dem so dong
		xoaDongMoiMed(line); // Goi ham xoa ky tu '\n' o cuoi dong neu co
		if (strlen(line) < 2) continue; // Neu do dai dong < 2 thi bo qua dong do

		if (*soLuong >= capacity) { // Neu so luong thuoc da dat toi da dung luong hien tai
			capacity = (capacity == 0) ? 10 : capacity * 2; // Tang dung luong gap doi
			Thuoc* temp = realloc(ds, capacity * sizeof(Thuoc)); // Cap phat lai bo nho cho danh sach thuoc
			if (temp == NULL) { // Neu cap phat lai that bai
				perror("Loi cap phat bo nho"); // In thong bao loi
				free(ds); // Giai phong bo nho da cap phat
				fclose(f); // Dong file
				*soLuong = 0; // Gan soLuong ve 0
				return NULL; // Tra ve NULL
			}
			ds = temp; // Cap nhat con tro toi danh sach thuoc moi
		}

		Thuoc t; // Bien tam de luu thong tin thuoc
		char* token; // Con tro de luu tru tung token

		token = strtok(line, ","); // Tach dong theo dau phay
		if (token && strlen(token) > 0) { // Neu token khong rong
			strncpy(t.maThuoc, token, sizeof(t.maThuoc) - 1); // Sao chep ma thuoc vao bien tam
			t.maThuoc[sizeof(t.maThuoc) - 1] = '\0'; // Bao dam ket thuc chuoi bang '\0'
		}
		else {
			fprintf(stderr, "Ma thuoc rong hoac khong hop le. Bo qua dong.\n", lineNumber); // In thong bao loi neu ma thuoc rong hoac khong hop le
			continue;
		}

		token = strtok(NULL, "\n"); // Tach token tiep theo (ten thuoc)
		if (token && strlen(token) > 0) { // Neu token khong rong
			strncpy(t.tenThuoc, token, sizeof(t.tenThuoc) - 1); // Sao chep ten thuoc vao bien tam
			t.tenThuoc[sizeof(t.tenThuoc) - 1] = '\0'; // Bao dam ket thuc chuoi bang '\0'
		}
		else {
			fprintf(stderr, "Ten thuoc rong hoac khong hop le cho MaThuoc '%s'. Bo qua dong.\n", lineNumber, t.maThuoc); // In thong bao loi neu ten thuoc rong hoac khong hop le
			continue;
		}

		ds[*soLuong] = t; // Gan thuoc moi vao danh sach thuoc
		(*soLuong)++; // Tang soLuong thuoc da doc
	}

	fclose(f); // Dong file
	return ds;
}

// Ham ghi danh sach thuoc ra file CSV
void ghiDanhSachThuocRaFile(const char* tenFile, const Thuoc dsThuoc[], int soLuong) {
	FILE* f = fopen(tenFile, "w"); // Mo file de ghi
	if (f == NULL) { // Neu mo file that bai
		perror("Loi mo file de ghi (ghiDanhSachThuocRaFile)"); // In thong bao loi
		return;
	}
	for (int i = 0; i < soLuong; ++i) { // Duyet qua danh sach thuoc
		const char* maThuocOut = (strlen(dsThuoc[i].maThuoc) > 0) ? dsThuoc[i].maThuoc : "MA_THUOC_KHONG_THOA_MAN"; // Neu ma thuoc rong, gan gia tri mac dinh 'MA_THUOC_KHONG
		const char* tenThuocOut = (strlen(dsThuoc[i].tenThuoc) > 0) ? dsThuoc[i].tenThuoc : "TEN_KHONG_THOA_MAN";// Neu ten thuoc rong, gan gia tri mac dinh 'TEN_KHONG_THOA_MAN'
		fprintf(f, "%s,%s\n", maThuocOut, tenThuocOut); // Ghi ma thuoc va ten thuoc vao file
	}
	fclose(f); // Dong file
}


// Ham tim thuoc theo ma - Linear
Thuoc* timThuocTheoMa(const Thuoc dsThuoc[], int soLuong, const char* maThuoc) {
	if (maThuoc == NULL) return NULL; // Neu ma thuoc la NULL thi return NULL
	char maThuocUpper[MAX_LEN_MA_THUOC]; // Mang de luu ma thuoc chuyen sang chuoi in hoa
	strncpy(maThuocUpper, maThuoc, sizeof(maThuocUpper) - 1); // Sao chep ma thuoc vao mang
	maThuocUpper[sizeof(maThuocUpper) - 1] = '\0';
	for (int i = 0; maThuocUpper[i]; i++) maThuocUpper[i] = toupper(maThuocUpper[i]); // Chuyen ma thuoc sang chuoi in hoa

	for (int i = 0; i < soLuong; ++i) { // Duyet qua danh sach thuoc
		if (strcmp(dsThuoc[i].maThuoc, maThuocUpper) == 0) { // Neu ma thuoc hien tai trung voi ma thuoc can tim
			return (Thuoc*)&dsThuoc[i]; // Tra ve con tro toi thuoc tim thay
		}
	}
	return NULL; // Neu khong tim thay thuoc, tra ve NULL
}

// Ham tim thuoc theo ma bang bam
Thuoc* timThuocTheoMaBangBam(BangBam* bbThuoc, const char* maThuoc) {
	if (bbThuoc == NULL || maThuoc == NULL) {
		return NULL; // Neu bang bam hoac ma thuoc la NULL thi return NULL
	}
	char maThuocTimKiem[MAX_LEN_MA_THUOC]; // Mang de luu ma thuoc tim kiem
	strncpy(maThuocTimKiem, maThuoc, sizeof(maThuocTimKiem) - 1); // Sao chep ma thuoc vao mang
	maThuocTimKiem[sizeof(maThuocTimKiem) - 1] = '\0';
	for (int k = 0; maThuocTimKiem[k]; k++) { // Duyet qua tung ky tu trong ma thuoc
		maThuocTimKiem[k] = toupper(maThuocTimKiem[k]); // Chuyen ky tu sang chuoi in hoa
	}
	return (Thuoc*)timKiemTrongBangBam(bbThuoc, maThuocTimKiem); // Tim kiem thuoc trong bang bam
}

// Ham nhap thong tin thuoc tu ban phim
int nhapThongTinThuocTuBanPhim(Thuoc* tConTro,
	BangBam* bbThuocHienCo,
	const Thuoc dsThuocHienCo[], int soLuongThuocHienCo) {
	char bufferMa[MAX_LEN_MA_THUOC]; // Mang de luu ma thuoc nhap vao
	printf("Nhap Ma Thuoc (VD: PARA500, toi da %zu ky tu): ", sizeof(tConTro->maThuoc) - 1); // In thong bao nhap ma thuoc
	if (docDongAnToan(bufferMa, sizeof(bufferMa), stdin) == NULL || strlen(bufferMa) == 0) { // Doc dong tu ban phim
		printf("LOI: Ma Thuoc khong duoc de trong. Huy them thuoc.\n"); // Neu ma thuoc rong thi in thong bao loi
		return 0;
	}
	for (int i = 0; bufferMa[i]; i++) { // Duyet qua tung ky tu trong ma thuoc
		bufferMa[i] = toupper(bufferMa[i]); // Chuyen ky tu sang chuoi in hoa
	}
	if (strlen(bufferMa) == 0) {
		printf("LOI:Khong the chuan hoa. Huy them thuoc.\n"); // Neu ma thuoc rong sau khi chuyen sang chuoi in hoa thi in thong bao loi
		return 0;
	}

	if (bbThuocHienCo != NULL) { // Neu bang bam thuoc hien co khong NULL
		if (timKiemTrongBangBam(bbThuocHienCo, bufferMa) != NULL) {
			printf("LOI: Ma Thuoc '%s' da ton tai. Huy them thuoc.\n", bufferMa); // Neu ma thuoc da ton tai trong bang bam thi in thong bao loi
			return 0;
		}
	}
	else {
		fprintf(stderr, "Bang bam thuoc khong kha dung. Khong the kiem tra trung lap MaThuoc. Huy them thuoc.\n"); // Neu bang bam thuoc hien co NULL thi in thong bao loi
		return 0;
	}
	strncpy(tConTro->maThuoc, bufferMa, sizeof(tConTro->maThuoc) - 1); // Sao chep ma thuoc vao bien tam
	tConTro->maThuoc[sizeof(tConTro->maThuoc) - 1] = '\0';

	//Nhap ten thuoc
	printf("Nhap Ten Thuoc: ", sizeof(tConTro->tenThuoc) - 1);
	if (docDongAnToan(tConTro->tenThuoc, sizeof(tConTro->tenThuoc), stdin) == NULL || strlen(tConTro->tenThuoc) == 0) {
		printf("LOI: Ten Thuoc khong duoc de trong. Huy them thuoc.\n"); // Neu ten thuoc rong thi in thong bao loi
		return 0;
	}
	const Thuoc* thuocTrungTen = timThuocTheoTen(dsThuocHienCo, soLuongThuocHienCo, tConTro->tenThuoc); // Tim thuoc theo ten trong danh sach thuoc hien co
	if (thuocTrungTen != NULL) {
		printf("LOI: Ten Thuoc '%s' da ton tai voi Ma Thuoc khac ('%s'). Huy them thuoc.\n", tConTro->tenThuoc, thuocTrungTen->maThuoc); // Neu ten thuoc trung voi ten thuoc da ton tai trong danh sach thuoc hien co thi in thong bao loi
		return 0; // Neu trung ten thuoc thi tra ve 0 de huy them thuoc
	}

	return 1; // Neu nhap thong tin thuoc thanh cong thi tra ve 1
}

// Ham them thuoc
int themThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
	BangBam** bbThuoc_ptr,
	Thuoc thuocMoi, const char* tenFile) {
	BangBam* bbHienTai = *bbThuoc_ptr;

	if (bbHienTai != NULL) { // Neu bang bam thuoc hien tai khong NULL
		if (timKiemTrongBangBam(bbHienTai, thuocMoi.maThuoc) != NULL) { // Neu ma thuoc da ton tai trong bang bam
			fprintf(stderr, "LOI NGHIEM TRONG: Ma Thuoc '%s' da ton tai!\n", thuocMoi.maThuoc);
			return 0; // Neu ma thuoc da ton tai thi in thong bao loi va tra ve 0 de huy them thuoc
		}
	}
	else {
		fprintf(stderr, "LOI HE THONG: Huy them thuoc '%s'.\n", thuocMoi.maThuoc);
		return 0; // Neu khong, in thong bao loi va tra ve 0 de huy them thuoc
	}
	//Kiem tra cac truong thuocMoi khong duoc rong (da duoc dam bao boi nhapThongTinThuocTuBanPhim)
	if (strlen(thuocMoi.maThuoc) == 0 || strlen(thuocMoi.tenThuoc) == 0) { // Neu ma thuoc hoac ten thuoc rong
		fprintf(stderr, "LOI: Thong tin thuoc moi khong hop le (ma hoac ten rong) khi them.\n"); // In thong bao loi
		return 0; // Huy them thuoc
	}


	Thuoc* temp = realloc(*dsThuocConTro, (*soLuongConTro + 1) * sizeof(Thuoc)); // Cap phat bo nho cho danh sach thuoc moi
	if (temp == NULL) { // Neu cap phat bo nho that bai
		perror("Loi cap phat bo nho"); // In thong bao loi
		return 0; // Huy them thuoc
	}
	*dsThuocConTro = temp; // Cap nhat con tro toi danh sach thuoc moi
	(*dsThuocConTro)[*soLuongConTro] = thuocMoi; // Gan thuoc moi vao vi tri cuoi cung cua danh sach thuoc
	Thuoc* conTroToiThuocTrongMang = &((*dsThuocConTro)[*soLuongConTro]); // Con tro toi thuoc moi trong mang

	if (!chenVaoBangBam(bbHienTai, thuocMoi.maThuoc, conTroToiThuocTrongMang)) { // Neu chen vao bang bam that bai
		fprintf(stderr, "LOI NGHIEM TRONG: Khong the chen Thuoc '%s' vao bang bam.\n", thuocMoi.maThuoc); // In thong bao loi
		return 0; // Huy them thuoc
	}

	(*soLuongConTro)++; // Tang so luong thuoc trong mang

	if ((*bbThuoc_ptr)->soLuongPhanTu / (double)(*bbThuoc_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) { // Neu so luong phan tu tren dung luong bang bam vuot qua nguong
		printf("Thong bao: He so tai cua bang bam THUOC vuot nguong (%d/%d). Dang chuan bi rehash...\n", // In thong bao he so tai cua bang bam thuoc vuot nguong. Dang chuan bi rehash
			(*bbThuoc_ptr)->soLuongPhanTu, (*bbThuoc_ptr)->dungLuong); // Rehash bang bam thuoc
		if (!rehashBangBam(bbThuoc_ptr)) { // Neu rehash that bai
			fprintf(stderr, "LOI: Rehash bang bam THUOC that bai. Chuong trinh co the tiep tuc nhung hieu suat se giam.\n"); // In thong bao loi
		}
	}

	ghiDanhSachThuocRaFile(tenFile, *dsThuocConTro, *soLuongConTro); // Ghi danh sach thuoc ra file
	return 1; // Thong bao them thuoc thanh cong
}

// Ham sua thong tin thuoc 
int suaThongTinThuoc(Thuoc dsThuoc[], int soLuong, const char* maThuocCanSua, Thuoc thongTinMoi, const char* tenFile) {
	char maThuocCanSuaUpper[MAX_LEN_MA_THUOC]; // Mang de luu ma thuoc can sua chuyen sang chuoi in hoa
	strncpy(maThuocCanSuaUpper, maThuocCanSua, sizeof(maThuocCanSuaUpper) - 1); // Sao chep ma thuoc can sua vao mang
	maThuocCanSuaUpper[sizeof(maThuocCanSuaUpper) - 1] = '\0';
	for (int i = 0; maThuocCanSuaUpper[i]; i++) maThuocCanSuaUpper[i] = toupper(maThuocCanSuaUpper[i]); // Chuyen ma thuoc can sua sang chuoi in hoa

	int indexThuocCanSua = -1; // Bien de luu vi tri thuoc can sua trong danh sach thuoc
	for (int i = 0; i < soLuong; ++i) { // Duyet qua danh sach thuoc
		if (strcmp(dsThuoc[i].maThuoc, maThuocCanSuaUpper) == 0) { // Neu ma thuoc hien tai trung voi ma thuoc can sua
			indexThuocCanSua = i; // Luu vi tri thuoc can sua
			break;
		}
	}
	if (indexThuocCanSua == -1) { // Neu khong tim thay thuoc can sua
		printf("Khong tim thay thuoc voi ma '%s' de sua.\n", maThuocCanSua);
		return 0; // Thong bao khong tim thay thuoc can sua
	}
	if (strlen(thongTinMoi.tenThuoc) == 0) { // Neu ten thuoc moi rong
		printf("LOI: Ten thuoc moi khong duoc de trong. Sua that bai.\n");
		return 0; // Thong bao sua that bai
	}
	for (int i = 0; i < soLuong; ++i) { // Duyet qua danh sach thuoc de kiem tra trung ten
		if (i == indexThuocCanSua) continue; // Neu la thuoc dang sua thi bo qua

		char tenThuocTrongDSLower[sizeof(dsThuoc[i].tenThuoc)];
		char tenThuocMoiLower[sizeof(thongTinMoi.tenThuoc)];

		strncpy(tenThuocTrongDSLower, dsThuoc[i].tenThuoc, sizeof(tenThuocTrongDSLower) - 1);
		tenThuocTrongDSLower[sizeof(tenThuocTrongDSLower) - 1] = '\0';
		for (int k = 0; tenThuocTrongDSLower[k]; k++) tenThuocTrongDSLower[k] = tolower(tenThuocTrongDSLower[k]); // Chuyen ten thuoc trong danh sach thuoc sang chuoi in thuong

		strncpy(tenThuocMoiLower, thongTinMoi.tenThuoc, sizeof(tenThuocMoiLower) - 1);
		tenThuocMoiLower[sizeof(tenThuocMoiLower) - 1] = '\0';
		for (int k = 0; tenThuocMoiLower[k]; k++) tenThuocMoiLower[k] = tolower(tenThuocMoiLower[k]); // Chuyen ten thuoc moi sang chuoi in thuong

		if (strcmp(tenThuocMoiLower, tenThuocTrongDSLower) == 0) {
			printf("LOI: Ten thuoc moi '%s' da ton tai voi Ma Thuoc '%s'. Khong the sua.\n", thongTinMoi.tenThuoc, dsThuoc[i].maThuoc);
			return 0; // Neu ten thuoc moi trung voi ten thuoc da ton tai trong danh sach thuoc thi in thong bao loi va tra ve 0
		}
	}
	strncpy(dsThuoc[indexThuocCanSua].tenThuoc, thongTinMoi.tenThuoc, sizeof(dsThuoc[indexThuocCanSua].tenThuoc) - 1); // Sao chep ten thuoc moi vao thuoc can sua
	dsThuoc[indexThuocCanSua].tenThuoc[sizeof(dsThuoc[indexThuocCanSua].tenThuoc) - 1] = '\0';

	ghiDanhSachThuocRaFile(tenFile, dsThuoc, soLuong); // Ghi danh sach thuoc ra file
	return 1;
}
// Ham xoa thuoc
int xoaThuoc(Thuoc** dsThuocConTro, int* soLuongConTro,
	BangBam* bbThuoc,
	const char* maThuocCanXoa, const char* tenFile) {
	char maXoaUpper[MAX_LEN_MA_THUOC]; // Mang luu ma thuoc can xoa chuyen sang chuoi in hoa
	strncpy(maXoaUpper, maThuocCanXoa, sizeof(maXoaUpper) - 1); // Sao chep ma thuoc can xoa vao mang
	maXoaUpper[sizeof(maXoaUpper) - 1] = '\0';
	for (int i = 0; maXoaUpper[i]; i++) maXoaUpper[i] = toupper(maXoaUpper[i]); // Chuyen ma thuoc can xoa sang chuoi in hoa

	int removedFromHashTable = 0; // Bien de kiem tra xem thuoc da duoc xoa khoi bang bam hay chua
	if (bbThuoc != NULL) { // Neu bang bam thuoc khong NULL
		if (timKiemTrongBangBam(bbThuoc, maXoaUpper) != NULL) { // Neu ma thuoc can xoa ton tai trong bang bam
			if (xoaKhoiBangBam(bbThuoc, maXoaUpper)) { // Neu xoa khoi bang bam thanh cong
				removedFromHashTable = 1; // Dat bien removedFromHashTable ve 1 de thong bao da xoa khoi bang bam
			}
			else { // Neu xoa khoi bang bam that bai thi in thong bao loi
				fprintf(stderr, "LOI: Khong the xoa Thuoc '%s' khoi bang bam mac du da tim thay.\n", maXoaUpper);
				return 0;
			}
		}
	}
	else { // Neu bang bam thuoc NULL thi in thong bao loi
		fprintf(stderr, "LOI HE THONG: Bang bam thuoc khong kha dung khi xoa.\n");
		return 0;
	}

	int viTriCanXoa = -1; // Bien de luu vi tri thuoc can xoa trong danh sach thuoc
	// Duyet qua danh sach thuoc de tim vi tri thuoc can xoa
	for (int i = 0; i < *soLuongConTro; ++i) {
		if (strcmp((*dsThuocConTro)[i].maThuoc, maXoaUpper) == 0) {
			viTriCanXoa = i; // Neu tim thay thuoc can xoa thi luu vi tri
			break;
		}
	}

	if (viTriCanXoa != -1) {
		// Neu tim thay thuoc can xoa trong danh sach thuoc nhung khong tim thay trong bang bam thi in thong bao canh bao
		if (!removedFromHashTable && timKiemTrongBangBam(bbThuoc, maXoaUpper) == NULL) {
			fprintf(stderr, "CANH BAO: Thuoc '%s' tim thay trong mang nhung khong co trong bang bam truoc khi xoa khoi mang.\n", maXoaUpper);

		}
		// Duyet qua danh sach thuoc de xoa thuoc can xoa roi dich tat ca cac thuoc con lai ve ben trai
		for (int i = viTriCanXoa; i < (*soLuongConTro - 1); ++i) {
			(*dsThuocConTro)[i] = (*dsThuocConTro)[i + 1];
		}
		(*soLuongConTro)--; // Giam so luong thuoc trong mang

		if (*soLuongConTro == 0) {
			free(*dsThuocConTro);
			*dsThuocConTro = NULL;
		} // Neu so luong thuoc con lai trong mang bang 0 thi giai phong bo nho va dat con tro ve NULL
		else {
			Thuoc* temp = realloc(*dsThuocConTro, (*soLuongConTro) * sizeof(Thuoc)); // Cap phat lai bo nho cho danh sach thuoc
			if (temp == NULL && *soLuongConTro > 0) {
				perror("Loi realloc khi xoa thuoc (thu nho mang)");
			}
			else if (*soLuongConTro > 0) {
				*dsThuocConTro = temp;
			} // Neu so luong thuoc con lai > 0 thi cap nhat con tro toi danh sach thuoc moi
		}
		ghiDanhSachThuocRaFile(tenFile, *dsThuocConTro, *soLuongConTro); // Ghi danh sach thuoc con lai ra file
		return 1;
	}

	if (removedFromHashTable && viTriCanXoa == -1) {
		fprintf(stderr, "Da xoa Thuoc '%s' khoi bang bam nhung khong tim thay trong mang.\n", maXoaUpper);
		return 0;
	} // Neu da xoa khoi bang bam nhung khong tim thay trong mang thi in thong bao loi dong bo
	return 0;
}

//Ham in danh sach thuoc ra man hinh
void inDanhSachThuocConsole(const Thuoc dsThuoc[], int soLuong) {
	printf("\n--- DANH SACH THUOC TRONG HE THONG ---\n"); // In tieu de
	if (soLuong == 0) {
		printf("Khong co loai thuoc nao trong danh sach.\n"); // Neu so luong thuoc bang 0 thi in thong bao khong co loai thuoc nao
		return;
	}
	// In tieu de bang
	printf("---------------------------------------------------------------------------\n");
	printf("| %-5s | %-20s | %-40s |\n", "STT", "Ma Thuoc", "Ten Thuoc");
	printf("---------------------------------------------------------------------------\n");
	// Duyet qua danh sach thuoc va in thong tin tung thuoc
	for (int i = 0; i < soLuong; ++i) {
		printf("| %-5d | %-20s | %-40s |\n",
			i + 1,
			dsThuoc[i].maThuoc,
			dsThuoc[i].tenThuoc);
	}
	printf("---------------------------------------------------------------------------\n");
}
// Ham tim kiem thuoc theo ten
void timKiemThuocTheoTenTuyenTinh(const Thuoc dsThuoc[], int soLuongThuoc,
	const char* tenThuocTimKiem,
	Thuoc** ketQuaTimKiem, int* soLuongKetQua) {
	*ketQuaTimKiem = NULL; // Khoi tao con tro ket qua tim kiem la NULL
	*soLuongKetQua = 0; // Khoi tao so luong ket qua tim kiem la 0
	if (soLuongThuoc == 0 || tenThuocTimKiem == NULL || strlen(tenThuocTimKiem) == 0) {
		return;
	} // Neu so luong thuoc bang 0 hoac ten thuoc tim kiem NULL hoac rong thi return ngay

	Thuoc* dsKetQuaTamThoi = (Thuoc*)malloc(soLuongThuoc * sizeof(Thuoc));
	if (dsKetQuaTamThoi == NULL) {
		perror("Loi cap phat bo nho ");
		return; // Neu cap phat bo nho that bai thi in thong bao loi va return ngay
	}
	int count = 0;

	char tenTimKiemLower[sizeof(((Thuoc*)0)->tenThuoc)];
	med_toLowerString(tenTimKiemLower, tenThuocTimKiem, sizeof(tenTimKiemLower)); // Chuyen chuoi ten thuoc tim kiem sang chuoi thuong

	for (int i = 0; i < soLuongThuoc; ++i) {
		char tenThuocTrongDSLower[sizeof(dsThuoc[i].tenThuoc)];
		med_toLowerString(tenThuocTrongDSLower, dsThuoc[i].tenThuoc, sizeof(tenThuocTrongDSLower)); // Chuyen chuoi ten thuoc trong danh sach thuoc sang chuoi thuong

		if (strstr(tenThuocTrongDSLower, tenTimKiemLower) != NULL) {
			if (count < soLuongThuoc) {
				dsKetQuaTamThoi[count++] = dsThuoc[i]; // Neu tim thay thuoc co ten trung voi ten thuoc can tim kiem, luu vao danh sach ket qua tam thoi
			}
		}
	}

	if (count > 0) {
		*ketQuaTimKiem = (Thuoc*)malloc(count * sizeof(Thuoc)); // Cap phat bo nho cho ket qua tim kiem
		if (*ketQuaTimKiem == NULL) {
			perror("Loi cap phat bo nho"); // Neu cap phat bo nho that bai thi in thong bao loi
			free(dsKetQuaTamThoi); // Giai phong bo nho da cap phat
			*soLuongKetQua = 0; // Dat so luong ket qua ve 0
			return;
		}
		memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(Thuoc)); // Sao chep ket qua tim kiem tam thoi vao ket qua tim kiem
		*soLuongKetQua = count; // Cap nhat so luong ket qua tim kiem
	}

	free(dsKetQuaTamThoi); // Giai phong bo nho da cap phat cho ket qua tim kiem tam thoi
}