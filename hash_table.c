#include "hash_table.h" 
#include <stdio.h>     
#include <stdlib.h>    
#include <string.h>   

// --- Trien khai cac ham cho bang bam ---
// Ham khoi bang bam
BangBam* taoBangBam(int dungLuongKLT) {
	if (dungLuongKLT <= 0) {
		dungLuongKLT = BANG_BAM_DUNG_LUONG_MAC_DINH;//Khoi tao bang bam voi dung luong mac dinh
	}

	BangBam* bb = (BangBam*)malloc(sizeof(BangBam));//Cap phat bo nho cho bang bam
	if (bb == NULL) {//Kiem tra loi cap phat
		perror("Loi cap phat bo nho cho BangBam");
		return NULL;
	}

	bb->dungLuong = dungLuongKLT;//Gan dung luong bang bam
	bb->soLuongPhanTu = 0;//Khoi tao so luong phan tu bang 0
	bb->cacBucket = (NutBam**)calloc(bb->dungLuong, sizeof(NutBam*));//Cap phat bo nho cho cac bucket cua bang bam
	if (bb->cacBucket == NULL) {
		perror("Loi cap phat bo nho cho cac bucket cua BangBam");//Kiem tra loi cap phat
		free(bb);
		return NULL;
	}
	return bb;
}
//Ham giai phong bo nho cua bang bam
void giaiPhongBangBam(BangBam* bb) {
	if (bb == NULL) { //Neu bang bam la NULL, khong lam gi ca
		return;
	}

	for (int i = 0; i < bb->dungLuong; ++i) {//Duyet qua tung bucket trong bang bam
		NutBam* nut = bb->cacBucket[i];// Chuyen den bucket i

		while (nut != NULL) {// Neu nut khong NULL, tiep tuc duyet
			NutBam* tam = nut; //Luu nut hien tai de giai phong sau
			nut = nut->tiepTheo; //Chuyen den nut tiep theo
			free(tam->khoa);// Giai phong bo nho cua khoa
			free(tam);
		}
	}
	free(bb->cacBucket);// Giai phong bo nho cua cac bucket
	free(bb);  // Giai phong bo nho cua bang bam
}
// Ham bam chuoi
unsigned int hamBamChuoi(const char* khoa, int dungLuongBang) {
	unsigned long giaTriBam = 5381; // Gia tri ban dau cua ham bam
	int c;
	while ((c = *khoa++)) {
		giaTriBam = ((giaTriBam << 5) + giaTriBam) + c; //Giai thua ham bam theo cong thuc D.J. Bernstein
	}
	return giaTriBam % dungLuongBang; //Tra ve chi so trong bang bam
}

