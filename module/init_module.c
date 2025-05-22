#include <wiringPi.h>
#include "led.h"
#include "buz.h"
#include "seg.h"
#include "cds.h"

void device_init() {
    wiringPiSetupGpio();  // ✅ 실제 초기화 실행
    led_init();           // ✅ 함수 호출
    buz_init();
    seg_init();
    cds_init();
}