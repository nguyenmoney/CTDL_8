    #include "patient.h"
    #include "csv_handler.h"
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>   
    #include <ctype.h>  
    #include "hash_table.h"
    #define PATIENT_HO_TEN_COL_WIDTH 30
    #define PATIENT_DIA_CHI_COL_WIDTH 30
    #define PATIENT_TIEU_SU_COL_WIDTH 20
 
//Ham chuyen kieu du lieu gioi tinh sang string
//Chuyen doi enum sang string de hien thi
    const char* gioiTinhToString(GioiTinh gt) {
        switch (gt) {
            case GT_NAM: return "Nam";
            case GT_NU: return "Nu";
            case GT_KHAC: return "Khac";
            default: return "KhongXacDinh";
        }
    }
// Chuyen doi string sang enum de luu tru
    GioiTinh stringToGioiTinh(const char* str) {
        if (str == NULL || strlen(str) == 0) return GT_KHAC;    
        char lowerStr[100];
        strncpy(lowerStr, str, 99);
        lowerStr[99] = '\0';
        for(int i = 0; lowerStr[i]; i++){
            lowerStr[i] = tolower(lowerStr[i]);
        }
    
        if (strcmp(lowerStr, "nam") == 0) return GT_NAM;
        if (strcmp(lowerStr, "nu") == 0) return GT_NU;
        if (strcmp(lowerStr, "khac") == 0) return GT_KHAC;
        return GT_KHAC; 
    }
    
    
