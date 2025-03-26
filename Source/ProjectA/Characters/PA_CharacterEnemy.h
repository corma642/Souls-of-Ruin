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

	/* ICombatInterface Interface */
	// ���� ������Ʈ �������� �������̽� �Լ�
	virtual UPA_PawnCombatComponent* GetPawnCombatComponent() const override;
	/* ICombatInterface Interface */

private:
	// �� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_EnemyCombatComponent> EnemyCombatComponent;

};
