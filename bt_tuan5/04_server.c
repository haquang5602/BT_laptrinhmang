#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_DATA_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <receiver_port>\n", argv[0]);
        exit(1);
    }

    int receiver_port = atoi(argv[1]);

    // Tạo socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(1);
    }

    // Khởi tạo địa chỉ receiver
    struct sockaddr_in receiver_addr;
    memset(&receiver_addr, 0, sizeof(receiver_addr));
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    receiver_addr.sin_port = htons(receiver_port);

    // Gán địa chỉ receiver vào socket
    if (bind(sockfd, (struct sockaddr*)&receiver_addr, sizeof(receiver_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    while (1) {
        // Nhận dữ liệu từ sender
        struct sockaddr_in sender_addr;
        memset(&sender_addr, 0, sizeof(sender_addr));
        socklen_t sender_addr_len = sizeof(sender_addr);
        char data[MAX_DATA_SIZE];
        int num_bytes = recvfrom(sockfd, data, MAX_DATA_SIZE, 0, (struct sockaddr*)&sender_addr, &sender_addr_len);
        if (num_bytes < 0) {
            perror("recvfrom");
            exit(1);
        }

        // In thông tin về nguồn gốc của dữ liệu
        printf("Received %d bytes from %s:%d\n", num_bytes, inet_ntoa(sender_addr.sin_addr), ntohs(sender_addr.sin_port));

        // Xử lý dữ liệu nhận được tại đây
        // ...

        // Tạm dừng chương trình trong 1 giây để giảm tốc độ nhận dữ liệu
        usleep(1000000);
    }

    // Đóng socket
    close(sockfd);

    return 0;
}