//Ham doc danh sach benh nhan
	BenhNhan* docDanhSachBenhNhan(const char* tenFile, int* soLuongBenhNhan) {
		FILE* f = fopen(tenFile, "r");// Mo file de doc
		if (f == NULL) {// Kiem tra an toan mo file
            *soLuongBenhNhan = 0;
            return NULL;
		}

		BenhNhan* ds = NULL;// Tao con tro den danh sach benh nhan
		int capacity = 0;//Suc chua cua danh sach
		*soLuongBenhNhan = 0;// So luong benh nhan hien tai
		char line[1500]; // Dung de luu tung dong doc duoc tu file

		while (fgets(line, sizeof(line), f) != NULL) {// Doc tung dong
			xoaKyTuXuongDong(line); // Loai bo ky tu xuong dong
			if (strlen(line) < 10) continue; // Neu do dai xau < 10, bo qua

			if (*soLuongBenhNhan >= capacity) {// Neu danh sach da day, cap phat bo nho moi
                capacity = (capacity == 0) ? 10 : capacity * 2; //Cap nhat suc chua *2
				BenhNhan* temp = realloc(ds, capacity * sizeof(BenhNhan));// Cap phat bo nho tam
				if (temp == NULL) {// Neu cap phat that bai, in thong bao loi
                    printf("Loi cap phat bo nho (realloc docDanhSachBenhNhan)");
                    free(ds); 
                    fclose(f);
					*soLuongBenhNhan = 0; // Reset so luong benh nhan ve 0
                    return NULL;
                }
				ds = temp;// Cap nhat con tro ds den bo nho moi
            }

			BenhNhan bn;// Khoi tao mot benh nhan moi
			char gioiTinhStr[20];// Bien dung de luu chuoi gioi tinh
			long ngayTaoTimestamp;// Bien dung de luu timestamp cua ngay tao ho so
			char* token;// Bien dung de tach chuoi
            
			token = strtok(line, ","); // Tach chuoi theo dau phay
			if (token) {// Neu co token dau tien, luu vao ma dinh danh
                strncpy(bn.maDinhDanh, token, sizeof(bn.maDinhDanh)-1);
                bn.maDinhDanh[sizeof(bn.maDinhDanh)-1] = '\0';
            } 
			else {// Neu khong co, in thong bao loi va bo qua dong
                fprintf(stderr, "Loi doc CSV Benh Nhan: Dong khong co Ma Dinh Danh.\n");
            continue; 
            }
            
			token = strtok(NULL, ",");// Doc ma bao hiem y te
            if (token) strncpy(bn.bhyt, token, sizeof(bn.bhyt) - 1); 
            else strcpy(bn.bhyt, ""); bn.bhyt[sizeof(bn.bhyt) - 1] = '\0';

			token = strtok(NULL, ","); // Doc so dien thoai
            if (token) strncpy(bn.soDienThoai, token, sizeof(bn.soDienThoai)-1);
            else strcpy(bn.soDienThoai, ""); bn.soDienThoai[sizeof(bn.soDienThoai)-1] = '\0';

			token = strtok(NULL, ","); // Doc ten benh nhan
            if (token) strncpy(bn.ten, token, sizeof(bn.ten)-1); 
            else strcpy(bn.ten, ""); bn.ten[sizeof(bn.ten)-1] = '\0';

			token = strtok(NULL, ","); // Doc ngay sinh
            if (token) bn.ngaySinh.ngay = atoi(token); 
            else bn.ngaySinh.ngay = 0;

			token = strtok(NULL, ","); // Doc thang sinh
            if (token) bn.ngaySinh.thang = atoi(token); 
            else bn.ngaySinh.thang = 0;

			token = strtok(NULL, ","); // Doc nam sinh
            if (token) bn.ngaySinh.nam =  atoi(token); 
            else bn.ngaySinh.nam = 0;

			token = strtok(NULL, ","); // Doc tuoi
            if (token) bn.tuoi = atoi(token);
            else bn.tuoi = 0;

			token = strtok(NULL, ","); // Doc gioi tinh
            if (token) strncpy(gioiTinhStr, token, sizeof(gioiTinhStr)-1); 
            else strcpy(gioiTinhStr, ""); gioiTinhStr[sizeof(gioiTinhStr)-1] = '\0';
            bn.gioiTinh = stringToGioiTinh(gioiTinhStr);

			token = strtok(NULL, ","); // Doc dia chi
            if (token) strncpy(bn.diaChi, token, sizeof(bn.diaChi)-1); 
            else strcpy(bn.diaChi, ""); bn.diaChi[sizeof(bn.diaChi)-1] = '\0';

			token = strtok(NULL, ","); // Doc tieu su benh ly
            if (token) strncpy(bn.tieuSuBenhLy, token, sizeof(bn.tieuSuBenhLy)-1); 
            else strcpy(bn.tieuSuBenhLy, ""); bn.tieuSuBenhLy[sizeof(bn.tieuSuBenhLy)-1] = '\0';

			token = strtok(NULL, "\n"); // Doc ngay tao ho so
            if (token) ngayTaoTimestamp = atol(token); 
            else ngayTaoTimestamp = 0;
            bn.ngayTaoHoSo = (time_t)ngayTaoTimestamp;

            ds[*soLuongBenhNhan] = bn;
            (*soLuongBenhNhan)++;
        }
        fclose(f);
        return ds;
    }
// Ham ghi danh sach benh nhan
    void ghiDanhSachBenhNhanRaFile(const char* tenFile, const BenhNhan dsBenhNhan[], int soLuongBenhNhan) {
		FILE* f = fopen(tenFile, "w"); // Mo file de ghi
		if (f == NULL) {// Kiem tra an toan mo file
            perror("Loi mo file de ghi (ghiDanhSachBenhNhanRaFile)");
            return;
        }
		for (int i = 0; i < soLuongBenhNhan; ++i) {// Duyet qua tung benh nhan
            fprintf(f, "%s,%s,%s,%s,%d,%d,%d,%d,%s,%s,%s,%ld\n", 
                    dsBenhNhan[i].maDinhDanh,
                    dsBenhNhan[i].bhyt,
                    dsBenhNhan[i].soDienThoai,
                    dsBenhNhan[i].ten,
                    dsBenhNhan[i].ngaySinh.ngay,
                    dsBenhNhan[i].ngaySinh.thang,
                    dsBenhNhan[i].ngaySinh.nam,
                    dsBenhNhan[i].tuoi,
                    gioiTinhToString(dsBenhNhan[i].gioiTinh),
                    dsBenhNhan[i].diaChi,
                    dsBenhNhan[i].tieuSuBenhLy,
                    (long)dsBenhNhan[i].ngayTaoHoSo); // Ghi tung thong tin benh nhan vao file
		}
        fclose(f);
    }
    

  // Ham tim benh nhan theo ma dinh danh (chuoi) - co the la static neu chi dung noi bo
