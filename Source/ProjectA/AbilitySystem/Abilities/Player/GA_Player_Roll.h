// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_Roll.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_Roll : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGA_Player_Roll();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// ��Ÿ�� ��� �Լ�
	void PlayRollMontage();

	// ������ ���� �� �Ÿ� ���� �Լ�
	void ComputeRollDirectionAndDistance();

protected:
	// ��Ÿ�� �÷��� �½�ũ
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// ������ ��Ÿ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TObjectPtr<class UAnimMontage> RollMontage;

	// ���� ��, ������ �� �ִ� ������Ʈ Ÿ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TArray<TEnumAsByte<EObjectTypeQuery>> RollObjectTypes;

	// ������ �Ÿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	float RollDistance = 500.f;

private:
	// ���� ������ Ÿ�̸� �ڵ�
	FTimerHandle DelayTimerHandle;
};
