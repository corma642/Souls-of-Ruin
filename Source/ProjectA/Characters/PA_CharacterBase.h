// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Abilities/GameplayAbility.h"
#include "PA_CharacterBase.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	APA_CharacterBase();
	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;


	///////////////////////////////////////////////////////////////////////////
	/* AbilitySystem */
public:
	/* IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* IAbilitySystemInterface */

	class UPA_AttributeSetBase* GetAttributeSet() const { return AttributeSet; }

	// 자신에게 게임플레이 이펙트 적용
	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContextHandle);

protected:
	virtual void PossessedBy(AController* NewController) override;

	// 기본 캐릭터 시작 어빌리티 부여 함수
	virtual void GiveStartUpAbilities(const TArray<TSubclassOf<UGameplayAbility>> StartUpAbilties);

	// 기본 캐릭터 시작 게임플레이 이펙트 적용 함수
	virtual void ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AttributeSetBase> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TSoftObjectPtr<class UDA_BaseStartUpData> CharacterStartUpData;
};
