// led.c

#include <wiringPi.h>
#include <softPwm.h>
#include <string.h>
#include <stdio.h>

#include "led.h"
#define LED_GPIO 4  // BCM 23 -> Physical 16

void led_init() {
    wiringPiSetupGpio();  // BCM 모드
    pinMode(LED_GPIO, OUTPUT);
    softPwmCreate(LED_GPIO, 0, 255);  // 초기값 0, 범위 0~255
}

void led_control(const char* cmd) {
    char action[16], level[16];
    int matched = sscanf(cmd, "LED %s %s", action, level);

    if (strcasecmp(action, "ON") == 0 && matched == 2) {
        if (strcasecmp(level, "MAX") == 0) {
            softPwmWrite(LED_GPIO, 255);
            printf("LED ON MAX (255)\n");
        } else if (strcasecmp(level, "MID") == 0) {
            softPwmWrite(LED_GPIO, 128);
            printf("LED ON MID (128)\n");
        } else if (strcasecmp(level, "MIN") == 0) {
            softPwmWrite(LED_GPIO, 64);
            printf("LED ON MIN (64)\n");
        } else {
            printf("잘못된 밝기 옵션입니다. (MAX/MID/MIN)\n");
        }
    } else if (strcasecmp(action, "OFF") == 0) {
        softPwmWrite(LED_GPIO, 0);
        printf("LED OFF\n");
    } else {
        printf("잘못된 명령입니다. 사용 예: LED ON MAX / LED OFF\n");
    }
}
