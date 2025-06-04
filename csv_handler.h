#ifndef CSV_HANDLER_H
#define CSV_HANDLER_H

#include <stdio.h> 

// --- Khai bao cac ham tien ich chung ---

FILE* moFileAnToan(const char* tenFile, const char* cheDo);
/*
 * Ham mo mot file voi che do chi dinh, kem theo kiem tra loi co ban
 * Muc dich: Mo file an toan voi xu ly loi, in thong bao khi mo that bai
 * Tham so dau vao:
 *   - tenFile: duong dan toi file can mo
 *   - cheDo: chuoi che do mo file (vi du: "r", "w", "a", "rb", "wb", "ab")
 * Gia tri tra ve: Con tro FILE neu mo thanh cong, NULL neu that bai (da in thong bao loi)
 */

void dongFileAnToan(FILE* f);
/*
 * Ham dong mot file da mo, kem theo kiem tra con tro FILE
 * Muc dich: Dong file an toan, tranh dong file NULL hoac da dong roi
 * Tham so dau vao:
 *   - f: con tro FILE can dong
 * Gia tri tra ve: Khong co (void)
 */

void xoaKyTuXuongDong(char* str);
/*
 * Ham loai bo ky tu xuong dong ('\n') o cuoi chuoi (neu co)
 * Muc dich: Xoa ky tu '\n' o cuoi chuoi, thay doi truc tiep chuoi dau vao
 * Tham so dau vao:
 *   - str: chuoi can xu ly (se bi thay doi)
 * Gia tri tra ve: Khong co (void)
 */

char* docDongAnToan(char* buffer, int kichThuocBuffer, FILE* stream);
/*
 * Ham doc mot dong tu stream dau vao mot cach an toan
 * Muc dich: Doc dong an toan bang fgets va tu dong xoa ky tu xuong dong
 * Tham so dau vao:
 *   - buffer: bo dem de luu tru chuoi doc duoc
 *   - kichThuocBuffer: kich thuoc cua bo dem
 *   - stream: stream dau vao (vi du: stdin, con tro FILE)
 * Gia tri tra ve: Con tro toi buffer neu doc thanh cong, NULL neu co loi hoac EOF
 * Luu y: Dam bao chuoi ket thuc bang null, neu loi thi buffer[0] se la '\0'
 */

int demSoDongTrongFile(const char* tenFile, int boQuaDongTieuDe);
/*
 * Ham dem so luong dong trong mot file CSV
 * Muc dich: Dem tong so dong du lieu trong file, co the bo qua dong tieu de
 * Tham so dau vao:
 *   - tenFile: ten file CSV can dem
 *   - boQuaDongTieuDe: 1 neu bo qua dong dau tien (tieu de), 0 neu khong
 * Gia tri tra ve: So luong dong dem duoc, hoac -1 neu co loi khi mo file
 */

void rutGonChuoiHienThi(char* outputBuffer, int bufferSize, const char* originalString, int displayWidth);
/*
 * Ham rut gon chuoi de hien thi trong bang voi do rong gioi han
 * Muc dich: Cat chuoi dai va them "..." de fit vao cot hien thi
 * Tham so dau vao:
 *   - outputBuffer: bo dem de luu chuoi ket qua
 *   - bufferSize: kich thuoc bo dem output
 *   - originalString: chuoi goc can rut gon
 *   - displayWidth: do rong toi da cho phep hien thi
 * Gia tri tra ve: Khong co (void)
 * Luu y: Cac ham xu ly CSV cu the cho tung struct (BenhNhan, LanKham,...)
 *        van nam trong cac module tuong ung vi chung phu thuoc vao cau truc du lieu cu the
 */

#endif // CSV_HANDLER_H