//Ham tim kiem trong bang bam
void* timKiemTrongBangBam(BangBam* bb, const char* khoa) {
	if (bb == NULL || khoa == NULL) { //Kiem tra xem bang bam va khoa co hop le khong
		return NULL;
	}

	unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong); //Tinh chi so bang ham bam chuoi
	NutBam* nut = bb->cacBucket[chiSo]; //Chuyen den bucket tuong ung

	while (nut != NULL) { //Neu nut khong NULL, tiep tuc duyet
		if (strcmp(nut->khoa, khoa) == 0) { //So sanh khoa
			return nut->giaTri; //Neu tim thay, tra ve gia tri
		}
		nut = nut->tiepTheo;//Neu khong tim thay, chuyen den nut tiep theo
	}
	return NULL;
}
// Ham xoa khoi bang bam
int xoaKhoiBangBam(BangBam* bb, const char* khoa) {
	if (bb == NULL || khoa == NULL) {// Kiem tra xem bang bam va khoa co hop le khong
		return 0;
	}

	unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong); //Tinh chi so bang ham bam chuoi
	NutBam* nut = bb->cacBucket[chiSo];//Chuyen den bucket tuong ung
	NutBam* truoc = NULL;// Khai bao con tro truoc de giu nut truoc do

	while (nut != NULL) {// Neu nut khong NULL, tiep tuc duyet
		if (strcmp(nut->khoa, khoa) == 0) {//Neu tim thay khoa can xoa
			if (truoc == NULL) {// Neu nut hien tai la nut dau tien trong bucket
				bb->cacBucket[chiSo] = nut->tiepTheo;//Chuyen nut tiep theo len dau bucket
			}
			else {
				truoc->tiepTheo = nut->tiepTheo; //Chuyen nut tiep theo len sau nut truoc 
			}
			free(nut->khoa); //Giai phong bo nho cua khoa
			free(nut);// Giai phong bo nho cua nut
			bb->soLuongPhanTu--;// Giam so luong phan tu trong bang bam
			return 1;
		}
		truoc = nut;// Luu nut hien tai de lam nut truoc cho lan duyet tiep theo
		nut = nut->tiepTheo;   // Chuyen den nut tiep theo
	}
	return 0;
}
//Ham rehash cho bang bam
int rehashBangBam(BangBam** bb_ptr) {
	if (bb_ptr == NULL || *bb_ptr == NULL) { // Kiem tra xem con tro bang bam co hop le khong
		fprintf(stderr, "LOI REHASH: Con tro BangBam khong hop le.\n");
		return 0;
	}
	BangBam* bbCu = *bb_ptr; //Lay con tro den bang bam cu

	int kichThuocMoi = bbCu->dungLuong * BANG_BAM_HE_SO_TANG_KICH_THUOC;//Khai bao kich thuoc moi
	if (kichThuocMoi <= bbCu->dungLuong) {
		kichThuocMoi = bbCu->dungLuong + BANG_BAM_DUNG_LUONG_MAC_DINH;//Neu kich thuoc moi khong hop le, gan kich thuoc mac dinh + dung luong cu
	}

	BangBam* bbMoi = taoBangBam(kichThuocMoi); //Tao bang bam moi voi dung luong moi
	if (bbMoi == NULL) {
		fprintf(stderr, "LOI REHASH: Khong the tao bang bam moi voi kich thuoc %d.\n", kichThuocMoi);//Kiem tra loi tao bang bam moi
		return 0; //Neu khong tao duoc bang bam moi, bang bam cu van con ton tai
	}

	printf("Thong bao: Thuc hien rehash. Kich thuoc cu: %d, Kich thuoc moi: %d, So luong phan tu: %d\n",
		bbCu->dungLuong, bbMoi->dungLuong, bbCu->soLuongPhanTu); // In thong bao ve rehash thanh cong

	for (int i = 0; i < bbCu->dungLuong; ++i) {
		NutBam* nutHienTai = bbCu->cacBucket[i];// Duyet qua tung bucket trong bang bam cu
		while (nutHienTai != NULL) { // Neu nut hien tai khong NULL, tiep tuc duyet
			if (!chenVaoBangBam(bbMoi, nutHienTai->khoa, nutHienTai->giaTri)) { //Chen vao bang bam moi
				fprintf(stderr, "Khong the chen khoa '%s' vao bang bam moi.\n", nutHienTai->khoa);
				giaiPhongBangBam(bbMoi);//Neu chen that bai, giai phong bang bam moi
				return 0; // Rehash that bai, bang bam cu van con ton tai
			}
			nutHienTai = nutHienTai->tiepTheo;//Chuyen den nut tiep theo trong bucket
		}
	}

	giaiPhongBangBam(bbCu); // giai phong bang bam cu

	*bb_ptr = bbMoi; // Cap nhat con tro bang bam den bang bam moi
	printf("Rehash hoan tat. So luong phan tu trong bang moi: %d\n", (*bb_ptr)->soLuongPhanTu);
	return 1;
}
//Ham chen vao BangBam
int chenVaoBangBam(BangBam* bb, const char* khoa, void* giaTri) {
	if (bb == NULL || khoa == NULL) { //Kiem tra xem bang bam va khoa co hop le khong
		return 0;
	}
	unsigned int chiSo = hamBamChuoi(khoa, bb->dungLuong); //Tinh chi so bang ham bam chuoi
	NutBam* nutHienTai = bb->cacBucket[chiSo];//Chuyen den bucket tuong ung

	while (nutHienTai != NULL) { //Neu nut hien tai khong NULL, tiep tuc duyet
		if (strcmp(nutHienTai->khoa, khoa) == 0) { //Neu tim thay khoa da ton tai
			return 0; // Khoa da ton tai
		}
		nutHienTai = nutHienTai->tiepTheo; //Chuyen den nut tiep theo trong bucket
	}

	NutBam* nutMoi = (NutBam*)malloc(sizeof(NutBam));// Cap phat bo nho cho nut bam moi
	if (nutMoi == NULL) {
		perror("Loi cap phat bo nho cho NutBam");
		return 0; // Neu khong cap phat duoc, in loi va tra ve 0
	}

	nutMoi->khoa = _strdup(khoa); //Sao chep chuoi khoa 
	if (nutMoi->khoa == NULL) {
		perror("Loi sao chep chuoi khoa (chen)");
		free(nutMoi); //Neu khong sao chep duoc, giai phong bo nho nut bam moi
		return 0;
	}
	nutMoi->giaTri = giaTri; //Gan gia tri cho nut bam moi
	nutMoi->tiepTheo = bb->cacBucket[chiSo]; //Gan nut tiep theo cua nut bam moi la nut dau tien trong bucket
	bb->cacBucket[chiSo] = nutMoi;// Gan bang bam moi vao dau bucket
	bb->soLuongPhanTu++; // Tang so luong phan tu trong bang bam
	return 1; // Chen thanh cong
}