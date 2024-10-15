#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Hàm để nối các đường dẫn tệp với tên tệp
void fmtname(char *path) {
    // Tìm phần cuối của đường dẫn (tên tệp hoặc thư mục)
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
}

// Hàm để tìm tệp với tên cụ thể trong thư mục
void find(char *path, char *filename) {
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // Mở thư mục
    if ((fd = open(path, 0)) < 0) {
        printf("find: cannot open %s\n", path);
        return;
    }

    // Lấy thông tin về thư mục
    if (fstat(fd, &st) < 0) {
        printf("find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // Nếu đối tượng không phải là thư mục thì thoát
    if (st.type != T_DIR) {
        close(fd);
        return;
    }

    // Đảm bảo đường dẫn không vượt quá kích thước của bộ đệm
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
        printf("find: path too long\n");
        return;
    }

    strcpy(buf, path);  // Sao chép đường dẫn hiện tại vào bộ đệm
    p = buf + strlen(buf);  // Trỏ đến cuối của đường dẫn
    *p++ = '/';  // Thêm dấu gạch chéo để chuẩn bị nối tên tệp

    // Đọc các mục trong thư mục
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0)  // Bỏ qua các mục không hợp lệ
            continue;

        // Bỏ qua "." và ".." để tránh đệ quy vô hạn
        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
            continue;

        // Nối tên tệp vào đường dẫn
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;  // Kết thúc chuỗi

        // Lấy thông tin về mục hiện tại
        if (stat(buf, &st) < 0) {
            printf("find: cannot stat %s\n", buf);
            continue;
        }

        // Nếu tên tệp khớp với tên cần tìm thì in ra đường dẫn
        if (strcmp(de.name, filename) == 0) {
            printf("%s\n", buf);
        }

        // Nếu mục hiện tại là thư mục, đệ quy để tìm trong thư mục đó
        if (st.type == T_DIR) {
            find(buf, filename);
        }
    }

    close(fd);  // Đóng thư mục
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: find <path> <filename>\n");
        exit(1);
    }

    find(argv[1], argv[2]);  // Gọi hàm find để tìm kiếm
    exit(0);
}
