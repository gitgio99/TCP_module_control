#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // sleep()

#include "seg.h"

#define BUZZER 27

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

void seg_init() {
    wiringPiSetupGpio();
    for (int i = 0; i < 4; ++i)
        pinMode(seg_pins[i], OUTPUT);
    pinMode(BUZZER, OUTPUT);
    softToneCreate(BUZZER);
}

void display_number(int num) {
    if (num < 0 || num > 9) return;
    for (int i = 0; i < 4; ++i)
        digitalWrite(seg_pins[i], numbers[num][i]);
}

void seg_control(const char* cmd) {
    char action[16];
    int value;
    int matched = sscanf(cmd, "SEG %d", &value);

    if (matched != 1 || value < 0 || value > 9) {
        printf("잘못된 SEG 명령입니다. 예: SEG (0~9)\n");
        return;
    }

    printf("7세그먼트: %d부터 카운트다운 시작\n", value);

    for (int i = value; i >= 0; --i) {
        display_number(i);
        printf("표시 중: %d\n", i);
        sleep(1);
    }

    printf("카운트다운 종료 → 부저 울림\n");
    play_buzzer_alert();
}