// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/PA_CharacterBase.h"
#include "PA_CharacterEnemy.generated.h"

UCLASS()
class PROJECTA_API APA_CharacterEnemy : public APA_CharacterBase
{
	GENERATED_BODY()

public:
	APA_CharacterEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;


	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
private:
	// �� ���� ������ �ʱ�ȭ
	void InitEnemyStartUpData();


	///////////////////////////////////////////////////////////////////////////
	/* Combat */
public:
	FORCEINLINE class UPA_EnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }

	// �� ���� ������Ʈ �������� �������̽� �Լ�
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

	// �� ��� �Լ� [��� ��Ƽ���� ȿ�� ��� �ð�, ȿ�� ��� ����, ��� ������ ���̾ư��� �ý���, ���̾ư��� ������ ����]
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void OnEnemyDied(float InDuration, float InUpdateInterval, TSoftObjectPtr<class UNiagaraSystem> DissolveNiagara, FLinearColor DissolveNiagaraColor);

private:
	// ��� ��Ƽ���� ȿ�� (������) �Լ�
	void UpdateMaterialParameter(float InDuration, float InUpdateInterval);
	void UpdateMaterialParameterCallback();

	// ��� ��Ƽ���� ȿ�� (������) ��� �ð�
	float ElapsedTime = 0.0f;

	// ��� ��Ƽ���� ȿ�� (������) �� ��� �ð�
	float Duration = 0.0f;

	// ��� ��Ƽ���� ȿ�� (������) ������Ʈ ����
	float UpdateInterval = 0.0f;

	// ��� ��Ƽ���� ȿ�� (������) Ÿ�̸�
	FTimerHandle EnemyDiedTimerHandle;

	UPROPERTY()
	class APA_BaseWeapon* EnemyWeapon;

	// �� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_EnemyCombatComponent> EnemyCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* UI & Widget */
public:
	// �� UI ������Ʈ �������� �������̽� �Լ�
	/* IPA_PawnUIInterface Interface */
	virtual UPA_PawnUIComponent* GetUIComponent() const override;
	virtual UPA_EnemyUIComponent* GetEnemyUIComponent() const override;
	/* IPA_PawnUIInterface Interface */

protected:
	// �� UI ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UPA_EnemyUIComponent> EnemyUIComponent;

	// �� ü�� �� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | UI")
	TObjectPtr<class UWidgetComponent> EnemyHealthWidgetComponent;
};