const BenhNhan* timBenhNhanTheoMaChuoi(const BenhNhan dsBenhNhan[], int soLuong, const char* maDD_str) {
	if (maDD_str == NULL) return NULL;// Kiem tra con tro ma dinh danh
	for (int i = 0; i < soLuong; ++i) {// Duyet qua danh sach benh nhan
        if (strcmp(dsBenhNhan[i].maDinhDanh, maDD_str) == 0) {// Neu tim thay benh nhan co ma dinh danh khop, tra ve con tro den benh nhan do
            return &dsBenhNhan[i];
		}
    }
	return NULL;// Neu khong, tra ve NULL
}
BenhNhan* timBenhNhanTheoMaBangBam(BangBam* bb, const char* maDD_str) {
	if (bb == NULL || maDD_str == NULL) {// Kiem tra con tro bang bam va ma dinh danh
        return NULL;
    }
	return (BenhNhan*)timKiemTrongBangBam(bb, maDD_str);// Tim kiem trong bang bam
}
int nhapThongTinBenhNhanTuBanPhim(BenhNhan* bn, const BenhNhan dsBenhNhanHienCo[], int soLuongHienCo, BangBam* bbBenhNhan) {
	char buffer[1000];// Bien dung de luu tru cac thong tin nhap vao
	char maDD_tam_str[MAX_LEN_MA_DINH_DANH_BN];// Bien tam de luu ma dinh danh

    printf("Nhap Ma Dinh Danh Benh Nhan (toi da %d ky tu, VD: BN001): ", MAX_LEN_MA_DINH_DANH_BN - 1);
	if (docDongAnToan(maDD_tam_str, sizeof(maDD_tam_str), stdin) == NULL || strlen(maDD_tam_str) == 0) {// Kiem tra nhap ma dinh danh

        printf("LOI: Ma Dinh Danh khong duoc de trong. Huy them benh nhan.\n");
        return 0;
    }

	for (int k = 0; maDD_tam_str[k]; k++) maDD_tam_str[k] = toupper(maDD_tam_str[k]);// Chuyen ma dinh danh ve dang in hoa

	if (bbBenhNhan != NULL) {// Kiem tra bang bam benh nhan
        if (timKiemTrongBangBam(bbBenhNhan, maDD_tam_str) != NULL) {}// Neu ma dinh danh da ton tai trong bang bam, in thong bao loi va huy them benh nhan
            printf("LOI: Ma Dinh Danh '%s' da ton tai (kiem tra bang bam). Huy them benh nhan.\n", maDD_tam_str);
            return 0;
		
    }
	else {// Neu bang bam khong ton tai, in thong bao loi va huy them benh nhan
        printf("Bang bam benh nhan khong kha dung. Khong the kiem tra trung lap Ma Dinh Danh. Huy them benh nhan.\n");
        return 0;
    }

    strncpy(bn->maDinhDanh, maDD_tam_str, sizeof(bn->maDinhDanh) - 1);// Sao chep ma dinh danh vao benh nhan 
	bn->maDinhDanh[sizeof(bn->maDinhDanh) - 1] = '\0';// Bao dam chuoi ket thuc bang '\0'

    printf("Nhap BHYT (toi da %d ky tu, bo trong neu khong co): ", (int)(sizeof(bn->bhyt) - 1));
    docDongAnToan(bn->bhyt, sizeof(bn->bhyt), stdin);

	if (strlen(bn->bhyt) > 0) {//Kiem tra trung lap ma BHYT
		for (int i = 0; i < soLuongHienCo; ++i) {// Duyet qua danh sach benh nhan hien co
			if (strlen(dsBenhNhanHienCo[i].bhyt) > 0 && strcmp(dsBenhNhanHienCo[i].bhyt, bn->bhyt) == 0) {
                printf("LOI: BHYT '%s' da ton tai. Huy them benh nhan.\n", bn->bhyt);
                return 0;
            }
        }
    }

	// Nhap so dien thoai
    printf("Nhap So Dien Thoai (toi da %d ky tu, bo trong neu khong co): ", (int)(sizeof(bn->soDienThoai) - 1));
    docDongAnToan(bn->soDienThoai, sizeof(bn->soDienThoai), stdin);
    if (strlen(bn->soDienThoai) > 0) {
		for (int i = 0; i < soLuongHienCo; ++i) {//Kiem tra trung lap so dien thoai
            if (strlen(dsBenhNhanHienCo[i].soDienThoai) > 0 && strcmp(dsBenhNhanHienCo[i].soDienThoai, bn->soDienThoai) == 0) {
                printf("LOI: So Dien Thoai '%s' da ton tai. Huy them benh nhan.\n", bn->soDienThoai);
                return 0;
            }
        }
    }

	// Nhap ten benh nhan
    printf("Nhap Ho va Ten (toi da %d ky tu, KHONG DUOC BO TRONG): ", (int)(sizeof(bn->ten) - 1));
    if (docDongAnToan(bn->ten, sizeof(bn->ten), stdin) == NULL || strlen(bn->ten) == 0) {
		printf("LOI: Ho va Ten khong duoc de trong. Huy them benh nhan.\n");// Kiem tra ten benh nhan
        return 0;
    }

	// Kiem tra ngay sinh
    printf("Nhap Ngay Sinh (ngay thang nam, vd: 15 01 1990, KHONG DUOC BO TRONG): ");
    if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || strlen(buffer) == 0) {
		printf("LOI: Ngay Sinh khong duoc de trong. Huy them benh nhan.\n");
        return 0;
    }
	// Kiem tra dinh dang ngay sinh
    if (sscanf(buffer, "%d %d %d", &bn->ngaySinh.ngay, &bn->ngaySinh.thang, &bn->ngaySinh.nam) != 3) {
        printf("LOI: Dinh dang Ngay Sinh khong hop le. Huy them benh nhan.\n");
        return 0;
    }
	// Kiem tra rang buoc ngay, thang, nam
    if (bn->ngaySinh.ngay < 1 || bn->ngaySinh.ngay > 31 || bn->ngaySinh.thang < 1 || bn->ngaySinh.thang > 12 || bn->ngaySinh.nam < 1900 || bn->ngaySinh.nam > 2100) {
        printf("LOI: Ngay Sinh khong hop le (ngay/thang/nam khong dung). Huy them benh nhan.\n");
        return 0;
    }

	// Kiem tra tuoi
    printf("Nhap Tuoi (KHONG DUOC BO TRONG): ");
    if (docDongAnToan(buffer, sizeof(buffer), stdin) == NULL || strlen(buffer) == 0) {
        printf("LOI: Tuoi khong duoc de trong. Huy them benh nhan.\n");
        return 0;
    }
	// Kiem tra rang buoc tuoi
    if (sscanf(buffer, "%d", &bn->tuoi) != 1 || bn->tuoi < 0) {
        printf("LOI: Tuoi khong hop le. Huy them benh nhan.\n");
        return 0;
    }

	// Nhap gioi tinh
    printf("Nhap Gioi Tinh (Nam, Nu, Khac): ");
    docDongAnToan(buffer, sizeof(buffer), stdin);
    bn->gioiTinh = stringToGioiTinh(buffer); 

	// Nhap dia chi
    printf("Nhap Dia Chi (toi da %d ky tu, bo trong neu khong co): ", (int)(sizeof(bn->diaChi) - 1));
    docDongAnToan(bn->diaChi, sizeof(bn->diaChi), stdin);
    if (strlen(bn->diaChi) == 0) {
        strcpy(bn->diaChi, "Khong co");
    }

	// Nhap tieu su benh ly
    printf("Nhap Tieu Su Benh Ly (neu co, toi da %d ky tu, bo trong neu khong co): ", (int)(sizeof(bn->tieuSuBenhLy) - 1));
    docDongAnToan(bn->tieuSuBenhLy, sizeof(bn->tieuSuBenhLy), stdin);
    if (strlen(bn->tieuSuBenhLy) == 0) {
        strcpy(bn->tieuSuBenhLy, "Khong co");
    }

	// Gan thoi gian tao ho so
    bn->ngayTaoHoSo = time(NULL);
    return 1;
}

