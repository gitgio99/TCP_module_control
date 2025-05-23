// buz.h
#ifndef BUZ_H
#define BUZ_H

void buz_init();
void play_song(int* melody, int* duration, int count);
int buz_control(const char* cmd);


extern volatile int buzzer_stop_flag;
// ⬇️ 여기에 extern 추가
extern int melody1[];
extern int duration1[];
extern int note_count1;

extern int melody2[];
extern int duration2[];
extern int note_count2;
// buz.h

#endif