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

    // �޺� ���� �ʱ�ȭ Ÿ�̸� ����
    void ResetComboAttack();

    // Notify �̺�Ʈ ��� �½�ũ �ݹ� �Լ�
    UFUNCTION(BlueprintCallable)
    void OnComboNotifyReceived(const FGameplayEventData Payload);

protected:
    // �޺� ���� ��Ÿ�� ��
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Ability")
    TMap<int32, TObjectPtr<class UAnimMontage>> ComboAttackMontagesMap;

    // ���� �޺� ��
    UPROPERTY(BlueprintReadOnly, Category = "Custom | Ability")
    mutable int32 CurrentCombo = 1;

    // ��Ÿ�� �÷��� �½�ũ
    UPROPERTY()
    TObjectPtr<class UAbilityTask_PlayMontageAndWait> MontageTask = nullptr;

private:
    // �޺� �Է� �÷��� (���� ������ �Է��ߴ��� ����)
    bool bComboInputReceived = false;

    // �޺� ���� �ʱ�ȭ Ÿ�̸� �ڵ�
    FTimerHandle ComboTimerHandle;
};
