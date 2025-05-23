#include <wiringPi.h>
#include <softPwm.h>
#include <stdio.h>
#include <string.h>
#include "cds.h"


#define CDS_PIN 17      // CDS 센서 입력 핀 (BCM 17)
#define LED_PIN 22      // 연동 LED 출력 핀 (BCM 22)

void cds_init() {
    //wiringPiSetupGpio();
    pinMode(CDS_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
}

int cds_control(const char* cmd) {
    char label[16], subcmd[16];

    if (sscanf(cmd, "%s %s", label, subcmd) == 2 && strcasecmp(label, "CDS") == 0) {
        if (strcasecmp(subcmd, "READ") == 0) {
            int light = digitalRead(CDS_PIN);
            printf("센서값(CDS_PIN %d) = %d → ", CDS_PIN, light);
            if (light == 0) {
                digitalWrite(LED_PIN, HIGH);
                printf("조도: 어두움 → LED ON\n");
                return 1;
            } else {
                digitalWrite(LED_PIN, LOW);
                printf("조도: 밝음 → LED OFF\n");
                return 2;
            }
        }
        else if (strcasecmp(subcmd, "NOW") == 0) {
            int light = digitalRead(CDS_PIN);
            printf("센서값(CDS_PIN %d) = %d → 현재 조도 상태만 확인\n", CDS_PIN, light);
            return (light == 0) ? 4 : 5;
        }
    }

    printf("잘못된 CDS 명령입니다. 예: CDS READ / CDS NOW\n");
    return 3;
}
