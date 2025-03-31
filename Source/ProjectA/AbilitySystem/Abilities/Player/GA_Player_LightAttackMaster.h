// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/PA_PlayerGameplayAbility.h"
#include "GA_Player_LightAttackMaster.generated.h"

UCLASS()
class PROJECTA_API UGA_Player_LightAttackMaster : public UPA_PlayerGameplayAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// ���� �޺� ���� ����
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability")
	void NextComboAttack();

	// �޺� ���� ī��Ʈ �ʱ�ȭ
	void ResetComboAttackCount();

protected:
	// ���� ��Ÿ�� ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TMap<int32, TObjectPtr<class UAnimMontage>> AttackMontageMap;

	// �÷������� ��Ÿ�� �½�ũ
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// ���� �޺� ��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	int32 CurrenCombo = 1;

	// �޺� Ÿ�̸�
	UPROPERTY()
	FTimerHandle ComboTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Ability")
	bool bComboAttack = false;
};
