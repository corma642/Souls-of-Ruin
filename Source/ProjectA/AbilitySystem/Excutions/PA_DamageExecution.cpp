// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Excutions/PA_DamageExecution.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "PA_GameplayTags.h"

// 피해 계산에 사용할 속성 캡처 구조체
struct FPA_DamageCapture
{
	// 피해 계산에 사용할 속성 캡처 선언
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FPA_DamageCapture()
	{
		// 캡처 정의 초기화 (Source와 Target 구분)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, Defense, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, DamageTaken, Source, false)
	}
};

// 속성 캡처 구조체의 정적 인스턴스를 반환하여 캡처 정의들을 전역에서 재사용
static const FPA_DamageCapture& GetPADamageCapture()
{
	static FPA_DamageCapture PADamageCapture;
	return PADamageCapture;
}

UPA_DamageExecution::UPA_DamageExecution()
{
	// 구조체에서 정의한 속성들의 캡처 정의를 추가한다.
	// 이를 통해 피해 계산 시에 해당 속성들의 값이 자동으로 캡처되어 계산에 사용된다.
	RelevantAttributesToCapture.Add(GetPADamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetPADamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetPADamageCapture().DamageTakenDef);
}

void UPA_DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// 현재 실행중인 게임플레이 이펙트 스펙을 가져옴
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	// 캡처된 소스 태그와 타겟 태그가 포함되도록 설정
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// 가해자의 공격력
	// 캡처된 값을 EvaluateParameters를 통해 계산
	float SourceAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPADamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	// 피해자의 방어력
	float TargetDefense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPADamageCapture().DefenseDef, EvaluateParameters, TargetDefense);

	// 무기 피해량
	float BaseDamage = 0.0f;

	// 약 공격 콤보 수
	int32 LightAttackComboCount = 0;

	// 게임플레이 어빌리티 스펙의 SetByCallerTag를 순회하여 각 태그에 저장한 값들을 가져옴
	// 이는 앞서, 스펙을 만들면서 <태그, 크기> 형태로 저장했음
	for (const TPair<FGameplayTag, float>& TagMagnitude : EffectSpec.SetByCallerTagMagnitudes)
	{
		if (TagMagnitude.Key.MatchesTagExact(PA_GameplayTags::Shared_SetByCaller_BaseDamage))
		{
			BaseDamage = TagMagnitude.Value;
		}
		else if (TagMagnitude.Key.MatchesTagExact(PA_GameplayTags::Player_SetByCaller_AttackType_Light))
		{
			LightAttackComboCount = TagMagnitude.Value;
		}
	}


	// 최종 대미지 계산

	// 약 공격 콤보 대미지 추가 (+5%) 콤보 1은 제외
	if (LightAttackComboCount > 0)
	{
		const float DamageIncreasePercentLight = (LightAttackComboCount - 1) * 0.05f + 1.0f;
		BaseDamage *= DamageIncreasePercentLight;
	}

	// 최종 공격력 = 캐릭터 공격력 * 무기 피해량(콤보 O)
	const float AttackPower = SourceAttackPower * BaseDamage;

	// 공격력/방어력 (상대에 방어력에 따른 비율)
	const float Divide = AttackPower / TargetDefense;

	// 최종 피해량 (엘든링 방어력 계산 수식 적용)
	float FinalDamage = 0.0f;

	// 상대의 방어력에 따른 비율을 통해 최종 피해량 계산
	if (Divide < 0.125)
	{
		// 공격력 x 0.1
		FinalDamage = AttackPower * BaseDamage * 0.1f;
	}
	else if (Divide < 1.0f)
	{
		// 공격력 x ( (19.2/49) x (Divide - 0.125)^2 + 0.1 )
		const float Ratio = (19.2f / 49.0f) * FMath::Pow(Divide - 0.125f, 2.0f) + 0.1f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else if (Divide < 2.5f)
	{
		// 공격력 x ( (-0.4/3) x (Divide - 2.5)^2 + 0.7 )
		const float Ratio = (-0.4f / 3.0f) * FMath::Pow(Divide - 2.5f, 2.0f) + 0.7f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else if (Divide < 8.0f)
	{
		// 공격력 x ( (-0.8/121) x (Divide - 8)^2 + 0.9 )
		const float Ratio = (-0.8f / 121.0f) * FMath::Pow(Divide - 8.0f, 2.0f) + 0.9f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else // Divide >= 8.0f
	{
		// 공격력 x 0.9
		FinalDamage = AttackPower * BaseDamage * 0.9f;
	}

	// 피해자의 받은 피해량 갱신
	if (FinalDamage > 0.0f)
	{
		// DamageTaken 속성에 대해 Override 연산자로 최종 대미지 값을 적용
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			GetPADamageCapture().DamageTakenProperty,
			EGameplayModOp::Override,
			FinalDamage
		));
	}
}