//Ham them benh nhan
int themBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan,
    BangBam** bbBenhNhan_ptr,
    BenhNhan benhNhanMoi, const char* tenFile) {
    BangBam* bbHienTai = *bbBenhNhan_ptr;
 
	if (bbHienTai != NULL) {// Kiem tra bang bam benh nhan
        if (timKiemTrongBangBam(bbHienTai, benhNhanMoi.maDinhDanh) != NULL) {
            fprintf(stderr, "Ma Dinh Danh '%s' da ton tai trong bang bam ngay truoc khi them vao mang!\n", benhNhanMoi.maDinhDanh);
            return 0;
        }
    }
    else {
        fprintf(stderr, "Bang bam benh nhan khong kha dung. Khong the them benh nhan '%s' an toan.\n", benhNhanMoi.maDinhDanh);
        return 0;
    }

	BenhNhan* temp = realloc(*dsBenhNhanConTro, (*soLuongBenhNhan + 1) * sizeof(BenhNhan));// Cap phat bo nho cho mang benh nhan
	if (temp == NULL) {// Kiem tra cap phat bo nho
        perror("Loi cap phat bo nho (realloc themBenhNhan)");
        return 0;
    }
	*dsBenhNhanConTro = temp;// Cap nhat con tro den mang benh nhan moi

	(*dsBenhNhanConTro)[*soLuongBenhNhan] = benhNhanMoi;// Gan benh nhan moi vao cuoi mang
	BenhNhan* conTroToiBnTrongMang = &((*dsBenhNhanConTro)[*soLuongBenhNhan]);// Luu dia chi cua benh nhan moi trong mang

	if (!chenVaoBangBam(bbHienTai, benhNhanMoi.maDinhDanh, conTroToiBnTrongMang)) {// Chen benh nhan moi vao bang bam
        fprintf(stderr, "LOI NGHIEM TRONG: Khong the chen Benh Nhan '%s' vao bang bam.\n", benhNhanMoi.maDinhDanh);
        return 0;
    }

	(*soLuongBenhNhan)++;// Tang so luong benh nhan

	// Kiem tra bang bam co vuot nguong tai toi da hay khong
    if ((*bbBenhNhan_ptr)->soLuongPhanTu / (double)(*bbBenhNhan_ptr)->dungLuong > BANG_BAM_NGUONG_TAI_TOI_DA) {
        printf("Thong bao: He so tai cua bang bam benh nhan vuot nguong (%d/%d). Dang chuan bi rehash...\n",
            (*bbBenhNhan_ptr)->soLuongPhanTu, (*bbBenhNhan_ptr)->dungLuong);
		if (!rehashBangBam(bbBenhNhan_ptr)) {
            fprintf(stderr, "LOI: Rehash bang bam benh nhan that bai. Chuong trinh co the tiep tuc nhung hieu suat se giam.\n");
    
        }
    }

    ghiDanhSachBenhNhanRaFile(tenFile, *dsBenhNhanConTro, *soLuongBenhNhan);
    return 1;
}
int suaThongTinBenhNhan(BenhNhan dsBenhNhan[], int soLuongBenhNhan, char* maDinhDanhCanSua, BenhNhan thongTinMoi, const char* tenFile) {
	int timThayIndex = -1;// Bien de luu vi tri cua benh nhan can sua
	for (int i = 0; i < soLuongBenhNhan; ++i) {// Duyet qua danh sach benh nhan
		if (strcmp(dsBenhNhan[i].maDinhDanh, maDinhDanhCanSua) == 0) {// Neu tim thay ma dinh danh can sua,luu lai vi tri
			timThayIndex = i;
            break;
        }
    }

	if (timThayIndex != -1) {// Neu tim thay benh nhan can sua, thuc hien sua thong tin
		strcpy(thongTinMoi.maDinhDanh, dsBenhNhan[timThayIndex].maDinhDanh);// Sao chep ma dinh danh cu vao thong tin moi
		thongTinMoi.ngayTaoHoSo = dsBenhNhan[timThayIndex].ngayTaoHoSo;// Sao chep ngay tao ho so cu vao thong tin moi
		dsBenhNhan[timThayIndex] = thongTinMoi;// Cap nhat thong tin benh nhan

        ghiDanhSachBenhNhanRaFile(tenFile, dsBenhNhan, soLuongBenhNhan);
        return 1;
    }
    return 0;
}


