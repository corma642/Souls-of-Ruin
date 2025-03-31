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

	// �÷��̾� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_PlayerCombatComponent> PlayerCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* Input */
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// �̵� �Լ�
	void OnMove(const FInputActionValue& InputActionValue);

	// ī�޶� ȸ�� �Լ�
	void OnLook(const FInputActionValue& InputActionValue);

	// ī�޶� �� ��-�ƿ� �Լ�
	void OnCameraZoom(const FInputActionValue& InputActionValue);

	// �����Ƽ �Է� �׼� ���ε� �Լ�
	void OnAbilityInputPressed(FGameplayTag InInputTag);
	void OnAbilityInputReleased(FGameplayTag InInputTag);

private:
	// �Է� �׼� ���� ������ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UDA_InputConfig> InputConfigData;


	///////////////////////////////////////////////////////////////////////////
	/* Attributes */
protected:
	// �ִ� �̵��ӵ� ���� �Լ�
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);
};
