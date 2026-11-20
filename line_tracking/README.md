# Line Tracking

KS0193 밸런싱 로봇 기반 라인 트래킹. 지게차 프로젝트와는 별개.

베이스 키트 핀·타이머 정보: [../docs/base_kit.md](../docs/base_kit.md)

## 구조

```
line_tracking/
├── firmware/
│   ├── line_tracking/line_tracking.ino   # 메인 스케치
│   └── tests/                            # 센서 단독 테스트 등
├── hardware/                             # 센서 배선도
└── docs/
```

## 미정 사항

- 센서 종류, 개수, 연결 핀 (비어 있는 핀: D2, D3, A0~A3)
