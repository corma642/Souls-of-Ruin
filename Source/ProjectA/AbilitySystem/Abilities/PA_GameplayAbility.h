// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "PA_GameplayAbility.generated.h"

UCLASS()
class PROJECTA_API UPA_GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	// �÷��̾� ĳ���� ��������
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class APA_CharacterPlayer* GetPlayerCharacterFromActorInfo() const;

	// �� ĳ���� ��������
	UFUNCTION(BlueprintCallable, BlueprintPure)
	class APA_CharacterEnemy* GetEnemyCharacterFromActorInfo() const;

	// ȿ���� ������ ���ŵǴ� ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToRemoveOnEnd;

	// ȿ���� ������ ���ŵǴ� ����Ʈ �ڵ�
	TArray<FActiveGameplayEffectHandle> RemoveOnEndEffectHandles;

	// ���� �� ��� ����Ǵ� ����Ʈ
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> OngoingEffectsToJustApplyOnStart;
};
