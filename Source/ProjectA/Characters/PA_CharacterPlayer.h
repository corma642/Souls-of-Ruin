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
	virtual void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) override;


	///////////////////////////////////////////////////////////////////////////
	/* CameraMask */
protected:
	// ī�޶� ����ũ ������Ʈ �Լ�
	void CameraMaskUpdate();

	// ī�޶� ����ũ ������Ʈ ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom | Camera", meta = (AllowPrivateAccess = "true"))
	float CameraMaskUpdateInterval = 0.15f;

private:
	FTimerHandle CameraMaskTimerHandle;

	// ī�޶� ����ŷ�� ��ֹ� �迭
	TArray<TWeakObjectPtr<UPrimitiveComponent>> CameraMaskingObstacle;


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	FORCEINLINE class UPA_PlayerCombatComponent* GetPlayerCombatComponent() const { return PlayerCombatComponent; }

	// �÷��̾� ���� ������Ʈ �������� �������̽� �Լ�
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

protected:
	// �÷��̾� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat")
	TObjectPtr<class UPA_PlayerCombatComponent> PlayerCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* UI & Widget */
public:
	// �÷��̾� UI ������Ʈ �������� �������̽� �Լ�
	/* IPA_PawnUIInterface Interface */
	virtual UPA_PawnUIComponent* GetUIComponent() const override;
	virtual UPA_PlayerUIComponent* GetPlayerUIComponent() const override;
	/* IPA_PawnUIInterface Interface */

protected:
	// �÷��̾� UI ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UPA_PlayerUIComponent> PlayerUIComponent;

};
