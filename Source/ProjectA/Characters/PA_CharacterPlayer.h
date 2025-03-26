// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PA_CharacterBase.h"
#include "InputActionValue.h"
#include "PA_CharacterPlayer.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterPlayer : public APA_CharacterBase
{
	GENERATED_BODY()

public:
	APA_CharacterPlayer();

	/* ICombatInterface Interface */
	// 전투 컴포넌트 가져오기 인터페이스 함수
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

protected:
	virtual void BeginPlay();
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;

	// 플레이어 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_PlayerCombatComponent> PlayerCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* Input */
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 이동 함수
	void OnMove(const FInputActionValue& InputActionValue);

	// 카메라 이동 함수
	void OnLook(const FInputActionValue& InputActionValue);

	// 카메라 확대 축소 함수
	void OnCameraZoom(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;


	///////////////////////////////////////////////////////////////////////////
	/* AbilitySystem */
protected:
	// 기본 캐릭터 시작 어빌리티 부여 함수
	virtual void GiveStartUpAbilities(const TArray<TSubclassOf<UPA_GameplayAbility>> StartUpAbilties, int32 ApplyLevel = 1) override;

	// 기본 캐릭터 시작 게임플레이 이펙트 적용 함수
	virtual void ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects) override;

};
