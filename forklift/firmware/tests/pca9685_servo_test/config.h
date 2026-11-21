#pragma once

// FT6325M 시험용 설정.
// 2026-11-21 시험에서 위치 제어형으로 확인됨: 펄스 폭 = 각도. 멈추면 그 각도를 스스로 버팀.

// ---- 통신 ----
// 업로드·시리얼 모니터 사용 시 쉴드의 블루투스 스위치 OFF
#define SERIAL_BAUD 115200

// ---- PCA9685 ----
#define PCA9685_ADDR   0x40
#define MPU6050_ADDR   0x68   // 같은 I2C 버스에 있는지 확인용
#define SERVO_CHANNEL  0
#define SERVO_FREQ_HZ  50

// PCA9685 내부 오실레이터 실제 주파수 (이론값 25MHz, 모듈마다 23~27MHz로 차이 있음).
// 실제 펄스 폭이 설정과 다르면 보정한다. Adafruit 예제 기본값 27MHz.
#define PCA9685_OSC_HZ 27000000UL

// ---- 펄스 폭 = 각도 (µs) ----
// 실측 범위 (2026-11-21): 감는 쪽 끝 2900µs, 푸는 쪽 끝 100µs.
// 이 밖으로는 서보가 더 돌지 않는다. 끝에서 "지지직" 소리를 내며 버티면 과열되므로 반대로 조금 풀 것.
#define SERVO_MIN_US    100
#define SERVO_MAX_US    2900
#define SERVO_CENTER_US 1500   // 위치를 모를 때(전원 켠 직후) 첫 키 입력에서 여기로 감

// 방향: 2026-11-21 시험에서 펄스를 키우면 줄이 감겼다(포크 올라감). 반대면 -1로.
#define UP_SIGN         (+1)

// ---- 키보드 수동 조종 ----
// 키를 누르고 있으면 글자가 반복해서 들어온다. 마지막 글자 후 이 시간이 지나면 이동을 멈추고 그 자리를 유지.
#define HOLD_MS          300
// 이동 주기와 속도. 속도 단계 1~5 → SPEED_STEP_US_PER_S × 단계 (µs/s)
#define STEP_INTERVAL_MS 20
#define SPEED_STEP_US_PER_S 200   // 단계 1 = 200µs/s ... 단계 5 = 1000µs/s
#define DEFAULT_SPEED_LEVEL 2
