// buz.c
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "buz.h"

#define BUZZER 27      // GPIO 25 (BCM 25 = 핀 22)
#define C2 131
#define D2 147
#define E2 165
#define F2 175
#define G2 196
#define A2 220
#define B2 247
#define C5 523
#define C4 523
#define D4 587
#define D5 587
#define E4 659
#define F4 698
#define G4 784
#define A4 880
#define B4 988


void buz_init() {
    pinMode(BUZZER, OUTPUT);
    softToneCreate(27);
}

// 🎵 Animal Crossing Theme (Full Ver)

int melody1[] = {
    G4, A4, B4, 0, G4, A4, B4, 0,
    G4, A4, B4, 0, G4, A4, B4, 0,
    A4, B4, G4, A4, B4, 0,
    E4, G4, A4, B4, A4, 0,
    G4, E4, D4, C4, D4, 0,
    E4, C4, B4, A4, B4, C4,
    A4, G4, E4, C4, A4, B4,
    C4, 0, C2, D2, E2, F2
};

int duration1[] = {
    75, 75, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75,
    75, 75, 75, 75, 75, 75,
    150, 225, 225, 1200
};

int note_count1 = sizeof(melody1) / sizeof(melody1[0]);


// 🎵 Für Elise (Full Ver)
int melody2[] = {
    659, 622, 659, 622, 659, 494, 587, 523, 440, 0,
    262, 330, 440, 494, 330, 415, 494, 523, 0,
    659, 622, 659, 622, 659, 494, 587, 523, 440
};

int duration2[] = {
    150, 150, 150, 150, 150, 150, 150, 150, 300, 150,
    150, 150, 150, 300, 150, 150, 150, 300, 150,
    150, 150, 150, 150, 150, 150, 150, 150, 300
};

int note_count2 = sizeof(melody2) / sizeof(melody2[0]);

int melody_pokemon[] = {
    E4, G4, A4, B4, 0, B4, A4, G4,
    E4, 0, E4, G4, A4, B4, 0, B4,
    A4, G4, E4, 0, A4, B4, D5, C5,
    B4, A4, G4, E4
};

int duration_pokemon[] = {
    300, 300, 300, 600, 150, 300, 300, 300,
    600, 150, 300, 300, 300, 600, 150, 300,
    300, 300, 600, 150, 300, 300, 300, 300,
    300, 300, 600, 600
};

int pokemon_len = sizeof(melody_pokemon) / sizeof(melody_pokemon[0]);


// 🎵 재생 함수 (공통)
void play_song(int* melody, int* duration, int count) {
    for (int i = 0; i < count; ++i) {
        softToneWrite(BUZZER, melody[i]);
        usleep(duration[i] * 1000);
    }
    softToneWrite(BUZZER, 0);
}

void buz_control(const char* cmd) {
    char action[16];
    sscanf(cmd, "BUZZER %s", action);

    if (strcasecmp(action, "ON1") == 0) {
        printf("BUZZER ON1: 동물의 숲 재생\n");
        play_song(melody1, duration1, note_count1);
    } else if (strcasecmp(action, "ON2") == 0) {
        printf("BUZZER ON2: 엘리제를 위하여 재생\n");
        play_song(melody2, duration2, note_count2);
    } else if (strcasecmp(action, "ON3") == 0) {
    printf("BUZZER ON3: 포켓몬 센터 BGM 재생\n");
    play_song(melody_pokemon, duration_pokemon, pokemon_len);
    }   
    else if (strcasecmp(action, "OFF") == 0) {
        softToneWrite(BUZZER, 0);
        printf("BUZZER OFF: 정지\n");
    } else {
        printf("잘못된 BUZZER 명령입니다. 예: BUZZER ON\n");
        printf("  BUZZER ON1  // 동물의 숲\n");
        printf("  BUZZER ON2  // 엘리제를 위하여\n");
        printf("  BUZZER OFF\n");
    }
}
