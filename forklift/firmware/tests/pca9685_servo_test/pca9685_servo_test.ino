// PCA9685 + FT6325M (위치 제어형) 단독 시험
//
// - 펄스 폭 = 각도. 키를 누르고 있는 동안 각도가 조금씩 바뀌고, 떼면 그 자리를 버틴다.
// - 전원을 켜면 출력 OFF. 첫 키 입력에서 SERVO_CENTER_US로 이동한 뒤부터 조종된다.
// - 밸런싱·바퀴 모터 코드 없음. MsTimer2를 쓰지 않으므로 I2C 충돌 걱정 없음.
// - 라이브러리: Adafruit PWM Servo Driver Library 3.0.3
//
// [키보드 수동 조종] 한 글자, Enter 필요 없음 (servo_keys.ps1 사용)
//   w        감기(포크 올림). 누르고 있는 동안 계속
//   s        풀기(포크 내림)
//   space/x  이동 멈춤 (그 자리 유지)
//   1~5      속도 단계
//
// [줄 명령] 시리얼 모니터 (115200, 줄 끝: 새 줄)
//   p <us>   그 펄스(각도)로 바로 이동
//   o        출력 OFF (서보 힘 풀림 → 줄에 걸린 하중으로 포크가 내려갈 수 있음)
//   ?        상태 출력

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "config.h"

Adafruit_PWMServoDriver pwm(PCA9685_ADDR);

int posUs = -1;              // 현재 펄스. -1: 출력 OFF / 위치 모름
int speedLevel = DEFAULT_SPEED_LEVEL;
char holdKey = 0;            // 'w' / 's' 누르는 중, 0: 아님
unsigned long lastKeyMs = 0;
unsigned long lastStepMs = 0;
bool limitReported = false;
String lineBuf;

bool i2cPresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

void writeUs(int us) {
  posUs = constrain(us, SERVO_MIN_US, SERVO_MAX_US);
  pwm.writeMicroseconds(SERVO_CHANNEL, posUs);
}

void outputOff() {
  pwm.setPWM(SERVO_CHANNEL, 0, 4096);  // full OFF
  posUs = -1;
  holdKey = 0;
  Serial.println(F("출력 OFF (서보 힘 풀림)"));
}

void printPos(const __FlashStringHelper *label) {
  Serial.print(label);
  Serial.print(posUs);
  Serial.println(F(" us"));
}

void printStatus() {
  Serial.print(F("출력: "));
  if (posUs < 0) Serial.print(F("OFF"));
  else { Serial.print(posUs); Serial.print(F(" us")); }
  Serial.print(F(", 범위: "));
  Serial.print(SERVO_MIN_US);
  Serial.print(F("~"));
  Serial.print(SERVO_MAX_US);
  Serial.print(F(", 속도 단계: "));
  Serial.print(speedLevel);
  Serial.print(F(" ("));
  Serial.print(speedLevel * SPEED_STEP_US_PER_S);
  Serial.println(F(" us/s)"));
}

void pressHold(char k) {
  if (posUs < 0) {
    writeUs(SERVO_CENTER_US);
    printPos(F("위치 모름 → 가운데로 이동: "));
  }
  if (holdKey != k) {
    Serial.println(k == 'w' ? F("감기...") : F("풀기..."));
    limitReported = false;
  }
  holdKey = k;
  lastKeyMs = millis();
}

void releaseHold(const __FlashStringHelper *why) {
  if (holdKey == 0) return;
  holdKey = 0;
  Serial.print(F("멈춤("));
  Serial.print(why);
  Serial.print(F("), 유지: "));
  Serial.print(posUs);
  Serial.println(F(" us"));
}

// 한 글자 키. 처리했으면 true.
bool handleKey(char ch) {
  if (ch == 'w' || ch == 's') { pressHold(ch); return true; }
  if (ch == ' ' || ch == 'x') { releaseHold(F("키")); return true; }
  if (ch >= '1' && ch <= '5') {
    speedLevel = ch - '0';
    Serial.print(F("속도 단계: "));
    Serial.print(speedLevel);
    Serial.print(F(" ("));
    Serial.print(speedLevel * SPEED_STEP_US_PER_S);
    Serial.println(F(" us/s)"));
    return true;
  }
  return false;
}

void handleCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;
  char c = cmd.charAt(0);

  if (c == 'p') {
    int sp = cmd.indexOf(' ');
    if (sp < 0) { Serial.println(F("사용법: p <us>")); return; }
    holdKey = 0;
    writeUs(cmd.substring(sp + 1).toInt());
    printPos(F("이동: "));
  } else if (c == 'o') {
    outputOff();
  } else if (c == '?') {
    printStatus();
  } else {
    Serial.println(F("알 수 없는 명령"));
  }
}

void stepHold() {
  unsigned long now = millis();
  if (holdKey == 0) return;
  if (now - lastKeyMs > HOLD_MS) { releaseHold(F("키 뗌")); return; }
  if (now - lastStepMs < STEP_INTERVAL_MS) return;
  lastStepMs = now;

  int delta = (long)speedLevel * SPEED_STEP_US_PER_S * STEP_INTERVAL_MS / 1000;
  int sign = (holdKey == 'w') ? UP_SIGN : -UP_SIGN;
  int target = posUs + sign * delta;

  if (target < SERVO_MIN_US || target > SERVO_MAX_US) {
    writeUs(target);  // 끝값으로 제한됨
    if (!limitReported) {
      printPos(F("한계 도달: "));
      limitReported = true;
    }
    return;
  }
  writeUs(target);
}

void setup() {
  Serial.begin(SERIAL_BAUD);
  Wire.begin();

  Serial.println(F("\n=== PCA9685 서보 시험 (위치 제어형) ==="));

  bool pcaOk = i2cPresent(PCA9685_ADDR);
  Serial.print(F("PCA9685 (0x40): "));
  Serial.println(pcaOk ? F("OK") : F("응답 없음"));
  Serial.print(F("MPU-6050 (0x68): "));
  Serial.println(i2cPresent(MPU6050_ADDR) ? F("OK") : F("응답 없음"));

  if (!pcaOk) {
    Serial.println(F("PCA9685가 없어 멈춤. SDA/SCL(A4/A5), VCC, GND 배선을 확인할 것."));
    while (true) {}
  }

  pwm.begin();
  pwm.setOscillatorFrequency(PCA9685_OSC_HZ);
  pwm.setPWMFreq(SERVO_FREQ_HZ);
  delay(10);
  outputOff();

  Serial.println(F("키: w 감기 | s 풀기 | space 멈춤 | 1~5 속도"));
  Serial.println(F("명령: p <us> | o | ?"));
  printStatus();
}

void loop() {
  stepHold();

  while (Serial.available()) {
    char ch = Serial.read();
    if (lineBuf.length() == 0) {
      if (handleKey(ch)) continue;
      if (ch == '\r' || ch == '\n') continue;
    }
    if (ch == '\r') continue;
    if (ch == '\n') { handleCommand(lineBuf); lineBuf = ""; continue; }
    if (lineBuf.length() < 32) lineBuf += ch;
  }
}
