// buz.c
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buz.h"

#define BUZZER 27      // GPIO 25 (BCM 25 = 핀 22)
// 저음 (낮은 옥타브)
#define E2 82
#define E3 165

// 중음 (C4 = 중간 도 기준)
#define C4 523
#define D4 587
#define E4 659
#define F4 698
#define G4 784
#define A4 880
#define B4 988

// 고음
#define C5 1047
#define D5 1175
#define E5 1319
#define F5 1397
#define G5 1568
#define A5 1760
#define B5 1976



void buz_init() {
    pinMode(BUZZER, OUTPUT);
    softToneCreate(27);
}

// 🎵 Animal Crossing Theme (Full Ver)

int melody1[] = {
    E3, C4, D4, D4, C5, F4, A4, B4, 0,
    E4, D4, C4, D4, E4, D4, C4, D4, E4, B4,
    C5, E5, B5, A5, G5, F5, E5, D5, F5, E5, A4, A4, 0,
    F4, E4, A4, C5, D5, A4, C4, G4, G4, A4, G4, C4, E4, A4
};

int duration1[] = {
    200, 200, 200, 400, 200, 200, 200, 200, 200,
    200, 200, 200, 400, 200, 400, 200, 200, 200, 200,
    400, 200, 200, 400, 200, 400, 200, 400, 200, 400, 200, 200, 200,
    400, 400, 200, 400, 400, 400, 400, 400, 100, 100, 100, 100, 200, 800
};

int note_count1 = sizeof(melody1) / sizeof(melody1[0]);

// 🎵 너의 이름은 (황혼의 시간)
int melody2[] = {
    G5, D5, E5, G5, F5, C5, 0,
    G5, G5, G5, G5, E5, D5, D5, C5, E5, 0,
    C5, G5, G5, C5, B5, A5, G5, G5, G5, A5,
    G5, E5, F5, E5, D5, C5, G5, G5, G5, G5, E5, D5, D5, C5, E5,
    0, G4, C5, D5, E5, D5, C5, C5, A4, C5, C5, C5, F4, C5, D5, E5, D5,
    G4, C5, G5, C5, D5, G4, C5, D5, E5, D5, C5, C5, A4, C5, C5, C5,
    F4, C5, D5, E5, D5, G4, G5, C5, D4, C4, D4, C4,
    G5, E5
};

int duration2[] = {
    800, 800, 1600, 800, 1200, 800, 200,
    400, 200, 100, 200, 200, 200, 100, 100, 800, 200,
    200, 400, 200, 100, 200, 100, 100, 200, 100, 100,
    400, 200, 200, 200, 200, 400, 200, 100, 200, 200,
    200, 100, 100, 100, 200, 100, 100, 100, 200, 100,
    200, 200, 100, 200, 200, 100, 100, 100, 200, 100,
    100, 100, 200, 100, 200, 200, 100, 100, 100, 150,
    200, 200
};

int note_count2 = sizeof(melody2) / sizeof(melody2[0]);


volatile int buzzer_stop_flag = 0;

// 🎵 재생 함수 (공통)
void play_song(int* melody, int* duration, int count) {
    for (int i = 0; i < count; ++i) {
        if (buzzer_stop_flag) break;
        softToneWrite(BUZZER, melody[i]);
        usleep(duration[i] * 1000);
    }
    softToneWrite(BUZZER, 0);
    buzzer_stop_flag = 0;  // 상태 초기화
}


// void play_song(int* melody, int* duration, int count) {
//     for (int i = 0; i < count; ++i) {
//         softToneWrite(BUZZER, melody[i]);
//         usleep(duration[i] * 1000);
//     }
//     softToneWrite(BUZZER, 0);
// }

int buz_control(const char* cmd) {
    char action[16];
    sscanf(cmd, "BUZZER %s", action);

    if (strcasecmp(action, "ON1") == 0) {
        printf("BUZZER ON1: 동물의 숲 재생\n");
        play_song(melody1, duration1, note_count1);
        return 1;
    } else if (strcasecmp(action, "ON2") == 0) {
        printf("BUZZER ON2: 너의이름은 재생\n");
        play_song(melody2, duration2, note_count2);
        return 2;
    } else if (strcasecmp(action, "OFF") == 0) {
        softToneWrite(BUZZER, 0);
        printf("BUZZER OFF: 정지\n");
        return 4;
    } else {
        printf("잘못된 BUZZER 명령입니다. 예: BUZZER ON\n");
        printf("  BUZZER ON1  // 동물의 숲\n");
        printf("  BUZZER ON2  // 너의 이름은\n");
        printf("  BUZZER OFF\n");
        return 3;
    }
}
