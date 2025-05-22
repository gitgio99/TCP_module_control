#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <string.h>
#include "cds.h"


#define CDS_PIN 17      // CDS 센서 입력 핀 (BCM 17)
#define LED_PIN 22      // 연동 LED 출력 핀 (BCM 22)

void cds_init() {
    wiringPiSetupGpio();
    pinMode(CDS_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

void cds_control(const char* cmd) {
    if (strcasecmp(cmd, "CDS READ") != 0) {
        printf("잘못된 CDS 명령입니다. 예: CDS READ\n");
        return;
    }

    int light = digitalRead(CDS_PIN);  // 0: 어두움, 1: 밝음

    printf("센서값(CDS_PIN %d) = %d → ", CDS_PIN, light);

    if (light == 0) {
        digitalWrite(LED_PIN, HIGH);  // 어두움 → LED 켬
        printf("조도: 어두움 → LED ON\n");
    } else {
        digitalWrite(LED_PIN, LOW);   // 밝음 → LED 끔
        printf("조도: 밝음 → LED OFF\n");
    }
}