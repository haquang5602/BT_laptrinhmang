#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_DATA_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listen_port>\n", argv[0]);
        exit(1);
    }

    // Lấy cổng chờ từ dòng lệnh
    int listen_port = atoi(argv[1]);

    // Tạo socket và kết nối đến cổng chờ
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(listen_port);
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Nhận tên file và nội dung từ sender
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char filename[MAX_DATA_SIZE];
    char data[MAX_DATA_SIZE];
    int filename_len = recvfrom(sockfd, filename, MAX_DATA_SIZE, 0, (struct sockaddr*)&sender_addr, &sender_addr_len);
    int data_len = recvfrom(sockfd, data, MAX_DATA_SIZE, 0, (struct sockaddr*)&sender_addr, &sender_addr_len);
    if (filename_len < 0 || data_len < 0) {
        perror("recvfrom");
        exit(1);
    }

    // Tạo file và ghi nội dung vào file
    FILE* file = fopen(filename, "wb");
    if (!file) {
        perror("fopen");
        exit(1);
    }
    if (fwrite(data, sizeof(char), data_len, file) != data_len) {
        perror("fwrite");
        exit(1);
    }
    fclose(file);

    // Đóng socket
    close(sockfd);

    return 0;
}