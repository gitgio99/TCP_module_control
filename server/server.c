// server_threaded.c
#include "../module/buz.h"
#include "../module/init_module.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <dlfcn.h>
#include <ctype.h>
#include <signal.h>


#define PORT 5200
#define BUF_SIZE 256

typedef void (*control_func_t)(const char*);

int server_fd = -1; // 글로벌로 선언

void sigint_handler(int signo) {
    printf("[SIGINT] 서버 종료 시그널 수신!\n");
    if (server_fd != -1) close(server_fd);
    exit(0);
}

void* buzzer_thread(void* arg) {
    int* song = (int*)arg;
    int* melody = NULL;
    int* duration = NULL;
    int count = 0;

    if (*song == 1) {
        melody = melody1;
        duration = duration1;
        count = note_count1;
    } else if (*song == 2) {
        melody = melody2;
        duration = duration2;
        count = note_count2;
    }
    free(song);

    play_song(melody, duration, count);  // ✅ 중복 제거
    return NULL;
}



void* client_handler(void* arg) {
    int client_fd = *(int*)arg;
    free(arg);  // 메모리 해제

    char buf[BUF_SIZE];

    while (1) {
        memset(buf, 0, BUF_SIZE);
        int n = read(client_fd, buf, BUF_SIZE - 1);
        if (n <= 0) {
            printf("클라이언트 종료됨\n");
            break;
        }

        printf("명령 수신: %s\n", buf);

        // 장치 추출 (LED, BUZZER, etc)
        char device[32];
        sscanf(buf, "%s", device);

        if (strcasecmp(device, "BUZZER") == 0)
        strcpy(device, "buz");  // libbuz.so와 맞추기 위해

        // 소문자로 변환 (예: LED → led)
        for (int i = 0; device[i]; i++)
            device[i] = tolower(device[i]);

        char lib_name[64];
        snprintf(lib_name, sizeof(lib_name), "../module/lib%s.so", device);

        void* handle = dlopen(lib_name, RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "dlopen 실패: %s\n", dlerror());
            continue;
        }

        char sym_name[64];
        snprintf(sym_name, sizeof(sym_name), "%s_control", device); 

        if (strcasecmp(device, "cds") == 0) {
            typedef int (*cds_func_t)(const char*);
            cds_func_t cds_control = (cds_func_t)dlsym(handle, sym_name);

            if (!cds_control) {
                fprintf(stderr, "dlsym 실패: %s\n", dlerror());
                dlclose(handle);
                continue;
            }

            int result = cds_control(buf);  // ✅ 조도 상태

            if (result == 1) {
                const char* msg = "🌑 앞이 안 보여요!\n";
                write(client_fd, msg, strlen(msg));
            } else if (result == 2) {
                const char* msg = "🌞 너무 밝아요!\n";
                write(client_fd, msg, strlen(msg));
            } else if (result == 4) {
                const char* msg = "📷 조도 상태: 어두움 (LED 제어 없음)\n";
                write(client_fd, msg, strlen(msg));
            } else if (result == 5) {
                const char* msg = "📷 조도 상태: 밝음 (LED 제어 없음)\n";
                write(client_fd, msg, strlen(msg));
            }
        }

        // SEG일 경우만 반환값을 받는 int 함수로 처리
        else if (strcasecmp(device, "seg") == 0) {
            typedef int (*seg_func_t)(const char*);
            seg_func_t seg_control = (seg_func_t)dlsym(handle, sym_name);

            if (!seg_control) {
                fprintf(stderr, "dlsym 실패: %s\n", dlerror());
                dlclose(handle);
                continue;
            }

            int result = seg_control(buf);  // ✅ SEG 명령 실행 후 결과 반환

            if (result == 1) {
                const char* msg = "💤 잠에 들 시간이에요~\n";
                write(client_fd, msg, strlen(msg));  // ✅ 알림 전송
            } else if (result == 2) {
                const char* msg = "💤 집가요~\n";
                write(client_fd, msg, strlen(msg));  // ✅ 알림 전송
            }
        } 

        else if (strcasecmp(device, "buz") == 0) {
        if (strcasecmp(buf, "BUZZER ON1") == 0 || strcasecmp(buf, "BUZZER ON2") == 0) {
            buzzer_stop_flag = 0;

            int* song = malloc(sizeof(int));
            *song = (strcasecmp(buf, "BUZZER ON1") == 0) ? 1 : 2;

            pthread_t tid;
            pthread_create(&tid, NULL, buzzer_thread, song);
            pthread_detach(tid);

            const char* msg = (*song == 1) ?
                "🍃 동물의 숲 재생 중... 개발자 힐링 타임입니다 🌿\n" :
                "🌇 너의 이름은 - 황혼의 시간 🎵 감성 충전 완료!\n";
            write(client_fd, msg, strlen(msg));
            } else if (strcasecmp(buf, "BUZZER OFF") == 0) {
                buzzer_stop_flag = 1;           // ✅ stop_flag로 중단 유도
                softToneWrite(27, 0);           // ✅ 즉시 부저 음 멈춤
                const char* msg = "🎵 재생이 종료되었습니다!\n";
                write(client_fd, msg, strlen(msg));
            } else {
                const char* msg =
                    "[BUZZER 명령 오류] 사용 예:\n"
                    "  BUZZER ON1  // 동물의 숲\n"
                    "  BUZZER ON2  // 너의 이름은\n"
                    "  BUZZER OFF\n";
                write(client_fd, msg, strlen(msg));
            }
        }
        
        else {
            // SEG가 아닌 다른 장치는 기존처럼 void로 실행
            typedef void (*control_func_t)(const char*);
            control_func_t control = (control_func_t)dlsym(handle, sym_name);

            if (!control) {
                fprintf(stderr, "dlsym 실패: %s\n", dlerror());
                dlclose(handle);
                continue;
            }

            control(buf);  // ✅ 명령 실행 (LED, BUZZER 등)
        }

        dlclose(handle);
    }

    close(client_fd);
    return NULL;
}


int main() {
    int server_fd;

    signal(SIGINT, sigint_handler);  // ✅ SIGINT에 대한 핸들러 등록

    device_init();
    
    struct sockaddr_in serv_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); exit(1);
    }

    listen(server_fd, 5);


    printf("멀티스레드 서버 대기 중 (port %d)...\n", PORT);

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        int* client_fd = malloc(sizeof(int));  // 쓰레드마다 고유 소켓 전달
        *client_fd = accept(server_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if (*client_fd < 0) { perror("accept"); free(client_fd); continue; }

        printf("클라이언트 연결 수락됨\n");

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, client_fd);
        pthread_detach(tid);  // 자원 자동 회수
    }

    close(server_fd);
    return 0;
}
