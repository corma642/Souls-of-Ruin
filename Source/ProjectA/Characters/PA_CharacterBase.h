// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "Interface/PawnCombatInterface.h"
#include "PA_CharacterBase.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterBase : public ACharacter, public IAbilitySystemInterface, public IPawnCombatInterface
{
	GENERATED_BODY()

public:
	APA_CharacterBase();
	virtual void PostInitializeComponents() override;

	/* ICombatInterface Interface */
	// ���� ������Ʈ �������� �������̽� �Լ�
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

protected:
	virtual void BeginPlay() override;


	///////////////////////////////////////////////////////////////////////////
	/* AbilitySystem */
public:
	/* IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* IAbilitySystemInterface */

	class UPA_AttributeSetBase* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AttributeSetBase> AttributeSet;

	// ĳ���� ���� �����Ƽ �� ���� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TSoftObjectPtr<class UDA_BaseStartUpData> CharacterStartUpData;
};
