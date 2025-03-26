// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PA_GameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	OnTriggered,	// �Է� �Ǵ� �̺�Ʈ�� ���� Ʈ�����ϴ� ���
	OnGiven,		// �����Ƽ�� �ο����� ���� / ��ȸ��
};

UCLASS()
class PROJECTA_API UPA_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// �� ���� ������Ʈ ��������
	UFUNCTION(BlueprintPure, Category = "Custom | Ability")
	class UPA_PawnCombatComponent* GetPawnCombatComponentFromActorInfo() const;

protected:
	// �����Ƽ Ȱ��ȭ ��å
	UPROPERTY(EditDefaultsOnly, Category = "Custom | Ability")
	EAbilityActivationPolicy AbilityActivationPolicy = EAbilityActivationPolicy::OnTriggered;
};
