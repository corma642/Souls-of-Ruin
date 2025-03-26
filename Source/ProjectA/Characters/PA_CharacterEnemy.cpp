// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PA_CharacterEnemy.h"

#include "Components/Combat/PA_EnemyCombatComponent.h"

APA_CharacterEnemy::APA_CharacterEnemy()
{

	// 전투 컴포넌트
	EnemyCombatComponent = CreateDefaultSubobject<UPA_EnemyCombatComponent>(TEXT("EnemyCombatComponent"));
}

UPA_PawnCombatComponent* APA_CharacterEnemy::GetPawnCombatComponent() const
{
	return EnemyCombatComponent;
}
