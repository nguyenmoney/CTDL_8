#include "csv_handler.h" 
#include <stdio.h>
#include <string.h>     
#include <stdlib.h>    

// Ham mo file an toan
FILE* moFileAnToan(const char* tenFile, const char* cheDo) {
    FILE* f = fopen(tenFile, cheDo);
    if (f == NULL) {
        // In ra stderr de phan biet voi output thong thuong cua chuong trinh
        fprintf(stderr, "LOI: Khong the mo file '%s' voi che do '%s'.\n", tenFile, cheDo);
        perror("Nguyen nhan chi tiet (fopen)"); // In ra loi he thong cu the
    }
    return f;
}

// Ham dong file an toan
void dongFileAnToan(FILE* f) {
    if (f != NULL) {
        if (fclose(f) == EOF) {
            // fclose tra ve EOF neu co loi
            perror("Loi khi dong file");
        }
    }
}
// Ham xoa ky tu xuong dong
void xoaKyTuXuongDong(char* str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    // Lap tu cuoi chuoi de xoa cac ky tu \n hoac \r
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = '\0'; // Thay the ky tu do bang null terminator
        len--;               // Giam do dai de kiem tra ky tu tiep theo (neu co)
    }
}

// Ham doc mot dong an toan tu file
char* docDongAnToan(char* buffer, int kichThuocBuffer, FILE* stream) {
    if (buffer == NULL || kichThuocBuffer <= 0 || stream == NULL) {
        if (buffer != NULL && kichThuocBuffer > 0) {
            buffer[0] = '\0';
        }
        return NULL;
    }

    if (fgets(buffer, kichThuocBuffer, stream) != NULL) {
        xoaKyTuXuongDong(buffer);
        return buffer;
    }
    else {
        buffer[0] = '\0';
        if (ferror(stream)) {
            // perror("Loi khi doc dong voi fgets trong docDongAnToan"); 
            // Ham goi nen kiem tra ferror(stream) neu can phan biet EOF va loi
        }
        return NULL;
    }
}
//Ham dem so dong trong file, co the bo qua dong tieu de
int demSoDongTrongFile(const char* tenFile, int boQuaDongTieuDe) {
    FILE* f = moFileAnToan(tenFile, "r");
    if (f == NULL) {
        return -1; // Loi mo file da duoc bao trong moFileAnToan
    }

    int soDong = 0;
    char line_buffer[2048]; // Buffer du lon de doc mot dong

    if (boQuaDongTieuDe) {
        // Doc va bo qua dong dau tien
        if (docDongAnToan(line_buffer, sizeof(line_buffer), f) == NULL) {
            // Co the file rong, chi co dong tieu de, hoac loi doc
            if (ferror(f)) {
                fprintf(stderr, "Loi doc dong tieu de file '%s' khi dem so dong.\n", tenFile);
                dongFileAnToan(f);
                return -1; // Loi doc
            }
            // Neu khong loi ma la EOF, nghia la file chi co 0 hoac 1 dong (tieu de)
            dongFileAnToan(f);
            return 0;
        }
    }

    // Dem cac dong con lai co noi dung
    while (docDongAnToan(line_buffer, sizeof(line_buffer), f) != NULL) {
        // Chi dem cac dong co it nhat mot ky tu khong phai la khoang trang (tuy chon)
        // Hien tai, chi can doc duoc va khong rong la dem
        if (strlen(line_buffer) > 0) {
            soDong++;
        }
    }

    if (ferror(f)) { // Kiem tra loi doc file sau khi vong lap ket thuc
        fprintf(stderr, "Loi doc file '%s' trong qua trinh dem so dong.\n", tenFile);
        dongFileAnToan(f);
        return -1; // Tra ve loi
    }

    dongFileAnToan(f);
    return soDong;
}
// Ham rut gon chuoi hien thi, cat bot neu can thiet
void rutGonChuoiHienThi(char* outputBuffer, int bufferSize, const char* originalString, int displayWidth) {
    if (outputBuffer == NULL || bufferSize <= 0 || originalString == NULL || displayWidth <= 0) {
        if (outputBuffer != NULL && bufferSize > 0) {
            outputBuffer[0] = '\0';
        }
        return;
    }

    size_t originalLen = strlen(originalString);

    if (originalLen > (size_t)displayWidth) {
        if (displayWidth < 3) { // Khong du cho de them "..."
            int lenToCopy = (bufferSize - 1 < displayWidth) ? (bufferSize - 1) : displayWidth;
            if (lenToCopy < 0) lenToCopy = 0;

            strncpy(outputBuffer, originalString, lenToCopy);
            outputBuffer[lenToCopy] = '\0';
        }
        else {
            // Kich thuoc buffer can it nhat la displayWidth + 1
            if (bufferSize >= displayWidth + 1) {
                // Sao chep displayWidth - 3 ky tu dau
                // snprintf se tu dong them null terminator
                // Phai ep kieu (size_t) cho displayWidth - 3 + 1 de tranh warning voi snprintf
                snprintf(outputBuffer, (size_t)displayWidth - 3 + 1, "%s", originalString);
                strcat(outputBuffer, "..."); // Noi them "..."
            }
            else if (bufferSize > 0) { // Neu buffer khong du lon cho ca "...", cat bot toi da co the
                strncpy(outputBuffer, originalString, bufferSize - 1);
                outputBuffer[bufferSize - 1] = '\0';
            }
        }
    }
    else {
        // Chuoi goc ngan hon hoac bang do dai cho phep, sao chep binh thuong
        if (bufferSize > originalLen) { // Dam bao bufferSize du lon
            strcpy(outputBuffer, originalString);
        }
        else if (bufferSize > 0) { // Neu buffer khong du lon, cat bot
            strncpy(outputBuffer, originalString, bufferSize - 1);
            outputBuffer[bufferSize - 1] = '\0';
        }
    }
}
