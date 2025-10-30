# Main ↔ Front Packet 구조 설명

## 개요
- **패킷 이름**: PKT_FRONT_REQ_LED_3
- **패킷 타입**: `0x04`
- **통신 방향**: Main → Front
- **파일 위치**: [parser_front.c:401](../Source/comm/parser_front.c#L401)
- **함수 이름**: `MakePktReqLed3()`

## 패킷 구조

### 전체 구조 요약
```
┌─────┬──────┬──────────┬────────────┬────────────┬──────┬──────┬──────┬─────┐
│ STX │ TYPE │ LED Data │ Voice ID   │ Voice Vol  │ MODE │ CRC  │ CRC  │ ETX │
│     │      │ (n bytes)│            │            │      │ (H)  │ (L)  │     │
└─────┴──────┴──────────┴────────────┴────────────┴──────┴──────┴──────┴─────┘
```

### 상세 필드 정의

| Offset | 필드명 | 크기 | 값 | 설명 |
|--------|--------|------|-----|------|
| 0 | STX | 1 byte | `0xAA` | Start of Packet |
| 1 | Message Type | 1 byte | `0x04` | PKT_FRONT_REQ_LED_3 |
| 2 ~ (2+n-1) | LED Data | n bytes | Variable | LED 밝기 데이터 (LED_ID_MAX개) |
| 2+n | Voice ID | 1 byte | `0x00 ~ 0xFF` | 재생할 음성 ID |
| 3+n | Voice Volume | 1 byte | `1 ~ 5` | 음성 볼륨 레벨 |
| 4+n | Mode | 1 byte | `0 or 1` | FOTA 디스플레이 모드 |
| 5+n | CRC-16 (H) | 1 byte | Calculated | CRC-16 상위 바이트 |
| 6+n | CRC-16 (L) | 1 byte | Calculated | CRC-16 하위 바이트 |
| 7+n | ETX | 1 byte | `0x55` | End of Packet |

> **참고**: n = LED_ID_MAX (현재 188개 LED)

## 필드 상세 설명

### 1. STX (Start of Transmission)
- **값**: `0xAA`
- **설명**: 패킷 시작 바이트

### 2. Message Type
- **값**: `0x04` (PKT_FRONT_REQ_LED_3)
- **설명**: 패킷 종류를 식별하는 타입 값

### 3. LED Data (LED 밝기 데이터)
- **크기**: LED_ID_MAX 바이트 (188 bytes)
- **설명**: 각 LED의 밝기(Duty)를 프로토콜 포맷으로 변환한 값
- **함수**: `ConvertDuty2Protocol(i)` 호출로 변환
- **LED ID 범위**: 0 ~ 187 (LED_ID_MAX - 1)

#### LED 밝기 프로토콜 매핑표

| Duty (%) | Protocol Value | 상수명 |
|----------|----------------|--------|
| 0 | 0x00 | PROTOCOL_LED_DUTY_0 |
| 10 | 0x01 | PROTOCOL_LED_DUTY_10 |
| 20 | 0x02 | PROTOCOL_LED_DUTY_20 |
| 30 | 0x03 | PROTOCOL_LED_DUTY_30 |
| 40 | 0x04 | PROTOCOL_LED_DUTY_40 |
| 50 | 0x05 | PROTOCOL_LED_DUTY_50 |
| 60 | 0x06 | PROTOCOL_LED_DUTY_60 |
| 70 | 0x07 | PROTOCOL_LED_DUTY_70 |
| 80 | 0x08 | PROTOCOL_LED_DUTY_80 |
| 90 | 0x09 | PROTOCOL_LED_DUTY_90 |
| 100 | 0x0A | PROTOCOL_LED_DUTY_100 |

#### 주요 LED ID 목록
LED ID는 [led.h](../Source/display/led.h#L38)에 정의되어 있으며, 총 188개입니다:
- **7-Segment LEDs**: SEG_1A ~ SEG_6G (0~41)
- **UV 살균 표시**: UV_HOT_STER_TXT, UV_STER_TXT, UV_WATER_FAUCET, etc. (42~47)
- **온도 단계**: TEMP_STEP_1 ~ TEMP_STEP_5 (48~52)
- **얼음 상태**: ICE_FULL_TXT, ICE_LACK_TXT (53~54)
- **기능 아이콘**: FAST_ICE_ICON, SLEEP_ICON, COLON, CELCIUS (55~58)
- **음료 아이콘**: RAMEN, DRIP_COFFEE, TEA, MY1~MY3 (59~64)
- **기타**: WIFI_ICON, SUNNY_ICON, CLOUD_ICON, 용량 표시, 물 선택 버튼 등 (65~187)

### 4. Voice ID
- **크기**: 1 byte
- **범위**: VoiceId_T 열거형 참조 ([voice.h](../Source/sound/voice.h))
- **특수값**:
  - `0xFF` (ID_NONE): 음성 없음
- **동작**:
  - 음성 ID가 ID_NONE이 아니면 해당 음성을 재생
  - 재생 후 자동으로 ID_NONE으로 초기화 (One-shot 방식)
  - 함수: `GetVoicePlayId()` / `PlayVoice(ID_NONE)`

#### 주요 음성 ID 예시
| ID | 값 | 설명 |
|----|-----|------|
| ID_COVER_OPEN_BOTTOM | 48 | 하단 본체 커버 열림 |
| ID_ERROR_COLD | 52 | 냉수 기능 이상 |
| ID_WIFI_CONNECTING | 64 | 와이파이 연결 시작 |
| ID_EFFLUENT | 101 | 추출 시작음 |
| ID_SELECT | 104 | 선택음 (땡) |
| ID_NONE | 255 | 음성 없음 |

### 5. Voice Volume
- **크기**: 1 byte
- **범위**: 1 ~ 5
- **상수 정의**:
  - `VOICE_VOLUME_1` (1): OFF (Mute)
  - `VOICE_VOLUME_2` (2): VOL 1
  - `VOICE_VOLUME_3` (3): VOL 2 (Default)
  - `VOICE_VOLUME_4` (4): VOL 3
  - `VOICE_VOLUME_5` (5): VOL 4 (Max)
- **특수 처리**:
  - 특정 음성 ID는 항상 최대 볼륨으로 재생 (`IsVoiceMaxVol()` 체크)
  - 최대 볼륨 대상이면 자동으로 `MAX_VOICE_VOLUME`(5)로 설정
- **함수**: `GetVoiceVolume()`, `IsVoiceMaxVol()`

### 6. Mode (FOTA Display Mode)
- **크기**: 1 byte
- **값**:
  - `0`: FOTA 디스플레이 OFF
  - `1`: FOTA 디스플레이 ON
- **함수**: `GetFotaDisp()`
- **설명**: FOTA (Firmware Over-The-Air) 업데이트 중 디스플레이 제어 모드

### 7. CRC-16
- **크기**: 2 bytes (High, Low)
- **알고리즘**: CRC-CCITT (Polynomial: 0x1021)
- **계산 범위**: STX부터 CRC 직전까지 (len - 3 bytes)
- **함수**: `Rx_CRC_CCITT()`, `Crc16_Front()`
- **바이트 순서**:
  - buf[len-3] = High Byte
  - buf[len-2] = Low Byte

### 8. ETX (End of Transmission)
- **값**: `0x55`
- **설명**: 패킷 종료 바이트

## 패킷 크기
```
Total Size = 1 (STX) + 1 (TYPE) + 188 (LEDs) + 1 (Voice ID) + 1 (Voice Vol)
             + 1 (Mode) + 2 (CRC) + 1 (ETX)
           = 196 bytes
```

### 호출 예시
```c
// process_sys_event.c:446
SetCommHeader( COMM_ID_FRONT, PKT_FRONT_REQ_LED_3 );
```

## 관련 파일
- **송신**: [parser_front.c](../Source/comm/parser_front.c)
- **헤더**: [parser_front.h](../Source/comm/parser_front.h)
- **LED 정의**: [led.h](../Source/display/led.h)
- **음성 정의**: [voice.h](../Source/sound/voice.h)
- **디스플레이**: [process_display.h](../Source/display/process_display.h)

## 응답 패킷
- **패킷 타입**: PKT_FRONT_ACK_LED_3 (`0x84`)
- **방향**: Front → Main
- **처리 함수**: `ParserAckLed()` ([parser_front.c:266](../Source/comm/parser_front.c#L266))

### 응답 패킷 필드 (PKT_FRONT_ACK_LED_3)
| Offset | 필드명 | 크기 | 설명 |
|--------|--------|------|------|
| 0 | STX | 1 byte | 0xAA |
| 1 | Message Type | 1 byte | 0x84 |
| 2~5 | Front Version | 4 bytes | Major, Event, Patch, Miner |
| 6 | Model Mask | 1 byte | bit0: Model (CHP/CP) |
| 7 | Input Mask | 1 byte | bit1: Filter Cover Status |
| 8~11 | Key Value | 4 bytes | 키 입력 상태 (32bit) |
| 12~13 | ADC UV Water | 2 bytes | UV 물 센서 ADC 값 |
| 14~15 | ADC UV Ice | 2 bytes | UV 얼음 센서 ADC 값 |
| 16~17 | ADC Reserved 1 | 2 bytes | 미사용 |
| 18~19 | ADC Reserved 2 | 2 bytes | 미사용 |
| 20~21 | CRC-16 | 2 bytes | CRC-16 (H, L) |
| 22 | ETX | 1 byte | 0x55 |

**Total Size**: 23 bytes

## 참고사항
1. LED 데이터는 188개의 LED ID 순서대로 전송됩니다.
2. 음성은 One-shot 방식으로 재생 후 자동으로 초기화됩니다.
3. 특정 음성은 볼륨 설정과 무관하게 항상 최대 볼륨으로 재생됩니다.
4. CRC-16은 CCITT 알고리즘을 사용합니다.
5. 패킷은 주기적으로 전송되어 Front 보드의 LED와 음성을 제어합니다.

## 버전 히스토리
- **PKT_FRONT_REQ_LED** (0x01): 구버전 (미사용)
- **PKT_FRONT_REQ_LED_2** (0x03): 중간 버전 (미사용)
- **PKT_FRONT_REQ_LED_3** (0x04): 현재 사용 중인 버전