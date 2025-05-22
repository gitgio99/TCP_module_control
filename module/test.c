// test.c
#include <stdio.h>
#include <string.h>
#include "led.h"
#include "buz.h"
#include "cds.h"
#include "seg.h"


int main() {
    char input[64];

    led_init();  // 초기화
    buz_init();
    cds_init();
    seg_init();

    printf("제어 프로그램 시작\n");
    printf("명령어 예시:\n");
    printf("  LED ON MAX, LED OFF, LED ON MID, LED ON MIN\n");
    printf("  BUZZER ON, BUZZER OFF\n");
    printf("  CDS READ (조도 감지 후 LED 제어)\n");
    printf("  SEG 9 → 9부터 0까지 표시 후 부저 울림\n");
    printf("  exit\n");

    while (1) {
        printf("\n명령어 입력 > ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;

        input[strcspn(input, "\n")] = 0;  // 개행 제거

        if (strlen(input) == 0) continue; // ⬅️ 입력 없으면 무시

        if (strcasecmp(input, "exit") == 0) break;
        else if (strncasecmp(input, "LED", 3) == 0)
            led_control(input);
        else if (strncasecmp(input, "BUZZER", 6) == 0)
            buz_control(input);
        else if (strncasecmp(input, "CDS", 3) == 0)
            cds_control(input);
        else if (strncasecmp(input, "SEG", 3) == 0)
            seg_control(input);
        else
            printf("알 수 없는 명령입니다.\n");
    }

    printf("프로그램 종료\n");
    return 0;
}
