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

protected:
	virtual void PossessedBy(AController* NewController) override;

private:
	// 적 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom | Combat", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPA_EnemyCombatComponent> EnemyCombatComponent;


	///////////////////////////////////////////////////////////////////////////
	/* StartUpData */
private:
	// 적 시작 데이터 초기화
	void InitEnemyStartUpData();
};
