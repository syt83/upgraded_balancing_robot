# Upgraded Balancing Robot

2륜 자가 밸런싱 로봇 키트 **keyestudio KS0193**을 베이스로 만드는 두 개의 독립 프로젝트입니다.

| 프로젝트 | 한 줄 요약 | 상태 |
|---|---|---|
| [🏗️ 밸런싱 지게차](forklift/) | 균형을 잡으며 달리면서 포크로 물체를 들어 옮기는 로봇 | 회로도 작업 중 |
| [🏁 라인 트래킹](line_tracking/) | IR 센서로 선을 따라 최대한 빨리 결승점에 도착하는 로봇 | 센서 선정 전 |

두 프로젝트는 같은 키트를 쓰지만 **코드와 하드웨어를 공유하지 않는 별개 프로젝트**입니다.

---

## 🏗️ 밸런싱 지게차 (`forklift/`)

두 바퀴로 균형을 잡는 로봇 앞쪽에 지게차 포크를 달아 물체를 들어 올립니다.

- **리프트:** 차체에 교수대처럼 세운 틀에 ㄴ자 포크가 줄로 매달려 있습니다. 서보(FT6325M) 1개가 줄을 감으면 포크가 올라가고, 풀면 내려갑니다. 높이는 앱에서 사람이 직접 올리고 내립니다.
- **밸런싱:** 아두이노가 MPU-6050 자세 센서와 엔코더 값으로 PD/PID 제어를 합니다. 포크와 화물 때문에 무게중심이 앞으로 쏠리는 것이 핵심 난제입니다.
- **조종:** 안드로이드 앱에서 블루투스(HC-06)로 주행과 리프트를 조종합니다.
- **영상:** 로봇에 단 ESP32-CAM이 자체 Wi-Fi를 띄우고, 폰으로 실시간 영상을 봅니다.

```
[안드로이드 앱] ──Bluetooth──▶ HC-06 ─▶ Arduino ─┬─▶ TB6612FNG ─▶ 바퀴 모터 ×2
                                        ▲        └─I2C─▶ PCA9685 ─▶ FT6325M (리프트)
                                  MPU-6050 (I2C)
[폰 브라우저/앱] ◀──Wi-Fi── ESP32-CAM (영상 전용, 아두이노와 데이터 연결 없음)
```

자세히: [forklift/README.md](forklift/README.md) · [설계 배경과 결정 사항](forklift/docs/project_overview.md) · [부품 목록(BOM)](forklift/docs/bom.md)

## 🏁 라인 트래킹 (`line_tracking/`)

로봇 앞쪽에 IR 센서를 달아 바닥의 선을 감지하고, 선을 따라 **최대한 빨리 결승 지점에 도착**하는 것이 목표입니다.

- **센서:** 앞쪽에 단 IR 센서로 선의 위치를 읽습니다. (센서 종류, 개수, 연결 핀은 미정)
- **제어:** 균형을 유지하면서 선 위치에 따라 조향합니다. 빠를수록 좋기 때문에 균형 안정성과 주행 속도 사이의 절충이 핵심입니다.

자세히: [line_tracking/README.md](line_tracking/README.md)

---

## 베이스 키트: KS0193

| 구성 | 내용 |
|---|---|
| 메인 보드 | Keyestudio V4.0 (Arduino UNO 호환) |
| 쉴드 | Balance Shield V3: TB6612FNG 모터 드라이버, MPU-6050, HC-06 소켓 |
| 모터 | GM37-520 홀 엔코더 DC 기어모터 ×2 (12V, 1:30) |
| 전원 | 3셀 리튬 배터리 (9~12.6V) |

핀 배치, 타이머 사용, 키트 제어 구조: [docs/base_kit.md](docs/base_kit.md)

## 저장소 구조

```
.
├── docs/base_kit.md     # 두 프로젝트 공통: 키트 하드웨어, 핀, 타이머
├── forklift/            # 밸런싱 지게차
│   ├── docs/            # 설계 문서, BOM
│   └── hardware/kicad/  # 회로도 (KiCad 7+)
└── line_tracking/       # 라인 트래킹
```

## 개발 환경

- **펌웨어:** Arduino IDE (보드: Arduino UNO)
- **회로도:** KiCad 7 이상. `forklift/hardware/kicad/upgraded_balancing_robot.kicad_pro`를 엽니다.
- **업로드 주의:** 코드를 올릴 때는 쉴드의 블루투스 스위치를 **OFF**로 둡니다. HC-06이 USB와 같은 시리얼 핀(D0/D1)을 쓰기 때문입니다.

## 함께 작업하기

2인 팀 프로젝트입니다. 작업 전에 **[CONTRIBUTING.md](CONTRIBUTING.md)** (브랜치·커밋 규칙, 펌웨어·KiCad 주의사항)를 읽어 주세요. AI 도구(Claude Code)도 같은 규칙을 따르며, 추가 규칙은 [CLAUDE.md](CLAUDE.md)에 있습니다.
