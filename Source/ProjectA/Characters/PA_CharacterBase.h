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

	// �ڽſ��� �����÷��� ����Ʈ ����
	bool ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, FGameplayEffectContextHandle InEffectContextHandle);

protected:
	virtual void PossessedBy(AController* NewController) override;

	// �⺻ ĳ���� ���� �����Ƽ �ο� �Լ�
	virtual void GiveStartUpAbilities(const TArray<TSubclassOf<UGameplayAbility>> StartUpAbilties);

	// �⺻ ĳ���� ���� �����÷��� ����Ʈ ���� �Լ�
	virtual void ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AttributeSetBase> AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TSoftObjectPtr<class UDA_BaseStartUpData> CharacterStartUpData;
};
