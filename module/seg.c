#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // sleep()

#include "seg.h"

#define BUZZER 27
#define LED3 21

int seg_pins[4] = {23, 18, 15, 14};

int numbers[10][4] = {
    {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1},
    {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1},
    {1,0,0,0}, {1,0,0,1}
};

void play_buzzer_alert() {
    softToneWrite(BUZZER, 261); delay(200);
    softToneWrite(BUZZER, 329); delay(200);
    softToneWrite(BUZZER, 392); delay(200);
    softToneWrite(BUZZER, 0);
}

void play_buzzer_alt() {
    int tones[] = {392, 440, 392, 330, 262, 0};  // 짧은 종결음
    for (int i = 0; tones[i]; ++i) {
        softToneWrite(BUZZER, tones[i]);
        delay(150);
    }
    softToneWrite(BUZZER, 0);
}

void seg_init() {
    for (int i = 0; i < 4; ++i)
        pinMode(seg_pins[i], OUTPUT);

    pinMode(BUZZER, OUTPUT);
    softToneCreate(BUZZER);

    pinMode(LED3, OUTPUT);  // 21번 LED 추가
}

void display_number(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 4; ++i)
        digitalWrite(seg_pins[i], numbers[num][i]);
}

int seg_control(const char* cmd) {
    char keyword[16];
    int value;

    // SEG LED <value>
    if (sscanf(cmd, "SEG %s %d", keyword, &value) == 2 && strcasecmp(keyword, "LED") == 0) {
        if (value < 0 || value > 9) {
            printf("잘못된 숫자입니다. 예: SEG LED 0~9\n");
            return 3;
        }

        printf("SEG LED %d → 카운트다운 + LED ON\n", value);
        digitalWrite(LED3, HIGH);

        for (int i = value; i >= 0; --i) {
            display_number(i);
            printf("표시 중: %d\n", i);
            sleep(1);
        }

        digitalWrite(LED3, LOW);
        printf("카운트다운 종료 → 대체 부저 울림\n");
        play_buzzer_alt();
        return 1; // ✅ LED 연동 → 클라이언트 알림 필요
    }

    // SEG <value> 기본
    if (sscanf(cmd, "SEG %d", &value) == 1) {
        if (value < 0 || value > 9) {
            printf("잘못된 숫자입니다. 예: SEG 0~9\n");
            return 3;
        }

        printf("7세그먼트: %d부터 카운트다운 시작\n", value);
        for (int i = value; i >= 0; --i) {
            display_number(i);
            printf("표시 중: %d\n", i);
            sleep(1);
        }

        printf("카운트다운 종료 → 기본 부저 울림\n");
        play_buzzer_alert();
        return 1; // ✅ 일반 SEG → 알림 불필요
    }

    // 예외 처리
    printf("잘못된 SEG 명령입니다. 예: SEG 5 / SEG LED 5\n");
}
