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
	
	FORCEINLINE class UPA_EnemyCombatComponent* GetEnemyCombatComponent() const { return EnemyCombatComponent; }
	
	/* ICombatInterface Interface */
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

	// �� ��� �Լ� [��� ��Ƽ���� ȿ�� ��� �ð�, ȿ�� ��� ����, ��� ������ ���̾ư��� �ý���, ���̾ư��� ������ ����]
	UFUNCTION(BlueprintCallable, Category = "Custom | Combat")
	void OnEnemyDied(float InDuration, float InUpdateInterval, TSoftObjectPtr<class UNiagaraSystem> DissolveNiagara, FLinearColor DissolveNiagaraColor);

protected:
	virtual void PossessedBy(AController* NewController) override;

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

	// �� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_EnemyCombatComponent> EnemyCombatComponent;

	UPROPERTY()
	class APA_BaseWeapon* EnemyWeapon;

	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
private:
	// �� ���� ������ �ʱ�ȭ
	void InitEnemyStartUpData();
};
