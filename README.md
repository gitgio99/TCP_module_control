
# 📡 TCP 기반 원격 장치 제어 시스템

---

## 📌 프로젝트 개요

이 프로젝트는 **TCP 소켓 통신을 활용한 원격 장치 제어 시스템**으로,  
**서버(Raspberry Pi 4)**는 물리 장치를 제어하고, **클라이언트(Ubuntu 터미널)**는 명령어를 입력하여 장치를 원격 제어합니다.

- ✅ 공유 라이브러리(`.so`) 기반 장치 모듈화
- ✅ `pthread`, `daemon`, `SIGINT`, `softPwm`, `softTone` 등 활용
- ✅ CLI UI에서 다양한 제어 및 감성 알림 제공 🎵🌑🌞

---

## 📂 프로젝트 디렉토리 구조

```bash
FINAL_PROJECT/
├── README.md
├── client/                    # 클라이언트 소스
│   ├── client.c               # 명령어 입력, 유효성 검사, 서버 응답 수신
│   └── makefile
│
├── module/                    # 장치 제어 모듈 및 공유 라이브러리
│   ├── buz.c / buz.h          # BUZZER (노래 재생 + 쓰레드)
│   ├── cds.c / cds.h          # 조도센서 + LED 연동
│   ├── led.c / led.h          # LED 밝기 조절
│   ├── seg.c / seg.h          # 7세그먼트 숫자 표시 + 타이머
│   ├── init_module.c / h      # 전체 장치 초기화
│   ├── test / test.c          # 개별 장치 테스트용
│   └── makefile               # .so 빌드 자동화
│
├── server/                    # 서버 소스
│   ├── server.c               # 멀티스레드, daemon, signal 처리 포함
│   ├── server.log             # 실행 로그 출력
│   └── makefile
🛠️ 실행 방법
📌 사전 준비
Raspberry Pi 4 + wiringPi 설치 필요

각 모듈 GPIO 연결 필요 (핀 정보는 아래 참조)

1. 🔨 모듈 빌드 (.so 생성)
bash
복사
편집
cd module
make
2. 🔨 서버 빌드
bash
복사
편집
cd ../server
make
3. 🖥️ 서버 실행 (데몬 + 로그 출력)
bash
복사
편집
./server &
tail -f server.log
4. 💻 클라이언트 실행
bash
복사
편집
cd ../client
./client <서버_IP>
🔌 장치 제어 기능
장치	명령어	기능	핀 번호 (BCM)
LED	LED ON MAX, LED OFF	밝기 조절 (MAX/MID/MIN)	GPIO 4
BUZZER	BUZZER ON1, OFF	노래 재생 (동물의 숲, 너의 이름은)	GPIO 27
CDS	CDS READ, CDS NOW	밝기 감지 후 LED ON/OFF	CDS: GPIO 17, LED: GPIO 22
7-SEGMENT	SEG 5, SEG LED 5	숫자 카운트다운 + 부저 + LED 연동	GPIO 23,18,15,14, LED: GPIO 21

📬 클라이언트 응답 예시
명령어	서버 응답 메시지
LED ON MAX	🌞 최대 밝기입니다!
BUZZER ON1	🍃 동물의 숲 재생 중...
CDS READ (어두움)	🌑 앞이 안 보여요!
SEG 3	💤 잠에 들 시간이에요~
SEG LED 4	💤 집가요~

🧠 주요 기능 요약
🧵 멀티스레드 서버: pthread로 동시 클라이언트 처리

🧠 SIGINT 핸들러: 안전한 종료 (Ctrl+C만 허용)

📦 공유 라이브러리 로딩: dlopen()으로 모듈 독립 실행

🎨 CLI UI: 이모지, 컬러 출력, 명령어 도움말 포함

🛠️ Makefile: 모듈 + 서버 + 클라이언트 자동 빌드

⚠️ 문제점 및 보완사항
문제점	보완 아이디어
softTone 음정 부정확	PWM + wav 재생 기능 도입
dlopen() 반복 오버헤드	미리 로딩 + 모듈 캐싱 처리
명령어 파싱 단순함	명령어 파서 분리 또는 정규식 도입
리소스 충돌	타이머 제어 분리 / 쓰레드 동기화 필요

✅ 테스트 환경
Raspberry Pi 4 Model B (Raspbian OS)

Ubuntu 22.04 (클라이언트)

GCC 9.x

wiringPi, softTone, softPwm 사용

👨‍💻 개발자
이름: Jioh Jeon

개발 기간: 3일 집중 프로젝트

목적: 임베디드 시스템 + 네트워크 연동 통합 실습