//Ham xoa benh nhan
int xoaBenhNhan(BenhNhan** dsBenhNhanConTro, int* soLuongBenhNhan, BangBam* bbBenhNhan, char* maDinhDanhCanXoa, const char* tenFile) {
	int viTriCanXoa = -1;// Bien de luu vi tri cua benh nhan can xoa
	for (int i = 0; i < *soLuongBenhNhan; ++i) {// Duyet qua danh sach benh nhan
		if (strcmp((*dsBenhNhanConTro)[i].maDinhDanh, maDinhDanhCanXoa) == 0) {// Neu tim thay ma dinh danh can xoa, luu lai vi tri
            viTriCanXoa = i;
            break;
        }
    }

	if (viTriCanXoa != -1) {// Neu tim thay benh nhan can xoa, thuc hien xoa
        if (bbBenhNhan != NULL) {
            if (!xoaKhoiBangBam(bbBenhNhan, maDinhDanhCanXoa)) {
                fprintf(stderr, "CANH BAO: Khong tim thay Benh Nhan '%s' trong bang bam de xoa (nhung co trong mang).\n", maDinhDanhCanXoa);
            }
        }
        
        //Dich chuyen benh nhan trong mang 
        for (int i = viTriCanXoa; i < (*soLuongBenhNhan - 1); ++i) {
			(*dsBenhNhanConTro)[i] = (*dsBenhNhanConTro)[i + 1];// Dich chuyen benh nhan ke tiep len
        }
		(*soLuongBenhNhan)--;// Giam so luong benh nhan

		if (*soLuongBenhNhan == 0) {// Neu danh sach benh nhan rong, giai phong bo nho
            free(*dsBenhNhanConTro);
            *dsBenhNhanConTro = NULL;
        }
        else {
			BenhNhan* temp = realloc(*dsBenhNhanConTro, (*soLuongBenhNhan) * sizeof(BenhNhan));// Cap phat bo nho lai cho mang benh nhan
			if (temp == NULL && *soLuongBenhNhan > 0) {// Kiem tra cap phat bo nho
                perror("Loi realloc khi xoa benh nhan (thu nho mang)");
            }
			else if (*soLuongBenhNhan > 0) {// Neu danh sach con benh nhan, cap nhat con tro
                *dsBenhNhanConTro = temp;
            }
			else {// Neu danh sach rong, giai phong bo nho
                free(*dsBenhNhanConTro); 
            }
        }
        ghiDanhSachBenhNhanRaFile(tenFile, *dsBenhNhanConTro, *soLuongBenhNhan);
        return 1;
    }
    return 0;
}

