// client.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5200
#define BUF_SIZE 256

void print_menu() {
    printf("\n\e[1;36m╔══════════════════════════════════════════════╗\e[0m\n");
    printf("  \e[1;33m원격 장치 제어 클라이언트 메뉴\e[0m\n");
    printf("\e[1;36m╠══════════════════════════════════════════════╣\e[0m\n");
    printf("  \e[1;32mLED\e[0m       : LED ON MAX | MID | MIN | OFF\n");
    printf("  \e[1;32mBUZZER\e[0m    : BUZZER ON | OFF\n");
    printf("  \e[1;32mCDS SENSOR\e[0m: CDS READ (밝기 측정)\n");
    printf("  \e[1;32mSEGMENT\e[0m   : SEG <0~9> (카운트다운 후 부저)\n");
    printf("\e[1;36m╠══════════════════════════════════════════════╣\e[0m\n");
    printf("  \e[1;35mhelp\e[0m      : 명령어 다시 보기\n");
    printf("  \e[1;35mexit\e[0m      : 프로그램 종료\n");
    printf("\e[1;36m╚══════════════════════════════════════════════╝\e[0m\n\n");
}

/// 유효한 명령어인지 확인하기
int is_valid_command(const char* cmd) {
    return strncasecmp(cmd, "LED", 3) == 0 ||
           strncasecmp(cmd, "BUZZER", 6) == 0 ||
           strncasecmp(cmd, "CDS", 3) == 0 ||
           strncasecmp(cmd, "SEG", 3) == 0;
}


int main(int argc, char* argv[]) {
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];

    if (argc != 2) {
        printf("사용법: %s <서버 IP>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &serv_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect"); exit(1);
    }

    print_menu();  // ✅ 안내 메시지 출력

    while (1) {
        printf("\e[1;34m명령어 입력 > \e[0m");
        fgets(buf, sizeof(buf), stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strcmp(buf, "exit") == 0) break;

        if (strcasecmp(buf, "help") == 0) {
            print_menu();
            continue;
        }

        if (!is_valid_command(buf)) {
            printf("\e[1;31m[오류] 유효하지 않은 명령어입니다. 'help'로 명령어 목록 확인\n\e[0m");
            continue;
        }
        write(sock, buf, strlen(buf));
    }

    close(sock);
    return 0;
}
