// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Excutions/PA_DamageExecution.h"
#include "AbilitySystem/AttributeSets/PA_AttributeSetBase.h"

#include "PA_GameplayTags.h"

// ���� ��꿡 ����� �Ӽ� ĸó ����ü
struct FPA_DamageCapture
{
	// ���� ��꿡 ����� �Ӽ� ĸó ����
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FPA_DamageCapture()
	{
		// ĸó ���� �ʱ�ȭ (Source�� Target ����)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, AttackPower, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, Defense, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UPA_AttributeSetBase, DamageTaken, Source, false)
	}
};

// �Ӽ� ĸó ����ü�� ���� �ν��Ͻ��� ��ȯ�Ͽ� ĸó ���ǵ��� �������� ����
static const FPA_DamageCapture& GetPADamageCapture()
{
	static FPA_DamageCapture PADamageCapture;
	return PADamageCapture;
}

UPA_DamageExecution::UPA_DamageExecution()
{
	// ����ü���� ������ �Ӽ����� ĸó ���Ǹ� �߰��Ѵ�.
	// �̸� ���� ���� ��� �ÿ� �ش� �Ӽ����� ���� �ڵ����� ĸó�Ǿ� ��꿡 ���ȴ�.
	RelevantAttributesToCapture.Add(GetPADamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetPADamageCapture().DefenseDef);
	RelevantAttributesToCapture.Add(GetPADamageCapture().DamageTakenDef);
}

void UPA_DamageExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// ���� �������� �����÷��� ����Ʈ ������ ������
	const FGameplayEffectSpec EffectSpec = ExecutionParams.GetOwningSpec();

	// ĸó�� �ҽ� �±׿� Ÿ�� �±װ� ���Եǵ��� ����
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	// �������� ���ݷ�
	// ĸó�� ���� EvaluateParameters�� ���� ���
	float SourceAttackPower = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPADamageCapture().AttackPowerDef, EvaluateParameters, SourceAttackPower);

	// �������� ����
	float TargetDefense = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetPADamageCapture().DefenseDef, EvaluateParameters, TargetDefense);

	// ���� ���ط�
	float BaseDamage = 0.0f;

	// �� ���� �޺� ��
	int32 LightAttackComboCount = 0;

	// �����÷��� �����Ƽ ������ SetByCallerTag�� ��ȸ�Ͽ� �� �±׿� ������ ������ ������
	// �̴� �ռ�, ������ ����鼭 <�±�, ũ��> ���·� ��������
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


	// ���� ����� ���

	// �� ���� �޺� ����� �߰� (+5%) �޺� 1�� ����
	if (LightAttackComboCount > 0)
	{
		const float DamageIncreasePercentLight = (LightAttackComboCount - 1) * 0.05f + 1.0f;
		BaseDamage *= DamageIncreasePercentLight;
	}

	// ���� ���ݷ� = ĳ���� ���ݷ� * ���� ���ط�(�޺� O)
	const float AttackPower = SourceAttackPower * BaseDamage;

	// ���ݷ�/���� (��뿡 ���¿� ���� ����)
	const float Divide = AttackPower / TargetDefense;

	// ���� ���ط� (���縵 ���� ��� ���� ����)
	float FinalDamage = 0.0f;

	// ����� ���¿� ���� ������ ���� ���� ���ط� ���
	if (Divide < 0.125)
	{
		// ���ݷ� x 0.1
		FinalDamage = AttackPower * BaseDamage * 0.1f;
	}
	else if (Divide < 1.0f)
	{
		// ���ݷ� x ( (19.2/49) x (Divide - 0.125)^2 + 0.1 )
		const float Ratio = (19.2f / 49.0f) * FMath::Pow(Divide - 0.125f, 2.0f) + 0.1f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else if (Divide < 2.5f)
	{
		// ���ݷ� x ( (-0.4/3) x (Divide - 2.5)^2 + 0.7 )
		const float Ratio = (-0.4f / 3.0f) * FMath::Pow(Divide - 2.5f, 2.0f) + 0.7f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else if (Divide < 8.0f)
	{
		// ���ݷ� x ( (-0.8/121) x (Divide - 8)^2 + 0.9 )
		const float Ratio = (-0.8f / 121.0f) * FMath::Pow(Divide - 8.0f, 2.0f) + 0.9f;
		FinalDamage = AttackPower * BaseDamage * Ratio;
	}
	else // Divide >= 8.0f
	{
		// ���ݷ� x 0.9
		FinalDamage = AttackPower * BaseDamage * 0.9f;
	}

	// �������� ���� ���ط� ����
	if (FinalDamage > 0.0f)
	{
		// DamageTaken �Ӽ��� ���� Override �����ڷ� ���� ����� ���� ����
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			GetPADamageCapture().DamageTakenProperty,
			EGameplayModOp::Override,
			FinalDamage
		));
	}
}
