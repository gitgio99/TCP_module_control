# 📡 TCP 기반 원격 장치 제어 시스템

---

## 📌 프로젝트 개요

이 프로젝트는 **TCP 소컴 통신을 활용한 원격 장치 제어 시스템**입니다.
\*\*서버(Raspberry Pi 4)\*\*는 물리 장치를 제어하고, \*\*클라이언트(Ubuntu 터미널)\*\*은 명령어를 입력해 장치를 원격 제어합니다.

* ✅ 공용 라이브러리(`.so`) 기반 장치 모듈화
* ✅ `pthread`, `daemon`, `SIGINT`, `softPwm`, `softTone` 등 활용
* ✅ CLI UI에서 다양한 제어 및 감성 알림 제공 🎵🌑🌞

---

## 📂 프로젝트 디렉토리 구조

```
FINAL_PROJECT/
├── README.md
├── client/
│   ├── client.c            # 명령어 입력, 유효성 검사, 서버 응답 수습
│   └── makefile
│
├── module/
│   ├── buz.c / buz.h       # BUZZER (노래 재생 + 스레드)
│   ├── cds.c / cds.h       # 조도센서 + LED 연동
│   ├── led.c / led.h       # LED 밝기 조절
│   ├── seg.c / seg.h       # 7세그멘트 숫자 표시 + 타이머
│   ├── init_module.c / h   # 전체 장치 초기화
│   ├── test/ test.c        # 개별 장치 테스트용
│   └── makefile            # .so 빌드 자동화
│
├── server/
│   ├── server.c            # 멀티스레드, daemon, signal 처리 포함
│   ├── server.log          # 실행 로그 출력
│   └── makefile
```

---

## 🛠️ 시험 방법

### 🔧 사전 준비

* Raspberry Pi 4
* `wiringPi`, `softPwm`, `softTone` 설치
* GPIO 장치 연결 (GPIO 핏 정보 포함)

### 1. 빌드: 모듈 .so 생성

```bash
cd module
make
```

### 2. 빌드: 서버 실행 파일

```bash
cd ../server
make
```

### 3. 서버 실행 (daemon + log)

```bash
./server &
tail -f server.log
```

### 4. 클라이언트 실행

```bash
cd ../client
./client <서버_IP>
```

---

## 🔌 장치 제어 기능

| 장치        | 명령어 예시               | 기능 설명                 | GPIO (BCM)                   |
| --------- | -------------------- | --------------------- | ---------------------------- |
| LED       | `LED ON MAX`, `OFF`  | 밝기 조절 (MAX, MID, MIN) | GPIO 4                       |
| BUZZER    | `BUZZER ON1`, `OFF`  | 노래 재생 (동물의 쇼 등)       | GPIO 27                      |
| CDS       | `CDS READ`, `NOW`    | 밝기 감지 후 LED 제어        | CDS: GPIO 17, LED: GPIO 22   |
| 7-SEGMENT | `SEG 5`, `SEG LED 4` | 숫자 카우트다운, LED, BUZZER | GPIO 23, 18, 15, 14, LED: 21 |

---

## 📨 클라이언트 응답 예시

| 명령어          | 서버 응답 메시지        |
| ------------ | ---------------- |
| `LED ON MAX` | 🌞 최대 밝기입니다!     |
| `BUZZER ON1` | 🍃 동물의 숲 재생 중... |
| `CDS READ`   | 🌑 앞이 안 보여요!     |
| `SEG 3`      | 💤 잘 시간이에요\~  |
| `SEG LED 4`  | 💤 집가요\~         |

---

## 🧠 주요 기능 요약

* 🧵 **멀티스레드 서버**: `pthread`로 동시 클라이언트 처리
* 🧠 **SIGINT 핸들러**: `Ctrl+C` 신호만 감지, 안전 종료
* 📆 **공용 라이브러리 로딩**: `dlopen()` 토너리 간 미체 시스템
* 🎨 **CLI UI**: 이모지, 컬러 출력, 명령어 도움말 제공
* 🛠️ **Makefile**: 모듈 + 서버 + 클라이언트 자동 빌드

---

## ✅ 테스트 환경

* Raspberry Pi 4 Model B (Raspbian OS)
* Ubuntu 22.04 (Client)
* GCC 9.x
* 사용 라이브러리: wiringPi, softTone, softPwm

---

## 👨‍💼 개발자 정보

* 이름: **Jioh Jeon**
* 개발 기간: **2025년 5월 21일 ~ 5월 23일 프로젝트**
