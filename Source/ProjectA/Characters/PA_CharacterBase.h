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

	// 전투 컴포넌트 가져오기 인터페이스 함수
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

	// UI 컴포넌트 가져오기 인터페이스 함수
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
	// 최대 이동속도 변경 함수
	virtual void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

	// 블루프린트에서 직접 최대 이동속도 변경 시, 이 함수를 호출해서 AttributeSet과 동기화
	UFUNCTION(BlueprintCallable, Category = "Custom | Attributes")
	virtual void SetMaxMovementSpeedAndAttribute(float NewSpeed);


	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
protected:
	// 캐릭터 시작 어빌리티 약 참조 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | StartUpData")
	TSoftObjectPtr<class UDA_BaseStartUpData> CharacterStartUpData;
};
