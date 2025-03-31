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
	// 다음 콤보 공격 수행
	UFUNCTION(BlueprintCallable, Category = "Custom | Ability")
	void NextComboAttack();

	// 콤보 공격 카운트 초기화
	void ResetComboAttackCount();

protected:
	// 공격 몽타주 맵
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	TMap<int32, TObjectPtr<class UAnimMontage>> AttackMontageMap;

	// 플레이중인 몽타주 태스크
	UPROPERTY()
	TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

	// 현재 콤보 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
	int32 CurrenCombo = 1;

	// 콤보 타이머
	UPROPERTY()
	FTimerHandle ComboTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Ability")
	bool bComboAttack = false;
};
