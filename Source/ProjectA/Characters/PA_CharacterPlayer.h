// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PA_CharacterBase.h"
#include "InputActionValue.h"
#include "GameplayTagContainer.h"
#include "PA_CharacterPlayer.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterPlayer : public APA_CharacterBase
{
	GENERATED_BODY()

public:
	APA_CharacterPlayer();

	FORCEINLINE class UPA_PlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

	/* ICombatInterface Interface */
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

	// 카메라 회전 함수
	void OnLook(const FInputActionValue& InputActionValue);

	// 카메라 줌 인-아웃 함수
	void OnCameraZoom(const FInputActionValue& InputActionValue);

	// 달리기 어빌리티 함수
	void OnSprintStarted(const FInputActionValue& InputActionValue);
	void OnSprintEnded(const FInputActionValue& InputActionValue);

	// 무기 장착 어빌리티 함수
	void OnEquipWeapon(const FInputActionValue& InputActionValue);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	// 이동 입력 액션 { W, A, S, D }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	// 카메라 회전 입력 액션 { Mouse X-Axis }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	// 카메라 줌 인-아웃 입력 액션 { Mouse Wheel }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> CameraZoomAction;

	// 달리기 입력 액션 { Shift }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> SprintAction;

	// 무기 장착 입력 액션 { E }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipWeaponAction;


	///////////////////////////////////////////////////////////////////////////
	/* Gameplay Tags */
protected:
	// 달리기 어빌리티 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Tags | Abilities")
	FGameplayTagContainer SprintTags;

protected:



	///////////////////////////////////////////////////////////////////////////
	/* AbilitySystem */
protected:
	// 기본 캐릭터 시작 어빌리티 부여 함수
	virtual void GiveStartUpAbilities(const TArray<TSubclassOf<UPA_GameplayAbility>> StartUpAbilties) override;

	// 기본 캐릭터 시작 게임플레이 이펙트 적용 함수
	virtual void ApplyStartUpEffects(const TArray<TSubclassOf<UGameplayEffect>> StartUpEffects) override;


	///////////////////////////////////////////////////////////////////////////
	/* Attributes */
protected:
	// 최대 이동속도 변경 함수
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
};