// Ham in danh sach benh nhan
    void inDanhSachBenhNhanConsole(const BenhNhan dsBenhNhan[], int soLuongBenhNhan) {
        printf("\n--- DANH SACH BENH NHAN ---\n");
		if (soLuongBenhNhan == 0) {
            printf("Khong co benh nhan nao trong danh sach.\n");
            return;
        }

		// In tieu de bang
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
        printf("| %-5s | %-18s | %-15s | %-15s | %-*s | %-10s | %-5s | %-8s | %-*s | %-*s |\n",
            "STT", "Ma DD", "BHYT", "SDT",
            PATIENT_HO_TEN_COL_WIDTH, "Ho Ten",
            "Ngay Sinh", "Tuoi", "GioiTinh",
            PATIENT_DIA_CHI_COL_WIDTH, "Dia Chi",
            PATIENT_TIEU_SU_COL_WIDTH, "Tieu Su Benh Ly");
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		// In thong tin benh nhan
        for (int i = 0; i < soLuongBenhNhan; ++i) {
			char ngaySinhStr[11];// Dinh dang chuoi cho ngay sinh
            snprintf(ngaySinhStr, sizeof(ngaySinhStr), "%02d/%02d/%04d",
				dsBenhNhan[i].ngaySinh.ngay, dsBenhNhan[i].ngaySinh.thang, dsBenhNhan[i].ngaySinh.nam);// Dinh dang ngay sinh thanh chuoi

			// Rut gon chuoi hien thi
            char hoTenDisplay[PATIENT_HO_TEN_COL_WIDTH + 1];
            rutGonChuoiHienThi(hoTenDisplay, sizeof(hoTenDisplay), dsBenhNhan[i].ten, PATIENT_HO_TEN_COL_WIDTH);

            char diaChiDisplay[PATIENT_DIA_CHI_COL_WIDTH + 1];
            rutGonChuoiHienThi(diaChiDisplay, sizeof(diaChiDisplay), dsBenhNhan[i].diaChi, PATIENT_DIA_CHI_COL_WIDTH);

            char tieuSuDisplay[PATIENT_TIEU_SU_COL_WIDTH + 1];
            rutGonChuoiHienThi(tieuSuDisplay, sizeof(tieuSuDisplay), dsBenhNhan[i].tieuSuBenhLy, PATIENT_TIEU_SU_COL_WIDTH);

			// In thong tin benh nhan
            printf("| %-5d | %-18s | %-15s | %-15s | %-*s | %-10s | %-5d | %-8s | %-*s | %-*s |\n",
                i + 1,
                dsBenhNhan[i].maDinhDanh,
                dsBenhNhan[i].bhyt,
                dsBenhNhan[i].soDienThoai,
                PATIENT_HO_TEN_COL_WIDTH, hoTenDisplay, 
                ngaySinhStr,
                dsBenhNhan[i].tuoi,
                gioiTinhToString(dsBenhNhan[i].gioiTinh),
                PATIENT_DIA_CHI_COL_WIDTH, diaChiDisplay,  
                PATIENT_TIEU_SU_COL_WIDTH, tieuSuDisplay); 
        }
        printf("-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    }
    void timKiemBenhNhan(const BenhNhan dsBenhNhan[], int soLuongBenhNhan,
                         const char* tieuChi, const char* giaTriTimKiem,
                         BenhNhan** ketQuaTimKiem, int* soLuongKetQua) {
		*ketQuaTimKiem = NULL;// Khoi tao con tro ket qua tim kiem
		*soLuongKetQua = 0;// Khoi tao so luong ket qua tim kiem
		if (soLuongBenhNhan == 0 || giaTriTimKiem == NULL || tieuChi == NULL) {// Kiem tra dieu kien dau vao
            return;
        }

		BenhNhan* dsKetQuaTamThoi = malloc(soLuongBenhNhan * sizeof(BenhNhan)); // Cap phat bo nho tam thoi cho ket qua tim kiem
		if (dsKetQuaTamThoi == NULL) {// Kiem tra cap phat bo nho
            perror("Loi cap phat bo nho (timKiemBenhNhan)");
            return;
        }
		int count = 0;// Bien dem so luong ket qua tim thay

		// Chuyen gia tri tim kiem ve chu thuong de so sanh
		char giaTriTimKiemLower[100];
        strncpy(giaTriTimKiemLower, giaTriTimKiem, sizeof(giaTriTimKiemLower)-1);
        giaTriTimKiemLower[sizeof(giaTriTimKiemLower)-1] = '\0';
        for(int i = 0; giaTriTimKiemLower[i]; i++) giaTriTimKiemLower[i] = tolower(giaTriTimKiemLower[i]);

		// Chuyen tieu chi tim kiem ve chu thuong de so sanh
        char tieuChiLower[50];
        strncpy(tieuChiLower, tieuChi, sizeof(tieuChiLower)-1);
        tieuChiLower[sizeof(tieuChiLower)-1] = '\0';
        for(int i = 0; tieuChiLower[i]; i++) tieuChiLower[i] = tolower(tieuChiLower[i]);

		// Duyet qua danh sach benh nhan
        for (int i = 0; i < soLuongBenhNhan; ++i) {
			int timThayTrongVongLapNay = 0;// Bien kiem tra co tim thay ket qua trong vong lap nay hay khong
			if (strcmp(tieuChiLower, "madinhdanh") == 0) {// Kiem tra ma dinh danh
                if (strcmp(dsBenhNhan[i].maDinhDanh, giaTriTimKiem) == 0) {
                    timThayTrongVongLapNay = 1;
                }
            } 
			else if (strcmp(tieuChiLower, "bhyt") == 0) {// Kiem tra ma bao hiem y te
                if (strcmp(dsBenhNhan[i].bhyt, giaTriTimKiem) == 0) {
                    timThayTrongVongLapNay = 1;
                }
            }
			else if (strcmp(tieuChiLower, "sodienthoai") == 0) {// Kiem tra so dien thoai
                if (strcmp(dsBenhNhan[i].soDienThoai, giaTriTimKiem) == 0) {
                    timThayTrongVongLapNay = 1;
                }
            }
			else if (strcmp(tieuChiLower, "ten") == 0) {// Kiem tra ten benh nhan
                char tenBenhNhanLower[100];
                strncpy(tenBenhNhanLower, dsBenhNhan[i].ten, sizeof(tenBenhNhanLower)-1);
                tenBenhNhanLower[sizeof(tenBenhNhanLower)-1] = '\0';
                for(int k=0; tenBenhNhanLower[k]; k++) tenBenhNhanLower[k] = tolower(tenBenhNhanLower[k]);

                if (strstr(tenBenhNhanLower, giaTriTimKiemLower) != NULL) {
                    timThayTrongVongLapNay = 1;
                }
            }

            if (timThayTrongVongLapNay) {
                dsKetQuaTamThoi[count++] = dsBenhNhan[i];
            }
        }

		if (count > 0) {// Neu co ket qua tim kiem, cap phat bo nho cho ket qua
			*ketQuaTimKiem = malloc(count * sizeof(BenhNhan));// Cap phat bo nho cho ket qua tim kiem
			if (*ketQuaTimKiem == NULL) {
                perror("Loi cap phat bo nho cho ket qua tim kiem");
                free(dsKetQuaTamThoi);
                return;
            }
			memcpy(*ketQuaTimKiem, dsKetQuaTamThoi, count * sizeof(BenhNhan));// Sao chep ket qua tim kiem vao con tro ket qua
			*soLuongKetQua = count;// Cap nhat so luong ket qua tim kiem
        }
        free(dsKetQuaTamThoi);
    }
    