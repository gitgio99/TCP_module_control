// client.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5300
#define BUF_SIZE 256

void print_menu() {
    printf("\n\e[1;36m╔════════════════════════════════════════════════════════════════════╗\e[0m\n");
    printf("        \e[1;33m📡 원격 장치 제어 클라이언트 - 명령어 목록\e[0m\n");
    printf("\e[1;36m╠════════════════════════════════════════════════════════════════════╣\e[0m\n");

    printf("  \e[1;32m💡 LED\e[0m\n");
    printf("      • LED ON MAX   → 밝기 최대 🌞\n");
    printf("      • LED ON MID   → 밝기 중간 🌤️\n");
    printf("      • LED ON MIN   → 밝기 최소 🌙\n");
    printf("      • LED OFF      → 끄기 ❌\n");

    printf("\n  \e[1;32m🎵 BUZZER\e[0m\n");
    printf("      • BUZZER ON1   → 🎶 동물의 숲 테마곡 (개발자 힐링곡)\n");
    printf("      • BUZZER ON2   → 🎼 너의 이름은 - 황혼의 시간\n");
    printf("      • BUZZER OFF   → 🔇 부저 정지\n");

    printf("\n  \e[1;32m🌞 CDS SENSOR\e[0m\n");
    printf("      • CDS READ     → 현재 밝기 측정 + LED 연동 🔦\n");
    printf("      • CDS NOW      → 조도 상태만 빠르게 확인 📷\n");

    printf("\n  \e[1;32m🔢 SEGMENT DISPLAY\e[0m\n");
    printf("      • SEG <0~9>        → 숫자 표시 + 기본 카운트다운 ⏱️\n");
    printf("      • SEG LED <0~9>    → LED 타이머 연동 + 부저 종료 알림 💡⏰\n");

    printf("\e[1;36m╠════════════════════════════════════════════════════════════════════╣\e[0m\n");
    printf("  \e[1;35m📖 help\e[0m        → 명령어 다시 보기\n");
    printf("  \e[1;35m🚪 exit\e[0m        → 프로그램 종료\n");
    printf("\e[1;36m╚════════════════════════════════════════════════════════════════════╝\e[0m\n\n");
}


/// 유효한 명령어인지 확인하기
int is_valid_command(const char* cmd) {
    return strncasecmp(cmd, "LED", 3) == 0 ||
           strncasecmp(cmd, "BUZZER", 6) == 0 ||
           strncasecmp(cmd, "CDS", 3) == 0 ||
           strncasecmp(cmd, "SEG", 3) == 0;
}

int validate_led_command(const char* cmd) {
    char action[16], level[16];
    int matched = sscanf(cmd, "LED %s %s", action, level);

    if (strcasecmp(action, "ON") == 0 && matched == 2) {
        return strcasecmp(level, "MAX") == 0 ||
               strcasecmp(level, "MID") == 0 ||
               strcasecmp(level, "MIN") == 0;
    } else if (strcasecmp(action, "OFF") == 0 && matched == 1) {
        return 1;
    }
    return 0;
}

int validate_buzzer_command(const char* cmd) {
    char action[16];
    int matched = sscanf(cmd, "BUZZER %s", action);

    if (matched != 1) return 0;

    return strcasecmp(action, "ON1") == 0 || strcasecmp(action, "ON2") == 0 || strcasecmp(action, "ON3") == 0 || strcasecmp(action, "OFF") == 0;
}

int validate_seg_command(const char* cmd) {
    char seg_label[16];
    char led_keyword[16];
    int number;

    // case 1: SEG LED <num>
    if (sscanf(cmd, "%s %s %d", seg_label, led_keyword, &number) == 3) {
        if (strcasecmp(seg_label, "SEG") == 0 && strcasecmp(led_keyword, "LED") == 0)
            return number >= 0 && number <= 9;
    }

    // case 2: SEG <num>
    if (sscanf(cmd, "%s %d", seg_label, &number) == 2) {
        if (strcasecmp(seg_label, "SEG") == 0)
            return number >= 0 && number <= 9;
    }

    return 0;
}

int validate_cds_command(const char* cmd) {
    return strcasecmp(cmd, "CDS READ") == 0 || strcasecmp(cmd, "CDS NOW") == 0;
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

        if (strncasecmp(buf, "LED", 3) == 0 && !validate_led_command(buf)) {
            printf("\e[1;31m[LED 명령 오류] 사용 예: LED ON MAX | LED OFF\e[0m\n");
            continue;
        }

        if (strncasecmp(buf, "BUZZER", 6) == 0 && !validate_buzzer_command(buf)) {
            printf("\e[1;31m[BUZZER 명령 오류] 사용 예: BUZZER ON1 | BUZZER ON2 | BUZZER ON2 | BUZZER OFF\e[0m\n");
            continue;
        }

        if (strncasecmp(buf, "CDS", 3) == 0 && !validate_cds_command(buf)) {
            printf("\e[1;31m[CDS 명령 오류] 사용 예: CDS READ / CDS NOW\e[0m\n");
            continue;
        }

        if (strncasecmp(buf, "SEG", 3) == 0 && !validate_seg_command(buf)) {
            printf("\e[1;31m[SEG 명령 오류] 사용 예:\n  SEG <0~9> (카운트다운)\n  SEG LED <0~9> (카운트다운 + LED)\e[0m\n");
            continue;
        }

        write(sock, buf, strlen(buf));

        // ✅ 여기! 서버가 보낼 메시지 수신
        char response[BUF_SIZE];
        int n = read(sock, response, sizeof(response) - 1);
        if (n > 0) {
            response[n] = '\0';
            printf("\e[1;35m[서버 알림]\e[0m %s", response);
        }
    }

    close(sock);
    return 0;
}
