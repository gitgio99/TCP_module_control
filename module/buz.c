// buz.c
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buz.h"

#define BUZZER 27      // GPIO 25 (BCM 25 = 핀 22)

void buz_init() {
    wiringPiSetupGpio();  // BCM 모드
    pinMode(BUZZER, OUTPUT);
    softToneCreate(27);
}

int notes[] = {
    262, 294, 330, 349, 392, 440, 494, 523, 0  // 도~시 + 휴식
};

#define NOTE_COUNT (sizeof(notes) / sizeof(notes[0]))


void buz_play() {
    for (int i = 0; i < NOTE_COUNT; ++i) {
        softToneWrite(BUZZER, notes[i]);
        usleep(300000);  // 0.3초
    }
    softToneWrite(BUZZER, 0);  // 정지
}

void buz_control(const char* cmd) {
    char action[16];
    sscanf(cmd, "BUZZER %s", action);

    if (strcasecmp(action, "ON") == 0) {
        printf("BUZZER ON: 음악 재생 시작\n");
        buz_play();
    } else if (strcasecmp(action, "OFF") == 0) {
        softToneWrite(BUZZER, 0);
        printf("BUZZER OFF: 정지\n");
    } else {
        printf("잘못된 BUZZER 명령입니다. 예: BUZZER ON\n");
    }
}
