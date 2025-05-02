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
    UGA_Player_LightAttackMaster();

    virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
    virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

    // 콤보 공격 초기화 타이머 수행
    void ResetComboAttack();

    // Notify 이벤트 대기 태스크 콜백 함수
    UFUNCTION(BlueprintCallable)
    void OnComboNotifyReceived(const FGameplayEventData Payload);

protected:
    // 콤보 공격 몽타주 맵
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
    TMap<int32, TObjectPtr<class UAnimMontage>> ComboAttackMontagesMap;

    // 현재 콤보 수
    UPROPERTY(BlueprintReadOnly, Category = "Custom | Ability")
    mutable int32 CurrentCombo = 1;

    // 몽타주 플레이 태스크
    UPROPERTY()
    TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

private:
    // 콤보 입력 플래그 (다음 공격을 입력했는지 여부)
    bool bComboInputReceived = false;

    // 콤보 공격 초기화 타이머 핸들
    FTimerHandle ComboTimerHandle;
};
