# 🚗 CAN 통신활용 자동차 정보 수집 프로젝트

블로그를 통해 자세한 내용을 확인하세요!!
> **프로젝트 링크:** [Velog 블로그 링크] (https://velog.io/@david1597/CAN%ED%86%B5%EC%8B%A0-%EC%9E%90%EB%8F%99%EC%B0%A8-%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B81%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8-%EA%B0%9C%EC%9A%94)

![Language](https://img.shields.io/badge/Language-C-blue)
![Board](https://img.shields.io/badge/MCU-STM32F429ZI-brightgreen)
![MCU](https://img.shields.io/badge/MCU-STM32F103RB-yellow)
![Status](https://img.shields.io/badge/Project_Stage-Completed-success)
![Interface](https://img.shields.io/badge/Protocol-CAN-lightgrey)
![IDE](https://img.shields.io/badge/IDE-STM32CubeIDE-blueviolet)
![Display](https://img.shields.io/badge/LCD-ILI9341_(Dashboard)-ff69b4)
![Steering](https://img.shields.io/badge/Mechanism-Rack__&__Pinion-orange)
--
본 프로젝트는 STM32 마이크로컨트롤러와 CAN 통신을 활용하여 **자동차의 센서 데이터 수집 및 시각화**를 구현한 프로젝트
**포텐셔미터**, **서보모터**, **TFT LCD (ILI9341)**, **CAN 트랜시버**, **STM32F429ZI / F103RB** 등 다양한 하드웨어 모듈을 융합하여 모터 제어 모듈이 수집한 데이터를 CAN 통신을 통해 메인 모듈에서 적절히 처리하게 함.

---

## 📌 프로젝트 개요

- ✅ 모터 제어 모듈이 포텐셔 미터 값을 활용해, 속도 조향각 제어
- ✅ 랙 피니언 기어를 활용해서 조향장치 구현
- ✅ CAN 통신을 활용하여 모듈간 데이터 송수신 환경 구축

---

## 🧩 시스템 구성도

| 부품명               | 용도                                 |
|--------------------|--------------------------------------|
| STM32F103RB        | 센서 입력 노드 / CAN 송신              |
| STM32F429ZI        | 메인 노드 / 데이터 수신 및 LCD 표시     |
| 포텐셔미터 x2       | 사용자 입력 (조향/가속 시뮬레이션)         |
| 서보모터           | 포텐셔미터 값 기반 조향 각도 구현       |
| TFT LCD ILI9341    | 수신한 데이터 실시간 시각화             |
| CAN 트랜시버 2개    | STM 간 CAN 물리 계층 통신 구현          |

---

## 🖥️ 주요 기능

- ✅ **포텐셔미터 값을 실시간으로 수집하여 모터제어 및 데이터 수집 (STM32F103RB)**
- ✅ **CAN 통신 기반 데이터 송수신**
- ✅ **수집된 데이터들을 활용하여 LCD 출력 및 기타 데이터 처리 (STM32F429ZI)**
- ✅ **TFT LCD에 센서 값 시각화**
- ✅ **트랜시버와 종단저항 활용해 안정적인 CAN 통신 프레임 구축 **
- ✅ **메인 모듈에서의 멀티 태스킹을 위한 비동기 처리 시스템 **

---

## 🔧 기술 스택 및 개발 환경

- **MCU**: STM32F103RB, STM32F429ZI  
- **IDE**: STM32CubeIDE  
- **프로그래밍 언어**: C  
- **CAN 트랜시버**: MCP2551 
- **TFT LCD 드라이버**: ILI9341 (SPI 인터페이스)  
- **LCD 라이브러리**: Adafruit ILI9341 기반 대쉬보드 역할
- **통신 프로토콜**: CAN (11-bit ID, 500kbps)

---

## 📼 시스템 구현 환경
<img width="761" height="574" alt="image" src="https://github.com/user-attachments/assets/e4458808-cbb4-46e0-828e-3bb2bc98ee33" />

---

## 📟 ILI9341 대쉬보드
<img width="331" height="187" alt="image" src="https://github.com/user-attachments/assets/e0d81cdc-ec91-43a9-aa70-49e704356197" />

---



