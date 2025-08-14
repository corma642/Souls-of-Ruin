// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Abilities/PA_GameplayAbility.h"
#include "Interface/PA_PawnCombatInterface.h"
#include "Interface/PA_PawnUIInterface.h"
#include "PA_CharacterBase.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterBase : public ACharacter, public IAbilitySystemInterface, public IPA_PawnCombatInterface, public IPA_PawnUIInterface
{
	GENERATED_BODY()

public:
	APA_CharacterBase();
	virtual void PostInitializeComponents() override;

	// ���� ������Ʈ �������� �������̽� �Լ�
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

	// UI ������Ʈ �������� �������̽� �Լ�
	/* IPA_PawnUIInterface Interface */
	virtual UPA_PawnUIComponent* GetUIComponent() const override;
	/* IPA_PawnUIInterface Interface */

protected:
	virtual void BeginPlay() override;


	///////////////////////////////////////////////////////////////////////////
	/* AbilitySystem */
public:
	/* IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/* IAbilitySystemInterface */

	virtual class UPA_AttributeSetBase* GetAttributeSet() const { return AttributeSet; }

	virtual class UMotionWarpingComponent* GetMotionWarpingComponent() const { return MotionWarpingComponent; }

protected:
	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | AbilitySystem")
	TObjectPtr<class UPA_AttributeSetBase> AttributeSet;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | MotionWarping")
	TObjectPtr<class UMotionWarpingComponent> MotionWarpingComponent;


	///////////////////////////////////////////////////////////////////////////
	/* Attributes */
protected:
	// �ִ� �̵��ӵ� ���� �Լ�
	virtual void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

	// �������Ʈ���� ���� �ִ� �̵��ӵ� ���� ��, �� �Լ��� ȣ���ؼ� AttributeSet�� ����ȭ
	UFUNCTION(BlueprintCallable, Category = "Custom | Attributes")
	virtual void SetMaxMovementSpeedAndAttribute(float NewSpeed);


	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
protected:
	// ĳ���� ���� �����Ƽ �� ���� ������
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TSoftObjectPtr<class UDA_BaseStartUpData> CharacterStartUpData;
};
