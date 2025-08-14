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

protected:
	virtual void BeginPlay();
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> SpringArm;


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

	// 어빌리티 입력 액션 바인딩 함수
	void OnAbilityInputPressed(FGameplayTag InInputTag);
	void OnAbilityInputReleased(FGameplayTag InInputTag);

private:
	// 입력 액션 구성 데이터 에셋
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDA_InputConfig> InputConfigData;


	///////////////////////////////////////////////////////////////////////////
	/* Attributes */
protected:
	// 최대 이동속도 변경 함수
	virtual void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) override;


	///////////////////////////////////////////////////////////////////////////
	/* CameraMask */
protected:
	// 카메라 마스크 업데이트 함수
	void CameraMaskUpdate();

	// 카메라 마스크 업데이트 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	float CameraMaskUpdateInterval = 0.15f;

private:
	FTimerHandle CameraMaskTimerHandle;

	// 카메라에 마스킹된 장애물 배열
	TArray<TWeakObjectPtr<UPrimitiveComponent>> CameraMaskingObstacle;


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	FORCEINLINE class UPA_PlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

	// 플레이어 전투 컴포넌트 가져오기 인터페이스 함수
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

protected:
	// 플레이어 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat")
	TObjectPtr<class UPA_PlayerCombatComponent> PlayerCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* UI & Widget */
public:
	// 플레이어 UI 컴포넌트 가져오기 인터페이스 함수
	/* IPA_PawnUIInterface Interface */
	virtual UPA_PawnUIComponent* GetUIComponent() const override;
	virtual UPA_PlayerUIComponent* GetPlayerUIComponent() const override;
	/* IPA_PawnUIInterface Interface */

protected:
	// 플레이어 UI 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UPA_PlayerUIComponent> PlayerUIComponent;

};
