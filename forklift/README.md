# Balancing Forklift

KS0193 밸런싱 로봇 앞쪽에 ㄴ자 포크를 달아 물체를 들어 올리는 밸런싱 지게차. 포크는 교수대형 틀에 줄로 매달려 있고, 서보(FT6325M) 1개가 줄을 감고 풀어 위아래로 움직이며, 높이는 앱에서 사람이 수동으로 조종한다.

- 밸런싱: 아두이노에서 키트 방식 PD/PID
- 조종: 안드로이드 앱 ↔ 쉴드의 HC-06
- 서보: PCA9685 경유 (I2C)
- 영상: ESP32-CAM (전원만 공유, 데이터 연결 없음)

자세한 배경과 결정 사항: [docs/project_overview.md](docs/project_overview.md)
부품 목록: [docs/bom.md](docs/bom.md)
이슈 기록: [docs/issues/](docs/issues/)
베이스 키트 핀·타이머 정보: [../docs/base_kit.md](../docs/base_kit.md)

## 구조

```
forklift/
├── firmware/
│   ├── forklift/forklift.ino     # 메인 스케치
│   └── tests/                    # pca9685_servo_test, i2c_scan 등
├── esp32cam/                     # CameraWebServer AP 모드
├── hardware/kicad/               # 회로도 (KiCad 7+)
├── app/                          # 안드로이드 앱
├── mechanical/                   # 리프트 틀, 스풀 CAD
└── docs/
```

## 펌웨어 라이브러리

| 라이브러리 | 용도 | 버전 |
|---|---|---|
| Adafruit PWM Servo Driver Library | PCA9685 제어 (`writeMicroseconds()` 사용) | 3.0.3 |

## 시험 스케치

| 스케치 | 내용 |
|---|---|
| [firmware/tests/pca9685_servo_test](firmware/tests/pca9685_servo_test/) | PCA9685 + FT6325M 단독 시험. 키보드(`servo_keys.ps1`)로 누르는 동안 감기/풀기, 떼면 위치 유지 |

## KiCad

`hardware/kicad/upgraded_balancing_robot.kicad_pro`를 연다. 커스텀 심볼은 `hardware/kicad/symbols/`에 있고 `sym-lib-table`이 `${KIPRJMOD}` 상대경로로 참조한다.
