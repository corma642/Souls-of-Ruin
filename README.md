# ⚔️ Souls of Ruin (소울즈 오브 루인)

![Unreal Engine](https://img.shields.io/badge/Unreal_Engine-5.4-black?style=for-the-badge&logo=unrealengine)
![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge&logo=cplusplus)
![GAS](https://img.shields.io/badge/System-Gameplay_Ability_System-green?style=for-the-badge)

> **"끝없는 어둠 속에서 피어나는 쌍검의 불꽃, 파멸의 끝에서 영혼의 투쟁이 시작된다."**

**Souls of Ruin**은 Unreal Engine 5의 강력한 성능과 **Gameplay Ability System(GAS)**을 기반으로 제작된 정통 3인칭 액션 RPG입니다. 이 프로젝트는 고품질의 애니메이션 리타겟팅과 정교한 전투 로직을 통해 묵직하면서도 속도감 있는 액션을 구현하는 데 집중했습니다.

---

## 🎮 주요 게임 특징

### ⚔️ 스타일리시한 이도류(Dual Wield) 전투
단순한 공격을 넘어, 두 자루의 검을 활용한 화려하고 연속적인 콤보 시스템을 경험하세요. **Motion Warping** 기술을 적용하여 공격 시 적과의 거리를 자동으로 보정, 더욱 박진감 넘치는 타격감을 제공합니다.

### 🔮 GAS 기반의 정교한 스킬 시스템
모든 스킬과 상태 이상은 **Gameplay Ability System**을 통해 관리됩니다.
*   **체계적인 속성 관리**: 체력, 마력, 스태미나 등 모든 스탯이 GE(Gameplay Effect)를 통해 유기적으로 변화합니다.
*   **다양한 액티브 스킬**: Q, E, R, T 키를 사용하여 전황을 뒤집는 강력한 스킬을 시전할 수 있습니다.

### 🛡️ 전략적인 전투 메커니즘
*   **타겟 록온(Lock-On)**: 소울류 스타일의 타겟 고정 시스템으로 적과의 1:1 대치 상황에서 긴장감을 유지합니다.
*   **정밀한 히트 리액트**: 피격 방향에 따른 리얼한 애니메이션 반응과 히트 스톱(Hit Stop) 효과로 타격의 재미를 극대화했습니다.
*   **회피 및 가드**: 적의 공격을 타이밍에 맞춰 회피하거나 방어하여 반격의 기회를 잡으세요.

### 👹 강력한 보스 에셋 (Paragon)
Epic Games의 **Paragon** 에셋(Grux, Khaimera, Rampage)을 고유한 패턴을 가진 보스로 재해석했습니다. 각 보스는 독자적인 AI 비헤이비어 트리와 GAS 기반의 특수 능력을 보유하고 있습니다.

---

## 🛠 기술적 강점

*   **Native Gameplay Tags**: 문자열 기반의 오류를 방지하기 위해 모든 상태와 입력을 C++ 상수에 매핑하여 관리합니다.
*   **데이터 주도 설계 (Data-Driven)**: 캐릭터의 초기 능력치, 스킬 구성 등을 Data Asset으로 분리하여 유지보수와 확장성을 높였습니다.
*   **확장 가능한 인터페이스**: `PawnCombatInterface`, `PawnUIInterface` 등을 통해 캐릭터와 컴포넌트 간의 결합도를 낮추고 기능을 모듈화했습니다.

---

## 🕹️ 조작 방법

| 명령 | 입력 |
| :--- | :--- |
| **이동** | `W`, `A`, `S`, `D` |
| **카메라 조작** | `마우스 이동` |
| **일반 공격** | `마우스 왼쪽 버튼` |
| **회피 (구르기)** | `Space Bar` |
| **질주** | `Left Shift` (홀드) |
| **방어 (가드)** | `마우스 오른쪽 버튼` (홀드) |
| **타겟 고정** | `마우스 휠 버튼 (클릭)` |
| **스킬 1 / 2 / 3 / 4** | `Q`, `E`, `R`, `T` |

---

## 🚀 시작 가이드

### 개발 환경
*   **Unreal Engine 5.4**
*   **Visual Studio 2022**

### 설치 방법
1.  저장소를 클론합니다.
2.  `ProjectA.uproject` 파일을 우클릭하여 **Generate Visual Studio project files**를 실행합니다.
3.  `.sln` 파일을 열고 **Development Editor** 구성에서 빌드합니다.
4.  에디터를 실행하고 `MainLevel`을 로드하여 플레이하세요.

---
*Created by [Your Name/Github ID]*